# Smart-Irrigation-System

Great work completing the project! Here's how you can structure the **README** for the **Cloud ML** part of your project in the subfolder. I'll outline the essential sections for clarity:

---

## Cloud ML Deployment

The folder smart-irrigation-predict contains the details for deploying the ML model in Azure, setting up an Azure Function App, and integrating it with your project.

### Overview

The Cloud ML setup enables the IoT project to utilize a machine learning model for predicting pump activation based on sensor data. The prediction is served via an Azure Function App, which is exposed as an API endpoint.

### Features
- **Deployment**: ML model deployed as an Azure Function App.
- **Integration**: API consumed by the ESP32 to control the pump based on predictions.
- **Scalability**: Hosted on Azure to support real-time data processing and predictions.

### Setup Instructions

#### 1. Prerequisites
- An **Azure account**.
- **Azure CLI** installed.
- A trained ML model saved as `model.pkl` and a scaler saved as `scaler.pkl`.
- Python installed locally for development.

#### 2. Steps to Deploy

##### a. Create an Azure Function App
1. **Log in to Azure:**
   ```bash
   az login
   ```
2. **Create a Resource Group:**
   ```bash
   az group create --name IoTProjectGroup --location eastus
   ```
3. **Create a Function App:**
   ```bash
   az functionapp create --resource-group IoTProjectGroup --consumption-plan-location eastus --runtime python --runtime-version 3.9 --functions-version 4 --name IoTProjectFunction --storage-account <STORAGE_ACCOUNT_NAME>
   ```

##### b. Deploy the Function App
1. **Clone the repository:**
   ```bash
   git clone https://github.com/taimoorsardar/Smart-Irrigation-System.git
   cd Smart-Irrigation-System.git/smart-irrigation-predict-cloud
   ```
2. **Install dependencies:**
   Navigate to the `smart-irrigation-predict-cloud` folder:
   ```bash
   cd cloud_ml
   pip install -r requirements.txt
   ```
3. **Add model files:**
   Ensure the `model.pkl` and `scaler.pkl` are in the `smart-irrigation-predict-cloud` folder.
4. **Deploy the function:**
   ```bash
   func azure functionapp publish IoTProjectFunction
   ```

##### c. Test the API Endpoint
- Use tools like **Postman** or **cURL** to send sample data to the API:
  ```bash
  curl -X POST -H "Content-Type: application/json" -d '{"moisture": 50, "humidity": 65, "rain": "no"}' <function_app_url>/api/predict
  ```

##### d. Integrate with ESP32
- Use the API URL in your ESP32 Arduino code to send data to the ML model.

#### 3. File Structure
```
cloud_ml/
│
├── host.json                 # Azure function initialization
├── local.settings.json                 # Azure function initialization
├── requirements.txt            # Python dependencies
├── function_app.py             # Main function app code
├── model.pkl                   # Trained ML model
└── scaler.pkl                  # Scaler for input data preprocessing
```

#### 4. Notes
- Replace `<STORAGE_ACCOUNT_NAME>` and other placeholders with actual values.
- Ensure the Azure Function App has internet access to interact with IoT devices.
