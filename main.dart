// main.dart

import 'package:flutter/material.dart';
import 'dart:io';
import 'dart:async';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Drone Control',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: DroneControlPage(),
    );
  }
}

class DroneControlPage extends StatefulWidget {
  @override
  _DroneControlPageState createState() => _DroneControlPageState();
}

class _DroneControlPageState extends State<DroneControlPage> {
  Socket? socket;
  String status = 'Not connected';
  double altitude = 0.0;
  bool isFlying = false;
  TextEditingController altitudeController = TextEditingController();

  @override
  void initState() {
    super.initState();
    connectToDrone();
  }

  void connectToDrone() async {
    try {
      socket = await Socket.connect('192.168.1.100', 5000); 
      setState(() {
        status = 'Connected';
      });
      socket!.listen(
        (data) {
          String message = String.fromCharCodes(data).trim();
          List<String> parts = message.split(',');
          for (String part in parts) {
            List<String> keyValue = part.split(':');
            if (keyValue.length == 2) {
              String key = keyValue[0];
              String value = keyValue[1];
              if (key == 'altitude') {
                setState(() {
                  altitude = double.parse(value);
                });
              } else if (key == 'flying') {
                setState(() {
                  isFlying = value.toLowerCase() == 'true';
                });
              }
            }
          }
        },
        onError: (error) {
          print(error);
          setState(() {
            status = 'Error: $error';
          });
        },
        onDone: () {
          print('Disconnected');
          setState(() {
            status = 'Disconnected';
          });
        },
      );
    } catch (e) {
      print('Failed to connect: $e');
      setState(() {
        status = 'Failed to connect: $e';
      });
    }
  }

  void sendCommand(String command) {
    if (socket != null) {
      socket!.write(command);
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Drone Control'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text('Status: $status'),
            SizedBox(height: 20),
            Text('Altitude: ${altitude.toStringAsFixed(2)}m'),
            SizedBox(height: 20),
            ElevatedButton(
              onPressed: () => sendCommand('takeoff'),
              child: Text('Take Off'),
            ),
            SizedBox(height: 10),
            ElevatedButton(
              onPressed: () => sendCommand('land'),
              child: Text('Land'),
            ),
            SizedBox(height: 20),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                SizedBox(
                  width: 100,
                  child: TextField(
                    controller: altitudeController,
                    keyboardType: TextInputType.number,
                    decoration: InputDecoration(
                      labelText: 'Altitude',
                    ),
                  ),
                ),
                SizedBox(width: 10),
                ElevatedButton(
                  onPressed: () {
                    double newAltitude = double.tryParse(altitudeController.text) ?? 0;
                    sendCommand('altitude $newAltitude');
                  },
                  child: Text('Set Altitude'),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }

  @override
  void dispose() {
    socket?.destroy();
    super.dispose();
  }
}
