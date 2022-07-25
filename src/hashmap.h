#ifndef HASHMAP_H_INCLUDED
#define HASHMAP_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>

#define HASHMAP(key_ty, val_ty)                                         \
    struct hashmap_ ## key_ty ## _ ## val_ty ## _entry {                \
        uint8_t alive;                                                  \
        key_ty key;                                                     \
        val_ty value;                                                   \
    };                                                                  \
                                                                        \
    struct hashmap_ ## key_ty ## _ ## val_ty {                          \
        size_t length;                                                  \
        size_t capacity;                                                \
        struct hashmap_ ## key_ty ## _ ## val_ty ## _entry *entries;    \
    };                                                                  \
                                                                        \
    typedef int hashmap_ ## key_ty ## _ ## val_ty ## _error_code;

/* some internal macros for convenience */
#define __HASHMAP(key_ty, val_ty) struct hashmap_ ## key_ty ## _ ## val_ty
#define __HASHENTRY(key_ty, val_ty) struct hashmap_ ## key_ty ## _ ## val_ty ## _entry
#define __HASHERROR(key_ty, val_ty) hashmap_ ## key_ty ## _ ## val_ty ## _error_code
#define __HASHFUNC(key_ty, val_ty, name) hashmap_ ## key_ty ## _ ## val_ty ## _ ## name

#define HASHMAP_FUNCS_DECL(key_ty, val_ty)                                                                                                  \
    __HASHERROR(key_ty, val_ty) __HASHFUNC(key_ty, val_ty, new)(__HASHMAP(key_ty, val_ty) *map);                                            \
    __HASHERROR(key_ty, val_ty) __HASHFUNC(key_ty, val_ty, with_capacity)(__HASHMAP(key_ty, val_ty) *map, uintptr_t cap);                   \
    __HASHERROR(key_ty, val_ty) __HASHFUNC(key_ty, val_ty, insert)(__HASHMAP(key_ty, val_ty) *map, key_ty key, val_ty value);               \
    __HASHERROR(key_ty, val_ty) __HASHFUNC(key_ty, val_ty, get)(const __HASHMAP(key_ty, val_ty) *map, const key_ty *key, val_ty *value);    \
    __HASHERROR(key_ty, val_ty) __HASHFUNC(key_ty, val_ty, remove)(__HASHMAP(key_ty, val_ty) *map, const key_ty *key, val_ty *value);       \
    void                        __HASHFUNC(key_ty, val_ty, free)(__HASHMAP(key_ty, val_ty) *map);

