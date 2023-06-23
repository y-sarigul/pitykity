#include "defines.h"

const int trigPin = 23; // Tetikleme pini
const int echoPin = 22; // Yankı pini

long duration; // Ultrasonic sensörde gönderilen ve alınan ses dalgası arasında geçen süre
float mesafeCm;
float distanceInch;

void ditance_sensor_setup()
{
    pinMode(trigPin, OUTPUT); // Tetikleme pinini çıkış olarak ayarla
    pinMode(echoPin, INPUT); // Yankı pinini giriş olarak ayarla
}

int ditance_sensor_get()
{
    // Sinyali temiz bir şekilde göndermek için tetikleme pinin LOW yap
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Tetikleme pinini yükselt 10 mikrosaniye bekle ve düşür
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // RYankı pini ile gönderilen ses dalgasının kaç saniyede geri döndüğünü hesapla 
    duration = pulseIn(echoPin, HIGH);

    // Mesafeyi hesapla
    mesafeCm = duration * SES_HIZI/2;
    // Mesafeyi Serial ekranda yazdır 
    Serial.print("Mesafe (cm): ");
    Serial.println(mesafeCm);
    return (mesafeCm);
}
