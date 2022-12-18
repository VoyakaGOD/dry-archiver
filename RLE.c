#include "RLE.h"

int RLE_encode(byte_buffer_t *src, byte_buffer_t *dst)
{
    byte_t current = byte_buffer_read(src);
    if (src->size == 0)
        return 0;
    byte_t count = 0;

    while (1)
    {
        byte_t byte = byte_buffer_read(src);
        if (src->size == 0)
            break;
        if (byte == current && count < (byte_t)255)
        {
            count++;
            continue;
        }
        byte_buffer_write(dst, count);
        byte_buffer_write(dst, current);
        current = byte;
        count = 0;
    }

    byte_buffer_write(dst, count);
    byte_buffer_write(dst, current);
    byte_buffer_unload(dst);
    return 0;
}

int RLE_decode(byte_buffer_t *src, byte_buffer_t *dst)
{
    while (1)
    {
        byte_t count = byte_buffer_read(src);
        if (src->size == 0)
            break;
        byte_t byte = byte_buffer_read(src);
        if (src->size == 0)
            return 1;

        while (1)
        {
            byte_buffer_write(dst, byte);
            if ((count--) == 0)
                break;
        }
    }

    byte_buffer_unload(dst);
    return 0;
}

int RLE_pack_bits(byte_buffer_t *src, byte_buffer_t *dst)
{
    byte_t previous = byte_buffer_read(src);
    if (src->size == 0)
        return 0;
    byte_t current = byte_buffer_read(src);
    if (src->size == 0)
    {
        byte_buffer_write(dst, 0);
        byte_buffer_write(dst, previous);
    }
    byte_t count = 0;
    byte_t sequence[128] = {};

    while (1)
    {
        if (previous == current)
        {
            while (previous == current)
            {
                count++;
                if (count == 128)
                {
                    byte_buffer_write(dst, 127);
                    byte_buffer_write(dst, previous);
                    count = 0;
                }
                previous = current;
                current = byte_buffer_read(src);
                if (src->size == 0)
                    break;
            }
            byte_buffer_write(dst, count);
            byte_buffer_write(dst, previous);
        }

        if (src->size == 0)
            break;

        if (count == 0)
            sequence[count++] = previous;
        else
            count = 0;
        while (previous != current)
        {
            if (count == 128)
            {
                byte_buffer_write(dst, 255);
                byte_buffer_write_bytes(dst, sequence, count);
                count = 0;
            }
            sequence[count++] = current;
            previous = current;
            current = byte_buffer_read(src);
            if (src->size == 0)
                break;
        }
        if (src->size != 0)
            count--;
        if (count > 0 || (src->size == 0))
        {
            byte_buffer_write(dst, count + 127);
            byte_buffer_write_bytes(dst, sequence, count);
            count = 0;
        }
    }

    byte_buffer_unload(dst);
    return 0;
}

int RLE_unpack_bits(byte_buffer_t *src, byte_buffer_t *dst)
{
    while (1)
    {
        byte_t count = byte_buffer_read(src);
        if (src->size == 0)
            break;
        byte_t skip = count & 0x80;

        if (skip)
        {
            count = count - 127;
            while (count > 0)
            {
                byte_buffer_write(dst, byte_buffer_read(src));
                if (src->size == 0)
                    return 1;
                count--;
            }
        }
        else
        {
            byte_t byte = byte_buffer_read(src);
            if (src->size == 0)
                return 1;
            while (1)
            {
                byte_buffer_write(dst, byte);
                if ((count--) == 0)
                    break;
            }
        }
    }

    byte_buffer_unload(dst);
    return 0;
}