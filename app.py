from flask import Flask, render_template, jsonify, request
import paho.mqtt.client as mqtt
from pymongo import MongoClient
from datetime import datetime, timedelta

# MongoDB setup
mongo_client = MongoClient("mongodb://localhost:27017/")
db = mongo_client['iot_dashboard']
collection = db['sensor_data']

# Flask app setup
app = Flask(__name__)

# Global variables
latest_data = {
    "IRs": [0, 0, 0],
    "LEDs": [25, 25, 25],
    "Rain Sensor": 0,
    "Temperature": 0.0,
    "Humidity": 0.0,
    "Energy Saved": 0.0
}
history = {
    "timestamps": [],
    "IRs": [],
    "LEDs": [],
    "Rain Sensor": [],
    "Temperature": [],
    "Humidity": []
}
brightness_levels = {25: 0.25, 50: 0.50, 100: 1.0}  # Brightness levels as percentages

# MQTT callback for incoming messages
def on_message(client, userdata, msg):
    global latest_data, history
    message = msg.payload.decode()
    print(f"Received message: {message}")

    try:
        now = datetime.now().strftime("%H:%M:%S")
        if "IR1" in message:
            latest_data["IRs"][0] = 1 if "DETECTED" in message else 0
            latest_data["LEDs"][0] = int(message.split("LED1: ")[1].replace("%", ""))
        if "IR2" in message:
            latest_data["IRs"][1] = 1 if "DETECTED" in message else 0
            latest_data["LEDs"][1] = int(message.split("LED2: ")[1].replace("%", ""))
        if "IR3" in message:
            latest_data["IRs"][2] = 1 if "DETECTED" in message else 0
            latest_data["LEDs"][2] = int(message.split("LED3: ")[1].replace("%", ""))
        if "Rain Sensor" in message:
            latest_data["Rain Sensor"] = 1 if "RAIN DETECTED" in message else 0
        if "Temperature" in message and "Humidity" in message:
            temp = float(message.split("Temperature: ")[1].split("°C")[0])
            humidity = float(message.split("Humidity: ")[1].split("%")[0])
            latest_data["Temperature"] = temp
            latest_data["Humidity"] = humidity

        # Calculate energy saved
        max_brightness = 3 * 1.0  # Max brightness with all LEDs at 100%
        current_brightness = sum([brightness_levels[led] for led in latest_data["LEDs"]])
        latest_data["Energy Saved"] = round((1 - current_brightness / max_brightness) * 100, 2)

        # Append history
        history["timestamps"].append(now)
        history["IRs"].append(list(latest_data["IRs"]))
        history["LEDs"].append(list(latest_data["LEDs"]))
        history["Rain Sensor"].append(latest_data["Rain Sensor"])
        history["Temperature"].append(latest_data["Temperature"])
        history["Humidity"].append(latest_data["Humidity"])

        # Store the data in MongoDB
        collection.insert_one({"timestamp": now, "data": latest_data})
    except Exception as e:
        print(f"Error processing message: {e}")

# MQTT setup
mqtt_client = mqtt.Client()
mqtt_client.on_message = on_message
mqtt_client.connect("localhost", 1883, 60)
mqtt_client.subscribe("iot")
mqtt_client.loop_start()

# Flask routes
@app.route('/')
def index():
    return render_template("dashboard.html")

@app.route('/analysis')
def analysis():
    return render_template("analysis.html")

@app.route('/data')
def get_data():
    return jsonify(latest_data)

@app.route('/history')
def get_history():
    return jsonify(history)

@app.route('/analysis/data')
def analysis_data():
    period = request.args.get('period', 'hours')
    now = datetime.now()

    if period == 'hours':
        start_time = now - timedelta(hours=1)
    elif period == 'days':
        start_time = now - timedelta(days=1)
    else:
        start_time = now

    # Filter MongoDB data based on the time period
    results = collection.find({"timestamp": {"$gte": start_time.strftime("%H:%M:%S")}})
    data = [{"timestamp": res["timestamp"], "LEDs": res["data"]["LEDs"]} for res in results]

    return jsonify(data)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)