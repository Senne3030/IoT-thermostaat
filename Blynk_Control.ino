#define BLYNK_TEMPLATE_ID "user8"
#define BLYNK_TEMPLATE_NAME "user8@server.wyns.it"
#define BLYNK_PRINT Serial

int BlueLed = 4;

int virtualPotValue;
int lastVirtualPotValue;

int temperature = 0.0;
int WantedValue = 0.0;
int potvalue = 0;
int lastPotValue = 0;
int AppValue = 0;
int PotWantedValue;
int pot = 34;

int LedValueOn;
int LedValueOff;

#include <DHT11.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
DHT11 dht11(5);

char auth[] = "FzSycUcNxUhvRDSq4-zC9jH58bM7z7Dn";

char ssid[] = "embed";
char pass[] = "weareincontrol";

void setup() {
  pinMode(BlueLed, OUTPUT);
  digitalWrite(BlueLed, LOW);
  Serial.begin(9600);

  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");

  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);
}

BLYNK_WRITE(V2) {
  virtualPotValue = param.asInt();
  AppValue = virtualPotValue;
}

void loop() {
  Blynk.run();

  temperature = dht11.readTemperature();

  potvalue = analogRead(pot);

  if (lastPotValue != potvalue) {
    PotWantedValue = map(potvalue, 0, 4095, 10, 30);
    WantedValue = PotWantedValue;
    lastPotValue = potvalue;
  } else {
    WantedValue = virtualPotValue;
  }

  if (temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    //Serial.println(potvalue);
    Serial.print("WantedValue: ");
    Serial.print(WantedValue);
    Serial.println(" °C");

  } else {
    Serial.println("Error: Something was wrong");
  }


  if (temperature < WantedValue) {
    digitalWrite(BlueLed, HIGH);
    LedValueOn = 1023;
    LedValueOff = 0;
  } else {
    digitalWrite(BlueLed, LOW);
    LedValueOn = 0;
    LedValueOff = 1023;
  }
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, WantedValue);
  Blynk.virtualWrite(V4, LedValueOff);
  Blynk.virtualWrite(V3, LedValueOn);
}