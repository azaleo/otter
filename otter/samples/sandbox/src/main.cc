#include <otter/support/memory.hh>
#include <stdio.h>

int main() {
  puts("Hello, world!");

  auto* test = MEM_NEW<int>();
  MEM_CONSTRUCT(test, 45);
  printf("*%p = %d\n", test, *test);
  MEM_DELETE(test);
}
