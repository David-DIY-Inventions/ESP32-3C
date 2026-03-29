// Copyright Notice 2026*
// This code is free to use, modify, sell, or publish by anyone who obtains a copy of it.

#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Light Switch"
#define BLYNK_AUTH_TOKEN    "YOUR_AUTH_TOKEN"

#include <BlynkSimpleEsp32.h>
#include <DHT.h>

const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

#define RELAY_PIN 3
#define DHT_PIN   2
#define DHT_TYPE  DHT11  //Change it to DHT22 if you have that type of sensor

DHT dht(DHT_PIN, DHT_TYPE);
BlynkTimer timer;

bool relayState = false;

void setRelay(bool on) {
  relayState = on;
  digitalWrite(RELAY_PIN, on ? HIGH : LOW);
  Blynk.virtualWrite(V0, on ? 1 : 0); // Update button in app
  Serial.println(on ? "Relay ON" : "Relay OFF");
}

// V0 controls the relay from the app
BLYNK_WRITE(V0) {
  setRelay(param.asInt() == 1);
}

void checkTemperature() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    Serial.printf("Temp: %.1f°C | Humidity: %.0f%%\n", t, h);
    Blynk.virtualWrite(V1, t);
    Blynk.virtualWrite(V2, h);

    // Auto shutoff at 20°C
//    if (t >= 20.0 && relayState) {
//      Serial.println("Temperature reached 20°C! Turning relay OFF.");
//      setRelay(false);
//      Blynk.logEvent("temp_alert", "Relay turned off! Temperature reached 20°C");
//    }
//  } else {
//    Serial.println("DHT read failed");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);
  timer.setInterval(10000L, checkTemperature); // Check every 10 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}
