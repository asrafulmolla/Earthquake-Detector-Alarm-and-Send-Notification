#define BLYNK_TEMPLATE_ID "TMPL3vRhDm3jP"
#define BLYNK_TEMPLATE_NAME "Earthquake Alert"
#define BLYNK_AUTH_TOKEN "jM8Cwgk31tKlQVTZcVIh6Q1Y_eocqcl8"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Falcon Squad";   
char pass[] = "Zaman7865@";     

const int vibrationPin = A0;
const int threshold = 600;    // Increased threshold value to filter out small vibrations
#define VIRTUAL_PIN_VIBRATION V1  
#define VIRTUAL_PIN_ALERT V2  // Virtual pin for alert

BlynkTimer timer;

bool earthquakeDetected = false;  
unsigned long lastAlertTime = 0;
const unsigned long alertCooldown = 5000;  // 5 sec cooldown between alerts
unsigned long vibrationDuration = 0;       // Track how long vibration stays above threshold
const int stableThreshold = 3;              // Number of consecutive readings above threshold for stability

void checkVibration() {
  int vibrationValue = analogRead(vibrationPin);
  Serial.print("Vibration Value: ");
  Serial.println(vibrationValue);

  Blynk.virtualWrite(VIRTUAL_PIN_VIBRATION, vibrationValue);

  // Check if vibration exceeds threshold
  if (vibrationValue > threshold) {
    vibrationDuration++;  // Count how long vibration stays above threshold
    Serial.println("🚨 Vibration DETECTED!");

    // If vibration stays above threshold for a certain duration
    if (vibrationDuration >= stableThreshold) {  
      if (!earthquakeDetected && millis() - lastAlertTime > alertCooldown) {
        Serial.println("📩 Sending ALERT to Blynk...");
        Blynk.virtualWrite(VIRTUAL_PIN_ALERT, 1);  // Set virtual pin value to 1 to trigger an alert in the app
        earthquakeDetected = true;
        lastAlertTime = millis();
      } else {
        Serial.println("❌ Alert not sent due to cooldown...");
      }
    }
  } else {
    // Reset vibrationDuration if vibration goes below threshold
    vibrationDuration = 0;
    Serial.println("✅ No earthquake detected, system normal.");

    // Reset the earthquake detection after 5 sec
    if (millis() - lastAlertTime > 5000) {  
      earthquakeDetected = false;
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");

  Blynk.begin(auth, ssid, pass);
  Serial.println("✅ Blynk Connected!");

  timer.setInterval(1000L, checkVibration);
}

void loop() {
  Blynk.run();
  timer.run();
}
