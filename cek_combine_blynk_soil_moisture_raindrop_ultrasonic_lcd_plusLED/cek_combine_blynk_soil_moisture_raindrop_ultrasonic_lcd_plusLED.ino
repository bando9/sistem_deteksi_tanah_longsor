// cek soil moisture + raindrop + ultrasonik + lcd
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

#define BLYNK_PRINT Serial
#define echoPin D3
#define trigPin D4
#define sensor_rain D5
// scl d1 (LCD)
// sda d2 (LCD)

BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";
char ssid[] = "UIN_Walisongo-AX";
char pass[] = "";


long durasi;
float jarak;
int hujan;
int adc;

void myTimerEvent() {
  adc = analogRead(A0);
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
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();

  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("-deteksi pergerakan tanah-");
  for(int j=1; j<=25; j++){
    lcd.scrollDisplayLeft();
    delay(500);
  }
  // selesai lcd

  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
  timer.setInterval(1000L, myTimerEvent); // 1000L = 1 detik

  Serial.println("siap digunakan");
  Blynk.notify("Rain Detector Ready");
}

void loop() {
  hujan = digitalRead(sensor_rain);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  durasi = pulseIn(echoPin, HIGH);
  jarak = durasi*0.034/2;

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

  Serial.print("Jarak: ");
  Serial.println(jarak);

  Serial.print("Lembab: ");
  Serial.println(adc);

   // tampilkan ke lcd
  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Jarak:");    // tampilkan jarak
  lcd.println(jarak);
  lcd.setCursor(0,1);
  lcd.print("Lembab: "); // tampilkan lembab
  lcd.print(adc);

  delay(1000);

  Blynk.run();
  timer.run();
}