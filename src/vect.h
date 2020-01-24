#ifndef VECT_H_INCLUDED
#define VECT_H_INCLUDED

#include <string.h>

#define ARRAY(type)                     \
    struct type ## _array {             \
        int capacity;                   \
        int count;                      \
        type* items;                    \
    }                  

#define ARRAY_FUNCS_DECL(type, array_type)                                  \
    void       type ## _array_free   (array_type* array);                   \
    type       type ## _array_get    (const array_type* array, int index);  \
    array_type type ## _array_new    (int capacity);                        \
    type       type ## _array_pop    (array_type* array);                   \
    void       type ## _array_push   (array_type* array, type item);        \
    type       type ## _array_remove (array_type* array, int index);

#define ARRAY_FUNCS_IMPL(type, array_type)                                    \
/* Creates a new array. `capacity` sets the initial capacity of the array. */ \
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
/* Pushes an element into the array. */                                       \
void type ## _array_push(array_type* arr, type item) {                        \
    int new_count = arr->count + 1;                                           \
                                                                              \
    /* Double the capacity if count > capacity. */                            \
    if (new_count > arr->capacity) {                                          \
        int new_cap = (arr->capacity > 0) ? arr->capacity * 2 : 1;            \
        arr->items = (type*)realloc(arr->items, new_cap * sizeof(type));      \
        arr->capacity = new_cap;                                              \
    }                                                                         \
                                                                              \
    (arr->items)[arr->count] = item;                                          \
    arr->count = new_count;                                                   \
}                                                                             \
                                                                              \
/* Gets an element from the array */                                          \
type type ## _array_get(const array_type* arr, int index) {                   \
    /*if (index > arr->count) return NULL;*/                                  \
    return arr->items[index];                                                 \
}                                                                             \
                                                                              \
/* Removes the last element from the array */                                 \
type type ## _array_pop(array_type* arr) {                                    \
    return type##_array_remove(arr, arr->count-1);                            \
}                                                                             \
                                                                              \
/* Frees the content held by the array. Sets the pointer to NULL. */          \
void type ## _array_free(array_type* arr) {                                   \
    free(arr->items);                                                         \
    arr->items = 0;                                                           \
    arr = 0;                                                                  \
}                                                                             \
                                                                              \
/* Removes an item from the array and returns said item. */                   \
type type ## _array_remove(array_type* arr, int index) {                      \
    type removed_item = arr->items[index];                                    \
    memmove(arr->items + index, arr->items + index + 1, arr->count - index);  \
    arr->count -= 1;                                                          \
    if (arr->count < arr->capacity / 2) {                                     \
        int new_capacity = arr->capacity / 2;                                 \
        arr->items = (type*)realloc(arr->items, new_capacity);                \
        arr->capacity = new_capacity;                                         \
    }                                                                         \
    return removed_item;                                                      \
}                                                                             \
                                                                              \
/* Drains the array, removing all elements in it. Retains the capacity. */    \
void type ## _array_drain(array_type* arr) {                                  \
    memset(arr->items, 0, arr->capacity * sizeof(type));                      \
    arr->count = 0;                                                           \
}                                                                             \
                                                                              \
/* Shrinks the capacity of the array to exactly the length of it */           \
void type ## _array_shrink(array_type* arr) {                                 \
    arr->items = realloc(arr->items, arr->count * sizeof(type));              \
}                                                                             \
                                                                              \
/* Extends the capacity of the array by n. */                                 \
void type ## _array_extend(array_type* arr, unsigned int n) {                 \
    unsigned int new_capacity = n + arr->capacity;                            \
    arr->items = realloc(arr->items, new_capacity * sizeof(type));            \
    arr->capacity = new_capacity;                                             \
}

/*
typedef ARRAY(int) int_array;
typedef ARRAY(char) char_array;
typedef ARRAY(long) long_array;

ARRAY_FUNCS_DECL(int, int_array);
ARRAY_FUNCS_DECL(char, char_array);
ARRAY_FUNCS_DECL(long, long_array);

// definitely not recommended to put in headers
ARRAY_FUNCS_IMPL(int, int_array);
ARRAY_FUNCS_IMPL(char, char_array);
ARRAY_FUNCS_IMPL(long, long_array);
*/

#endif
