CPC357 - Group 19
IoT Project - Smart Streetlight

project Directory Structure

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

Prerequisites
1. Connect all the hardwares and components like shown in the diagram.
2. Prepare Maker Feather AIoT S3 board or any IoT microcontroller
3. Arduino IDE with necessary libraries installed
4. Google Cloud Platform (GCP) SSH is running and connected using the right internet connection.
4. Python 3.8 or newer installed
5. MongoDB server running locally or accessible remotely
6. Download chart.js from website (run the command on CLI)

curl -o chart.js https://cdn.jsdelivr.net/npm/chart.js

 OR

wget -O chart.js https://cdn.jsdelivr.net/npm/chart.js

mv chart.js static/


How to execute the project
1. Run the arduino_gcp.cpp code from Arduino IDE
2. Open GCP SSH and wait until the data shown in the CLI
3. run the command on CLI: python3 app.py
4. from your web browser, navigate to http://localhost:5000 or http://<gcp_public_ip>:5000 (if access remotely)
5. dashboard will appear in the web browser.