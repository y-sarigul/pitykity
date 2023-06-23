#include "defines.h"

void led_pin_setup()
{
    pinMode(LED_kirmizi, OUTPUT);
    pinMode(LED_mavi1, OUTPUT);
    pinMode(LED_mavi2, OUTPUT);
    pinMode(LED_mavi3, OUTPUT);
}

void led_distance(int dis)
{
    int fullness;

    fullness = map(dis, 3, 25, 0, 100);

    if(fullness <= 100 && fullness >= 77){
        // buzzer kodu eklenecek
        empty_buzzer();
        digitalWrite(LED_kirmizi, HIGH);
        digitalWrite(LED_mavi1, LOW);
        digitalWrite(LED_mavi2, LOW);
        digitalWrite(LED_mavi3, LOW);
    }else if(fullness < 72 && fullness >= 52){
        digitalWrite(LED_kirmizi, LOW);
        digitalWrite(LED_mavi1, HIGH);
        digitalWrite(LED_mavi2, LOW);
        digitalWrite(LED_mavi3, LOW);
    }else if(fullness < 47 && fullness >= 27){
        digitalWrite(LED_kirmizi, LOW);
        digitalWrite(LED_mavi1, HIGH);
        digitalWrite(LED_mavi2, HIGH);
        digitalWrite(LED_mavi3, LOW);
    }else if(fullness < 22 && fullness >= 3){
        digitalWrite(LED_kirmizi, LOW);
        digitalWrite(LED_mavi1, HIGH);
        digitalWrite(LED_mavi2, HIGH);
        digitalWrite(LED_mavi3, HIGH);
    }
    else {
        digitalWrite(LED_kirmizi, HIGH);
        digitalWrite(LED_mavi1, LOW);
        digitalWrite(LED_mavi2, LOW);
        digitalWrite(LED_mavi3, LOW);
    }
}
