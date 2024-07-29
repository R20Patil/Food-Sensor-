#include <ArduinoHttpClient.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "UltrasonicSensor.h"

// WiFi credentials
const char* ssid = "sid";
const char* password = "hellohello";

// ThingSpeak API settings
const char* apiWriteKey = "9R46YJL87HYE6HEK";
const char* thingSpeakChannel = "2599999";

// Create a WiFiClient object
WiFiClient wifiClient;

// Create an HttpClient object using the WiFiClient object
HttpClient client = HttpClient(wifiClient, "api.thingspeak.com", 80);

// Create an UltrasonicSensor object
UltrasonicSensor ultrasonicSensor(23, 22);

void setup() {
  Serial.begin(115200);  // Initialize serial communication for debugging
  delay(1000);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi..");
    delay(500);
  }
  Serial.println("Connected to the WiFi network");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up the ultrasonic sensor
  ultrasonicSensor.setup();
}

void loop() {
  // Measure the distance using the ultrasonic sensor
  ultrasonicSensor.measureDistance();
  float distance = ultrasonicSensor.getDistance();
  
 
  // Read other sensor values (replace with your sensor reading code)
  float phsensor = distance;
  char* color = readColorSensor();
  float turbidity = readTurbiditySensor();
  float methane = readMethaneSensor();
  float alcohol = readAlcSensor();

  // Prepare the data string
  String dataString = "api_key=" + String(apiWriteKey) + "&field1=" + String(phsensor) + "&field2=" + String(color) + "&field3=" + String(turbidity) + "&field4=" + String(methane) + "&field5=" + String(alcohol) + "&field6=" + String(distance);

  // Send HTTP POST request
  client.beginRequest();
  client.post("/update");
  client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
  client.sendHeader("Content-Length", dataString.length());
  client.beginBody();
  client.print(dataString);
  client.endRequest();

  // Check for a successful response
  int statusCode = client.responseStatusCode();
  if (statusCode == 200) {
    Serial.println("Data sent successfully!");
  } else {
    Serial.print("Error sending data: ");
    Serial.println(statusCode);
  }

  delay(5000);  // Delay between uploads (adjust as needed)
}

// Replace these functions with your actual sensor reading code
float readPHSensor() {
  // Code to read pH sensor and return value
  return 7.0;  // Example value
}

char* readColorSensor() {
  // Code to read color sensor and return value
  return "red";  // Example value
}

float readMethaneSensor(){
  return 554.0;
}

float readAlcSensor(){
  return rand();
}

float readTurbiditySensor(){
  return rand();
}
