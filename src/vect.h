#ifndef VECT_H_INCLUDED
#define VECT_H_INCLUDED

#include <string.h>

#define ARRAY(type)                     \
    struct type ## _array {             \
        int capacity;                   \
        int count;                      \
        type* items;                    \
    }                  

#define ARRAY_FUNCS_DECL(type)                                                          \
    void        type ## _array_free   (struct type ## _array* array);                   \
    type        type ## _array_get    (const struct type ## _array* array, int index);  \
    struct type ## _array type ## _array_new    (int capacity);                         \
    type        type ## _array_pop    (struct type ## _array* array);                   \
    void        type ## _array_push   (struct type ## _array* array, type item);        \
    type        type ## _array_remove (struct type ## _array* array, int index);

#define ARRAY_FUNCS_IMPL(type)                                                \
/* Creates a new array. `capacity` sets the initial capacity of the array. */ \
struct type ## _array type ## _array_new(int capacity) {                      \
    int cap = capacity * sizeof(type);                                        \
    type* cont = (type*)malloc(cap);                                          \
                                                                              \
    struct type ## _array new_array;                                          \
    new_array.count = 0;                                                      \
    new_array.items = cont;                                                   \
    new_array.capacity = capacity;                                            \
                                                                              \
    return new_array;                                                         \
}                                                                             \
                                                                              \
/* Pushes an element into the array. */                                       \
void type ## _array_push(struct type ## _array* arr, type item) {             \
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
type type ## _array_get(const struct type ## _array* arr, int index) {        \
    /*if (index > arr->count) return NULL;*/                                  \
    return arr->items[index];                                                 \
}                                                                             \
                                                                              \
/* Removes the last element from the array */                                 \
type type ## _array_pop(struct type ## _array* arr) {                         \
    return type##_array_remove(arr, arr->count-1);                            \
}                                                                             \
                                                                              \
/* Frees the content held by the array. Sets the pointer to NULL. */          \
void type ## _array_free(struct type ## _array* arr) {                        \
    free(arr->items);                                                         \
    arr->items = 0;                                                           \
    arr->count = 0;                                                           \
    arr->capacity = 0;                                                        \
}                                                                             \
                                                                              \
/* Removes an item from the array and returns said item. */                   \
type type ## _array_remove(struct type ## _array* arr, int index) {           \
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
void type ## _array_drain(struct type ## _array* arr) {                       \
    memset(arr->items, 0, arr->capacity * sizeof(type));                      \
    arr->count = 0;                                                           \
}                                                                             \
                                                                              \
/* Shrinks the capacity of the array to exactly the length of it */           \
void type ## _array_shrink(struct type ## _array* arr) {                      \
    arr->items = realloc(arr->items, arr->count * sizeof(type));              \
}                                                                             \
                                                                              \
/* Extends the capacity of the array by n. */                                 \
void type ## _array_extend(struct type ## _array* arr, unsigned int n) {      \
    unsigned int new_capacity = n + arr->capacity;                            \
    arr->items = realloc(arr->items, new_capacity * sizeof(type));            \
    arr->capacity = new_capacity;                                             \
}

/*
typedef ARRAY(int) int_array;
typedef ARRAY(char) char_array;
typedef ARRAY(long) long_array;

ARRAY_FUNCS_DECL(int);
ARRAY_FUNCS_DECL(char);
ARRAY_FUNCS_DECL(long);

// definitely not recommended to put in headers
ARRAY_FUNCS_IMPL(int);
ARRAY_FUNCS_IMPL(char);
ARRAY_FUNCS_IMPL(long);
*/

#endif
