#include <otter/support/core.hh>
#include <stdio.h>

using namespace otter;

int main() {
  puts("Hello, world!");

  mem::Allocator& al = mem::getDefaultAllocator();

  auto* test = (int*)mem::allocAligned(sizeof(int), alignof(int), al);
  *test = 45;
  printf("%d\n", *test);
  printf("%p, %p\n", test, mem::align(test, 1024));
  mem::deallocAligned(test, sizeof(int), alignof(int), al);
}
