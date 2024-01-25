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
WidgetLED led1(V6);
WidgetLED led2(V7);
WidgetLED led3(V8);

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Default address 0x27

// Blynk Authentication and WiFi Credentials
char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";
// char ssid[] = "UIN_Walisongo-AX";
// char pass[] = "";
char ssid[] = "realme_C53";
char pass[] = "12345678";

// Sensor Variables
long pulseDuration;
int currentDistance;
int deltaDistance;
int rainStatus;
int humidityValue;

int previousDistance = 0;
const int thresholdDistance = 5;

// Threshold
const int distanceThresholdSafe = 3;
const int distanceThresholdWarning = 6;
const int distanceThresholdDanger = 9;
const int humidityThresholdWarning = 50;
const int humidityThresholdDanger = 80;

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

  // Read humadity from analog pin A0
  humidityValue = analogRead(A0);
  float voltage = humidityValue * (5.0 / 1023.0);
  humidityValue = map(humidityValue, 400, 1023, 100, 0);

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
  currentDistance = pulseDuration * 0.034 / 2;

  currentDistance = max(currentDistance, 0);

  Serial.println("==== ====");
  Serial.print("Status Hujan : ");
  Serial.println((rainStatus == 0) ? "Ya" : "Tidak");

  Serial.print("Lembab: ");
  Serial.println(humidityValue);

  Serial.print("Jarak Awal: ");
  Serial.println(previousDistance);

  Serial.print("Jarak Akhir: ");
  Serial.println(currentDistance);

  deltaDistance = currentDistance - previousDistance;

  deltaDistance = abs(deltaDistance);

  Serial.print("Delta Jarak: ");
  Serial.println(deltaDistance);

  // Display on LCD
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jarak Awal:");  // tampilkan distance
  lcd.print(previousDistance);
  lcd.setCursor(0, 1);
  lcd.print("Jarak Akhir: ");  // tampilkan lembab
  lcd.print(currentDistance);

  // Send sensor data to Blynk app
  Blynk.virtualWrite(V1, previousDistance);
  Blynk.virtualWrite(V2, currentDistance);
  Blynk.virtualWrite(V3, deltaDistance);
  Blynk.virtualWrite(V4, humidityValue);
  Blynk.virtualWrite(V5, rainStatus);

  if (deltaDistance > 5) {
    Serial.println("Perubahan jarak yang signifikan terdeteksi!");
  }

  previousDistance = currentDistance;


  // System Algorithm
  if (rainStatus == 0) {
    Serial.println("status: hujan");
    Blynk.notify("Status: Hujan");
  }

  if (humidityValue > humidityThresholdDanger || deltaDistance > distanceThresholdDanger) {
    Serial.println("Status: Bahaya");
    setDangerLEDs();
  } else if (humidityValue > humidityThresholdWarning || deltaDistance > distanceThresholdWarning) {
    Serial.println("Status: Bahaya");
    setWarningLEDs();
  } else if (deltaDistance > distanceThresholdSafe) {
    Serial.println("Status: Bahaya");
    setSafeLEDs();
  } else {
    Serial.println("Status: System Still Working");
    turnOffAllLEDs();
  }


  // logika 1
  // if (humidityValue > 40 || distance > 33) {
  //   Serial.println("kelembaban tinggi, bergerak agak jauh");
  //   setDangerLEDs();
  // } else if (humidityValue > 40 || distance < 17) {
  //   Serial.println("kelembaban tinggi, bergerak agak jauh");
  //   setDangerLEDs();
  // } else if (humidityValue > 30 || distance > 30) {
  //   Serial.println("kelembaban sedang, bergerak");
  //   setWarningLEDs();
  // } else if (humidityValue > 30 || distance < 20) {
  //   Serial.println("kelembaban sedang, bergerak");
  //   setWarningLEDs();
  // } else {
  //   // No significant movement
  //   turnOffAllLEDs();
  // }

  // // logika 2
  // if (rainStatus == HIGH) {
  //   Serial.println("hujan");
  //   led3.on();
  //   digitalWrite(LED_GREEN_PIN, HIGH);
  //   delay(500);
  //   digitalWrite(LED_GREEN_PIN, LOW);
  // }

  // if (humidityValue > humidityThresholdWarning) {
  //   Serial.println("lembab banget");
  //   setDangerLEDs();
  // } else if (humidityValue > humidityThresholdSafe) {
  //   Serial.println("lembab");
  //   setWarningLEDs();
  // } else {
  //   turnOffAllLEDs();
  // }

  // if (distance > distanceThresholdWarning) {
  //   Serial.println("bergrak jauh banget");
  //   setDangerLEDs();
  // } else if (distance > distanceThresholdSafe) {
  //   Serial.println("bergerak");
  //   setWarningLEDs();
  // } else {
  //   turnOffAllLEDs();
  // }
  

  delay(5000);
}

// LED dimatikan semua
void turnOffAllLEDs() {
  led1.off();
  led2.off();
  led3.off();
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);
}

// LED hijau dinyalakan
void setSafeLEDs() {
  led1.off();
  led2.off();
  led3.on();
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, HIGH);
}

// LED kuning dinyalakan
void setWarningLEDs() {
  led1.off();
  led2.on();
  led3.off();
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, HIGH);
  digitalWrite(LED_GREEN_PIN, LOW);
}

// LED merah dinyalakan
void setDangerLEDs() {
  led1.on();
  led2.off();
  led3.off();
  digitalWrite(LED_RED_PIN, HIGH);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);
}
