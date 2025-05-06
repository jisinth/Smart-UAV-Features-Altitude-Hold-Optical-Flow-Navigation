#include "barometric.h"

MS5611 BarometricSensor::ms5611;
float BarometricSensor::referenceAltitude = 0;

bool BarometricSensor::init() {
    if (!ms5611.begin()) {
        return false;
    }
    
    calibrate();
    return true;
}

void BarometricSensor::calibrate() {
    float sum = 0;
    const int samples = 100;
    
    for (int i = 0; i < samples; i++) {
        sum += ms5611.getAltitude();
        delay(10);
    }
    
    referenceAltitude = sum / samples;
}

float BarometricSensor::getAltitude() {
    return ms5611.getAltitude() - referenceAltitude;
}
