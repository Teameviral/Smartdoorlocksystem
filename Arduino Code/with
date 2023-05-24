#include <Adafruit_Fingerprint.h> // Library for fingerprint sensor

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1); // Create an instance of the fingerprint sensor

String inputString = "";
String command = "";
String value = "";
String gotstatus = "";
String password = "write your password here"; // This is the password for opening and closing your door.
                                              // You can set any password you like using digits and symbols.
boolean stringComplete = false;

int motorPin1 = 10; // Pin 2 on L293D IC
int motorPin2 = 11; // Pin 7 on L293D IC
int motorEnablePin = 9; // Pin 1 on L293D IC
int Speed = 10;

// Data structure to store access log history
struct AccessLog {
  String timeStamp;
  String status;
};

const int maxAccessLogs = 10; // Maximum number of access logs to store
AccessLog accessLogs[maxAccessLogs]; // Array to store access logs
int accessLogCount = 0; // Counter for the number of access logs

void setup() {
  // Start serial connections
  Serial.begin(9600);  // Baud rate is 9600, must match with Bluetooth
  Serial1.begin(57600); // Baud rate for fingerprint sensor, adjust according to your sensor
  
  inputString.reserve(50);  // Reserve 50 bytes in memory for string manipulation 
  command.reserve(50);
  value.reserve(50);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorEnablePin, OUTPUT);
  pinMode(2, INPUT_PULLUP);

  // Initialize fingerprint sensor
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor found!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  
  finger.getTemplateCount();
  Serial.print("Sensor contains "); 
  Serial.print(finger.templateCount); 
  Serial.println(" templates");
}

void loop() {
  if (stringComplete) {
    int sensorVal = digitalRead(2);
    delay(100);

    if (inputString == "GETSTATUS\n") {
      if (sensorVal == HIGH) {
        gotstatus = "UNLOCKED";
        Serial.println("UNLOCKED");
      }
      if (sensorVal == LOW) {
        gotstatus = "LOCKED";
        Serial.println("LOCKED");
      }
    }

    int pos = inputString.indexOf('=');

    if (pos > -1) {
      command = inputString.substring(0, pos);
      value = inputString.substring(pos + 1, inputString.length() - 1);

      if (!password.compareTo(value) && (command == "OPEN") && digitalRead(2) == LOW) {
        if (authenticateFingerprint()) {
          openDoor();
          logAccess("UNLOCKED");
          Serial.println("OPENING");
          delay(100);
        } else {
          Serial.println("Fingerprint authentication failed");
          delay(100);
        }
      } else if (!password.compareTo(value) && (command == "CLOSE") && digitalRead(2) == HIGH) {
        if (authenticateFingerprint()) {
          closeDoor();
          logAccess("LOCKED");
          Serial.println("CLOSING");
          delay(100);
        } else {
          Serial.println("Fingerprint authentication failed");
          delay(100);
        }
      } else if (password.compareTo(value)) {
        Serial.println("Wrong password");
              delay(100);
      }
    }

    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;

    if (inChar == '\n' || inChar == '\r') {
      stringComplete = true;
    }
  }
}

void openDoor() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorEnablePin, HIGH);
  delay(375);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}

void closeDoor() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorEnablePin, HIGH);
  delay(375);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}

bool authenticateFingerprint() {
  Serial.println("Place your finger on the sensor");
  
  int fingerprintID = finger.getImage();
  
  if (fingerprintID == FINGERPRINT_OK) {
    Serial.println("Image taken");
    fingerprintID = finger.image2Tz();
    
    if (fingerprintID == FINGERPRINT_OK) {
      Serial.println("Remove your finger");
      delay(2000);
      Serial.println("Place the same finger again");
      
      fingerprintID = finger.getImage();
      
      if (fingerprintID == FINGERPRINT_OK) {
        Serial.println("Image taken");
        fingerprintID = finger.image2Tz(1);
        
        if (fingerprintID == FINGERPRINT_OK) {
          Serial.println("Fingerprint captured");
          fingerprintID = finger.fingerFastSearch();
          
          if (fingerprintID == FINGERPRINT_OK) {
            Serial.println("Fingerprint matched");
            return true;
          } else {
            Serial.println("Fingerprint not found");
          }
        } else {
          Serial.println("Failed to convert image to template");
        }
      } else {
        Serial.println("Failed to take second finger image");
      }
    } else {
      Serial.println("Failed to convert image to template");
    }
  } else {
    Serial.println("Failed to take finger image");
  }
  
  return false;
}

void logAccess(String status) {
  // Get current timestamp
  String timeStamp = getTimeStamp();
  
  // Create new access log entry
  AccessLog newLog;
  newLog.timeStamp = timeStamp;
  newLog.status = status;
  
  // Add the new log entry to the access logs array
  accessLogs[accessLogCount] = newLog;
  
  // Increment access log count
  accessLogCount++;
  
  // If the maximum number of logs is reached, start overwriting the oldest logs
  if (accessLogCount >= maxAccessLogs) {
    accessLogCount = 0;
  }
  
  // Print the access log entry
  Serial.print("Access Log - ");
  Serial.print("Timestamp: ");
  Serial.print(newLog.timeStamp);
  Serial.print(", Status: ");
  Serial.println(newLog.status);
}

String getTimeStamp() {
  // Get current time and date
  // Adjust according to your time zone and RTC library
  
  // Example using built-in millis() function
  unsigned long currentTime = millis();
  int seconds = currentTime / 1000;
  int minutes = seconds / 60;
  int hours = minutes / 60;
  int days = hours / 24;
  
  String timeStamp = String(days) + "d " + String(hours % 24) + "h " + String(minutes % 60) + "m " + String(seconds % 60) + "s";
  return timeStamp;
}

void setup() {
  // Setup code goes here
}

void loop() {
  // Loop code goes here
}

