#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

#include <Arduino.h>

#define SOUND_SPEED 0.034
#define TRIG_PULSE_DURATION_US 10

class UltrasonicSensor {
private:
    int trigPin;
    int echoPin;
    long ultrasonDuration;
    float distanceCm;

public:
    UltrasonicSensor(int trigPin, int echoPin);
    void setup();
    void measureDistance();
    float getDistance();
};

#endif // ULTRASONICSENSOR_H
