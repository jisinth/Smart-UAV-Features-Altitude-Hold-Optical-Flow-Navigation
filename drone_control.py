# drone_control.py

import socket
import threading
import time

class DroneControl:
    def __init__(self, host='0.0.0.0', port=5000):
        self.host = host
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.bind((self.host, self.port))
        self.socket.listen(1)
        self.client = None
        self.altitude = 0
        self.is_flying = False

    def start(self):
        print(f"Drone control server started on {self.host}:{self.port}")
        self.accept_connections()

    def accept_connections(self):
        while True:
            client, address = self.socket.accept()
            print(f"New connection from {address}")
            self.client = client
            threading.Thread(target=self.handle_client, args=(client,)).start()

    def handle_client(self, client):
        while True:
            try:
                data = client.recv(1024).decode('utf-8')
                if not data:
                    break
                self.process_command(data)
            except Exception as e:
                print(f"Error handling client: {e}")
                break
        client.close()

    def process_command(self, command):
        if command == 'takeoff':
            self.takeoff()
        elif command == 'land':
            self.land()
        elif command.startswith('altitude'):
            _, altitude = command.split()
            self.set_altitude(float(altitude))
        # Add more commands as needed

    def takeoff(self):
        if not self.is_flying:
            print("Taking off...")
            self.is_flying = True
            self.altitude = 1.0  # Default takeoff altitude

    def land(self):
        if self.is_flying:
            print("Landing...")
            self.is_flying = False
            self.altitude = 0

    def set_altitude(self, altitude):
        if self.is_flying:
            print(f"Setting altitude to {altitude}m")
            self.altitude = altitude

    def send_telemetry(self):
        while True:
            if self.client:
                telemetry = f"altitude:{self.altitude:.2f},flying:{self.is_flying}"
                self.client.send(telemetry.encode('utf-8'))
            time.sleep(1)

if __name__ == '__main__':
    drone = DroneControl()
    telemetry_thread = threading.Thread(target=drone.send_telemetry)
    telemetry_thread.start()
    drone.start()
