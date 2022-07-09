#include <otter/support/core.hh>
#include <stdio.h>

int main() {
  auto* test = OTTER_NEW(int);

  new (test, otter::New) int(42);
  printf("%d\n", *test);

  OTTER_DELETE(int, test);
}
