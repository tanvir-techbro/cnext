#include <stdio.h>

#include "cnext/ds/list.h"

List(int) IntList;

static void print_all(const char *tag, IntList *xs) {
      printf("%s len=%zu cap=%zu:", tag, xs->size, xs->cap);
      for (size_t i = 0; i < xs->size; i++) {
            printf(" %d", xs->data[i]);
      }
      printf("\n");
}

int main(void) {
      IntList xs = {0};
      printf("empty=%d len=%zu cap=%zu\n", list_empty(&xs), list_len(&xs),
             list_cap(&xs));

      int src[] = {1, 2, 3, 4};
      list_append_n(&xs, src, 4);
      print_all("append", &xs);

      list_insert(&xs, 0, 0);
      list_insert(&xs, 3, 99);
      list_insert(&xs, list_len(&xs), 5);
      print_all("insert", &xs);
      if (!list_insert(&xs, 99, 7)) {
            printf("insert oob=ok\n");
      }

      int v = 0;
      list_set(&xs, 0, 10);
      if (!list_set(&xs, 99, 10)) {
            printf("set oob=ok\n");
      }
      list_first(&xs, &v);
      printf("first=%d\n", v);
      list_last(&xs, &v);
      printf("last=%d\n", v);
      print_all("set", &xs);

      list_remove(&xs, 3, &v);
      printf("removed=%d\n", v);
      list_remove(&xs, 0, &v);
      printf("removed=%d\n", v);
      if (!list_remove(&xs, 99, &v)) {
            printf("remove oob=ok\n");
      }
      print_all("remove", &xs);

      list_drop(&xs);
      list_last(&xs, &v);
      printf("after drop len=%zu last=%d\n", list_len(&xs), v);

      size_t cap_kept = list_cap(&xs);
      list_clear(&xs);
      printf("clear: len=%zu cap=%zu kept=%d empty=%d\n", list_len(&xs),
             list_cap(&xs), list_cap(&xs) == cap_kept, list_empty(&xs));

      if (!list_drop(&xs)) {
            printf("drop empty=ok\n");
      }
      if (!list_first(&xs, &v) && !list_last(&xs, &v)) {
            printf("peek empty=ok\n");
      }
      if (!list_pop(&xs, &v)) {
            printf("pop empty=ok\n");
      }

      list_push(&xs, 1);
      list_push(&xs, 2);
      printf("before shrink len=%zu cap=%zu\n", list_len(&xs), list_cap(&xs));
      list_shrink(&xs);
      printf("after shrink len=%zu cap=%zu\n", list_len(&xs), list_cap(&xs));
      list_clear(&xs);
      list_shrink(&xs);
      printf("shrink empty: len=%zu cap=%zu null=%d\n", list_len(&xs),
             list_cap(&xs), xs.data == NULL);

      IntList ys = {0};
      if (list_append_n(&ys, NULL, 0)) {
            printf("append0=ok len=%zu\n", list_len(&ys));
      }

      list_free(&xs);
      list_free(&ys);
      printf("freed=%d\n", xs.data == NULL && ys.data == NULL);
      return 0;
}
