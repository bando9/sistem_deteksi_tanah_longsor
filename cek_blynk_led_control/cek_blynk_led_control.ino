// #define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Token Blynk Anda
char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";

// // Koneksi WiFi
char ssid[] = "realme_C53";
char pass[] = "12345678";

// Pin LED
int led1 = D4;  // Pin LED 1
int led2 = D3;  // Pin LED 2
int led3 = D2;  // Pin LED 3

WidgetLED led_1(V6);   // led 1 blynk
WidgetLED led_2(V7);   // led 2 blynk
WidgetLED led_3(V8);   // led 3 blynk

void setup() {
  // Serial monitor untuk debugging
  Serial.begin(9600);
  
  // Mengatur pin LED sebagai output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  
  // Menghubungkan ke Blynk
  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
  Serial.println("sistem siap digunakan");  
}


BLYNK_WRITE(V10) {  // Tombol untuk mematikan semua LED
  int pinValue = param.asInt();  // Mendapatkan nilai dari tombol

  if (pinValue == HIGH) {  // Jika tombol ditekan
    
    Serial.println("nilai pin: ");
    Serial.println(pinValue);

    kedipKedip();
  }

}

void turnOffAllLED(){
  led_1.off();
  led_2.off();
  led_3.off();
  digitalWrite(led1, LOW);  // Matikan LED 1
  digitalWrite(led2, LOW);  // Matikan LED 2
  digitalWrite(led3, LOW);  // Matikan LED 3
}

void kedipKedip(){
  digitalWrite(led1, HIGH);
  led_1.on();
  delay(500);
  digitalWrite(led2, HIGH);
  led_2.on();
  delay(500);
  digitalWrite(led3, HIGH);
  led_3.on();
  delay(500);

  digitalWrite(led1, LOW);
  led_1.off();
  delay(500);
  digitalWrite(led2, LOW);
  led_2.off();
  delay(500);
  digitalWrite(led3, LOW);
  led_3.off();
  delay(500);
}


void loop() {
  // kedipKedip();
  Blynk.run();  // Menjalankan Blynk
}
