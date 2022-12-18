#ifndef RLE_HEADER
#define RLE_HEADER

#include <stdlib.h>
#include <stdio.h>

typedef unsigned char byte_t;

int RLE_encode(FILE *src, FILE *dst, long max_block_size);

int RLE_decode(FILE *src, FILE *dst, long max_block_size);

#endif //RLE_HEADER