#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// WiFi credentials
const char *ssid = "Falcon Squad";
const char *password = "Zaman7865@";

// Telegram bot details
const char *botToken = "###";
const char *chatID = "###";

// Define pins
const int vibrationPin = D0; // SW-420 sensor digital output
const int buzzerPin = D8;    // Buzzer pin
const int ledPin = D4;        // LED pin

WiFiClientSecure client;
unsigned long lastAlertTime = 0; // শেষবার নোটিফিকেশন পাঠানোর সময়
const int alertInterval = 2000;  // 2 সেকেন্ড পর আবার পাঠাতে পারবে

void setup()
{
  pinMode(vibrationPin, INPUT);  // Set vibration sensor as input
  pinMode(buzzerPin, OUTPUT);    // Set buzzer as output
  pinMode(ledPin, OUTPUT);       // Set LED as output
  digitalWrite(buzzerPin, LOW);  // Start with buzzer off
  digitalWrite(ledPin, LOW);     // Start with LED off

  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void sendTelegramMessage(String message)
{
  client.setInsecure(); // Bypass SSL certificate
  if (client.connect("api.telegram.org", 443))
  {
    String url = "/bot" + String(botToken) + "/sendMessage?chat_id=" + String(chatID) + "&text=" + message;
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: api.telegram.org\r\n" +
                 "User-Agent: ESP8266\r\n" +
                 "Accept: */*\r\n" +
                 "Connection: close\r\n\r\n");
    delay(100);
    while (client.available())
    {
      String response = client.readString();
      Serial.println(response);
    }
    client.stop();
  }
  else
  {
    Serial.println("Failed to connect to Telegram API");
  }
}

void loop()
{
  int vibrationState = digitalRead(vibrationPin);
  unsigned long currentTime = millis();

  if (vibrationState == HIGH)
  {
    Serial.println("🚨 Earthquake Detected!");

    // Turn on buzzer and LED for 2 seconds
    for (int i = 0; i < 2; i++)
    {
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(ledPin, HIGH);
      delay(2000);

      // Turn off buzzer and LED
      digitalWrite(buzzerPin, LOW);
      digitalWrite(ledPin, LOW);
      delay(250);
    }

    // Send Telegram alert with interval
    if (currentTime - lastAlertTime >= alertInterval)
    {
      sendTelegramMessage("🚨 ভূমিকম্প! ভূমিকম্প! অতি দ্রুত বাসা ত্যাগ করুন!");
      lastAlertTime = currentTime;
    }

    // Wait until vibration stops before detecting again
    while (digitalRead(vibrationPin) == HIGH)
    {
      delay(5); // Prevents multiple detections from same vibration event
    }
  }
  delay(5); // Small delay for faster response
}