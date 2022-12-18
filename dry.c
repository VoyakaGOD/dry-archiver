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
    byte_buffer_t input = {};
    byte_buffer_t output = {};
    if(byte_buffer_init(&input, argv[2], 1024, "rb"))
    {
        printf("Incorrect input file!");
        return 1;
    }
    if(byte_buffer_init(&output, argv[3], 1024, "wb"))
    {
        printf("Incorrect output file!");
        return 1;
    }

    if(!strcmp(argv[1], "-c"))
    {
        error = RLE_encode(&input, &output);
    }
    else if(!strcmp(argv[1], "-d"))
    {
        error = RLE_decode(&input, &output);
    }
    else if(!strcmp(argv[1], "-p"))
    {
        error = RLE_pack_bits(&input, &output);
    }
    else if(!strcmp(argv[1], "-u"))
    {
        error = RLE_unpack_bits(&input, &output);
    }
    else
    {
        printf("Unexpected mode!");
        error = 1;
    }

    byte_buffer_release(&input);
    byte_buffer_release(&output);
    if(error)
        remove(argv[3]);
    return error;
}