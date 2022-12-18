#include "RLE.h"

int RLE_encode(byte_buffer_t *src, byte_buffer_t *dst)
{
    byte_t current = byte_buffer_read(src);
    if(src->size == 0)
        return 0;
    byte_t count = 0;

    while(1)
    {
        byte_t byte = byte_buffer_read(src);
        if(src->size == 0)
            break;
        if(byte == current && count < (byte_t)255)
        {
            count++;
            continue;
        }
        byte_buffer_write(dst, count);
        byte_buffer_write(dst, current);
        current = byte;
        count = 0;
    }
    if(count > 0)
    {
        byte_buffer_write(dst, count);
        byte_buffer_write(dst, current);
    }
    
    byte_buffer_unload(dst);
    return 0;
}

int RLE_decode(byte_buffer_t *src, byte_buffer_t *dst)
{
    while(1)
    {
        byte_t count = byte_buffer_read(src);
        if(src->size == 0)
            break;
        byte_t byte = byte_buffer_read(src);
        if(src->size == 0)
            return 1;
        
        while(1)
        {
            byte_buffer_write(dst, byte);
            if((count--) == 0)
                break;
        }
    }

    byte_buffer_unload(dst);
    return 0;
}

int RLE_pack_bits(byte_buffer_t *src, byte_buffer_t *dst)
{
    return -100;
}

int RLE_unpack_bits(byte_buffer_t *src, byte_buffer_t *dst)
{
    return -100;
}