#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define DHTPIN 2       // DHT11 connected to digital pin 2
#define DHTTYPE DHT11  // DHT 11 sensor
DHT dht(DHTPIN, DHTTYPE);

char auth[] = "YourAuthToken";  // Blynk auth token
char ssid[] = "YourWiFiSSID";   // Wi-Fi SSID
char pass[] = "YourWiFiPassword"; // Wi-Fi password

int lightPin = 7;  // Light control pin
int fanPin = 8;    // Fan control pin

BlynkTimer timer;

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);
  
  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);

  // Initialize the DHT sensor
  dht.begin();
  
  // Set relay pins as output
  pinMode(lightPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  
  // Set up temperature reading function to be called every 5 seconds
  timer.setInterval(5000L, sendSensorData);
}

void loop() {
  Blynk.run();   // Run the Blynk application
  timer.run();   // Run the timer
}

// Virtual pin V1 for light control
BLYNK_WRITE(V1) {
  int lightState = param.asInt();  // Get the state of the button
  digitalWrite(lightPin, lightState);  // Turn the light on/off
}

// Virtual pin V2 for fan control
BLYNK_WRITE(V2) {
  int fanState = param.asInt();  // Get the state of the button
  digitalWrite(fanPin, fanState);  // Turn the fan on/off
}

// Send temperature and humidity data to Blynk app
void sendSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Send data to Blynk virtual pins
  Blynk.virtualWrite(V3, temperature);
  Blynk.virtualWrite(V4, humidity);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
}
