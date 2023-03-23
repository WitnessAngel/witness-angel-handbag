#include "vector.h"

t_vector* vecnew(size_t def_alloc, size_t data_size, size_t realloc_size)
{
    struct s_vector* ret;

    if (!data_size)
        return (0);
    if (realloc_size == 0)
        realloc_size = 32;
    ret = calloc(1, sizeof(struct s_vector));
    if (!ret)
        return (0);
    ret->data = calloc(def_alloc, data_size);
    if (!ret->data)
    {
        free(ret);
        return (0);
    }
    ret->capacity = def_alloc;
    ret->length = 0;
    ret->realloc_size = realloc_size;
    ret->data_size = data_size;
    return (ret);
}
