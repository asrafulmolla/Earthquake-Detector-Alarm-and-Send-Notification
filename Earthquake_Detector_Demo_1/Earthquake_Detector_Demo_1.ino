// Define pins
const int vibrationPin = D0; // SW-420 sensor digital output
const int buzzerPin = D8;  // Buzzer pin

void setup() {
  pinMode(vibrationPin, INPUT);  // Set vibration sensor as input
  pinMode(buzzerPin, OUTPUT); // Set buzzer as output
  Serial.begin(115200);           // Initialize serial monitor
}

void loop() {
  int vibrationState = digitalRead(vibrationPin); // Read the vibration sensor

  if (vibrationState == HIGH) { 
    Serial.println("Vibration Detected!"); // Print message to serial monitor

    // Buzzer beeps twice for 2 seconds each
    for (int i = 0; i < 2; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(2000); // Buzzer on for 2 seconds
      digitalWrite(buzzerPin, LOW);
      delay(500); // Small delay between beeps
    }

    // Wait until vibration stops before detecting again
    while (digitalRead(vibrationPin) == HIGH) {
      delay(5);  // Prevents multiple detections from same vibration event
    }
  }

  delay(5); // Small delay for faster response
}
