// Library Ultrasonic untuk sensor HC-SR04
#include <Ultrasonic.h>

// Definisikan pin trigger dan echo untuk sensor ultrasonik
const int triggerPin = D1; // GPIO pin D1 pada NodeMCU
const int echoPin = D2;    // GPIO pin D2 pada NodeMCU

// Library LCD
#include <LiquidCrystal_I2C.h>

// Konstruk object LCD dengan alamat I2C
// Ganti 0x3F sesuai dengan alamat I2C modul kalian
// Jika tidak tahu dapat menggunakan LCD I2C Scanner
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Buat objek Ultrasonic
Ultrasonic ultrasonic(triggerPin, echoPin);

void setup() {
  Serial.begin(9600);
  Serial.println("Sensor Ultrasonik HC-SR04");

  // Pemanggilan pertama memerlukan parameter jumlah kolom dan baris
  // Ini harus sama dengan yang dimasukan pada konstruktor.
  lcd.begin(16,2);
  lcd.init();
}
 
void loop() {
  // Baca jarak dari sensor ultrasonik (dalam cm)
  float distance = ultrasonic.read();
 
  // Nyalakan backlight
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(distance);
  // lcd.print(" cm");


  // Tampilkan hasil pembacaan di Serial Monitor
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000); // Beri jeda 1 detik antara setiap pembacaan
}