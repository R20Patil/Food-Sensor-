#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(int trigPin, int echoPin) {
    this->trigPin = trigPin;
    this->echoPin = echoPin;
    this->ultrasonDuration = 0;
    this->distanceCm = 0.0;
}

void UltrasonicSensor::setup() {
    pinMode(trigPin, OUTPUT); // Configure the trig pin as output
    pinMode(echoPin, INPUT);  // Configure the echo pin as input
}

void UltrasonicSensor::measureDistance() {
    // Set up the signal
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Create a 10 µs impulse
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(TRIG_PULSE_DURATION_US);
    digitalWrite(trigPin, LOW);

    // Return the wave propagation time (in µs)
    ultrasonDuration = pulseIn(echoPin, HIGH);

    // Distance calculation
    distanceCm = (ultrasonDuration * SOUND_SPEED) / 2;
}

float UltrasonicSensor::getDistance() {
    return distanceCm;
}
