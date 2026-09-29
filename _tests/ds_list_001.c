#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "cnext/ds/list.h"

List(int) IntList;
List(uint32_t) U32List;
List(int64_t) I64List;
List(float) FloatList;
List(double) DoubleList;
List(char) CharList;
List(char *) StrList;
List(bool) BoolList;

int main(void) {
      IntList xs = {0};
      list_push(&xs, -5);
      list_push(&xs, 42);
      int iv = 0;
      list_get(&xs, 0, &iv);
      printf("int: len=%zu get0=%d\n", xs.size, iv);
      list_pop(&xs, &iv);
      printf("int: pop=%d len=%zu\n", iv, xs.size);
      if (!list_get(&xs, 99, &iv)) {
            printf("int: oob=ok\n");
      }
      list_free(&xs);

      U32List us = {0};
      list_push(&us, 4000000000u);
      uint32_t uv = 0;
      list_get(&us, 0, &uv);
      printf("u32: %u\n", uv);
      list_free(&us);

      I64List ls = {0};
      list_push(&ls, -9000000000LL);
      int64_t lv = 0;
      list_get(&ls, 0, &lv);
      printf("i64: %lld\n", (long long)lv);
      list_free(&ls);

      FloatList fs = {0};
      list_push(&fs, 1.5f);
      float fv = 0.0f;
      list_get(&fs, 0, &fv);
      printf("float: %f\n", (double)fv);
      list_free(&fs);

      DoubleList ds = {0};
      list_push(&ds, 3.25);
      double dv = 0.0;
      list_get(&ds, 0, &dv);
      printf("double: %f\n", dv);
      list_free(&ds);

      CharList cs = {0};
      list_push(&cs, 'a');
      list_push(&cs, 'b');
      list_push(&cs, 'c');
      printf("char: len=%zu %c%c%c\n", cs.size, cs.data[0], cs.data[1], cs.data[2]);
      list_free(&cs);

      StrList ss = {0};
      list_push(&ss, "foo");
      list_push(&ss, "bar");
      char *sv = NULL;
      list_get(&ss, 1, &sv);
      printf("str: len=%zu get1=%s\n", ss.size, sv);
      list_pop(&ss, &sv);
      printf("str: pop=%s len=%zu\n", sv, ss.size);
      list_free(&ss);

      BoolList bs = {0};
      list_push(&bs, true);
      list_push(&bs, false);
      bool bv = false;
      list_get(&bs, 0, &bv);
      printf("bool: %d %d\n", bv, bs.data[1]);
      list_free(&bs);

      printf("freed=%d\n", xs.data == NULL && ss.data == NULL);
      return 0;
}
