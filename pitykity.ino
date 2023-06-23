#include "defines.h"


void setup() {
    Serial.begin(115200);
    // Internet baglatisi wifi sifresi ve mevcut default
    // durumlar gecerlidir

    thingsboard_setup();
    set_web_server();
    led_pin_setup();
    ditance_sensor_setup();
    step_set();
    file_setup();
}

void loop() {
    // Mesafeye gore ledleri yakar ve sondurur
    led_distance(ditance_sensor_get());
    thingsboard_loop();
    get_web_server();
    delay(1000);
}
