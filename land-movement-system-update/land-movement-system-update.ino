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

// Threshold
const int previousDistance = 0;
const int thresholdSafe = 5;
const int thresholdWarning = 15;
const int thresholdDanger = 30;
const int humidityThresholdLow = 40;
const int humidityThresholdHigh = 80;

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

  // WiFi notification
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

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

void loop() {
  rainStatus = digitalRead(RAIN_SENSOR_PIN);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  int currentDistance = calculateDistance();

  int deltaDistance = abs(currentDistance - previousDistance);
  handleNotification(deltaDistance, humidity, raindrop);

  previousDistance = currentDistance;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jarak: ");
  lcd.print(currentDistance);
  lcd.print(" cm");

  lcd.setCursor(0, 1);
  lcd.print("Hujan: ");
  lcd.print((raindrop == HIGH) ? "Ya" : "Tidak");

  delay(1000);
  Blynk.run();
}

int calculateDistance() {
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;
  return max(distance, 0);
}

void sendToBlynk(int deltaDistance, int humidity, int raindrop) {
  Blynk.virtualWrite(V1, deltaDistance);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, (raindrop == HIGH) ? "Hujan" : "Tidak Hujan");
}

void handleNotification(int deltaDistance, int humidity, int raindrop) {
  Serial.print("Jarak: ");
  Serial.print(deltaDistance);
  Serial.println(" cm");

  Serial.print("Kelembaban: ");
  Serial.print(humidity);
  Serial.println("%");

  Serial.print("Hujan: ");
  Serial.println((raindrop == HIGH) ? "Ya" : "Tidak");

  displayOnLCD(deltaDistance, raindrop);
  sendToBlynk(deltaDistance, humidity, raindrop);
}




// #include <ESP8266WiFi.h>
// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// #include <BlynkSimpleEsp8266.h>

// char auth[] = "Your_Blynk_Auth_Token";
// const char* ssid = "Your_WiFi_SSID";
// const char* password = "Your_WiFi_Password";

// const int trigPin = D1;
// const int echoPin = D2;
// const int humidityPin = A0;
// const int raindropPin = D5;

// int previousDistance = 0;
// const int thresholdSafe = 5;
// const int thresholdWarning = 15;
// const int thresholdDanger = 30;
// const int humidityThresholdLow = 40;
// const int humidityThresholdHigh = 80;

// LiquidCrystal_I2C lcd(0x27, 16, 2);

// void setup() {
//   Serial.begin(9600);
//   pinMode(trigPin, OUTPUT);
//   pinMode(echoPin, INPUT);
//   pinMode(raindropPin, INPUT);

//   Wire.begin(D5, D6);

//   lcd.begin(16, 2);
//   lcd.print("Monitoring Jarak");
//   delay(2000);

//   Blynk.begin(auth, ssid, password);
// }

// void sendUltrasonicPulse() {
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);
// }

// int calculateDistance() {
//   long duration = pulseIn(echoPin, HIGH);
//   int distance = duration * 0.034 / 2;
//   return max(distance, 0);
// }

// int readHumidity() {
//   return analogRead(humidityPin);
// }

// int readRaindrop() {
//   return digitalRead(raindropPin);
// }

// void displayOnLCD(int deltaDistance, int raindrop) {
//   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print("Jarak: ");
//   lcd.print(deltaDistance);
//   lcd.print(" cm");

//   lcd.setCursor(0, 1);
//   lcd.print("Hujan: ");
//   lcd.print((raindrop == HIGH) ? "Ya" : "Tidak");
// }

// void sendToBlynk(int deltaDistance, int humidity, int raindrop) {
//   Blynk.virtualWrite(V1, deltaDistance);
//   Blynk.virtualWrite(V2, humidity);
//   Blynk.virtualWrite(V3, (raindrop == HIGH) ? "Hujan" : "Tidak Hujan");
// }

// void handleNotification(int deltaDistance, int humidity, int raindrop) {
//   Serial.print("Jarak: ");
//   Serial.print(deltaDistance);
//   Serial.println(" cm");

//   Serial.print("Kelembaban: ");
//   Serial.print(humidity);
//   Serial.println("%");

//   Serial.print("Hujan: ");
//   Serial.println((raindrop == HIGH) ? "Ya" : "Tidak");

//   displayOnLCD(deltaDistance, raindrop);
//   sendToBlynk(deltaDistance, humidity, raindrop);
// }

// void loop() {
//   sendUltrasonicPulse();
//   int currentDistance = calculateDistance();

//   int humidity = readHumidity();
//   int raindrop = readRaindrop();

//   int deltaDistance = abs(currentDistance - previousDistance);
//   handleNotification(deltaDistance, humidity, raindrop);

//   previousDistance = currentDistance;

//   delay(1000);

//   Blynk.run();
// }














// // Libraries
// #include <ESP8266WiFi.h>
// #include <BlynkSimpleEsp8266.h>
// #include <LiquidCrystal_I2C.h>

// // Pin Definitions
// #define BLYNK_PRINT Serial
// #define ECHO_PIN D3
// #define TRIG_PIN D4
// #define RAIN_SENSOR_PIN D5
// #define LED_RED_PIN D6
// #define LED_YELLOW_PIN D7
// #define LED_GREEN_PIN D8
// // SCL (LCD) D1
// // SDA (LCD) D2

