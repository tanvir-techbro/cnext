#include <stdio.h>

#include "cnext/ds/list.h"

typedef struct {
      char name[16];
      int age;
} Person;

typedef struct {
      int x;
      int y;
} Point;

List(Person) PersonList;
List(Point) PointList;

int main(void) {
      PersonList ps = {0};
      list_reserve(&ps, 4);

      Person ada = {"ada", 36};
      Person bob = {"bob", 41};
      Person cy = {"cy", 29};
      list_push(&ps, ada);
      list_push(&ps, bob);
      list_push(&ps, cy);

      printf("person: len=%zu\n", ps.size);
      for (size_t i = 0; i < ps.size; i++) {
            printf("person: %s %d\n", ps.data[i].name, ps.data[i].age);
      }

      Person p = {"", 0};
      list_get(&ps, 1, &p);
      printf("person: get1=%s %d\n", p.name, p.age);

      list_pop(&ps, &p);
      printf("person: pop=%s %d len=%zu\n", p.name, p.age, ps.size);
      if (!list_get(&ps, 99, &p)) {
            printf("person: oob=ok\n");
      }
      list_free(&ps);

      PointList pts = {0};
      Point o = {0, 0};
      Point e = {3, 4};
      list_push(&pts, o);
      list_push(&pts, e);
      Point q = {0, 0};
      list_get(&pts, 1, &q);
      printf("point: (%d,%d) len=%zu\n", q.x, q.y, pts.size);
      list_free(&pts);

      printf("freed=%d\n", ps.data == NULL && pts.data == NULL);
      return 0;
}
