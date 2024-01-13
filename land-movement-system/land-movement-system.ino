// insert module
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

// declaration
#define BLYNK_PRINT Serial
#define echoPin D3
#define trigPin D4
#define rain_sensor D5
#define LED_Red D6
#define LED_Yellow D7
#define LED_Green D8
// scl d1 (LCD)
// sda d2 (LCD)

BlynkTimer timer;
WidgetLED led1(V4);
WidgetLED led2(V5);
WidgetLED led3(V6);

// default address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

// connecting to WiFi and the blynk app
char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";
// WiFi credentials
char ssid[] = "UIN_Walisongo-AX";
char pass[] = "";

// initialize variable
long duration;
float distance;
int rain;
int humadity;

void myTimerEvent() {
  humadity = analogRead(A0);
  float voltage = humadity * (5.0 / 1023.0);
  humadity = map(humadity, 400, 1023, 100, 0);

  Blynk.virtualWrite(V1, distance);
  Blynk.virtualWrite(V2, humadity);
  Blynk.virtualWrite(V3, voltage);
}

// the setup function runs once when you press reset or power the board
void setup() {
  // Debug console
  Serial.begin(9600);

  // initialize the sensors
  pinMode(rain_sensor, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED_Red, OUTPUT);
  pinMode(LED_Yellow, OUTPUT);
  pinMode(LED_Green, OUTPUT);

  // initialize lcd screen
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();

  // scroll lcd to left
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("-deteksi pergerakan tanah-");
  for (int j = 1; j <= 25; j++) {
    lcd.scrollDisplayLeft();
    delay(500);
  }

  // connecting
  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
  timer.setInterval(1000L, myTimerEvent);  // 1000L = 1 sec

  Serial.println("sistem siap digunakan");
  Blynk.notify("Land Movement Ready");
}

// the loop function runs and over again forever
void loop() {
  sendSensor();
  Blynk.run();
  timer.run();
}

// 
void sendSensor() {
  rain = digitalRead(rain_sensor);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.println("==== ====");
  Serial.println("Raindrop Sensor");
  Serial.print("Digital value : ");
  Serial.println(rain);

  if (rain == 0) {
    Serial.println("hujan");
    led3.on();
    digitalWrite(LED_Green, HIGH);
    delay(500);
    digitalWrite(LED_Green, LOW);
  }

  // if (rain == 0) {
  //   Serial.println("Rain");
  //   Blynk.notify("rain bro");
  // } else {
  //   Serial.println("kering");
  // }

  Serial.print("distance: ");
  Serial.println(distance);

  Serial.print("Lembab: ");
  Serial.println(humadity);

  // system algorithm
  if (humadity > 40 || distance > 33) {
    Serial.println("kelembaban tinggi, bergerak agak jauh");
    led1.on();
    led2.off();
    led3.off();
    digitalWrite(LED_Red, HIGH);
    digitalWrite(LED_Yellow, LOW);
    digitalWrite(LED_Green, LOW);
  } else if (humadity > 40 || distance < 17) {
    Serial.println("kelembaban tinggi, bergerak agak jauh");
    led1.on();
    led2.off();
    led3.off();
    digitalWrite(LED_Red, HIGH);
    digitalWrite(LED_Yellow, LOW);
    digitalWrite(LED_Green, LOW);
  } else if (humadity > 30 || distance > 30) {
    Serial.println("kelembaban sedang, bergerak");
    led1.off();
    led2.on();
    led3.off();
    digitalWrite(LED_Red, LOW);
    digitalWrite(LED_Yellow, HIGH);
    digitalWrite(LED_Green, LOW);
  } else if (humadity > 30 || distance < 20) {
    Serial.println("kelembaban sedang, bergerak");
    led1.off();
    led2.on();
    led3.off();
    digitalWrite(LED_Red, LOW);
    digitalWrite(LED_Yellow, HIGH);
    digitalWrite(LED_Green, LOW);
  } else {
    led1.off();
    led2.off();
    led3.off();
    digitalWrite(LED_Red, LOW);
    digitalWrite(LED_Yellow, LOW);
    digitalWrite(LED_Green, LOW);
  }

  // tampilkan ke lcd
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jarak:");  // tampilkan distance
  lcd.println(distance);
  lcd.setCursor(0, 1);
  lcd.print("Lembab: ");  // tampilkan lembab
  lcd.print(humadity);

  delay(1000);
}