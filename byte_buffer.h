#ifndef BYTE_BUFFER_HEADER
#define BYTE_BUFFER_HEADER

#include <stdlib.h>
#include <stdio.h>

typedef unsigned char byte_t;

typedef struct 
{
    FILE *file;
    byte_t *content;
    long index;
    long size;
    long max_size;
} byte_buffer_t;

int byte_buffer_init(byte_buffer_t *buffer, const char *filename, long max_buffer_size, const char *mode);

int byte_buffer_release(byte_buffer_t *buffer);

void byte_buffer_load(byte_buffer_t *buffer);

void byte_buffer_unload(byte_buffer_t *buffer);

void byte_buffer_write(byte_buffer_t *buffer, byte_t byte);

void byte_buffer_write_bytes(byte_buffer_t *buffer, byte_t *bytes, long count);

byte_t byte_buffer_read(byte_buffer_t *buffer);


#endif //BYTE_BUFFER_HEADER