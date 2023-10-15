#include <LiquidCrystal_I2C.h>

#define sensor_hujan D5  //Deklarasi Sensor hujan D0 pada pin D5 NodeMcu
#define LEDh D6             //Deklarasi LED hijau pada pin D6 NodeMcu
#define LEDm D7            //Deklarasi LED merah pada pin D7 NodeMcu
// #define LEDy D4            // Deklarasi LED kuning pada pin D4 NodeMcu

// sensor soil moisture
#define sense_Pin D8  //Deklarasi Sensor hujan pada pin D5 NodeMcu

// sensor ultrasonic
#define echoPin D3
#define trigPin D4

// note: free D0


// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// inisiasi 
int value = 0;

float duration;
float distance;

void setup () {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  pinMode (sensor_hujan, INPUT); //Deklarasi sensor hujan sebagai input
  pinMode (LEDh, OUTPUT);        //Deklarasi LED hijau sebagai Output
  pinMode (LEDm, OUTPUT);        //Deklarasi LED merah sebagai Output
  pinMode (LEDy, OUTPUT);

  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();

  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);

  lcd.print("Hello World!");
  lcd.setCursor(0,1);
  lcd.print("- slidesenrty -");
  delay(5000);

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

  // tampilkan sensor ultrasonic
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm\n");

  //Intruksi untuk mengaktifkan LED hijau saat sensor mendeteksi hujan
  if (kondisi_sensor == 0 && value<50){ //Saat hujan terdeteksi maka nilai digital = 0                 
    digitalWrite(LEDh, LOW);
    digitalWrite(LEDy, LOW);
    digitalWrite(LEDm,HIGH);
    Serial.print("Rainy and humid\n");
  } 

  //Intruksi untuk mengaktifkan LED kuning saat sensor tidak mendeteksi hujan, namun lembab
  else if (kondisi_sensor != 0 && value<50) {
    digitalWrite(LEDh, LOW);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDm,LOW);
    Serial.print("humid\n");
  }

  //Intruksi untuk mengaktifkan LED kuning saat sensor mendeteksi hujan, namun tidak lembab
  else if (kondisi_sensor == 0 && value>50) {
    digitalWrite(LEDh, LOW);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDm, LOW);
    Serial.print("rainy\n");
  }
  
  //Intruksi untuk mengaktifkan LED merah saat sensor mendeteksi tidak hujan dan tidak lembab
  else{
    digitalWrite(LEDh,HIGH); //Saat hujan tidak terdeteksi maka nilai digital = 1
    digitalWrite(LEDy, LOW);
    digitalWrite(LEDm,LOW);
    Serial.print("Dry\n");
  } 

  // tampilkan ke lcd
  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dist: ");    // tampilkan jarak
  lcd.println(distance);
  lcd.setCursor(0,1);
  lcd.print("lembab"); // tampilkan lembab
  lcd.println(sense_Pin);

  // tampilkan ke lcd x2
  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("rain value: ");    // tampilkan jarak
  lcd.println(kondisi_sensor);
  lcd.setCursor(0,1);
  lcd.print("contoh"); // tampilkan lembab
 
  delay(2000);
  lcd.clear();
}