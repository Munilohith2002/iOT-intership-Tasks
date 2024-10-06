#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>
#include <WiFi.h>  // For ESP32
#include <BlynkSimpleEsp32.h>  // If using Blynk as a dashboard

// Replace with your network credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Blynk authentication token
char auth[] = "Your_Blynk_Auth_Token";

#define DHTPIN 2      // Digital pin connected to the DHT22
#define DHTTYPE DHT22 // DHT22 (AM2302)

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// BMP280 sensor setup
Adafruit_BMP280 bmp;

void setup() {
  Serial.begin(115200);
  
  // Initialize the DHT sensor
  dht.begin();
  
  // Initialize BMP280 sensor
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  // Initialize Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Blynk
  Blynk.begin(auth, ssid, password);
}

void loop() {
  // Read data from sensors
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float pressure = bmp.readPressure() / 100.0F; // Convert to hPa

  // Check if any readings failed
  if (isnan(temperature) || isnan(humidity) || isnan(pressure)) {
    Serial.println("Failed to read from sensors");
    return;
  }

  // Print the data to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  // Send the data to Blynk app (or other platforms)
  Blynk.virtualWrite(V1, temperature);  // Display temperature on Blynk
  Blynk.virtualWrite(V2, humidity);     // Display humidity on Blynk
  Blynk.virtualWrite(V3, pressure);     // Display pressure on Blynk

  delay(2000); // Wait for 2 seconds before next reading
}
