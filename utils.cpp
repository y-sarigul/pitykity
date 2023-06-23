#include "defines.h"

int get_fullness()
{
        int fullness;
        fullness = map(ditance_sensor_get(), 3, 25, 100, 0);
        return (fullness);
}
