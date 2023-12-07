#pragma once

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

namespace aseed {
namespace platform {

constexpr size_t __cash_line_size = 64;

inline bool      likely(bool expr) noexcept {
#ifdef __GNUC__
  return __builtin_expect(expr, true);
#else
  return expr;
#endif
}

inline bool unlikely(bool expr) noexcept {
#ifdef __GNUC__
  return __builtin_expect(expr, false);
#else
  return expr;
#endif
}

inline void assume(bool condition) noexcept {
#if defined(__GNUC__)
  // FIXME check that this is still working
  if (!condition)
    __builtin_unreachable();
#elif defined(_MSC_VER)
  __assume(condition);
#else
  static_assert(false, "assume() is not implemented for this compiler");
#endif
}

inline void pause() noexcept {
#if defined(__clang__)
  asm volatile("pause");
#elif defined(__GNUC__)
  __builtin_ia32_pause();
#elif defined(_MSC_VER)
  _mm_pause();
#else
  static_assert(false, "pause() is not implemented for this compiler");
#endif
}

// useless method
inline unsigned long long rdtsc() noexcept {
  unsigned int       u;
  unsigned long long c = __rdtscp(&u);
  return c;

  // other option if rdtscp is not supported
  // read more https://learn.microsoft.com/en-us/cpp/intrinsics/rdtscp?view=msvc-170

  // unsigned long long c;
  // _mm_lfence();  // optionally wait for earlier insns to retire before reading the clock
  // c = __rdtsc();
  // _mm_lfence();  // optionally block later instructions until rdtsc retires
  // return c;
}

}  // namespace platform
}  // namespace aseed