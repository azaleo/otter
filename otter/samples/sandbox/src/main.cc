#include <otter/support/core.hh>
#include <stdio.h>

using namespace otter;

int main() {
  puts("Hello, world!");

  OSAllocator al;

  auto* test = (int*)al.alloc(sizeof(int));
  *test = 45;
  printf("%d\n", *test);
  al.dealloc(test, sizeof(int));
}
