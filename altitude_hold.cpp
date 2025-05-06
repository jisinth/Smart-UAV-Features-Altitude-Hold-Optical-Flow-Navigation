#include "altitude_hold.h"

AltitudeHold::AltitudeHold(float target_altitude) 
    : pid(1.8, 0.2, 0.4), target(target_altitude) {
}

float AltitudeHold::update(float current_altitude) {
    float error = target - current_altitude;
    float output = pid.compute(error);
    
    // Limit output to safe range
    output = constrain(output, -MAX_THRUST_CHANGE, MAX_THRUST_CHANGE);
    
    return base_thrust + output;
}
