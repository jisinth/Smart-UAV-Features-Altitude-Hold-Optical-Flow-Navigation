#ifndef PID_H
#define PID_H

class PIDController {
public:
    PIDController(float kp, float ki, float kd);
    float compute(float setpoint, float measured_value);
    void reset();
    
private:
    float kp, ki, kd;
    float previous_error;
    float integral;
    unsigned long last_time;
};

#endif
