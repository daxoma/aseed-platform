#pragma once
#include <stdio.h>
#include <time.h>

#include "errno.h"

namespace aseed {
namespace platform {

inline errno_t gmtime_s(struct tm* b, const time_t* t) {
#if defined(_MSC_VER)
  return ::gmtime_s(b, t);
#else
  auto* r = gmtime_r(t, b);
  if (r == nullptr) return errno;
  return 0;
#endif
}

inline errno_t localtime_s(struct tm* b, const time_t* t) {
#if defined(_MSC_VER)
  return ::localtime_s(b, t);
#else
  auto* r = localtime_r(t, b);
  if (r == nullptr) return errno;
  return 0;
#endif
}

inline time_t timegm(struct tm& e) {
#if defined(_MSC_VER)
  return ::_mkgmtime(&e);
#else
  return ::timegm(&e);
#endif
}

}  // namespace platform
}  // namespace aseed