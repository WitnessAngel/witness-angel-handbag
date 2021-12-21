#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define OBJ_MEMDESC(o)                                                         \
    {                                                                          \
        sizeof(o), (uint8_t*)&(o)                                              \
    }
#define DESC_COUNT(descs) (sizeof(descs) / sizeof((descs)[0]))

struct memdesc
{
    size_t length;
    uint8_t* ptr;
};

/*
 * memcat - concatenate several regions of memory into one region
 *
 *  dst - the destination pointer
 *  descriptors - an ordered array of memory descriptors to be copied
 *  descriptor_count - the number of descriptors in the array
 *  max_len - the maximum number of bytes to copy into dst
 */
size_t memcat(uint8_t* dst, struct memdesc* descriptors,
              size_t descriptor_count, size_t max_len);

uint32_t find_prev_multiple_of(uint32_t n, uint32_t multiple_to_check);

void* uint8_ncpy(uint8_t* dest, const uint8_t* src, size_t src_length,
                 size_t dest_length);
