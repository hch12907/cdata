# Vec
A simple library for making variable-sized arrays.

## Usage
In your *.h file:
```c

```

In your *.c file:
```c
ARRAY
#include "vec.h"

int main() {
    // In the below code, replace T with desired type/typedef.

    // Creation of the arrays
    // T is the type of the array, the constant 10 is the capacity
    ARRAY(T) arr = T_array_new(10);

    // Pushing items into the arrays.
    T_array_push(&arr, item);

    // Getting items from the arrays.
    T item = T_array_get(&arr, index);

    // Getting the last item from the array, and removing it in the process.
    T item = T_array_get(&arr);

    // Removing items from the arrays.
    T returned_item = T_array_remove(&arr, index);

    // Remember to use this before the array goes out of scope!
    // For your convenience, the function sets arr to NULL.
    T_array_free(&arr);
}
```