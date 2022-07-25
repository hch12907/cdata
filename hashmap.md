# `hashmap.h`
A simple C header for hashmaps.

**NOTE:** There is a `hashmap_str.h` library available for people who which to
use a string as the key. It is a specialisation of this library. The differences
will be described in this document later.

## Usage
In your *.h file:
```c
// Include the library
#include "hashmap.h"

// In the below code, replace K, V with the desired type/typedef.
// Note that K and V can only be 1 token long, i.e. "unsigned long" is
// disallowed.
// To use those types, typedef them and supply the typedef to the macros. 

// This declares the structs hashmap_K_V and hashmap_K_V_entry.
// A typedef hashmap_K_V_error_code is created too.
//
// The main struct contains 3 fields: capacity, length, entries.
// `capacity`: the maximum capacity of the hashmap, when the length exceeds
//             the load factor (0.8) * capacity, the hashmap will grow.  
// `length`: the number of key-value pairs stored in this hashmap.
// `entries`: the pointer to a heap-allocated contiguous array, storing key-
//            value pairs.
//
// The error code shows whether an operation was completed successfully.
// 0 - success
// 1 - out of memory
// 2 - element not found
//
// The entry struct is an implementation detail.
//
// NOTE: it is insta-UB to mutate ANY field outside of the provided methods!
HASHMAP(K, V);

// This declares the functions for HASHMAP(K, V).
HASHMAP_FUNCS_DECL(K, V);
```

In your *.c file:
```c
// Include the above header
#include "path/to/that/header.h"

// In the below code, replace K, V with desired type/typedef.

// This defines/implements the functions needed for HASHMAP(K, V).
HASHMAP_FUNCS_IMPL(K, V, hasher, cmp);

int main() {
    // Creation and initialiation of the hashmap
    HASHMAP(K, V) map;
    hashmap_K_V_new(&map);

    // Alternatively, one can initialise the hashmap with greater capacity
    // to avoid frequent allocations. NOTE: the capacity must be a power of 2!
    HASHMAP(K, V) map_with_cap;
    hashmap_K_V_with_capacity(&map_with_cap, 16);

    // Inserting a value into the hashmap.
    const K key = make_key(); const V value = make_value();
    hashmap_K_V_error_code err = hashmap_K_V_insert(&map, key, value);

    // Getting items from the hashmap.
    const K key = make_key(); V value;
    hashmap_K_V_error_code err = hashmap_K_V_get(&map, &key, &value);

    // Removing items from the arrays.
    const K key = make_key(); V value;
    hashmap_K_V_error_code err = hashmap_K_V_remove(&map, &key, &value);

    // Remember to use this before the array goes out of scope!
    // For convenience, this function sets all fields to 0.
    hashmap_K_V_free(&map);
}
```

## Differences between `hashmap.h` and `hashmap_str.h`

* `HASHMAP_STR` instead of `HASHMAP`. This applies to everything prefixed with
  `HASHMAP`, so `HASHMAP_STR`, `HASHMAP_STR_FUNCS_DECL`, and so on.

* The key is fixed to `str`, for example `hashmap_str_V_free`.

* `hashmap_str_V_insert` accepts the key as a pointer, unlike its non-specialized
  counterpart which accepts it as a value. The key will be copied into the hashmap
  though - feel free to destroy the key after it has been inserted.

* `HASHMAP_STR_FUNCS_IMPL` doesn't require the `cmp` field. The `strcmp` function
  provided in the standard library is used instead.
