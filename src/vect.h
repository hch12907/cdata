#ifndef VECT_H_INCLUDED
#define VECT_H_INCLUDED

#include <stdlib.h>

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



/*
typedef ARRAY(int) int_array;
typedef ARRAY(char) char_array;
typedef ARRAY(long) long_array;

ARRAY_FUNCS_DECL(int, int_array);
ARRAY_FUNCS_DECL(char, char_array);
ARRAY_FUNCS_DECL(long, long_array);
*/

#endif