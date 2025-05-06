#ifndef OPTICAL_FLOW_H
#define OPTICAL_FLOW_H

#include <SPI.h>
#include "PMW3901.h"

class OpticalFlow {
public:
    static bool init();
    static void readMotion(float &delta_x, float &delta_y);
    
private:
    static PMW3901 flow;
    static void processRawData(int16_t raw_x, int16_t raw_y, 
                             float &delta_x, float &delta_y);
};

#endif
