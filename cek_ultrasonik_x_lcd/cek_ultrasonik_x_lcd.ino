#include <LiquidCrystal_I2C.h>

#define LEDg D0
#define echoPin D3
#define trigPin D4

LiquidCrystal_I2C lcd(0x27, 16, 2);

float duration;
float distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  // nyalakan lcd
  lcd.begin(16,2);
  lcd.init();

  lcd.backlight();

  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);

  lcd.print("Hello World! ^_^");
  lcd.setCursor(0,1);
  lcd.print("-slidesentry-");

  delay(5000);

  for(int j=1; j<=16; j++) {
    lcd.scrollDisplayLeft();
    delay(100);
  }

}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  
  distance = duration*0.034/2;
  Serial.print("Distance: ");
  Serial.println(distance);

  // tampilkan ke lcd
  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Distance: ");
  lcd.println(distance);
  lcd.print(" cm");

  if(distance>=100) {
    Serial.print("Jauh");
    lcd.setCursor(0,1);
    lcd.print("note: Jauh");
  } else if(distance<100) {
    Serial.print("Deket\n");
    lcd.setCursor(0,1);
    lcd.print("note: Deket");
  };

  delay(2000);
  lcd.clear();

}