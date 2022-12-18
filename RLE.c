#include "RLE.h"

static int check_files(FILE *src, FILE *dst)
{
    if(!src)
    {
        printf("Incorrect input file!");
        return 1;
    }
    if(!dst)
    {
        printf("Incorrect output file!");
        return 1;
    }
    return 0;
}

int RLE_encode(FILE *src, FILE *dst, long max_block_size)
{
    if(check_files(src, dst))
        return 1;

    byte_t *src_buffer = (byte_t *)calloc(max_block_size, sizeof(byte_t));
    byte_t *dst_buffer = (byte_t *)calloc(2 * max_block_size, sizeof(byte_t));

    while(1)
    {
        long src_index = 0;
        long dst_index = 0;
        long block_size = fread(src_buffer, sizeof(byte_t), max_block_size, src);
        if(block_size == 0)
            return 0;
        byte_t current = src_buffer[0];
        byte_t count = 1;

        for(long i = 1; i < block_size; i++)
        {
            if(src_buffer[i] == current && count < (byte_t)255)
            {
                count++;
                continue;
            }
            dst_buffer[dst_index++] = count;
            dst_buffer[dst_index++] = current;
            current = src_buffer[i];
            count = 1;
        }
        if(count > 0)
        {
            dst_buffer[dst_index++] = count;
            dst_buffer[dst_index++] = current;
        }

        fwrite(dst_buffer, sizeof(byte_t), dst_index, dst);
        if(block_size < max_block_size)
            break;
    }
    return 0;
}

int RLE_decode(FILE *src, FILE *dst, long max_block_size)
{
    return -100;
}