#define BLYNK_TEMPLATE_ID "TMPL3zzs8dRxb"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "J5sf8xbUSsCBJ6Ron8pnFReQvb04G_F8"

#include "SensorManager.h"
#include <ColorName.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <Wire.h>
#include <EEPROM.h>

const char* ssid = "ampm";
const char* password = "29122811";
char auth[] = "J5sf8xbUSsCBJ6Ron8pnFReQvb04G_F8"; // Replace with your Blynk Authentication Token

BlynkTimer timerr;

SensorManager::SensorManager()
    : calibration_value(20.24 - 0.7), menuDisplayed(false) {}

void SensorManager::begin() {
    Serial.begin(9600);  // Initialize serial communication for debugging
    delay(1000);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connecting to WiFi..");
        delay(500);
    }
    Serial.println("Connected to the WiFi network");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Blynk.begin(auth, ssid, password);

    // Initialize MQ-4 sensor pin
    pinMode(mq4Pin, INPUT);

    // Initialize pH sensor pin
    pinMode(phSensorPin, INPUT);

    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    // Setting frequency-scaling to 20%
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    // Set up a function to be called every second
    timerr.setInterval(1000L, [this]() { this->sendToBlynk(); });
}

void SensorManager::readSensors() {
    float phsensor = readPHSensor();
    const char* color = readColorSensor();
    float turbidity = readTurbiditySensor();
    float methane = readMethaneSensor();
    float alcohol = readAlcSensor();

    Serial.print("pH Sensor: ");
    Serial.println(phsensor);
    Serial.print("Color Sensor: ");
    Serial.println(color);
    Serial.print("Turbidity Sensor: ");
    Serial.println(turbidity);
    Serial.print("Methane Sensor: ");
    Serial.println(methane);
    Serial.print("Alcohol Sensor: ");
    Serial.println(alcohol);

    delay(100);  // Small delay to prevent continuous polling
}

void SensorManager::sendToBlynk() {
    float phsensor = readPHSensor();
    const char* color = readColorSensor();
    float turbidity = readTurbiditySensor();
    float methane = readMethaneSensor();
    float alcohol = readAlcSensor();

    Serial.print("pH Sensor: ");
    Serial.println(phsensor);
    Serial.print("Color Sensor: ");
    Serial.println(color);
    Serial.print("Turbidity Sensor: ");
    Serial.println(turbidity);
    Serial.print("Methane Sensor: ");
    Serial.println(methane);
    Serial.print("Alcohol Sensor: ");
    Serial.println(alcohol);

    // Send data to Blynk
    Blynk.virtualWrite(V1, phsensor);
    Blynk.virtualWrite(V3, color);
    Blynk.virtualWrite(V0, turbidity);
    Blynk.virtualWrite(V32, methane);
    Blynk.virtualWrite(V2, alcohol);

    Serial.println("Data sent to Blynk!");
}

float SensorManager::readPHSensor() {
    return calculatePHValue();
}

const char* SensorManager::readColorSensor() {
    int red = getRed();
    int redValue = map(red, 280, 30, 255, 0); // Map the Red Color Value
    delay(200);

    int green = getGreen();
    int greenValue = map(green, 330, 30, 255, 0); // Map the Green Color Value
    delay(200);

    int blue = getBlue();
    int blueValue = map(blue, 320, 30, 255, 0); // Map the Blue Color Value
    delay(200);

    Serial.print("Red = ");
    Serial.print(red);
    Serial.print("    ");
    Serial.print("Green = ");
    Serial.print(green);
    Serial.print("    ");
    Serial.print("Blue = ");
    Serial.println(blue);

    return ColorNameString(redValue, greenValue, blueValue);
}

float SensorManager::readTurbiditySensor() {
    int sensor = 0; // variable for averaging
    int n = 25; // number of samples to average
    int sensorValue = 0;
    float voltage = 0.00;
    float turbidity = 0.00;
    float Vclear = 3.3; 
    EEPROM.get(0, Vclear);

    for (int i = 0; i < n; i++) {
        sensor += analogRead(39); // read the input on analog pin 3 (turbidity sensor analog output)
        delay(10);
    }
    sensorValue = sensor / n; // average the n values
    voltage = sensorValue * (3.300 / 4095.000); // Convert analog (0-4095) to voltage (0 - 3.3V)

    turbidity = 20.00 * (100.00 - (voltage / Vclear) * 100.00); // as relative percentage of 2000 NTU; 0% = clear water; // Factor 20 at the front instead of 100 to limit the result to 2000

    // Serial display
    Serial.print("Raw voltage: ");
    Serial.print(voltage, 3);
    Serial.print(" V, Turbidity: ");
    Serial.print(turbidity, 3);
    Serial.println(" NTU");
    Serial.println(sensorValue);

    sensor = 0; // resets for averaging

    return sensorValue;
}

float SensorManager::readMethaneSensor() {
    int sensorValue = analogRead(mq4Pin);
    
    return sensorValue;  // Use the MQ-4 sensor value
}

float SensorManager::readAlcSensor() {
    float sensor_volt;
    float RS_gas; // Get value of RS in a GAS
    float ratio; // Get ratio RS_GAS/RS_air
    int sensorValue = analogRead(32);
    sensor_volt = (float)sensorValue/ 4095 * 5.0;
    RS_gas = (5.0 - sensor_volt) / sensor_volt; // omit *RL

    ratio = RS_gas / 0.03;  // ratio = RS/R0

    return sensorValue;
}

float SensorManager::calculatePHValue() {
    int buffer[10];
    for (int i = 0; i < 10; i++) {
        buffer[i] = analogRead(phSensorPin);
        delay(30);
    }

    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 10; j++) {
            if (buffer[i] > buffer[j]) {
                int temp = buffer[i];
                buffer[i] = buffer[j];
                buffer[j] = temp;
            }
        }
    }

    unsigned long int avgval = 0;
    for (int i = 2; i < 8; i++) {
        avgval += buffer[i];
    }

    float volt = (float)avgval * 3.3 / 4096.0 / 6;
    return -5.70 * volt + calibration_value;
}

int SensorManager::getRed() {
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    return pulseIn(sensorOut, LOW);
}

int SensorManager::getGreen() {
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    return pulseIn(sensorOut, LOW);
}

int SensorManager::getBlue() {
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    return pulseIn(sensorOut, LOW);
}

SensorManager sensorManager;

void setup() {
    sensorManager.begin();
}

void loop() {
    sensorManager.sendToBlynk();
    delay(1000);
}
