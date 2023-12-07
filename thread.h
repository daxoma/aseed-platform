#pragma once
#include <stdint.h>

#if defined(_MSC_VER)
#include <errhandlingapi.h>
#include <processthreadsapi.h>
#else
#include <pthread.h>  // pthread_setname_np, pthread_self
#include <sys/syscall.h>
#include <unistd.h>
#endif

namespace aseed {
namespace platform {

inline int32_t get_current_thread_id() noexcept {
#if defined(_MSC_VER)
  return ::GetCurrentThreadId();
#elif defined(SYS_gettid)
  return static_cast<int32_t>(syscall(SYS_gettid));
#endif
}

inline void set_current_thread_name(char const* name) {
  // visual studio
#if defined(_MSC_VER)
  // windows 10
#if _WIN32_WINNT >= 0x0A00  // win 10
  constexpr size_t __max_length = 127;
  wchar_t wname[__max_length + 1];
  size_t ix = 0;
  for (; ix != __max_length && name[ix] != 0; ++ix)
    wname[ix] = name[ix];
  wname[ix] = 0;
  ::SetThreadDescription(::GetCurrentThread(), wname);
#else
  // othe windows wersion, this method is only suitable for debugger
  (void)name;
  /*
  // How to: Set a Thread Name in Native Code
  // https://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
  DWORD const MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push, 8)
  struct THREADNAME_INFO {
    DWORD dwType;      // Must be 0x1000.
    LPCSTR szName;     // Pointer to name (in user addr space).
    DWORD dwThreadID;  // Thread ID (-1=caller thread).
    DWORD dwFlags;     // Reserved for future use, must be zero.
  };
#pragma pack(pop)

  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = name;
  info.dwThreadID = static_cast<DWORD>(-1);
  info.dwFlags = 0;
  __try {
    RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
  } __except (EXCEPTION_EXECUTE_HANDLER) {
  }
  */
  //
#endif
  // pthread
#else
  pthread_setname_np(pthread_self(), name);
#endif  // _MSC_VER
}

}  // namespace platform
}  // namespace aseed