// #include <Wire.h>
#include <Ultrasonic.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Alamat I2C dan ukuran LCD (16x2)

const int trigPin = D1; // Pin Trig sensor ultrasonik
const int echoPin = D2; // Pin Echo sensor ultrasonik

void setup() {
  lcd.init(); // Inisialisasi LCD
  lcd.backlight(); // Aktifkan lampu latar (backlight)
  
  Serial.begin(9600); // Inisialisasi komunikasi serial

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  float duration, distance;
  
  // Kirim pulse 10us ke sensor ultrasonik
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Baca durasi pulsa dari sensor
  duration = pulseIn(echoPin, HIGH);
  
  // Hitung jarak berdasarkan durasi pulsa
  distance = (duration * 0.034) / 2.0; // Rumus konversi

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Tampilkan jarak pada LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jarak: ");
  lcd.print(distance);
  lcd.print(" cm");
  
  delay(1000); // Tunggu sebentar sebelum membaca ulang
}
