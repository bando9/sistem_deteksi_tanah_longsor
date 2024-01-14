// Libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define BLYNK_PRINT Serial
#define ECHO_PIN D3         // Pin Trigger sensor ultrasonik
#define TRIG_PIN D4         // Pin Echo sensor ultrasonik
#define RAIN_SENSOR_PIN D5  // Pin sensor raindrop
#define LED_RED_PIN D6      // Pin LED bahaya (merah)
#define LED_YELLOW_PIN D7   // Pin LED waspada (kuning)
#define LED_GREEN_PIN D8    // Pin LED aman (hijau)
// scl d1 (LCD)
// sda d2 (LCD)

// Blynk Setup
BlynkTimer timer;
WidgetLED led1(V4);
WidgetLED led2(V5);
WidgetLED led3(V6);

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // Default address 0x27

// Blynk Authentication and WiFi Credentials
char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";
char ssid[] = "UIN_Walisongo-AX";
char pass[] = "";

// Sensor Variables
long pulseDuration;
float distance;
int rainStatus;
int humadityValue;

// Timer Event for Blynk
void myTimerEvent() {
  // Read humadity from analog pin A0
  humadityValue = analogRead(A0);
  float voltage = humadityValue * (5.0 / 1023.0);
  humadityValue = map(humadityValue, 400, 1023, 100, 0);

  // Send sensor data to Blynk app
  Blynk.virtualWrite(V1, distance);
  Blynk.virtualWrite(V2, humadityValue);
  Blynk.virtualWrite(V3, voltage);
}

// Setup Function
void setup() {
  // Start Serial Communication
  Serial.begin(9600);

  // Initialize Sensor Pins
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);

  // Initialize LCD Screen
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();

  // Scroll LCD to left
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("-deteksi pergerakan tanah-");
  for (int j = 1; j <= 25; j++) {
    lcd.scrollDisplayLeft();
    delay(500);
  }

  // Connect to Blynk
  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
  timer.setInterval(1000L, myTimerEvent);  // 1000L = 1 sec

  Serial.println("sistem siap digunakan");
  Blynk.notify("Land Movement Ready");
}

// Loop Function
void loop() {
  sendSensor();
  Blynk.run();
  timer.run();
}

// Function to Read and Send Sensor Data
void sendSensor() {
  // Read Raindrop Sensor
  rainStatus = digitalRead(RAIN_SENSOR_PIN);

  // Trigger Ultrasonic Sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure Pulse Duration
  pulseDuration = pulseIn(ECHO_PIN, HIGH);
  distance = pulseDuration * 0.034 / 2;

  Serial.println("==== ====");
  Serial.println("Raindrop Sensor");
  Serial.print("Digital value : ");
  Serial.println(rainStatus);

  if (rainStatus == 0) {
    Serial.println("hujan");
    led3.on();
    digitalWrite(LED_GREEN_PIN, HIGH);
    delay(500);
    digitalWrite(LED_GREEN_PIN, LOW);
  }

  // if (rainStatus == 0) {
  //   Serial.println("Rain");
  //   Blynk.notify("rain bro");
  // } else {
  //   Serial.println("kering");
  // }

  Serial.print("Jarak: ");
  Serial.println(distance);

  Serial.print("Lembab: ");
  Serial.println(humadityValue);

  // System Algorithm
  if (humadityValue > 40 || distance > 33) {
    Serial.println("kelembaban tinggi, bergerak agak jauh");
    led1.on();
    led2.off();
    led3.off();
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
  } else if (humadityValue > 40 || distance < 17) {
    Serial.println("kelembaban tinggi, bergerak agak jauh");
    led1.on();
    led2.off();
    led3.off();
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
  } else if (humadityValue > 30 || distance > 30) {
    Serial.println("kelembaban sedang, bergerak");
    led1.off();
    led2.on();
    led3.off();
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, HIGH);
    digitalWrite(LED_GREEN_PIN, LOW);
  } else if (humadityValue > 30 || distance < 20) {
    Serial.println("kelembaban sedang, bergerak");
    led1.off();
    led2.on();
    led3.off();
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, HIGH);
    digitalWrite(LED_GREEN_PIN, LOW);
  } else {
    // No significant movement
    led1.off();
    led2.off();
    led3.off();
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
  }

  // Display on LCD
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jarak:");  // tampilkan distance
  lcd.println(distance);
  lcd.setCursor(0, 1);
  lcd.print("Lembab: ");  // tampilkan lembab
  lcd.print(humadityValue);

  delay(1000);
}