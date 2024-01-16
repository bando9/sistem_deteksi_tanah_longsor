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
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Default address 0x27

// Blynk Authentication and WiFi Credentials
char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";
char ssid[] = "UIN_Walisongo-AX";
char pass[] = "";

// Treshold
const int thresholdSafe = 5;
const int thresholdWarning = 15;
const int thresholdDanger = 30;
const int humadityThresholdHigh = 80;
const int humadityThresholdLow = 40;

// Sensor Variables
long pulseDuration;
int previousDistance;
int currentDistance;
int deltaDistance;
int rainStatus;
int humadityValue;
float voltage;

// Timer Event for Blynk
void myTimerEvent() {

  // Send sensor data to Blynk app
  Blynk.virtualWrite(V1, currentDistance);
  Blynk.virtualWrite(V4, deltaDistance);
  Blynk.virtualWrite(V2, humadityValue);
  Blynk.virtualWrite(V3, voltage);
}

// Setup Function
void setup() {
  // Start Serial Communication
  Serial.begin(9600);

  previousDistance = -1;

  // // WiFi notification
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(250);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.println("WiFi connected");

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
  lcd.print("deteksi pergerakan tanah");
  for (int j = 1; j <= 25; j++) {
    lcd.scrollDisplayLeft();
    delay(300);
  }

  // Connect to Blynk
  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
  timer.setInterval(1000L, myTimerEvent);  // 1000L = 1 sec

  Serial.println("sistem siap digunakan");
  Blynk.notify("System Ready to Use");
}

// Loop Function
void loop() {

  // Trigger Ultrasonic Sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Baca waktu perjalanan pulsa ultrasonik
  pulseDuration = pulseIn(ECHO_PIN, HIGH);
  currentDistance = pulseDuration * 0.034 / 2;

  currentDistance = max(currentDistance, 0);  // Nilai jarak selalu positif

  // Hitung perubahan jarak selalu bernilai positif
  deltaDistance = abs(currentDistance - previousDistance);

  // Read humadity from analog pin A0
  humadityValue = analogRead(A0);
  voltage = humadityValue * (5.0 / 1023.0);
  humadityValue = map(humadityValue, 400, 1023, 100, 0);

  // Read Raindrop Sensor
  rainStatus = digitalRead(RAIN_SENSOR_PIN);

  Serial.print("Jarak Awal: ");
  Serial.print(currentDistance);
  Serial.println(" cm");

  Serial.print("Perubahan Jarak: ");
  Serial.print(deltaDistance);
  Serial.println(" cm");

  Serial.print("Kelembaban: ");
  Serial.print(humadityValue);
  Serial.println("%");

  Serial.print("Hujan: ");
  Serial.println((rainStatus == HIGH) ? "Ya" : "Tidak");
  
  // Display on LCD
  // lcd.home();
  // lcd.clear();

  // System Algorithm
  if (rainStatus == HIGH) {
    Serial.println("Status: Hujan");
    lcd.setCursor(0, 1);
    lcd.print("Status: Hujan");
  } else if (deltaDistance <= thresholdSafe && humadityValue <= humadityThresholdLow) {
    Serial.println("Status: Aman");
    lcd.setCursor(0, 1);
    lcd.print("Status: Aman");
    led3.on();                           // Lampu aman blynk menyala
    digitalWrite(LED_GREEN_PIN, HIGH);   // Lampu aman sistem menyala
    led1.off();                          // Lampu aman blynk menyala
    digitalWrite(LED_RED_PIN, LOW);     // Lampu aman sistem menyala
    led2.off();                          // Lampu aman blynk menyala
    digitalWrite(LED_YELLOW_PIN, LOW);  // Lampu aman sistem menyala
  } else if (deltaDistance <= thresholdWarning || (humadityValue > humadityThresholdLow && humadityValue <= humadityThresholdHigh)) {
    Serial.println("Status: Waspada");
    lcd.setCursor(0, 1);
    lcd.print("Status: Waspada");
    led2.on();
    digitalWrite(LED_YELLOW_PIN, HIGH);
    led1.off();                         // Lampu aman blynk menyala
    digitalWrite(LED_RED_PIN, LOW);    // Lampu aman sistem menyala
    led3.off();                         // Lampu aman blynk menyala
    digitalWrite(LED_GREEN_PIN, LOW);  // Lampu aman sistem menyala
  } else {
    Serial.println("Status: Bahaya");
    lcd.setCursor(0, 1);
    lcd.print("Status: Waspada");
    led1.on();
    digitalWrite(LED_RED_PIN, HIGH);
    led3.off();                          // Lampu aman blynk menyala
    digitalWrite(LED_GREEN_PIN, LOW);   // Lampu aman sistem menyala
    led2.off();                          // Lampu aman blynk menyala
    digitalWrite(LED_YELLOW_PIN, LOW);  // Lampu aman sistem menyala
  }

  Blynk.run();
  timer.run();

  delay(1000);
}