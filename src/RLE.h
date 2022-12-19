#ifndef RLE_HEADER
#define RLE_HEADER

#include "byte_buffer.h"

int RLE_encode(byte_buffer_t *src, byte_buffer_t *dst);

int RLE_decode(byte_buffer_t *src, byte_buffer_t *dst);

int RLE_pack_bits(byte_buffer_t *src, byte_buffer_t *dst);

int RLE_unpack_bits(byte_buffer_t *src, byte_buffer_t *dst);

#endif //RLE_HEADER