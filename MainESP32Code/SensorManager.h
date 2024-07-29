#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <IRremote.h>
#include <Wire.h>
#include <BlynkSimpleEsp32.h>

class SensorManager {
public:
    SensorManager();
    void begin();
    void loop();
    void readSensors();
    float readPHSensor();
    const char* readColorSensor();
    float readTurbiditySensor();
    float readMethaneSensor();
    float readAlcSensor();
    void displayMenu();
   
int handleRemoteInput(unsigned long code);
int codeToDigit(unsigned long code);
    void sendToBlynk(); // Ensure this is declared

private:
    float calculatePHValue();
    int getRed();
    int getGreen();
    int getBlue();
    IRrecv irrecv;
    decode_results results;
    float calibration_value;
    bool menuDisplayed;
    int firstDigit;
    int secondDigit;
    int buffer_arr[10];
    int readIRInput();
    void handleRemote();
    static const int RECV_PIN = 15;
    static const int mq4Pin = 32;
    static const int phSensorPin = 34;
    static const int S0 = 19;
    static const int S1 = 18;
    static const int S2 = 5;
    static const int S3 = 17;
    static const int sensorOut = 4;
};

#endif // SENSOR_MANAGER_H
