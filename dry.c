#include <string.h>
#include "RLE.h"

int main(int argc, char **argv)
{
    if(argc != 4)
    {
        printf("dry [-c|-d|-p|-u] {in} {out}\n");
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
    else if(!strcmp(argv[1], "-p"))
    {
        error = RLE_pack_bits(input, output, 1024);
    }
    else if(!strcmp(argv[1], "-u"))
    {
        error = RLE_unpack_bits(input, output, 1024);
    }
    else
    {
        printf("Unexpected mode!");
        error = 1;
    }

    fclose(input);
    fclose(output);
    if(error)
        remove(argv[3]);
    return error;
}