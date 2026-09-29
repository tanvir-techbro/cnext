/*
 * cnext - Modern "NonStandard" C library.
 *
 * Copyright (c) 2026-present Tanvir.
 * SPDX-License-Identifier: MPL-2.0
 */

/*
 * ds/list.h - implementation for cnlist.
 * cnlist is dynamically allocated array.
 */

#ifndef CN_DS_LIST_H
#define CN_DS_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline bool cn_raw_reserve(void **data, size_t *cap,
                                  size_t elem, size_t need) {
      if (need <= *cap) {
            return true;
      }
      if (elem != 0 && need > SIZE_MAX / elem) {
            return false;
      }
      size_t ncap = *cap ? *cap : 4;
      while (ncap < need) {
            if (ncap > SIZE_MAX / 2) {
                  ncap = need;
                  break;
            }
            ncap *= 2;
      }
      void *nd = realloc(*data, ncap * elem);
      if (!nd) {
            return false;
      }

      *data = nd;
      *cap = ncap;
      return true;
}

static inline bool cn_idx_lt(size_t len, size_t idx) {
      return idx < len;
}

static inline bool cn_idx_le(size_t len, size_t idx) {
      return idx <= len;
}

static inline bool cn_raw_shrink(void **data, size_t *cap,
                                 size_t elem, size_t len) {
      if (len == *cap) {
            return true;
      }
      if (len == 0) {
            free(*data);
            *data = NULL;
            *cap = 0;
            return true;
      }
      void *nd = realloc(*data, len * elem);
      if (!nd) {
            return false;
      }
      *data = nd;
      *cap = len;
      return true;
}

/* List init */
#define List(T)          \
      typedef struct {   \
            T *data;     \
            size_t size; \
            size_t cap;  \
      }

#define list_reserve(l, n) \
      cn_raw_reserve((void **)&(l)->data, &(l)->cap, sizeof(*(l)->data), (n))

#define list_free(l) \
      (free((l)->data), (l)->data = NULL, (l)->size = (l)->cap = 0)

#define list_push(l, v)                                                                  \
      (cn_raw_reserve((void **)&(l)->data, &(l)->cap, sizeof(*(l)->data), (l)->size + 1) \
           ? ((l)->data[(l)->size++] = (v), true)                                        \
           : false)

// put the output in var
// list_get(mylist, index, var)
#define list_get(l, i, out)                          \
      ((size_t)(i) < (l)->size                       \
           ? (*(out) = (l)->data[(size_t)(i)], true) \
           : false)

// pop the output to var
// list_pop(mylist, var)
#define list_pop(l, out)                             \
      ((l)->size > 0                                 \
           ? (*(out) = (l)->data[--(l)->size], true) \
           : false)

#define list_len(l) ((l)->size)
#define list_cap(l) ((l)->cap)
#define list_empty(l) ((l)->size == 0)
#define list_clear(l) ((l)->size = 0)

// drop last element without returning it
#define list_drop(l) (((l)->size > 0) ? (--(l)->size, true) : false)

// set element, false if oob
// list_set(mylist, index, value)
#define list_set(l, i, v)                         \
      ((size_t)(i) < (l)->size                    \
           ? ((l)->data[(size_t)(i)] = (v), true) \
           : false)

// peek first/last without removing, false if empty
#define list_first(l, out) \
      (((l)->size > 0) ? (*(out) = (l)->data[0], true) : false)
#define list_last(l, out) \
      (((l)->size > 0) ? (*(out) = (l)->data[(l)->size - 1], true) : false)

// insert value at index, shifting tail right; index == size appends
// list_insert(mylist, index, value)
#define list_insert(l, i, v)                                                                                                  \
      (cn_idx_le((l)->size, (size_t)(i)) && cn_raw_reserve((void **)&(l)->data, &(l)->cap, sizeof(*(l)->data), (l)->size + 1) \
           ? (memmove(&(l)->data[(size_t)(i) + 1], &(l)->data[(size_t)(i)], ((l)->size - (size_t)(i)) * sizeof(*(l)->data)),  \
              (l)->data[(size_t)(i)] = (v), ++(l)->size, true)                                                                \
           : false)

// remove element at index, output old value
// list_remove(mylist, index, var)
#define list_remove(l, i, out)                                              \
      ((size_t)(i) < (l)->size                                              \
           ? (*(out) = (l)->data[(size_t)(i)],                              \
              memmove(&(l)->data[(size_t)(i)], &(l)->data[(size_t)(i) + 1], \
                      ((l)->size - (size_t)(i) - 1) * sizeof(*(l)->data)),  \
              --(l)->size, true)                                            \
           : false)

// append n elements from ptr; n == 0 is a no-op and ignores ptr
// list_append_n(mylist, ptr, n)
#define list_append_n(l, ptr, n)                                                                          \
      ((size_t)(n) == 0                                                                                   \
           ? true                                                                                         \
           : (cn_raw_reserve((void **)&(l)->data, &(l)->cap, sizeof(*(l)->data), (l)->size + (size_t)(n)) \
                  ? (memcpy(&(l)->data[(l)->size], (ptr), (size_t)(n) * sizeof(*(l)->data)),              \
                     (l)->size += (size_t)(n), true)                                                      \
                  : false))

// release unused capacity; old buffer stays valid on failure
#define list_shrink(l) \
      (cn_raw_shrink((void **)&(l)->data, &(l)->cap, sizeof(*(l)->data), (l)->size))

#endif // !CN_DS_LIST_H
