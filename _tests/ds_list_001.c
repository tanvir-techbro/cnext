#include <stdio.h>

#include "cnext/ds/list.h"

List(int) IntList;

int main(void) {
      IntList xs = {0};

      if (!list_reserve(&xs, 4)) {
            printf("reserve failed\n");
            return 1;
      }

      list_push(&xs, 10);
      list_push(&xs, 20);
      list_push(&xs, 30);

      printf("len=%zu\n", xs.size);
      for (size_t i = 0; i < xs.size; i++) {
            printf("%d\n", xs.data[i]);
      }

      int v = 0;
      if (list_get(&xs, 1, &v)) {
            printf("get1=%d\n", v);
      }
      if (!list_get(&xs, 99, &v)) {
            printf("oob=ok\n");
      }

      list_free(&xs);
      printf("freed=%d\n", xs.data == NULL);
      return 0;
}
