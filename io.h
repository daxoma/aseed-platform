#pragma once
#include <stdio.h>
#include "errno.h"

#if defined(_MSC_VER)
#include <share.h>
#endif

namespace aseed {
namespace platform {

inline errno_t fopen_s(FILE** handle, const char* path, const char* mode) noexcept {
#if defined(_MSC_VER)
  *handle = _fsopen(path, mode, _SH_DENYNO);
  if (*handle == nullptr) return errno;
  return 0;
#else
  errno_t r = 0;
  assert(handle != nullptr);
  *handle = fopen(path, mode);
  /* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
  if (!*handle) r = errno;
  return r;
#endif
}

}  // namespace platform
}  // namespace aseed