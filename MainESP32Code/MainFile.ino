#define BLYNK_TEMPLATE_ID "TMPL3zzs8dRxb"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "J5sf8xbUSsCBJ6Ron8pnFReQvb04G_F8"
#include "SensorManager.h"
#include <ColorName.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <Wire.h>

const char* ssid = "sid";
const char* password = "hellohello";
char auth[] = "J5sf8xbUSsCBJ6Ron8pnFReQvb04G_F8"; // Replace with your Blynk Authentication Token

BlynkTimer timerr;

const unsigned long BUTTON_0 = 0xFF6897;
const unsigned long BUTTON_1 = 0xFF30CF;
const unsigned long BUTTON_2 = 0xFF18E7;
const unsigned long BUTTON_3 = 0xFF7A85;
const unsigned long BUTTON_4 = 0xFF10EF;
const unsigned long BUTTON_5 = 0xFF38C7;
const unsigned long BUTTON_6 = 0xFF5AA5;
const unsigned long BUTTON_7 = 0xFF42BD;
const unsigned long BUTTON_8 = 0xFF4AB5;
const unsigned long BUTTON_9 = 0xFF52AD;

SensorManager::SensorManager()
    : irrecv(RECV_PIN), calibration_value(20.24 - 0.7), menuDisplayed(false), firstDigit(-1), secondDigit(-1) {}

void SensorManager::begin() {
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

    Wire.begin();

    // Initialize IR receiver
    irrecv.enableIRIn();
    Serial.println("IR Receiver Initialized");

    // Set up a function to be called every second
    timerr.setInterval(1000L, [this]() { this->sendToBlynk(); });
}

void SensorManager::readSensors() {
    int IRInput = readIRInput();
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
  //  int IRInput = readIRInput();
    float phsensor = readPHSensor();
    Serial.print("pH Sensor: ");
    const char* color = readColorSensor();
    Serial.print("pH Sensor: ");
    float turbidity = readTurbiditySensor();
 Serial.print("pH Sensor: ");
    float methane = readMethaneSensor();
 Serial.print("pH Sensor: ");
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
    //Blynk.virtualWrite(V6, IRInput);

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
    return rand();
}

float SensorManager::readMethaneSensor() {
    int sensorValue = analogRead(mq4Pin);
    Serial.print("MQ-4 Sensor Value: ");
    Serial.println(sensorValue);
    return sensorValue;  // Use the MQ-4 sensor value
}

float SensorManager::readAlcSensor() {
    float sensor_volt;
    float RS_gas; // Get value of RS in a GAS
    float ratio; // Get ratio RS_GAS/RS_air
    int sensorValue = analogRead(33);
    sensor_volt = (float)sensorValue/ 4095 * 5.0;
    RS_gas = (5.0 - sensor_volt) / sensor_volt; // omit *RL

    ratio = RS_gas / 0.03;  // ratio = RS/R0

    return sensorValue;
}

float SensorManager::calculatePHValue() {
    for (int i = 0; i < 10; i++) {
        buffer_arr[i] = analogRead(phSensorPin);
        delay(30);
    }

    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 10; j++) {
            if (buffer_arr[i] > buffer_arr[j]) {
                int temp = buffer_arr[i];
                buffer_arr[i] = buffer_arr[j];
                buffer_arr[j] = temp;
            }
        }
    }

    unsigned long int avgval = 0;
    for (int i = 2; i < 8; i++) {
        avgval += buffer_arr[i];
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

void SensorManager::displayMenu() {
    Serial.println("Please select a menu option:");
    for (int i = 1; i <= 20; i++) {
        Serial.print(i);
        Serial.print(". Choice ");
        Serial.println(i);
    }
}

int SensorManager::handleRemoteInput(unsigned long code) {
    int digit = codeToDigit(code);
    if (digit == -1) {
        Serial.println("Reenter value");
        digit = codeToDigit(code);
    }
    return digit;
}

int SensorManager::codeToDigit(unsigned long code) {
    switch (code) {
        case BUTTON_0: return 0;
        case BUTTON_1: return 1;
        case BUTTON_2: return 2;
        case BUTTON_3: return 3;
        case BUTTON_4: return 4;
        case BUTTON_5: return 5;
        case BUTTON_6: return 6;
        case BUTTON_7: return 7;
        case BUTTON_8: return 8;
        case BUTTON_9: return 9;
        default: return -1;
    }
}

int SensorManager::readIRInput() {
    displayMenu();
    int IRReadStatus = 0;
    int finalIRValue = 0;

    while (IRReadStatus == 0) {
        if (irrecv.decode(&results)) {
            finalIRValue = handleRemoteInput(results.value);

            if (finalIRValue == -1) {
                IRReadStatus = 0;
                irrecv.resume();
            } else {
                IRReadStatus = 1;
                Serial.print(finalIRValue);
            }
        }
        delay(100);
    }

    Serial.println("Enter part B");
    irrecv.resume();

    while (IRReadStatus == 1) {
        if (irrecv.decode(&results)) {
            if (handleRemoteInput(results.value) == -1) {
                IRReadStatus = 1;
                irrecv.resume();
            } else {
                finalIRValue = finalIRValue * 10 + handleRemoteInput(results.value);
                IRReadStatus = 2;
            }
        }
    }

    irrecv.resume();
    delay(100);
    return finalIRValue;
}

SensorManager sensorManager;

void setup() {
    sensorManager.begin();
}

void loop() {
    sensorManager.sendToBlynk();
    delay(1000);
}
