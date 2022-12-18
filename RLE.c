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

    if(max_block_size < 1)
        max_block_size = 1;
    byte_t *src_buffer = (byte_t *)calloc(max_block_size, sizeof(byte_t));
    byte_t *dst_buffer = (byte_t *)calloc(2 * max_block_size, sizeof(byte_t));

    while(1)
    {
        long dst_index = 0;
        long block_size = fread(src_buffer, sizeof(byte_t), max_block_size, src);
        if(block_size == 0)
            break;
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

    free(src_buffer);
    free(dst_buffer);
    return 0;
}

int RLE_decode(FILE *src, FILE *dst, long max_block_size)
{
    if(check_files(src, dst))
        return 1;

    if(max_block_size < 1)
        max_block_size = 1;
    max_block_size += max_block_size % 2;
    byte_t *src_buffer = (byte_t *)calloc(max_block_size, sizeof(byte_t));
    byte_t *dst_buffer = (byte_t *)calloc(max_block_size, sizeof(byte_t));

    while(1)
    {
        long dst_index = 0;
        long block_size = fread(src_buffer, sizeof(byte_t), max_block_size, src);
        if(block_size == 0)
            break;
        if(block_size % 2 != 0)
        {
            printf("Incorrect file size!");
            return 1;
        }

        for(long i = 0; i < block_size; i++)
        {
            byte_t count = src_buffer[i++];
            byte_t byte = src_buffer[i];
            while(count > 0)
            {
                if(dst_index == max_block_size)
                {
                    fwrite(dst_buffer, sizeof(byte_t), dst_index, dst);
                    dst_index = 0;
                }
                dst_buffer[dst_index++] = byte;
                count--;
            }
        }

        fwrite(dst_buffer, sizeof(byte_t), dst_index, dst);
        if(block_size < max_block_size)
            break;
    }

    free(src_buffer);
    free(dst_buffer);
    return 0;
}

int RLE_pack_bits(FILE *src, FILE *dst, long max_block_size)
{
    return -100;
}

int RLE_unpack_bits(FILE *src, FILE *dst, long max_block_size)
{
    if(check_files(src, dst))
        return 1;

    if(max_block_size < 1)
        max_block_size = 1;
    byte_t *src_buffer = (byte_t *)calloc(max_block_size, sizeof(byte_t));
    byte_t *dst_buffer = (byte_t *)calloc(max_block_size, sizeof(byte_t));

    while(1)
    {
        long dst_index = 0;
        long block_size = fread(src_buffer, sizeof(byte_t), max_block_size, src);
        if(block_size == 0)
            break;

        for(long i = 0; i < block_size;)
        {
            if(i == block_size)
            {
                if(block_size < max_block_size)
                    break;
                block_size = fread(src_buffer, sizeof(byte_t), max_block_size, src);
                i = 0;
            }
            byte_t count = src_buffer[i++];
            byte_t skip = count & 0x80;
            if(skip)
            {
                count = count - 127;
                while(count > 0)
                {
                    if(dst_index == max_block_size)
                    {
                        fwrite(dst_buffer, sizeof(byte_t), dst_index, dst);
                        dst_index = 0;
                    }
                    if(i == block_size)
                    {
                        if(block_size < max_block_size)
                        {
                            printf("Incorrect file data!");
                            free(src_buffer);
                            free(dst_buffer);
                            return 1;
                        }
                        block_size = fread(src_buffer, sizeof(byte_t), max_block_size, src);
                        i = 0;
                    }
                    dst_buffer[dst_index++] = src_buffer[i++];
                    count--;
                }
            }
            else
            {
                count++;
                byte_t byte = src_buffer[i++];
                while(count > 0)
                {
                    if(dst_index == max_block_size)
                    {
                        fwrite(dst_buffer, sizeof(byte_t), dst_index, dst);
                        dst_index = 0;
                    }
                    dst_buffer[dst_index++] = byte;
                    count--;
                }
            }
        }

        fwrite(dst_buffer, sizeof(byte_t), dst_index, dst);
        if(block_size < max_block_size)
            break;
    }

    free(src_buffer);
    free(dst_buffer);
    return 0;
}