// // Blynk Setup
// BlynkTimer timer;
// WidgetLED led1(V4);
// WidgetLED led2(V5);
// WidgetLED led3(V6);

// // LCD Setup
// LiquidCrystal_I2C lcd(0x27, 16, 2);  // Default address 0x27

// // Blynk Authentication and WiFi Credentials
// char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";
// char ssid[] = "UIN_Walisongo-AX";
// char pass[] = "";

// // Sensor Variables
// long pulseDuration;
// float distance;
// float initialDistance = 18.00;
// float distanceChange;
// int rainStatus;
// int humidityValue;

// // Timer Event for Blynk
// void myTimerEvent() {
//   // Read humidity from analog pin A0
//   humidityValue = analogRead(A0);
//   float voltage = humidityValue * (5.0 / 1023.0);
//   humidityValue = map(humidityValue, 400, 1023, 100, 0);

//   // Send sensor data to Blynk app
//   Blynk.virtualWrite(V1, distance);
//   Blynk.virtualWrite(V2, humidityValue);
//   Blynk.virtualWrite(V3, voltage);
// }

// // Setup Function
// void setup() {
//   // Start Serial Communication
//   Serial.begin(9600);

//   // Initialize Sensor Pins
//   pinMode(RAIN_SENSOR_PIN, INPUT);
//   pinMode(TRIG_PIN, OUTPUT);
//   pinMode(ECHO_PIN, INPUT);
//   pinMode(LED_RED_PIN, OUTPUT);
//   pinMode(LED_YELLOW_PIN, OUTPUT);
//   pinMode(LED_GREEN_PIN, OUTPUT);

//   // Initialize LCD Screen
//   lcd.begin(16, 2);
//   lcd.init();
//   lcd.backlight();

//   // Scroll LCD to left
//   lcd.home();
//   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print("- Land Movement Detection -");
//   for (int j = 1; j <= 27; j++) {
//     lcd.scrollDisplayLeft();
//     delay(500);
//   }

//   // Connect to Blynk
//   Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
//   timer.setInterval(1000L, myTimerEvent);  // 1000L = 1 sec

//   Serial.println("System is ready");
//   Blynk.notify("Land Movement Detection System Ready");
// }

// // Loop Function
// void loop() {
//   sendSensorData();
//   Blynk.run();
//   timer.run();
// }

// // Function to Read and Send Sensor Data
// void sendSensorData() {
//   // Read Raindrop Sensor
//   rainStatus = digitalRead(RAIN_SENSOR_PIN);

//   // Trigger Ultrasonic Sensor
//   digitalWrite(TRIG_PIN, LOW);
//   delayMicroseconds(5);
//   digitalWrite(TRIG_PIN, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG_PIN, LOW);

//   // Measure Pulse Duration
//   pulseDuration = pulseIn(ECHO_PIN, HIGH);
//   distance = pulseDuration * 0.034 / 2;

//   Serial.println("==== ====");
//   Serial.println("Raindrop Sensor");
//   Serial.print("Digital value : ");
//   Serial.println(rainStatus);

//   if (rainStatus == 0) {
//     Serial.println("Rain detected");
//     led3.on();
//     digitalWrite(LED_GREEN_PIN, HIGH);
//     delay(500);
//     digitalWrite(LED_GREEN_PIN, LOW);
//   }

//   // Calculate distance change
//   distanceChange = distance - initialDistance;

//   Serial.print("Distance: ");
//   Serial.println(distance);

//   Serial.print("Initial Dist: ");
//   Serial.println(initialDistance);

//   Serial.print("Dist Change: ");
//   Serial.println(distanceChange);

//   Serial.print("Humidity: ");
//   Serial.println(humidityValue);

//   // System Algorithm
//   if ((humidityValue > 40 && (distanceChange > 15 || distanceChange < -15)) ||
//       (humidityValue > 30 && (distance > 30 || distance < 20))) {
//     Serial.println("High humidity, moving somewhat far");
//     led1.on();
//     led2.off();
//     led3.off();
//     digitalWrite(LED_RED_PIN, HIGH);
//     digitalWrite(LED_YELLOW_PIN, LOW);
//     digitalWrite(LED_GREEN_PIN, LOW);
//   } else if ((humidityValue > 30 && (distanceChange > 15 || distanceChange < -15)) ||
//              (humidityValue > 30 && (distance > 30 || distance < 20))) {
//     Serial.println("Moderate humidity, moving");
//     led1.off();
//     led2.on();
//     led3.off();
//     digitalWrite(LED_RED_PIN, LOW);
//     digitalWrite(LED_YELLOW_PIN, HIGH);
//     digitalWrite(LED_GREEN_PIN, LOW);
//   } else {
//     // No significant movement
//     led1.off();
//     led2.off();
//     led3.off();
//     digitalWrite(LED_RED_PIN, LOW);
//     digitalWrite(LED_YELLOW_PIN, LOW);
//     digitalWrite(LED_GREEN_PIN, LOW);
//   }

//   // Display on LCD
//   lcd.home();
//   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print("Distance:");  // Display distance
//   lcd.println(distance);
//   lcd.setCursor(0, 1);
//   lcd.print("Humidity: ");  // Display humidity
//   lcd.print(humidityValue);

//   delay(1000);
// }
