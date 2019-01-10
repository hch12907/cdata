# Vec
A simple library for making variable-sized arrays.

## Usage
In your *.h file:
```c
// Include the library
#include "vec.h"

// In the below code, replace T with desired type/typedef.

// You can choose to typedef ARRAY(T) and replace it with the typedef too.
// typedef ARRAY(T) T_arr;

// This declares the functions for ARRAY(T).
ARRAY_FUNCS_DECL(T, ARRAY(T));
```

In your *.c file:
```c
// Include the above header
#include "path/to/that/header.h"

// Include the library
#include "vec.h"

// This shares the same arguments as ARRAY_FUNCS_DECL above.
// It defines/implements the functions needed for ARRAY(T).
ARRAY_FUNCS_IMPL(T, ARRAY(T));

int main() {
    // In the below code, replace T with desired type/typedef.

    // Creation of the arrays
    // T is the type of the array, the constant 10 is the initial capacity
    // The capacity grows as the more elements are pushed into the array
    ARRAY(T) arr = T_array_new(10);

    // Pushing items into the arrays.
    T_array_push(&arr, item);

    // Getting items from the arrays.
    T item = T_array_get(&arr, index);

    // Getting the last item from the array, and removing it in the process.
    T item = T_array_pop(&arr);

    // Removing items from the arrays.
    T returned_item = T_array_remove(&arr, index);

    // Extends the capacity of array by N.
    T_array_extend(&arr, 10);

    // Removes all elements in the array.
    T_array_drain(&arr);

    // Shrinks the capacity of the array to the length of it
    T_array_shrink(&arr);

    // Remember to use this before the array goes out of scope!
    // For your convenience, the function sets arr to NULL.
    T_array_free(&arr);
}
```
