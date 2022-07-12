#include <otter/support/core.hh>
#include <stdio.h>

using namespace otter;

int main() {
  puts("Hello, world!");

  auto* test = (int*)MEM_ALLOC_ALIGNED(sizeof(int), alignof(int));
  *test = 45;
  printf("%d\n", *test);
  printf("%p, %p\n", test, mem::align(test, 1024));
  MEM_DEALLOC_ALIGNED(test, sizeof(int), alignof(int));
}
