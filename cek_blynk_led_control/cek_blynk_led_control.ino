#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

// Token Blynk Anda
char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";

// Koneksi WiFi
char ssid[] = "realme_C53";
char pass[] = "12345678";

// Pin LED
int led1 = D6;  // Pin LED 1
int led2 = D7;  // Pin LED 2
int led3 = D8;  // Pin LED 3

WidgetLED led_1(V6);   // LED 1 Blynk
WidgetLED led_2(V7);   // LED 2 Blynk
WidgetLED led_3(V8);   // LED 3 Blynk

LiquidCrystal_I2C lcd(0x27, 16, 2); // Alamat I2C untuk LCD bisa berbeda

bool kedipKedipEnabled = true;  // Flag untuk mengontrol kedipKedip
bool led1Status = false;
bool led2Status = false;
bool led3Status = false;

void setup() {
  // Serial monitor untuk debugging
  Serial.begin(9600);

  // Mengatur pin LED sebagai output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  // Menghubungkan ke Blynk
  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
  Serial.println("Sistem siap digunakan");

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
}

BLYNK_WRITE(V10) {  // Tombol untuk mematikan semua LED
  int pinValue = param.asInt();  // Mendapatkan nilai dari tombol

  if (pinValue == HIGH) {  // Jika tombol ditekan
    Serial.println("Tombol ditekan, mematikan semua LED");
    kedipKedipEnabled = false;  // Menonaktifkan kedipKedip
    turnOffAllLED();
  } else {
    Serial.println("Tombol dilepas, mengaktifkan kembali kedipKedip");
    kedipKedipEnabled = true;  // Mengaktifkan kembali kedipKedip
  }
}

void turnOffAllLED(){
  led1Status = false;
  led2Status = false;
  led3Status = false;

  led_1.off();
  led_2.off();
  led_3.off();
  digitalWrite(led1, LOW);  // Matikan LED 1
  digitalWrite(led2, LOW);  // Matikan LED 2
  digitalWrite(led3, LOW);  // Matikan LED 3
}

void updateLEDs(){
  if (kedipKedipEnabled) {
    led1Status = !led1Status;
    led2Status = !led2Status;
    led3Status = !led3Status;

    if (led1Status) {
      digitalWrite(led1, HIGH);
      led_1.on();
    } else {
      digitalWrite(led1, LOW);
      led_1.off();
    }

    if (led2Status) {
      digitalWrite(led2, HIGH);
      led_2.on();
    } else {
      digitalWrite(led2, LOW);
      led_2.off();
    }

    if (led3Status) {
      digitalWrite(led3, HIGH);
      led_3.on();
    } else {
      digitalWrite(led3, LOW);
      led_3.off();
    }

    delay(500);
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Slide 1");
  delay(1000);  // Delay untuk slide pertama

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Slide 2");
  // delay(1000);  // Delay untuk slide kedua
}

void loop() {
  Blynk.run();  // Menjalankan Blynk

  if (kedipKedipEnabled) {
    updateLEDs();  // Menjalankan kedipKedip jika flag diizinkan
  }

  updateLCD();  // Menjalankan updateLCD
}
