# **Smart Irrigation System**  

## Overview  

The **Smart Irrigation System** is an IoT-based solution designed to automate agricultural irrigation by leveraging sensor data, machine learning, and cloud computing. The system collects real-time data from soil moisture, temperature, humidity, and rain sensors, processes it via a machine learning model deployed on Microsoft Azure, and controls a water pump. The system also integrates with the **Blynk IoT platform** for visualization and remote control.  

---

## **Key Features**  

### **IoT Functionality**  
- **Sensors**: Monitors soil moisture, humidity, and rain presence.  
- **Control**: Automates the irrigation process using a relay and water pump.  
- **Visualization**: Displays real-time sensor readings and pump status on an LCD and the Blynk app.  

### **Cloud ML Deployment**  
- **Azure Integration**: The ML model predicts irrigation needs based on sensor data and runs on an Azure Function App.  
- **API**: Predictions are accessed via a REST API, enabling real-time decision-making by the ESP32.  

### **Blynk IoT**  
- **Data Visualization**: Real-time sensor data displayed via mobile widgets.  
- **Remote Control**: Enables users to control the pump remotely.  
- **Custom Dashboard**: Configurable interface for better usability.  

---

## **Setup Instructions**  

### **1. IoT System Setup**  

#### **Components**  
- ESP32 microcontroller  
- Soil moisture sensor, DHT11 sensor, rain sensor  
- Relay module  
- Water pump  
- LCD  

#### **Circuit Connections**  
- Sensors and relay are connected to ESP32 GPIO pins.  
- Use an LCD to display sensor readings and pump status.  

---

### **2. Cloud ML Deployment**  

#### **Folder**: `smart-irrigation-predict-cloud`  

This folder contains all files and instructions for deploying the ML model on Microsoft Azure.  

#### **Features**  
- **Deployment**: Azure Function App hosts the ML model.  
- **Integration**: API consumed by ESP32 for real-time irrigation control.  

#### **Steps to Deploy**  

1. **Prerequisites**  
   - Azure account with CLI installed  
   - Trained ML model (`model.pkl`) and scaler (`scaler.pkl`)  
   - Python for development  

2. **Deployment Process**  
   - **Create Azure Function App**:  
     ```bash  
     az login  
     az group create --name IoTProjectGroup --location eastus  
     az functionapp create --resource-group IoTProjectGroup --consumption-plan-location eastus --runtime python --runtime-version 3.9 --functions-version 4 --name IoTProjectFunction --storage-account <STORAGE_ACCOUNT_NAME>  
     ```  

   - **Deploy Function App**:  
     ```bash  
     git clone https://github.com/taimoorsardar/Smart-Irrigation-System.git  
     cd Smart-Irrigation-System/smart-irrigation-predict-cloud  
     pip install -r requirements.txt  
     func azure functionapp publish IoTProjectFunction  
     ```  

3. **Test API**  
   - Use cURL or Postman:  
     ```bash  
     curl -X POST -H "Content-Type: application/json" -d '{"moisture": 50, "humidity": 65, "rain": "no"}' <function_app_url>/api/predict  
     ```  

4. **Integrate with ESP32**  
   - Update your ESP32 code with the API endpoint URL to send sensor data and process predictions.  

#### **File Structure**  

```  
cloud_ml/  
│  
├── host.json                 # Azure function initialization  
├── local.settings.json       # Azure function configuration  
├── requirements.txt          # Python dependencies  
├── function_app.py           # Main function code  
├── model.pkl                 # Trained ML model  
└── scaler.pkl                # Scaler for preprocessing data  
```  

---

## **System Workflow**  

### **1. Data Collection and Control**  
1. Sensors send real-time data to the ESP32 microcontroller.  
2. ESP32 transmits data to the Azure Function API for prediction.  
3. Prediction determines pump operation.  

### **2. Remote Monitoring via Blynk**  
1. Real-time data is visualized on the Blynk app.  
2. Users can control the pump and receive alerts.  
 
---

## **Conclusion**  

The **Smart Irrigation System** combines IoT, cloud computing, and ML for efficient water management in agriculture. Its real-time analytics and remote control capabilities make it a scalable and sustainable solution for modern farming practices.  

