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
#include <stdlib.h>

static inline bool cn_raw_reserve(void **data, size_t *cap,
                                  size_t elem, size_t need) {
      if (need <= *cap) {
            return true;
      }
      size_t ncap = *cap ? *cap : 4;
      while (ncap < need) {
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

#endif // !CN_DS_LIST_H
