#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// WiFi credentials
const char *ssid = "Falcon Squad";
const char *password = "Zaman7865@";

// Telegram bot details
const char *botToken = "7572778703:AAG_IkmACkGAY-h5P2oSLYWqrNiTDn-Xxm8";
const char *chatID = "6026678067";

// Define pins
const int vibrationPin = D0; // SW-420 sensor digital output
const int buzzerPin = D8;    // Buzzer pin

WiFiClientSecure client;
unsigned long lastAlertTime = 0; // শেষবার নোটিফিকেশন পাঠানোর সময়
const int alertInterval = 2000;  // 5 সেকেন্ড পর আবার পাঠাতে পারবে

void setup()
{
  pinMode(vibrationPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); // শুরুতে বুজার বন্ধ থাকবে

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

    // Buzzer beeps twice for 2 seconds each
    for (int i = 0; i < 2; i++)
    {
      digitalWrite(buzzerPin, HIGH);
      delay(1000); // Buzzer on for 2 seconds
      digitalWrite(buzzerPin, LOW);
      delay(500); // Small delay between beeps
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
