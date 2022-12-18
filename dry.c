#include <string.h>
#include "RLE.h"

int main(int argc, char **argv)
{
    if(argc != 4)
    {
        printf("dry [-c|-d] {in} {out}\n");
        return 0;
    }

    int error = 0;
    FILE *input = fopen(argv[2], "rb");
    FILE *output = fopen(argv[3], "wb");

    if(!strcmp(argv[1], "-c"))
    {
        error = RLE_encode(input, output, 1024);
    }
    else if(!strcmp(argv[1], "-d"))
    {
        error = RLE_decode(input, output, 1024);
    }
    else
    {
        printf("Unexpected mode!");
        error = 1;
    }

    fclose(input);
    fclose(output);
    if(error)
        remove(output);
    return error;
}