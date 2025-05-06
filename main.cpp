#include "sensors/barometric.h"
#include "sensors/optical_flow.h"
#include "controllers/pid.h"
#include "controllers/altitude_hold.h"
#include "utils/sensor_fusion.h"

// Constants
#define LOOP_FREQUENCY 500
#define ALTITUDE_TARGET 150 // cm
#define POSITION_THRESHOLD 20 // cm

// Global variables
float current_altitude = 0;
float current_position_x = 0;
float current_position_y = 0;

void setup() {
    // Initialize sensors
    if (!BarometricSensor::init()) {
        Serial.println("Barometric sensor initialization failed!");
        return;
    }
    
    if (!OpticalFlow::init()) {
        Serial.println("Optical flow sensor initialization failed!");
        return;
    }
    
    // Initialize controllers
    PIDController altitude_pid(1.8, 0.2, 0.4);
    PIDController position_pid(1.5, 0.1, 0.3);
    
    // Initialize communication
    Serial.begin(115200);
}

void loop() {
    static unsigned long last_loop_time = 0;
    const unsigned long current_time = millis();
    
    // Ensure consistent loop timing
    if (current_time - last_loop_time < (1000/LOOP_FREQUENCY)) {
        return;
    }
    last_loop_time = current_time;
    
    // Read sensors
    SensorData sensor_data = readSensors();
    
    // Process data through sensor fusion
    ProcessedData processed_data = SensorFusion::process(sensor_data);
    
    // Update control systems
    updateFlightControls(processed_data);
    
    // Send telemetry
    sendTelemetry(processed_data);
}
