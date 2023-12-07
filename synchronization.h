#pragma once
#include <chrono>

#if (WINVER > 0x0601)
// windows 8, 10 ...
#include <synchapi.h>
// it is obligatory to add next lib	synchronization.lib
#pragma comment(lib, "synchronization.lib")

namespace aseed {
namespace platform {

template <typename Type>
void futex_wake_one(Type& value) noexcept {
  ::WakeByAddressSingle(&value);
}

template <typename Type>
void futex_wake_all(Type& value) noexcept {
  ::WakeByAddressAll(&value);
}

template <typename Type, typename Rep, typename Period>
void futex_wait(Type& value, Type undesired, const std::chrono::duration<Rep, Period>& waittime) noexcept {
  // https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitonaddress
  static_assert(sizeof(value) == 1 || sizeof(value) == 2 || sizeof(value) == 4 || sizeof(value) == 8, "invalid value size");
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(waittime);
  ::WaitOnAddress(&value, &undesired, sizeof(value), static_cast<DWORD>(t.count()));
}

}  // namespace platform
}  // namespace aseed

#elif defined(__linux__)
#include <linux/futex.h> /* Definition of FUTEX_* constants */
#include <stdint.h>
#include <sys/syscall.h> /* Definition of SYS_* constants */
#include <sys/time.h>
#include <unistd.h>

namespace aseed {
namespace platform {

template <typename Type>
void futex_wake_one(Type& value) noexcept {
  syscall(SYS_futex, &value, FUTEX_WAKE_PRIVATE, 1, nullptr, nullptr, 0);
}

template <typename Type>
void futex_wake_all(Type& value) noexcept {
  syscall(SYS_futex, &value, FUTEX_WAKE_PRIVATE, INT_MAX, nullptr, nullptr, 0);
}

template <typename Rep, typename Period>
void futex_wait(uint32_t& value, uint32_t undesired, const std::chrono::duration<Rep, Period>& waittime) noexcept {
  auto sec = std::chrono::duration_cast<std::chrono::seconds>(waittime);
  auto nanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(waittime - sec);
  timespec t{static_cast<std::time_t>(sec.count()), static_cast<long>(nanosec.count())};
  syscall(SYS_futex, &value, FUTEX_WAIT_PRIVATE, undesired, &t, nullptr, 0);
}

}  // namespace platform
}  // namespace aseed

#else
#error "target platform is not supported"
#endif