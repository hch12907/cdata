#include "vect.h"

#define ARRAY_FUNCS_IMPL(type, array_type)                                    \
array_type type ## _array_new(int capacity) {                                 \
    int cap = capacity * sizeof(type);                                        \
    type* cont = (type*)malloc(cap);                                          \
                                                                              \
    array_type new_array;                                                     \
    new_array.count = 0;                                                      \
    new_array.items = cont;                                                   \
    new_array.capacity = capacity;                                            \
                                                                              \
    return new_array;                                                         \
}                                                                             \
                                                                              \
void type ## _array_push(array_type* arr, type item) {                        \
    int new_count = arr->count + 1;                                           \
                                                                              \
    /* Double the capacity if count > capacity. */                            \
    if (new_count > arr->capacity) {                                          \
        int new_cap = arr->capacity * 2;                                      \
        arr->items = (type*)realloc(arr->items, new_cap * sizeof(type));      \
        arr->capacity = new_cap;                                              \
    }                                                                         \
                                                                              \
    (arr->items)[arr->count] = item;                                          \
    arr->count = new_count;                                                   \
}                                                                             \
                                                                              \
type type ## _array_get(const array_type* arr, int index) {                   \
    /*if (index > arr->count) return NULL;*/                                  \
    return arr->items[index];                                                 \
}                                                                             \
                                                                              \
type type ## _array_pop(array_type* arr) {                                    \
    return type##_array_remove(arr, arr->count-1);                            \
}                                                                             \
                                                                              \
void type ## _array_free(array_type* arr) {                                   \
    free(arr->items);                                                         \
    arr->items = 0;                                                           \
    arr = 0;                                                                  \
}                                                                             \
                                                                              \
type type ## _array_remove(array_type* arr, int index) {                      \
    type removed_item = arr->items[index];                                    \
    memmove(arr->items + index, arr->items + index + 1, arr->count - index);  \
    arr->count -= 1;                                                          \
    if (arr->count < arr->capacity / 2) {                                     \
        int new_capacity = arr->capacity / 2;                                 \
        arr->items = (type*)realloc(arr->items, new_capacity);                \
        arr->capacity = new_capacity;                                         \
    } 
    return removed_item;                                                      \
}



// ARRAY_FUNS_IMPL(TYPE, FOR_ARRAY)
// ARRAY_FUNCS_IMPL(int, int_array);
// ARRAY_FUNCS_IMPL(char, char_array);
// ARRAY_FUNCS_IMPL(long, long_array);