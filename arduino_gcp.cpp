#include <PubSubClient.h>
#include <WiFi.h>
#include <DHT.h>

// WiFi and MQTT server credentials
const char* WIFI_SSID = "your_ssid";           // Your WiFi SSID
const char* WIFI_PASSWORD = "your_password";   // Your WiFi password
const char* MQTT_SERVER = "your_gcp_public_ip";  // Your VM instance public IP address
const int MQTT_PORT = 1883;                // MQTT port (default is 1883)
const char* MQTT_TOPIC = "iot";            // MQTT topic for publishing

// DHT sensor setup
#define DHTTYPE DHT11
const int dhtPin = 21; // DHT11 sensor pin
DHT dht(dhtPin, DHTTYPE);

// IR sensors and LEDs pins
const int irPin1 = 7;    // IR sensor 1
const int ledPin1 = 9;   // LED 1
const int irPin2 = 42;   // IR sensor 2
const int ledPin2 = 6;   // LED 2
const int irPin3 = 48;   // IR sensor 3
const int ledPin3 = 47;  // LED 3

// Rain sensor pin
const int rainPin = 5;

// Brightness levels
const int brightness25 = 64;   // 25% brightness
const int brightness50 = 128;  // 50% brightness
const int brightness75 = 192;  // 75% brightness
const int brightness100 = 255; // 100% brightness

// MQTT client setup
WiFiClient espClient;
PubSubClient client(espClient);
char buffer[256];

// Function to connect to WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Function to reconnect to MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT server");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);             // Start serial communication
  dht.begin();                      // Initialize DHT sensor
  setup_wifi();                     // Connect to WiFi
  client.setServer(MQTT_SERVER, MQTT_PORT); // Set up MQTT client

  // Initialize IR sensors and LEDs
  pinMode(irPin1, INPUT_PULLUP);
  pinMode(ledPin1, OUTPUT);
  pinMode(irPin2, INPUT_PULLUP);
  pinMode(ledPin2, OUTPUT);
  pinMode(irPin3, INPUT_PULLUP);
  pinMode(ledPin3, OUTPUT);
  pinMode(rainPin, INPUT_PULLUP);
}

void loop() {
  // Ensure MQTT connection
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check DHT readings
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  // Read rain sensor
  int rainSensorVal = !digitalRead(rainPin);

  // Determine rain detected status
  bool rainDetected = rainSensorVal && (humidity > 65.0) && (temperature < 32.0);

  // Read IR sensors
  int irVal1 = !digitalRead(irPin1);
  int irVal2 = !digitalRead(irPin2);
  int irVal3 = !digitalRead(irPin3);

  // Publish IR sensors and LED status
  sprintf(buffer, "IR1: %s, LED1: %d%%", irVal1 ? "DETECTED" : "NOT DETECTED", irVal1 ? 100 : (rainDetected ? 50 : 25));
  client.publish(MQTT_TOPIC, buffer);
  Serial.println(buffer);

  sprintf(buffer, "IR2: %s, LED2: %d%%", irVal2 ? "DETECTED" : "NOT DETECTED", irVal2 ? 100 : (rainDetected ? 50 : 25));
  client.publish(MQTT_TOPIC, buffer);
  Serial.println(buffer);

  sprintf(buffer, "IR3: %s, LED3: %d%%", irVal3 ? "DETECTED" : "NOT DETECTED", irVal3 ? 100 : (rainDetected ? 50 : 25));
  client.publish(MQTT_TOPIC, buffer);
  Serial.println(buffer);

  // Publish rain sensor status
  sprintf(buffer, "Rain Sensor: %s", rainDetected ? "RAIN DETECTED" : "NO RAIN");
  client.publish(MQTT_TOPIC, buffer);
  Serial.println(buffer);

  // Publish DHT readings
  sprintf(buffer, "Temperature: %.2f °C, Humidity: %.2f%%", temperature, humidity);
  client.publish(MQTT_TOPIC, buffer);
  Serial.println(buffer);

  // Adjust LEDs based on IR and rain sensor
  analogWrite(ledPin1, irVal1 ? brightness100 : (rainDetected ? brightness50 : brightness25));
  analogWrite(ledPin2, irVal2 ? brightness100 : (rainDetected ? brightness50 : brightness25));
  analogWrite(ledPin3, irVal3 ? brightness100 : (rainDetected ? brightness50 : brightness25));

  delay(2000); // Delay for 5 seconds
}