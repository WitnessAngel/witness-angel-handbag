#include "vector.h"

void vecalloc(t_vector* vec, size_t alloc_size)
{
    void* tmp;

    if (!vec)
        return;
    tmp = vec->data;
    vec->data = calloc((vec->length + alloc_size), vec->data_size);
    if (!vec->data)
    {
        vec->data = tmp;
        return;
    }
    memcpy(vec->data, tmp, vec->data_size * vec->length);
    vec->length += alloc_size;
    free(tmp);
}
