import azure.functions as func
import datetime
import json
import logging
import pickle
import numpy as np
from sklearn.preprocessing import StandardScaler
import os
app = func.FunctionApp()

# Load the trained model and scaler
with open('model.pkl', 'rb') as model_file:
    classifier = pickle.load(model_file)

with open('scaler.pkl', 'rb') as scaler_file:
    scaler = pickle.load(scaler_file)

@app.route(route="predict", auth_level=func.AuthLevel.ANONYMOUS)
def predict(req: func.HttpRequest) -> func.HttpResponse:
    logging.info('Python HTTP trigger function processed a request.')

    try:
        # Parse JSON input
        req_body = req.get_json()
        moisture = float(req_body.get('moisture', 0))
        humidity = float(req_body.get('humidity', 0))
        rain = req_body.get('rain', '').strip().lower()

        if rain not in ['yes', 'no']:
            return func.HttpResponse(
                json.dumps({"error": "Invalid rain value. Use 'yes' or 'no'."}),
                mimetype="application/json",
                status_code=400
            )

        # Encode rain as 1 for 'yes' and 0 for 'no'
        rain_encoded = 1 if rain == 'yes' else 0

        # Prepare data for the model
        input_data = np.array([[rain_encoded, moisture, humidity]])
        scaled_data = scaler.transform(input_data)

        # Make prediction
        prediction = classifier.predict(scaled_data)

        # Map prediction to output
        result = "Pump ON" if prediction[0] == 1 else "Pump OFF"

        # Return the result as JSON
        return func.HttpResponse(
            json.dumps({
                "moisture": moisture,
                "humidity": humidity,
                "rain": rain,
                "prediction": result
            }),
            mimetype="application/json",
            status_code=200
        )

    except Exception as e:
        logging.error(f"Error processing the request: {e}")
        return func.HttpResponse(
            json.dumps({"error": str(e)}),
            mimetype="application/json",
            status_code=500
        )
