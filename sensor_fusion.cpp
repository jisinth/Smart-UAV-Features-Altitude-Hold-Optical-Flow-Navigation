#include "sensor_fusion.h"
#include <Eigen.h>

KalmanFilter SensorFusion::kf;

void SensorFusion::init() {
    // Initialize Kalman filter matrices
    kf.P = Matrix<float, 6, 6>::Identity() * 0.1;
    kf.Q = Matrix<float, 6, 6>::Identity() * 0.1;
    kf.R = Matrix<float, 3, 3>::Identity() * 0.1;
}

ProcessedData SensorFusion::process(const SensorData& data) {
    // Predict step
    kf.predict();
    
    // Update step with sensor measurements
    Measurement z;
    z << data.altitude,
         data.optical_flow_x,
         data.optical_flow_y;
    
    kf.update(z);
    
    // Return processed data
    ProcessedData output;
    output.position = kf.getState().segment<3>(0);
    output.velocity = kf.getState().segment<3>(3);
    
    return output;
}