#define HASHMAP_FUNCS_IMPL(key_ty, val_ty, hasher, cmp)                                                                                     \
    __HASHERROR(key_ty, val_ty) __HASHFUNC(key_ty, val_ty, new)(__HASHMAP(key_ty, val_ty) *map) {                                           \
        return __HASHFUNC(key_ty, val_ty, with_capacity)(map, 4);                                                                           \
    }                                                                                                                                       \
                                                                                                                                            \
    /* NOTE: cap MUST be a power of 2 */                                                                                                    \
    __HASHERROR(key_ty, val_ty) __HASHFUNC(key_ty, val_ty, with_capacity)(__HASHMAP(key_ty, val_ty) *map, uintptr_t cap) {                  \
        size_t alloc_size = sizeof(__HASHENTRY(key_ty, val_ty)) * cap;                                                                      \
        __HASHENTRY(key_ty, val_ty) *new_entries = malloc(alloc_size);                                                                      \
                                                                                                                                            \
        if (!map)                                                                                                                           \
            return 1;                                                                                                                       \
                                                                                                                                            \
        map->capacity = cap;                                                                                                                \
        map->length = 0;                                                                                                                    \
        map->entries = new_entries;                                                                                                         \
                                                                                                                                            \
        for (uintptr_t i = 0; i < cap; i++)                                                                                                 \
            map->entries[i].alive = 0;                                                                                                      \
                                                                                                                                            \
        return 0;                                                                                                                           \
    }                                                                                                                                       \
                                                                                                                                            \
    __HASHERROR(key_ty, val_ty) _hashmap_ ## key_ty ## _ ## val_ty ## _double(__HASHMAP(key_ty, val_ty) *old_map) {                         \
        __HASHMAP(key_ty, val_ty) new_map;                                                                                                  \
                                                                                                                                            \
        __HASHERROR(key_ty, val_ty) result = __HASHFUNC(key_ty, val_ty, with_capacity)(&new_map, old_map->capacity * 2);                    \
                                                                                                                                            \
        if (result)                                                                                                                         \
            return result;                                                                                                                  \
                                                                                                                                            \
        for (uintptr_t i = 0; i < old_map->capacity; i++) {                                                                                 \
            __HASHENTRY(key_ty, val_ty) entry = old_map->entries[i];                                                                        \
                                                                                                                                            \
            if (!entry.alive)                                                                                                               \
                continue;                                                                                                                   \
                                                                                                                                            \
            __HASHFUNC(key_ty, val_ty, insert)(&new_map, entry.key, entry.value);                                                           \
        }                                                                                                                                   \
                                                                                                                                            \
        __HASHFUNC(key_ty, val_ty, free)(old_map);                                                                                          \
                                                                                                                                            \
        old_map->length = new_map.length;                                                                                                   \
        old_map->capacity = new_map.capacity;                                                                                               \
        old_map->entries = new_map.entries;                                                                                                 \
                                                                                                                                            \
        return 0;                                                                                                                           \
    }                                                                                                                                       \
                                                                                                                                            \
    __HASHERROR(key_ty, val_ty) __HASHFUNC(key_ty, val_ty, insert)(__HASHMAP(key_ty, val_ty) *map, key_ty key, val_ty value) {              \
        uint64_t hash = hasher(&key);                                                                                                       \
                                                                                                                                            \
        /* if the length of hashmap exceeds 80% of the capacity, expand the map */                                                          \
        if ((map->length + 1) * 5 >= map->capacity * 4) {                                                                                   \
            __HASHERROR(key_ty, val_ty) result = _hashmap_ ## key_ty ## _ ## val_ty ## _double(map);                                        \
                                                                                                                                            \
            if (result)                                                                                                                     \
                return result;                                                                                                              \
        }                                                                                                                                   \
                                                                                                                                            \
        uintptr_t index = hash & (map->capacity - 1);                                                                                       \
                                                                                                                                            \
        for (uintptr_t i = index; i < map->capacity && (i + 1) != index; i = (i + 1) & (map->capacity - 1)) {                               \
            __HASHENTRY(key_ty, val_ty) *entry = &map->entries[i];                                                                          \
                                                                                                                                            \
            if (!entry->alive) {                                                                                                            \
                entry->key = key;                                                                                                           \
                entry->value = value;                                                                                                       \
                entry->alive = 1;                                                                                                           \
                map->length += 1;                                                                                                           \
                                                                                                                                            \
                break;                                                                                                                      \
            } else {                                                                                                                        \
                if (cmp(&entry->key, &key)) {                                                                                               \
                    entry->value = value;                                                                                                   \
                    break;                                                                                                                  \
                } else {                                                                                                                    \
                    continue;                                                                                                               \
                }                                                                                                                           \
            }                                                                                                                               \
        }                                                                                                                                   \
                                                                                                                                            \
        return 0;                                                                                                                           \
    }                                                                                                                                       \
                                                                                                                                            \
    __HASHERROR(key_ty, val_ty) __HASHFUNC(key_ty, val_ty, get)(const __HASHMAP(key_ty, val_ty) *map, const key_ty *key, val_ty *value) {   \
        uint64_t hash = hasher(key);                                                                                                        \
        uintptr_t index = hash & (map->capacity - 1);                                                                                       \
                                                                                                                                            \
        uintptr_t found = ~0ULL; /* ~0 is a sentinel value for "not found" */                                                               \
                                                                                                                                            \
        for (uintptr_t i = index; (i + 1) != index; i = (i + 1) & (map->capacity - 1)) {                                                    \
            __HASHENTRY(key_ty, val_ty) *entry = &map->entries[i];                                                                          \
                                                                                                                                            \
            if (!entry->alive)                                                                                                              \
                break;                                                                                                                      \
                                                                                                                                            \
            if (cmp(&entry->key, key)) {                                                                                                    \
                found = i;                                                                                                                  \
                break;                                                                                                                      \
            }                                                                                                                               \
        }                                                                                                                                   \
                                                                                                                                            \
        if (found != ~0ULL) {                                                                                                               \
            *value = map->entries[found].value;                                                                                             \
            return 0;                                                                                                                       \
        } else {                                                                                                                            \
            return 2;                                                                                                                       \
        }                                                                                                                                   \
    }                                                                                                                                       \
                                                                                                                                            \
    __HASHERROR(key_ty, val_ty) __HASHFUNC(key_ty, val_ty, remove)(__HASHMAP(key_ty, val_ty) *map, const key_ty *key, val_ty *value) {      \
        uint64_t hash = hasher(key);                                                                                                        \
        uintptr_t index = hash & (map->capacity - 1);                                                                                       \
                                                                                                                                            \
        uintptr_t found = ~0ULL; /* ~0 is a sentinel value for "not found" */                                                               \
                                                                                                                                            \
        for (uintptr_t i = index; (i + 1) != index; i = (i + 1) & (map->capacity - 1)) {                                                    \
            __HASHENTRY(key_ty, val_ty) *entry = &map->entries[i];                                                                          \
                                                                                                                                            \
            if (!entry->alive)                                                                                                              \
                break;                                                                                                                      \
                                                                                                                                            \
            if (cmp(&entry->key, key)) {                                                                                                    \
                found = i;                                                                                                                  \
                break;                                                                                                                      \
            }                                                                                                                               \
        }                                                                                                                                   \
                                                                                                                                            \
        if (found != ~0ULL) {                                                                                                               \
            *value = map->entries[found].value;                                                                                             \
            map->entries[found].alive = 0;                                                                                                  \
            map->length -= 1;                                                                                                               \
        } else {                                                                                                                            \
            return 2; /* element not found */                                                                                               \
        }                                                                                                                                   \
                                                                                                                                            \
        /* if possible, move key-values with the same hash back to this cell */                                                             \
        if (found != ~0ULL) {                                                                                                               \
            /* the starting index is going to be later than `found` */                                                                      \
            index = (found + 1) & (map->capacity - 1);                                                                                      \
            for (uintptr_t i = index; (i + 1) != index; i = (i + 1) & (map->capacity - 1)) {                                                \
                __HASHENTRY(key_ty, val_ty) *entry = &map->entries[i];                                                                      \
                                                                                                                                            \
                if (!entry->alive)                                                                                                          \
                    break;                                                                                                                  \
                                                                                                                                            \
                if ((hasher(&entry->key) & (map->capacity - 1)) <= found) {                                                                 \
                    memcpy(&map->entries[found], entry, sizeof(__HASHENTRY(key_ty, val_ty)));                                               \
                    entry->alive = 0;                                                                                                       \
                    break;                                                                                                                  \
                }                                                                                                                           \
            }                                                                                                                               \
        }                                                                                                                                   \
                                                                                                                                            \
        return 0;                                                                                                                           \
    }                                                                                                                                       \
                                                                                                                                            \
    void __HASHFUNC(key_ty, val_ty, free)(__HASHMAP(key_ty, val_ty) *map) {                                                                 \
        free(map->entries);                                                                                                                 \
        map->capacity = 0;                                                                                                                  \
        map->length = 0;                                                                                                                    \
    }                                                                                                                                       \

#endif
