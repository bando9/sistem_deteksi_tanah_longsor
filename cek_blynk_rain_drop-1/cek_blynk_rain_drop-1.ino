// cek raindrop
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Blynk char
char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";
char ssid[] = "realme_C53";
char pass[] = "12345678";

// inisiasi variabel
int intensity = 20;
int sensor = A0;
int val;

BlynkTimer timer;

void myTimerEvent() {
  int val 
}

void setup() {
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);

  Serial.println("siap digunakan");
  Blynk.notify("Rain Detector Ready");
}

void loop() {
  val = analogRead(sensor);
  val = map(val,0,1023,100,0); // mengubah nilai 0-1023 menjadi 0-100
  Blynk.run();
  Serial.println(val);
  if (val > intensity) {
    Blynk.notify("hujan bro");
    Serial.println("Succes Send Notify");
  }
  delay(2000);
}