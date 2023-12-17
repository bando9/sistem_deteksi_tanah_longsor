// cek soil moisture + raindrop + ultrasonik
#define BLYNK_PRINT Serial
#define sensor_rain D1
#define trigPin D2
#define echoPin D3

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";
char ssid[] = "UIN_Walisongo-AX";
char pass[] = "";

BlynkTimer timer;

long durasi;
int jarak;
int rain;

void myTimerEvent() {
  int adc = analogRead(A0);
  float tegangan = adc * (5.0 / 1023.0);
  adc = map(adc,0,1024,1024,0);
  Blynk.virtualWrite(V1, jarak);
  Blynk.virtualWrite(V2, adc);
  Blynk.virtualWrite(V3, tegangan);
}

void setup() {
  Serial.begin(9600);

  pinMode(sensor_rain, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
  timer.setInterval(1000L, myTimerEvent); // 1000L = 1 detik

  Serial.println("siap digunakan");
  Blynk.notify("Rain Detector Ready");
}

void loop() {
  rain = digitalRead(sensor_rain);
  Serial.println("==== ====");
  Serial.println("Raindrop Sensor");
  Serial.print("Digital value : ");
  Serial.println(rain);

  if (rain == 0) {
    Serial.println("Rain");
    Blynk.notify("hujan bro");
  } else {
    Serial.println("kering");
  }

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  durasi = pulseIn(echoPin, HIGH);
  jarak = durasi*0.034/2;

  Serial.print("Distance: ");
  Serial.println(jarak);

  Blynk.run();
  timer.run();

  
  delay(1000);
}