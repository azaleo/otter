#include <otter/support/memory.hh>
#include <stdio.h>

int main() {
  puts("Hello, world!");

  auto* test = (int*)MEM_ALLOC(sizeof(int));
  *test = 45;
  printf("*%p = %d\n", test, *test);
  MEM_DEALLOC(test, sizeof(int));
}
