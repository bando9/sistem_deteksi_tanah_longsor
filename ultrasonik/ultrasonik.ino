// Library Ultrasonic untuk sensor HC-SR04
#include <Ultrasonic.h>

// Definisikan pin trigger dan echo untuk sensor ultrasonik
const int triggerPin = D1; // GPIO pin D1 pada NodeMCU
const int echoPin = D2;    // GPIO pin D2 pada NodeMCU

// Buat objek Ultrasonic
Ultrasonic ultrasonic(triggerPin, echoPin);

void setup() {
  Serial.begin(9600);
  Serial.println("Sensor Ultrasonik HC-SR04");
}

void loop() {
  // Baca jarak dari sensor ultrasonik (dalam cm)
  float distance = ultrasonic.read();

  // Tampilkan hasil pembacaan di Serial Monitor
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000); // Beri jeda 1 detik antara setiap pembacaan
}