// cek soil moisture
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";
char ssid[] = "UIN_Walisongo-AX";
char pass[] = "";

BlynkTimer timer;

void myTimerEvent() {
  int adc = analogRead(A0);
  float tegangan = adc * (5.0 / 1023.0);
  adc = map(adc,400,1023,100,0);
  Blynk.virtualWrite(V2, adc);
  Blynk.virtualWrite(V3, tegangan);
}

void setup() {
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
  timer.setInterval(1000L, myTimerEvent); // 1000L = 1 detik
}

void loop() {
  Blynk.run();
  timer.run();
}