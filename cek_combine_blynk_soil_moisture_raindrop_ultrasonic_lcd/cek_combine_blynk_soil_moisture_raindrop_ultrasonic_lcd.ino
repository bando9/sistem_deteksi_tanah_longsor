// cek soil moisture + raindrop + ultrasonik + lcd
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

#define BLYNK_PRINT Serial
#define trigPin D3
#define echoPin D4
#define sensor_rain D5
// scl d1 (LCD)
// sda d2 (LCD)

BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";
char ssid[] = "UIN_Walisongo-AX";
char pass[] = "";


long durasi;
int jarak;
int hujan;

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

  // setting pin raindrop + ultrasonic
  pinMode(sensor_rain, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // start lcd
  lcd.init();
  lcd.backlight();

  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
  timer.setInterval(1000L, myTimerEvent); // 1000L = 1 detik

  Serial.println("siap digunakan");
  Blynk.notify("Rain Detector Ready");
}

void loop() {
  hujan = digitalRead(sensor_rain);
  Serial.println("==== ====");
  Serial.println("Raindrop Sensor");
  Serial.print("Digital value : ");
  Serial.println(hujan);

  if (hujan == 0) {
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

    // tampilkan ke lcd
  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Distance: ");
  lcd.println(distance);
  lcd.print(" cm");

  if(distance>=100) {
    Serial.print("Jauh");
    lcd.setCursor(0,1);
    lcd.print("note: Jauh");
  } else if(distance<100) {
    Serial.print("Deket\n");
    lcd.setCursor(0,1);
    lcd.print("note: Deket");
  };

  Blynk.run();
  timer.run();

  delay(1000);
  lcd.clear();
}