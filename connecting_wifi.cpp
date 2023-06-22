// #include "pitykity.h"

// const char* ssid = "SOFTITO";         // Wi-Fi network name
// const char* password = "Soft2022iTO"; // Wi-Fi password
// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "pool.ntp.org", 10800, 3600);

// // setup
// void connecting_wifi(){
//     // Wi-Fi'ye bağlan
//     WiFi.begin(ssid, password);

//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }
//     Serial.println("Connected to WiFi");
// }

// void get_time_c(){
//     timeClient.update();

//     // Zaman bilgisini al
//     Serial.print("Current Time: ");
//     Serial.println(timeClient.getFormattedTime());
// }
