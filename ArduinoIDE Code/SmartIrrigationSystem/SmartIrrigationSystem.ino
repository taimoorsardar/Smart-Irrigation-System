#define BLYNK_TEMPLATE_ID "TMPL688EvlIjt"
#define BLYNK_TEMPLATE_NAME "Iot Project"
#define BLYNK_AUTH_TOKEN "CWrxFQqRDtFQISLIaRDw03FvZ_D5OHYE"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <ArduinoJson.h> // Include the ArduinoJson library

// WiFi credentials
const char* ssid = "Hamza";
const char* password = "36809911";
LiquidCrystal_I2C lcd(0x27,20,4); 
// Azure server URL
const char* serverName = "https://smart-irrigation-predict.azurewebsites.net/api/predict";

// Sensor pins
const int soilMoisturePin = 34;  // Soil moisture sensor connected to GPIO34
const int humidityPin = 4;       // Humidity sensor connected to GPIO4 (DHT sensor)
const int rainPin = 2;           // Rain sensor connected to GPIO2 (button)

// Fan (LED) pin
const int fanPin = 27;            // LED as a fan connected to GPIO3

// Blynk virtual pins
#define VPIN_SOIL V1
#define VPIN_HUMIDITY V2
#define VPIN_RAIN V3
#define VPIN_FAN V4

#define DHT_TYPE DHT11
DHT dht(humidityPin, DHT_TYPE);

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);
  lcd.begin();                      // initialize the lcd 
  lcd.begin();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" IoT Project ");
  // Initialize pins
  pinMode(rainPin, INPUT_PULLUP);
  pinMode(fanPin, OUTPUT);

  // Initialize DHT sensor
  dht.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {
  // Run Blynk
  Blynk.run();

  // Check WiFi connection before sending API requests
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Read sensor data
    float soilMoistureValue = 4095 - analogRead(soilMoisturePin);
    int rainDetected = digitalRead(rainPin) == LOW ? 1 : 0;
    float humidity = dht.readHumidity(); // Read humidity
    Serial.println(humidity);

    // Send sensor data to Blynk
    Blynk.virtualWrite(VPIN_SOIL, soilMoistureValue);
    Blynk.virtualWrite(VPIN_HUMIDITY, humidity);
    Blynk.virtualWrite(VPIN_RAIN, rainDetected == 1 ? "Yes" : "No");

    // Construct JSON payload using ArduinoJson
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["moisture"] = soilMoistureValue;
    jsonDoc["humidity"] = humidity;
    jsonDoc["rain"] = rainDetected == 1 ? "yes" : "no";

    String jsonData;
    serializeJson(jsonDoc, jsonData); // Serialize JSON object to string

    // Send HTTP POST request to Flask API
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json"); // Explicitly set JSON content type
    int httpResponseCode = http.POST(jsonData);
    Serial.println(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Prediction Response: " + response);

      // Parse the JSON response
      StaticJsonDocument<200> responseDoc;
      DeserializationError error = deserializeJson(responseDoc, response);

      if (!error) {
        // Extract values from JSON response
        const char* prediction = responseDoc["prediction"];
        Serial.println("Prediction: " + String(prediction));
        lcd.setCursor(0,0);
 lcd.print("Moisture=" );
 lcd.print(soilMoistureValue);
  lcd.setCursor(0,1);
 lcd.print("Humidity=");
 lcd.print(humidity);
 delay(3000);
 lcd.clear();
lcd.setCursor(0,0);
 lcd.print("rain:");
 lcd.print(rainDetected);
 lcd.setCursor(0,1);
 lcd.print("Pump:");
 lcd.print(prediction);
        // Control fan based on prediction
        if (String(prediction) == "Pump ON") {
          digitalWrite(fanPin, HIGH); // Turn fan ON
          Blynk.virtualWrite(VPIN_FAN, 1); // Update Blynk fan status
        } else if (String(prediction) == "Pump OFF") {
          digitalWrite(fanPin, LOW); // Turn fan OFF
          Blynk.virtualWrite(VPIN_FAN, 0); // Update Blynk fan status
        }
      } else {
        Serial.println("Failed to parse JSON response: " + String(error.c_str()));
      }
    } else {
      Serial.println("Error in HTTP request to Flask API");
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
 
  // Delay for stability
  delay(500);
}