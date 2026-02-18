#include "../thermocouple.h"
#include <stdio.h>

int main()
{
    float out = 0.0f;
    float v = -53.795f;
    TC_status_t status = TC_Volts2Temp(v, -200, &out);
    printf("STATUS: %i    V:%f    T:%f\n", status, v, out);
    return 0;
}