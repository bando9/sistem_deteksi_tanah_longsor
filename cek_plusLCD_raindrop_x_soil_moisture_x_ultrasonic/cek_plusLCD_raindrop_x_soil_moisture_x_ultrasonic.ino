// LCD
#include <LiquidCrystal_I2C.h>

// SCL (LCD) = D1
// SDA (LCD) = D2

LiquidCrystal_I2C lcd(0x27, 16, 2);

// indikator LED 
#define LEDr D7         // Deklarasi LED red pada pin D7 NodeMcu
#define LEDy D6         // Deklarasi LED kuning pada pin D6 NodeMcu
// #define LEDg D0      // Deklarasi LED hijau pada pin D4 NodeMcu

// ultrasonik
#define echoPin D3  // Deklarasi sensor ultrasonic echo pada pin D3
#define trigPin D4  // Deklarasi sensor ultrasonic trigger pada pin D4

// sensor raindrop
#define sensor_hujan D5 //Deklarasi Sensor hujan D0 pada pin D5 NodeMcu

// sensor soil moisture
#define sense_Pin D8    // Deklarasi Sensor kelembaban pada pin D8 NodeMcu


// inisiasi variabel
int value = 0;  // lembab

float duration; // waktu ultrasonik
float distance; // jarak ultrasonik

void setup () {
  // deklarasi sensor ultrasonik
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // tampilkan pada serial monitor dengan baud rate 9600
  Serial.begin(9600);

  // deklarasi 
  pinMode (sensor_hujan, INPUT); // Deklarasi sensor hujan sebagai input

  // pinMode (LEDg, OUTPUT);   // Deklarasi LED Hijau sebagai Output
  pinMode (LEDy, OUTPUT);     // Deklarasi LED kuning sebagai Output
  pinMode (LEDr, OUTPUT);     // Deklarasi LED Merah sebagai Output

  // tampilan awal LCD
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();

  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);

  lcd.print("Hello World!");
  lcd.setCursor(0,1);
  lcd.print("- slidesenrty -");
  delay(5000);  // jeda 5 detik di tampilan home

  // tampilan LCD geser ke kiri  
  for(int j=1; j<=16; j++){
    lcd.scrollDisplayLeft();
    delay(100);
  }
}
  
void loop() {

  // sensor ultrasonic
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  
  // sensor raindrop
  int kondisi_sensor = digitalRead(sensor_hujan); //Instruksi untuk membaca nilai digital
  Serial.print("Raindrop Value: ");      //Instruksi untuk menampilkan nilai digital pada Serial Monitor
  Serial.println(kondisi_sensor);

  // sensor soil moisture
  Serial.print("MOISTURE LEVEL : ");
  value= analogRead(sense_Pin);
  value= value/10;
  Serial.println(value);

  // sensor ultrasonic
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm\n");

  //Intruksi untuk mengaktifkan LED merah saat sensor mendeteksi hujan dan lembab
  if (kondisi_sensor == 0 && value<50){ //Saat hujan terdeteksi maka nilai digital = 0                 
    digitalWrite(LEDr, HIGH);
    digitalWrite(LEDy, LOW);
    // digitalWrite(LEDm,HIGH);
    Serial.print("Hujan dan Lembab\n");
  }

  //Intruksi untuk mengaktifkan LED kuning saat sensor tidak mendeteksi hujan, namun lembab
  else if (kondisi_sensor != 0 && value<50) {
    digitalWrite(LEDr, LOW);
    digitalWrite(LEDy, HIGH);
    // digitalWrite(LEDg,LOW);
    Serial.print("Lembab\n");
  }

  //Intruksi untuk mengaktifkan LED kuning saat sensor mendeteksi hujan, namun tidak lembab
  else if (kondisi_sensor == 0 && value>50) {
    digitalWrite(LEDr, LOW);
    digitalWrite(LEDy, HIGH);
    // digitalWrite(LEDg, LOW);
    Serial.print("Hujan\n");
  }
  
  //Intruksi untuk mengaktifkan LED hijau saat sensor mendeteksi tidak hujan dan tidak lembab
  else{
    digitalWrite(LEDr,LOW); //Saat hujan tidak terdeteksi maka nilai digital = 1
    digitalWrite(LEDy, LOW);
    // digitalWrite(LEDg,HIGH);
    Serial.print("Kering\n");
  }

  // tampilkan ke lcd
  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dist: ");    // tampilkan jarak
  lcd.println(distance);
  lcd.setCursor(0,1);
  lcd.print("lembab: "); // tampilkan lembab
  lcd.print(sense_Pin);
  delay(3000);

  // tampilkan ke lcd x2
  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("rain value: ");    // tampilkan jarak
  lcd.println(kondisi_sensor);
  lcd.setCursor(0,1);
  lcd.print("hasil"); // tampilkan lembab
 
  delay(2000);
  lcd.clear();
}