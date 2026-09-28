/*
 * cnext - Modern "NonStandard" C library.
 *
 * Copyright (c) 2026-present Tanvir.
 * SPDX-License-Identifier: MPL-2.0
 */

/*
 * ds/list.h - implimentation for cnlist.
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

#define list_push(l, v) ({                                           \
      __typeof__(*(l)->data) _v = v;                                 \
      bool _ok = true;                                               \
      if ((l)->size + 1 > (l)->cap) {                                \
            _ok = cn_raw_reserve((void **)&(l)->data, &(l)->cap,     \
                                 sizeof(*(l)->data), (l)->size + 1); \
      }                                                              \
      if (_ok) {                                                     \
            (l)->data[(l)->size++] = _v;                             \
      }                                                              \
      _ok;                                                           \
})

#define list_get(l, i, out) ({      \
      size_t _i = (i);              \
      bool _ok = _i < (l)->size;    \
      if (_ok) {                    \
            *(out) = (l)->data[_i]; \
      }                             \
      _ok;                          \
})

#endif // !CN_DS_LIST_H
