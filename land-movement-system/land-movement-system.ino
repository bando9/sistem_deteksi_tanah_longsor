// Libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define BLYNK_PRINT Serial
#define ECHO_PIN D3         // trigger pin of the ultrasonic sensor
#define TRIG_PIN D4         // echo pin of the ultrasonic sensor
#define RAIN_SENSOR_PIN D5  // raindrop sensor pin
#define LED_RED_PIN D6      // LED red pin (bahaya)
#define LED_YELLOW_PIN D7   // LED yellow pin (waspada)
#define LED_GREEN_PIN D8    // LED green pin (aman)
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

// WiFi 1
// char ssid[] = "UIN_Walisongo-AX";
// char pass[] = "";

// WiFi 2
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
const int distanceThresholdSafe = 2;
const int distanceThresholdWarning = 4;
const int distanceThresholdDanger = 6;
const int humidityThresholdSafe = 612;
const int humidityThresholdWarning = 408;
const int humidityThresholdDanger = 254;

bool systemEnabled = true;

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

// control system
BLYNK_WRITE(V10) {
  int pinValue = param.asInt();
  if(pinValue == HIGH) {
    Serial.println("Tombol ditekan, mematikan semua system");
    systemEnabled = false;
    turnOffAllLEDs();
  } else {
    Serial.println("Tombol dilepas, mengaktifkan kembali system");
    systemEnabled = true;
    resetSystem();
  }
}

// Loop Function
void loop() {
  if(systemEnabled) {
    sendSensor();
  }
  Blynk.run();
  timer.run();
}

// Function to Read and Send Sensor Data
void sendSensor() {
  if (!systemEnabled) return;

  // Read humadity from analog pin A0
  humidityValue = analogRead(A0);
  float voltage = humidityValue * (5.0 / 1023.0);

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

  Serial.print("Jarak Awal: ");
  Serial.print(previousDistance);
  Serial.println(" cm");

  Serial.print("Jarak Akhir: ");
  Serial.print(currentDistance);
  Serial.println(" cm");

  deltaDistance = currentDistance - previousDistance;

  // function abs
  deltaDistance = abs(deltaDistance);

  Serial.print("Beda Jarak: ");
  Serial.print(deltaDistance);
  Serial.println(" cm");

  // Display on LCD
  // Display 1
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jarak Awal:");
  lcd.print(previousDistance);
  lcd.print("cm");
  lcd.setCursor(0, 1);
  lcd.print("Jarak Akhir:");
  lcd.print(currentDistance);
  lcd.print("cm");
  delay(3500);

  if(!systemEnabled) return;

  // Display 2
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Beda Jarak:");
  lcd.print(deltaDistance);
  lcd.print("cm");
  lcd.setCursor(0, 1);
  lcd.print("Lembab:");
  Serial.print("Lembab: ");

  // Moisture Category
  if (humidityValue >= 816) {
    Serial.println("Kering+");
    lcd.print("Kering+");
    Blynk.virtualWrite(V4, "Kering+");
  } else if (humidityValue >= 612) {
    Serial.println("Kering");
    lcd.print("Kering");
    Blynk.virtualWrite(V4, "Kering");
  } else if (humidityValue >= 408) {
    Serial.println("Normal");
    lcd.print("Normal");
    Blynk.virtualWrite(V4, "Normal");
  } else if (humidityValue >= 254) {
    Serial.println("Basah");
    lcd.print("Basah");
    Blynk.virtualWrite(V4, "Basah");
  } else if (humidityValue >= 0) {
    Serial.println("Basah+");
    lcd.print("Basah+");
    Blynk.virtualWrite(V4, "Basah+");
  } else {
    Serial.println("Sensor tidak terbaca");
  }

  if(!systemEnabled) return;

  // Send sensor data to Blynk app
  Blynk.virtualWrite(V1, previousDistance);
  Blynk.virtualWrite(V2, currentDistance);
  Blynk.virtualWrite(V3, deltaDistance);
  Blynk.virtualWrite(V5, rainStatus);

  if (deltaDistance > 5) {
    Serial.println("Perubahan jarak yang signifikan terdeteksi!");
  }

  previousDistance = currentDistance;

  // System Algorithm
  if (rainStatus == 0) {
    Serial.println("Status hujan dikirim!");
    Blynk.notify("Status: Hujan");
  }

  if(!systemEnabled) return;

  if (humidityValue < humidityThresholdDanger || deltaDistance > distanceThresholdDanger) {
    Serial.println("Status: Bahaya");
    setDangerLEDs();
  } else if (humidityValue < humidityThresholdWarning || deltaDistance > distanceThresholdWarning) {
    Serial.println("Status: Waspada");
    setWarningLEDs();
  } else if (humidityValue < humidityThresholdSafe || deltaDistance > distanceThresholdSafe) {
    Serial.println("Status: Aman");
    setSafeLEDs();
  } else {
    Serial.println("Status: System Still Working");
    turnOffAllLEDs();
  }

  delay(3000);
}

// Function to reset system state
void resetSystem() {
  // Reset variables and sytem state
  previousDistance = 0;
  currentDistance = 0;
  deltaDistance = 0;
  humidityValue = 0;
  rainStatus = 0;
  Serial.println("Sistem telah direset");
}

// Turn off all LEDs
void turnOffAllLEDs() {
  led1.off();
  led2.off();
  led3.off();
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);
}

// Turn on the green LED
void setSafeLEDs() {
  if(!systemEnabled) return;
  led1.off();
  led2.off();
  led3.on();
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, HIGH);
}

// Turn on the yellow LED
void setWarningLEDs() {
  if(!systemEnabled) return;
  led1.off();
  led2.on();
  led3.off();
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, HIGH);
  digitalWrite(LED_GREEN_PIN, LOW);
}

// Turn on the red LED
void setDangerLEDs() {
  if(!systemEnabled) return;
  led1.on();
  led2.off();
  led3.off();
  digitalWrite(LED_RED_PIN, HIGH);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);
}
