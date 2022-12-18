#include "byte_buffer.h"

int byte_buffer_init(byte_buffer_t *buffer, const char *filename, long max_buffer_size, const char *mode)
{
    if(max_buffer_size < 1)
        max_buffer_size = 1;
    
    buffer->file = fopen(filename, mode);
    if(!buffer->file)
        return 1;
    buffer->content = (byte_t *)calloc(max_buffer_size, sizeof(byte_t));
    if(!buffer->content)
        return 2;
    buffer->index = 0;
    buffer->size = 0;
    buffer->max_size = max_buffer_size;
    return 0;
}

void byte_buffer_release(byte_buffer_t *buffer)
{
    fclose(buffer->file);
    free(buffer->content);
}

void byte_buffer_load(byte_buffer_t *buffer)
{
    buffer->size = fread(buffer->content, sizeof(byte_t), buffer->max_size, buffer->file);
    buffer->index = 0;
}

void byte_buffer_unload(byte_buffer_t *buffer)
{
    fwrite(buffer->content, sizeof(byte_t), buffer->index, buffer->file);
    buffer->index = 0;
}

void byte_buffer_write(byte_buffer_t *buffer, byte_t byte)
{
    if(buffer->index == buffer->max_size)
        byte_buffer_unload(buffer);
    buffer->content[buffer->index++] = byte;
}

void byte_buffer_write_bytes(byte_buffer_t *buffer, byte_t *bytes, long count)
{
    if(count >= buffer->max_size - buffer->index)
    {
        byte_buffer_unload(buffer);
        fwrite(bytes, sizeof(byte_t), count, buffer->file);
    }
    else
    {
        while((count--) > 0)
            buffer->content[buffer->index++] = *(bytes++);
    }
}

byte_t byte_buffer_read(byte_buffer_t *buffer)
{
    if(buffer->index == buffer->size)
    {
        byte_buffer_load(buffer);
        if(buffer->size == 0)
            return 0;
    }
    return buffer->content[buffer->index++];
}