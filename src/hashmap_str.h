#ifndef HASHMAP_STR_H_INCLUDED
#define HASHMAP_STR_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>

#define HASHMAP_STR(val_ty)                                   \
    struct hashmap_str_ ## val_ty ## _entry {                 \
        char *key;                                            \
        val_ty value;                                         \
    };                                                        \
                                                              \
    struct hashmap_str_ ## val_ty {                           \
        size_t length;                                        \
        size_t capacity;                                      \
        struct hashmap_str_ ## val_ty ## _entry *entries;     \
    };                                                        \
                                                              \
    typedef int hashmap_str_ ## val_ty ## _error_code;

#define __HSTRMAP(val_ty) struct hashmap_str_ ## val_ty
#define __HSTRENTRY(val_ty) struct hashmap_str_ ## val_ty ## _entry
#define __HSTRERROR(val_ty) hashmap_str_ ## val_ty ## _error_code
#define __HSTRFUNC(val_ty, name) hashmap_str_ ## val_ty ## _ ## name

#define HASHMAP_STR_FUNCS_DECL(val_ty)                                                                          \
    __HSTRERROR(val_ty) __HSTRFUNC(val_ty, new)(__HSTRMAP(val_ty) *map);                                        \
    __HSTRERROR(val_ty) __HSTRFUNC(val_ty, with_capacity)(__HSTRMAP(val_ty) *map, uintptr_t cap);               \
    __HSTRERROR(val_ty) __HSTRFUNC(val_ty, insert)(__HSTRMAP(val_ty) *map, const char *key, val_ty value);      \
    __HSTRERROR(val_ty) __HSTRFUNC(val_ty, get)(const __HSTRMAP(val_ty) *map, const char *key, val_ty *value);  \
    __HSTRERROR(val_ty) __HSTRFUNC(val_ty, remove)(__HSTRMAP(val_ty) *map, const char *key, val_ty *value);     \
    void                __HSTRFUNC(val_ty, free)(__HSTRMAP(val_ty) *map);

