#include "../lowpassfilter.h"
#include "stdio.h"


int main(int argc, char* argv[])
{
    FILE* filein;
    FILE* fileout;
    filein = fopen("data.log", "r");
    fileout = fopen("result.log", "w");
    int size = 0;
    float a = 0.0f;
    fscanf(filein, "%i", &size);

    lpfilter_t rectfilter, blackfilter;
    LPF_init(&rectfilter, RECTANGULAR);
    LPF_init(&blackfilter, BLACKMAN);


    for(size_t i = 0; i < size; ++i)
    {
        fscanf(filein, "%f", &a);
        //printf("%f   %f   %f\n", a, LPF_filter(&rectfilter, a), LPF_filter(&blackfilter, a));
        fprintf(fileout, "%f %f %f\n", a, LPF_filter(&rectfilter, a), LPF_filter(&blackfilter, a));
    }


    printf("\n");
    fclose(filein);
    fclose(fileout);
    return 0;
}