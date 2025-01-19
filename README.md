# Smart Streetlight Project

### CPC357 - Group 19

## IoT Project: Smart Streetlight

This project is a smart streetlight system designed to monitor and manage streetlights efficiently using IoT technology. The project integrates Arduino-based hardware with a Flask web application backend and a Google Cloud Platform (GCP) environment for data handling and analysis.

---

## Hardware Setup

Below is the hardware setup for the project (refer 'diagram' folder for more):

![Hardware Setup](diagram/hardware%20setup.JPG)

![Circuit Diagram](diagram/circuit%20diagram.png)

![Dashboard 1](diagram/dashboard%20interfaces/dashboard%201.png)

---

## Project Directory Structure

```
project/
│
├── arduino_gcp.cpp               # Arduino code to publish the data from IoT Board to GCP
├── app.py                        # Flask app backend
├── static/                       # Static files for frontend
│   ├── chart.js                  # JavaScript library for charts
│   └── style.css                 # CSS styles
├── templates/                    # HTML templates for Flask
│   ├── dashboard.html            # Dashboard page
│   └── analysis.html             # Analysis page
├── requirements.txt              # List of libraries needed for this project
└── README.md                     # Documentation
```

---

## Prerequisites

Ensure the following are set up before running the project:

1. **Hardware Setup:**
   - Connect all hardware components as shown in the circuit diagram.
   - Use a Maker Feather AIoT S3 board or any compatible IoT microcontroller.

2. **Software Setup:**
   - Install **Arduino IDE** with necessary libraries.
   - Ensure **Python 3.8** or newer is installed.
   - Have a **MongoDB server** running locally or accessible remotely.

3. **Google Cloud Platform (GCP):**
   - Set up and connect GCP SSH using the correct internet connection.

4. **Chart.js Library:**
   - Download the Chart.js library for frontend visualizations. Use one of the following commands:

   ```bash
   curl -o chart.js https://cdn.jsdelivr.net/npm/chart.js
   ```
   OR
   ```bash
   wget -O chart.js https://cdn.jsdelivr.net/npm/chart.js
   ```
   - Move the downloaded file to the `static/` directory:

   ```bash
   mv chart.js static/
   ```

---

## How to Execute the Project

1. **Run the Arduino Code:**
   - Open `arduino_gcp.cpp` in the Arduino IDE.
   - Upload the code to the IoT microcontroller.

2. **Monitor GCP:**
   - Open GCP SSH and verify that the data is being transmitted correctly.

3. **Run the Flask Application:**
   - Execute the Flask backend:

     ```bash
     python3 app.py
     ```

4. **Access the Dashboard:**
   - Open your web browser and navigate to:
     - **Local Access:** `http://localhost:5000`
     - **Remote Access:** `http://<gcp_public_ip>:5000`

5. **View the Dashboard:**
   - The dashboard will display the smart streetlight data and analysis.

---

## Features

- **IoT Integration:** Real-time data collection from the streetlight sensors.
- **Data Visualization:** Interactive charts powered by Chart.js.
- **Cloud Connectivity:** Data storage and processing via GCP.
- **Responsive Design:** Web dashboard accessible on multiple devices.

---

**Contributors:**
- Group 19, CPC357
1. Malihah Munirah 
2. Nur Qistina 

For inquiries, please contact: 
1. malihahmunirah3@gmail.com
2. qistinasolleh7@gmail.com
