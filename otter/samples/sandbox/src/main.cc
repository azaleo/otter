#include <otter/support/core.hh>
#include <stdio.h>

using namespace otter;

int main() {
  puts("Hello, world!");

  auto* test = (int*)mem::alloc(sizeof(int));
  *test = 45;
  printf("%d\n", *test);
  mem::dealloc(test, sizeof(int));
}
