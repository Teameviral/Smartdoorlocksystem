// Made by Avishek Bhattacharjee

#include <SoftwareSerial.h>

SoftwareSerial bluetooth(10, 11); // Bluetooth module connected to pins 10 and 11

String inputString = "";
String command = "";
String value = "";
String password = "write your password here"; // Password for opening and closing the door
boolean stringComplete = false;
boolean authenticated = false;
boolean fingerprintAdded = false;

int motorPin1 = 2; // Pin 2 on L293D IC
int motorPin2 = 3; // Pin 7 on L293D IC
int motorEnablePin = 9; // Pin 1 on L293D IC

void setup() {
  // Start serial connection
  Serial.begin(9600);
  bluetooth.begin(9600);

  inputString.reserve(50);
  command.reserve(50);
  value.reserve(50);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorEnablePin, OUTPUT);

  // Initialize Bluetooth connection
  waitForConnection();
}

void loop() {
  if (stringComplete) {
    command = inputString.substring(0, inputString.indexOf('='));
    value = inputString.substring(inputString.indexOf('=') + 1, inputString.length() - 1);
    inputString = "";
    stringComplete = false;

    if (command == "PASSWORD") {
      if (authenticated) {
        sendBluetoothMessage("ALREADY_AUTHENTICATED");
      } else if (password == value) {
        authenticated = true;
        sendBluetoothMessage("AUTHENTICATION_SUCCESS");
        sendBluetoothMessage("ADD_FINGERPRINT?");
      } else {
        sendBluetoothMessage("AUTHENTICATION_FAILURE");
      }
    } else if (command == "FINGERPRINT") {
      if (!authenticated) {
        sendBluetoothMessage("NOT_AUTHENTICATED");
      } else if (value == "ADD") {
        if (fingerprintAdded) {
          sendBluetoothMessage("FINGERPRINT_ALREADY_ADDED");
        } else {
          fingerprintAdded = true;
          sendBluetoothMessage("FINGERPRINT_ADDED");
          sendBluetoothMessage("OPEN_DOOR?");
        }
      } else if (value == "REMOVE") {
        if (fingerprintAdded) {
          fingerprintAdded = false;
          sendBluetoothMessage("FINGERPRINT_REMOVED");
        } else {
          sendBluetoothMessage("NO_FINGERPRINT_ADDED");
        }
      }
    } else if (command == "OPEN") {
      if (!authenticated) {
        sendBluetoothMessage("NOT_AUTHENTICATED");
      } else if (!fingerprintAdded) {
        sendBluetoothMessage("NO_FINGERPRINT_ADDED");
      } else if (password == value) {
        sendBluetoothMessage("ACCESS_GRANTED");
        openDoor();
      } else {
        sendBluetoothMessage("ACCESS_DENIED");
      }
    } else if (command == "CLOSE") {
      if (!authenticated) {
        sendBluetoothMessage("NOT_AUTHENTICATED");
      } else if (password == value) {
        sendBluetoothMessage("DOOR_CLOSED");
        closeDoor();
      } else {
        sendBluetoothMessage("ACCESS_DENIED");
      }
    }
  }
}

void sendBluetoothMessage(String message) {
  bluetooth.println(message);
}

void waitForConnection() {
  while (!bluetooth.available()) {
    delay(100);
  }
  bluetooth.println("CONNECTED");
}

void serialEvent() {
  while (bluetooth.available()) {
    char inChar = (char)bluetooth.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void openDoor() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorEnablePin, HIGH);
  delay(375);
  digitalWrite(motorEnablePin, LOW);
  delay(500);
  digitalWrite(motorEnablePin, HIGH);
  delay(1000);
  digitalWrite(motorEnablePin, LOW);
}

void closeDoor() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorEnablePin, HIGH);
  delay(375);
  digitalWrite(motorEnablePin, LOW);
}
