#include "RLE.h"

int main()
{
    FILE *input = fopen("test_inp.hex", "rb");
    FILE *output = fopen("test_out.hex", "wb");
    if(RLE_encode(input, output, 1024))
        getchar();
}