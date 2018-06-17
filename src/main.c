#include <stdio.h>

#include "vect.h"

int main(int argc, char** argv) {
    int_array arr = int_array_new(1); // capacity = 1
    
    printf("\n\nArray pushing\n=============\n");
    int_array_push(&arr, 10);
    printf("Current count: %d\n", arr.count);
    printf("Current capacity: %d\n", arr.capacity);

    int_array_push(&arr, 20);
    printf("Current count: %d\n", arr.count);
    printf("Current capacity: %d\n", arr.capacity);

    int_array_push(&arr, 30);
    printf("Current count: %d\n", arr.count);
    printf("Current capacity: %d\n", arr.capacity);

    int_array_push(&arr, 50);
    printf("Current count: %d\n", arr.count);
    printf("Current capacity: %d\n", arr.capacity);

    int_array_push(&arr, 100);
    printf("Current count: %d\n", arr.count);
    printf("Current capacity: %d\n", arr.capacity);

    int_array_push(&arr, 10);
    printf("Current count: %d\n", arr.count);
    printf("Current capacity: %d\n", arr.capacity);
    printf("=============");

    printf("\n\nArray indexing\n=============\n");
    printf("#1: %d\n", int_array_get(&arr, 0));
    printf("#2: %d\n", int_array_get(&arr, 1));
    printf("#3: %d\n", int_array_get(&arr, 2));
    printf("#4: %d\n", int_array_get(&arr, 3));
    printf("#5: %d\n", int_array_get(&arr, 4));
    printf("#6: %d\n", int_array_get(&arr, 5));
    printf("=============");

    printf("\nRemoved %d\n", int_array_remove(&arr, 0));
    printf("Count: %d\n", arr.count);
    printf("#1 now: %d\n", int_array_get(&arr, 0));

    printf("\nRemoved %d\n", int_array_remove(&arr, 0));
    printf("Count: %d\n", arr.count);
    printf("#1 now: %d\n", int_array_get(&arr, 0));
}
