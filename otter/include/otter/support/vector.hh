#pragma once

#include "build.hh"
#include "memory.hh"

template <typename T>
struct Vector {
  mem::Allocator* _alloc = nullptr;
  T*              _data = nullptr;
  usize           _length = 0;
  usize           _capacity = 0;
};
