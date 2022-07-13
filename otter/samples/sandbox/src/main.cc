#include <otter/support/memory.hh>
#include <stdio.h>

using namespace otter;

int main() {
  puts("Hello, world!");

  auto* test = (int*)MEM_ALLOC_ALIGNED(sizeof(int), 256);
  *test = 45;
  printf("*%p = %d\n", test, *test);
  MEM_DEALLOC_ALIGNED(test, sizeof(int), alignof(int));
}
