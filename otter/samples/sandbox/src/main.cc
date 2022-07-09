#include <otter/support/core.hh>
#include <stdio.h>

using namespace otter;

int main() {
  OSAllocator al;

  auto* test = (int*)al.alloc(sizeof(int));
  puts("Hello, world!");
}
