#include <LiquidCrystal_I2C.h>
 
// Konstruk object LCD dengan alamat I2C
// Ganti 0x3F sesuai dengan alamat I2C modul kalian
// Jika tidak tahu dapat menggunakan LCD I2C Scanner
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
void setup() {
 
  // Pemanggilan pertama memerlukan parameter jumlah kolom dan baris
  // Ini harus sama dengan yang dimasukan pada konstruktor.
  lcd.begin(16,2);
  lcd.init();
 
  // Nyalakan backlight
  lcd.backlight();

  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("HALO SEMUA! ^_^");
  lcd.setCursor(0, 1);      
  lcd.print("__SLIDESENTRY__");
  
  delay(3500);

  for(int j = 1; j<= 16; j++) {
    lcd.scrollDisplayLeft();
    delay(100);
  }
}
 
void loop() {
  
  // Info pertama
  // lcd.home();
  // lcd.clear();
  // lcd.setCursor(0, 0);
 
  // lcd.print("HALO SEMUA! ^_^");
  // lcd.setCursor(0, 1);      
  // lcd.print("__SLIDESENTRY__");
  
  // delay(3000);

  // for(int j = 1; j<= 16; j++) {
  //   lcd.scrollDisplayLeft();
  //   delay(100);
  // }

  // info kedua
  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Moisture lv: 102");
  lcd.setCursor(0,1);
  lcd.print("rain sensor: 1");
  
  delay(2500);
  lcd.clear();
}