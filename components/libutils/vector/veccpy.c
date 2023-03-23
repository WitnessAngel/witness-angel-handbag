#include "vector.h"

t_vector* veccpy(const t_vector* const vec)
{
    t_vector* ret;

    if (!vec)
        return (0);
    ret = malloc(sizeof(struct s_vector));
    if (!ret)
        return (0);
    ret->length = vec->length;
    ret->capacity = vec->capacity;
    ret->realloc_size = vec->realloc_size;
    ret->data = malloc(ret->capacity);
    if (!ret->data)
    {
        free(ret);
        return (0);
    }
    memcpy(ret->data, vec->data, ret->length);
    return (ret);
}
