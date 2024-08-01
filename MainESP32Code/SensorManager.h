#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
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
    void sendToBlynk(); // Ensure this is declared

private:
    float calculatePHValue();
    int getRed();
    int getGreen();
    int getBlue();
    float calibration_value;
    bool menuDisplayed;
    static const int mq4Pin = 33;
    static const int phSensorPin = 35;
    static const int S0 = 18;
    static const int S1 = 19;
    static const int S2 = 16;
    static const int S3 = 17;
    static const int sensorOut = 25;
};

#endif // SENSOR_MANAGER_H
