#define sensor_hujan D5  //Deklarasi Sensor hujan pada pin D5 NodeMcu
#define LEDh D6             //Deklarasi LED hijau pada pin D6 NodeMcu
#define LEDm D7            //Deklarasi LED merah pada pin D7 NodeMcu
  
void setup () {
 Serial.begin(9600);
 pinMode (sensor_hujan, INPUT); //Deklarasi sensor hujan sebagai input
 pinMode (LEDh, OUTPUT);        //Deklarasi LED hijau sebagai Output
 pinMode (LEDm, OUTPUT);        //Deklarasi LED merah sebagai Output
}
  
void loop() {
int kondisi_sensor = digitalRead(sensor_hujan); //Instruksi untuk membaca nilai digital
Serial.print("Raindrop Sensor\n");      
Serial.print("Digital Value: ");                //Instruksi untuk menampilkan nilai digital pada Serial Monitor
Serial.println(kondisi_sensor);
 
//Intruksi untuk mengaktifkan LED hijau saat sensor tidak mendeteksi hujan
if (kondisi_sensor == 0){ //Saat hujan terdeteksi maka nilai digital = 0                 
digitalWrite(LEDh, LOW);
digitalWrite(LEDm,HIGH);
Serial.print("Rain\n");
}
 
//Intruksi untuk mengaktifkan LED merah saat sensor tidak mendeteksi hujan
else{
digitalWrite(LEDh,HIGH); //Saat hujan tidak terdeteksi maka nilai digital = 1
digitalWrite(LEDm,LOW);
Serial.print("Dry\n");
} 
 delay(1000);
}