#define HASHMAP_STR_FUNCS_IMPL(val_ty, hasher)                                                                  \
    __HSTRERROR(val_ty) __HSTRFUNC(val_ty, new)(__HSTRMAP(val_ty) *map) {                                      \
        return __HSTRFUNC(val_ty, with_capacity)(map, 4);                                                       \
    }                                                                                                           \
                                                                                                                \
    /* NOTE: cap MUST be a power of 2 */                                                                        \
    __HSTRERROR(val_ty) __HSTRFUNC(val_ty, with_capacity)(__HSTRMAP(val_ty) *map, uintptr_t cap) {              \
        size_t alloc_size = sizeof(__HSTRENTRY(val_ty)) * cap;                                                  \
        __HSTRENTRY(val_ty) *new_entries = malloc(alloc_size);                                                  \
                                                                                                                \
        if (!map)                                                                                               \
            return 1;                                                                                           \
                                                                                                                \
        map->capacity = cap;                                                                                    \
        map->length = 0;                                                                                        \
        map->entries = new_entries;                                                                             \
                                                                                                                \
        for (uintptr_t i = 0; i < cap; i++)                                                                     \
            map->entries[i].key = (char*) 0;                                                                    \
                                                                                                                \
        return 0;                                                                                               \
    }                                                                                                           \
                                                                                                                \
    __HSTRERROR(val_ty) _hashmap_str_ ## val_ty ## _double(__HSTRMAP(val_ty) *old_map) {                        \
        __HSTRMAP(val_ty) new_map;                                                                              \
                                                                                                                \
        __HSTRERROR(val_ty) result = __HSTRFUNC(val_ty, with_capacity)(&new_map, old_map->capacity * 2);        \
                                                                                                                \
        if (result)                                                                                             \
            return result;                                                                                      \
                                                                                                                \
        for (uintptr_t i = 0; i < old_map->capacity; i++) {                                                     \
            __HSTRENTRY(val_ty) entry = old_map->entries[i];                                                    \
                                                                                                                \
            if (!entry.key)                                                                                     \
                continue;                                                                                       \
                                                                                                                \
            __HSTRFUNC(val_ty, insert)(&new_map, entry.key, entry.value);                                       \
        }                                                                                                       \
                                                                                                                \
        __HSTRFUNC(val_ty, free)(old_map);                                                                      \
                                                                                                                \
        old_map->length = new_map.length;                                                                       \
        old_map->capacity = new_map.capacity;                                                                   \
        old_map->entries = new_map.entries;                                                                     \
                                                                                                                \
        return 0;                                                                                               \
    }                                                                                                           \
                                                                                                                \
    __HSTRERROR(val_ty) __HSTRFUNC(val_ty, insert)(__HSTRMAP(val_ty) *map, const char *key, val_ty value) {     \
        uint64_t hash = hasher(key);                                                                            \
        uint64_t key_length = strlen(key);                                                                      \
                                                                                                                \
        /* if the length of hashmap exceeds 80% of the capacity, expand the map */                              \
        if ((map->length + 1) * 5 >= map->capacity * 4) {                                                       \
            __HSTRERROR(val_ty) result = _hashmap_str_ ## val_ty ## _double (map);                              \
                                                                                                                \
            if (result)                                                                                         \
                return result;                                                                                  \
        }                                                                                                       \
                                                                                                                \
        uintptr_t index = hash & (map->capacity - 1);                                                           \
                                                                                                                \
        for (uintptr_t i = index; (i + 1) != index; i = (i + 1) & (map->capacity - 1)) {                        \
            __HSTRENTRY(val_ty) *entry = &map->entries[i];                                                      \
                                                                                                                \
            if (!entry->key) {                                                                                  \
                entry->key = malloc(key_length * sizeof(char) + 1);                                             \
                                                                                                                \
                for (uintptr_t k = 0; k < key_length; k++)                                                      \
                    entry->key[k] = key[k];                                                                     \
                                                                                                                \
                entry->key[key_length] = 0; /* terminating NUL char */                                          \
                entry->value = value;                                                                           \
                map->length += 1;                                                                               \
                                                                                                                \
                break;                                                                                          \
            } else {                                                                                            \
                if (strcmp(entry->key, key) == 0) {                                                             \
                    entry->value = value;                                                                       \
                    break;                                                                                      \
                } else {                                                                                        \
                    continue;                                                                                   \
                }                                                                                               \
            }                                                                                                   \
        }                                                                                                       \
                                                                                                                \
        return 0;                                                                                               \
    }                                                                                                           \
                                                                                                                \
    __HSTRERROR(val_ty) __HSTRFUNC(val_ty, get)(const __HSTRMAP(val_ty) *map, const char *key, val_ty *value) { \
        uint64_t hash = hasher(key);                                                                            \
        uintptr_t index = hash & (map->capacity - 1);                                                           \
                                                                                                                \
        uintptr_t found = ~0ULL; /* ~0 is a sentinel value for "not found" */                                   \
                                                                                                                \
        for (uintptr_t i = index; (i + 1) != index; i = (i + 1) & (map->capacity - 1)) {                        \
            __HSTRENTRY(val_ty) *entry = &map->entries[i];                                                      \
                                                                                                                \
            if (!entry->key)                                                                                    \
                break;                                                                                          \
                                                                                                                \
            if (entry->key && strcmp(entry->key, key) == 0) {                                                   \
                found = i;                                                                                      \
                break;                                                                                          \
            }                                                                                                   \
        }                                                                                                       \
                                                                                                                \
        if (found != ~0ULL) {                                                                                   \
            *value = map->entries[found].value;                                                                 \
            return 0;                                                                                           \
        } else {                                                                                                \
            return 2;                                                                                           \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    __HSTRERROR(val_ty) __HSTRFUNC(val_ty, remove)(__HSTRMAP(val_ty) *map, const char *key, val_ty *value) {    \
        uint64_t hash = hasher(key);                                                                            \
        uintptr_t index = hash & (map->capacity - 1);                                                           \
                                                                                                                \
        uintptr_t found = ~0ULL; /* ~0 is a sentinel value for "not found" */                                   \
                                                                                                                \
        for (uintptr_t i = index; (i + 1) != index; i = (i + 1) & (map->capacity - 1)) {                        \
            __HSTRENTRY(val_ty) *entry = &map->entries[i];                                                      \
                                                                                                                \
            if (!entry->key)                                                                                    \
                break;                                                                                          \
                                                                                                                \
            if (entry->key && strcmp(entry->key, key) == 0) {                                                   \
                found = i;                                                                                      \
                break;                                                                                          \
            }                                                                                                   \
        }                                                                                                       \
                                                                                                                \
        if (found != ~0ULL) {                                                                                   \
            *value = map->entries[found].value;                                                                 \
                                                                                                                \
            free(map->entries[found].key);                                                                      \
            map->entries[found].key = (char*) 0;                                                                \
            map->length -= 1;                                                                                   \
        } else {                                                                                                \
            return 2;                                                                                           \
        }                                                                                                       \
                                                                                                                \
        /* if possible, move key-values with the same hash back to this cell */                                 \
        if (found != ~0ULL) {                                                                                   \
            /* the starting index is going to be later than `found` */                                          \
            index = (found + 1) & (map->capacity - 1);                                                          \
            for (uintptr_t i = index; (i + 1) != index; i = (i + 1) & (map->capacity - 1)) {                    \
                __HSTRENTRY(val_ty) *entry = &map->entries[i];                                                  \
                                                                                                                \
                if (!entry->key)                                                                                \
                    break;                                                                                      \
                                                                                                                \
                if ((hasher(entry->key) & (map->capacity - 1)) <= found) {                                      \
                    free(map->entries[found].key);                                                              \
                    memcpy(&map->entries[found], entry, sizeof(__HSTRENTRY(val_ty)));                           \
                    break;                                                                                      \
                }                                                                                               \
            }                                                                                                   \
        }                                                                                                       \
                                                                                                                \
        return 0;                                                                                               \
    }                                                                                                           \
                                                                                                                \
    void __HSTRFUNC(val_ty, free)(__HSTRMAP(val_ty) *map) {                                                     \
        for (uintptr_t i = 0; i < map->capacity; i++) {                                                         \
            __HSTRENTRY(val_ty) *entry = &map->entries[i];                                                      \
                                                                                                                \
            if (entry->key) {                                                                                   \
                free(entry->key);                                                                               \
            }                                                                                                   \
        }                                                                                                       \
                                                                                                                \
        free(map->entries);                                                                                     \
        map->length = 0;                                                                                        \
        map->capacity = 0;                                                                                      \
    }

#endif
