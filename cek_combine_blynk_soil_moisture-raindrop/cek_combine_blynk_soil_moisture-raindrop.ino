// cek soil moisture + raindrop
#define BLYNK_PRINT Serial
#define sensor_rain D1

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";
char ssid[] = "UIN_Walisongo-AX";
char pass[] = "";

BlynkTimer timer;

void myTimerEvent() {
  int adc = analogRead(A0);
  float tegangan = adc * (5.0 / 1023.0);
  adc = map(adc,0,1024,1024,0);
  Blynk.virtualWrite(V2, adc);
  Blynk.virtualWrite(V3, tegangan);
}

void setup() {
  Serial.begin(9600);

  pinMode(sensor_rain, INPUT);

  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
  timer.setInterval(1000L, myTimerEvent); // 1000L = 1 detik

  Serial.println("siap digunakan");
  Blynk.notify("Rain Detector Ready");
}

void loop() {
  int rain = digitalRead(sensor_rain);
  Serial.println("Raindrop Sensor");
  Serial.print("Digital value : ");
  Serial.println(rain);

  Blynk.run();
  timer.run();

  if (rain == 0) {
    Serial.println("Rain");
    Blynk.notify("hujan bro");
  } else {
    Serial.println("kering");
  }
  delay(1000);
}