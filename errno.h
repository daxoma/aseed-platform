#pragma once
#include <errno.h>

namespace aseed {
namespace platform {

#if defined(_MSC_VER)
#else
using errno_t = int;
#endif

}  // namespace platform
}  // namespace aseed