# Vec
A simple library for making variable-sized arrays.

## Usage
In your *.h file:
```c
// Include the library
#include "vec.h"

// In the below code, replace T with the desired type/typedef.
// Note that T can only be 1 token long, i.e. "unsigned long" is disallowed.
// To use those types, typedef them and supply the typedef to the macros. 

// This declares a struct of T_array.
// The struct contains 3 fields: capacity, count, items.
// `capacity`: the maximum capacity of the T_array, when it is exceeded the
//             array will reallocate automatically.
// `count`: the amount of elements stored in this array.
// `items`: a pointer to a heap-allocated contiguous array.
//
// NOTE: it is insta-UB to mutate ANY field outside of the provided methods!
ARRAY(T);

// You can choose to typedef ARRAY(T) too.
// typedef ARRAY(T) T_arr;

// This declares the functions for ARRAY(T).
ARRAY_FUNCS_DECL(T);
```

In your *.c file:
```c
// Include the above header
#include "path/to/that/header.h"

// In the below code, replace T with desired type/typedef.

// This defines/implements the functions needed for ARRAY(T).
ARRAY_FUNCS_IMPL(T);

int main() {
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
    // For convenience, this function sets all fields to 0.
    T_array_free(&arr);
}
```
