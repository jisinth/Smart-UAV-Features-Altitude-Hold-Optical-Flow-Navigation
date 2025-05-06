#ifndef BAROMETRIC_H
#define BAROMETRIC_H

#include <Wire.h>
#include "MS5611.h"

class BarometricSensor {
public:
    static bool init();
    static float getAltitude();
    static float getPressure();
    
private:
    static MS5611 ms5611;
    static float referenceAltitude;
    static void calibrate();
};

#endif
