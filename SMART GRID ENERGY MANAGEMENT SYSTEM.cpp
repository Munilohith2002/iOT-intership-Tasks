#include <ESP8266WiFi.h>
#include <PubSubClient.h>   // For MQTT
#include <LiquidCrystal.h>  // For LCD display
#include "EmonLib.h"        // For current sensor library (SCT-013)

// Initialize Wi-Fi connection
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";
const char* mqtt_server = "Your_MQTT_Broker_IP";

// Create an instance of the current sensor library
EnergyMonitor emon1;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Initialize LCD pins

WiFiClient espClient;
PubSubClient client(espClient);
#define relayPin 7       // Relay pin to control the load

// Function to connect to Wi-Fi
void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

// Function to send data to MQTT broker
void sendToMQTT(float powerConsumption, float solarPower, float voltage) {
  String powerString = String(powerConsumption);
  String solarString = String(solarPower);
  String voltageString = String(voltage);
  
  client.publish("smartgrid/power", powerString.c_str());
  client.publish("smartgrid/solar", solarString.c_str());
  client.publish("smartgrid/voltage", voltageString.c_str());
}

// Setup
void setup() {
  // Start serial communication
  Serial.begin(115200);
  // Start LCD display
  lcd.begin(16, 2);
  // Initialize the relay pin as output
  pinMode(relayPin, OUTPUT);
  // Initialize current sensor
  emon1.current(1, 111.1); // Current sensor input (pin A1)

  // Connect to Wi-Fi
  setup_wifi();
  
  // Set MQTT server
  client.setServer(mqtt_server, 1883);
}

// Loop
void loop() {
  // Read current and calculate power consumption
  double Irms = emon1.calcIrms(1480);
  float powerConsumption = Irms * 230.0; // Assuming voltage is 230V
  
  // Simulate solar power (use LDR and actual solar data in real implementation)
  float solarPower = analogRead(A0); // Replace with solar panel sensor
  
  // Simulate grid voltage (replace with voltage sensor readings)
  float voltage = 230; // Placeholder for actual voltage data
  
  // Print to Serial
  Serial.print("Power: ");
  Serial.print(powerConsumption);
  Serial.println("W");

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power: ");
  lcd.print(powerConsumption);
  lcd.print("W");

  // Send data to MQTT broker
  sendToMQTT(powerConsumption, solarPower, voltage);

  // Simple demand-response control (disconnect load if demand > threshold)
  if (powerConsumption > 1000) { // Threshold in watts
    digitalWrite(relayPin, LOW);  // Disconnect load
  } else {
    digitalWrite(relayPin, HIGH); // Keep load connected
  }
  
  delay(2000);  // Wait for 2 seconds before the next reading
}
