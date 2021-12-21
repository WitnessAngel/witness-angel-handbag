#include <stdio.h>

#include "utils.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

size_t memcat(uint8_t* dst, struct memdesc* descriptors,
              size_t descriptor_count, size_t max_len)
{
    size_t remaining = max_len;
    uint8_t* next = dst;

    for (size_t i = 0; i < descriptor_count; i++)
    {
        struct memdesc* desc = &descriptors[i];
        size_t copy_len = MIN(remaining, desc->length);

        memcpy(next, desc->ptr, copy_len);
        next += copy_len;
        remaining -= copy_len;

        if (remaining <= 0)
            break;
    }

    return (max_len - remaining);
}

uint32_t find_prev_multiple_of(uint32_t n, uint32_t multiple_to_check)
{
    while (multiple_to_check % n)
        multiple_to_check--;
    return (multiple_to_check);
}

void* uint8_ncpy(uint8_t* dest, const uint8_t* src, size_t src_length,
                 size_t dest_length)
{
    for (size_t i = 0; i < src_length; i++)
    {
        dest[i] = src[i];
    }
    for (size_t i = src_length; i < dest_length; i++)
    {
        dest[i] = 0;
    }
    return (dest);
}
