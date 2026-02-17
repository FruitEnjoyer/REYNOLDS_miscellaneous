#include "../thermocouple.h"
#include <stdio.h>

int main()
{
    float v = 53.795f;
    printf("V:%f    T:%f\n", v, TC_Volts2Temp(v, 10));
    return 0;
}