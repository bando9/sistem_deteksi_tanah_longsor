// cek raindrop
#define BLYNK_PRINT Serial
#define sensor D5

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Blynk char
char auth[] = "BmNHecm3Rsia6ZL7dQwpNo_Rg5zSDgBJ";
char ssid[] = "UIN_Walisongo-AX";
char pass[] = "";

// inisiasi variabel
int val;

void setup() {
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);

  Serial.println("siap digunakan");
  Blynk.notify("Rain Detector Ready");
}

void loop() {
  val = digitalRead(sensor);
  Blynk.run();
  Serial.println(val);
  if (val == 0) {
    Blynk.notify("hujan bro");
    Serial.println("Succes Send Notify");
  }
  delay(2000);
}