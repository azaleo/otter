#pragma once

#ifndef NDEBUG
#define OTTER_DEBUG
#endif

#ifdef _MSC_VER
#define OTTER_MSVC
#else
#error "unsupported compiler"
#endif

#ifdef _WIN32
#define OTTER_WIN32
#else
#error "unupported OS"
#endif

#ifdef _M_X64
#define OTTER_X86_64
#else
#error "unsupported arch"
#endif

#ifdef OTTER_X86_64
#define OTTER_64_BITS
#else
#error "unsupported pointer width"
#endif

#if defined(OTTER_WIN32) && defined(OTTER_64_BITS)
#define OTTER_LLP64
#else
#error "unsupported data model"
#endif

#ifdef OTTER_MSVC
#define OTTER_NORETURN __declspec(noreturn)
#else
#error "missing compiler-specific attribs"
#endif

#if defined(OTTER_WIN32) && defined(OTTER_64_BITS)
#define OTTER_MAX_ALIGN (16)
#else
#error "missing max align"
#endif

#ifdef OTTER_LLP64
typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long long   i64;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
#else
#error "missing integer types"
#endif

#ifdef OTTER_64_BITS
typedef i64 isize;
typedef u64 usize;
#else
#error "missing size types"
#endif

#ifdef OTTER_MSVC
typedef float  f32;
typedef double f64;
#else
#error "missing floating-point types"
#endif
