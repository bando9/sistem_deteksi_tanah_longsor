// cek soil moisture
#define BLYNK_PRINT Serial
#define trigPin D3
#define echoPin D4

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

long durasi;
int jarak;

char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";
char ssid[] = "realme_C53";
char pass[] = "12345678";

BlynkTimer timer;

void myTimerEvent() {
  Blynk.virtualWrite(V1, jarak);
}

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  durasi = pulseIn(echoPin, HIGH);
  jarak = durasi * 0.034 / 2;

  Serial.print("Distane: ");
  Serial.print(jarak);

  Blynk.run();
  timer.run();
}