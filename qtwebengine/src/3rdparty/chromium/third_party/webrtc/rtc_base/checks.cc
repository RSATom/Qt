/*
 *  Copyright 2006 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// Most of this was borrowed (with minor modifications) from V8's and Chromium's
// src/base/logging.cc.

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#if defined(WEBRTC_ANDROID)
#define RTC_LOG_TAG_ANDROID "rtc"
#include <android/log.h>  // NOLINT
#endif

#if defined(WEBRTC_WIN)
#include <windows.h>
#endif

#if defined(WEBRTC_WIN)
#define LAST_SYSTEM_ERROR (::GetLastError())
#elif defined(__native_client__) && __native_client__
#define LAST_SYSTEM_ERROR (0)
#elif defined(WEBRTC_POSIX)
#include <errno.h>
#define LAST_SYSTEM_ERROR (errno)
#endif  // WEBRTC_WIN

#include "rtc_base/checks.h"

namespace {
#if defined(__GNUC__)
__attribute__((__format__(__printf__, 2, 3)))
#endif
  void AppendFormat(std::string* s, const char* fmt, ...) {
  va_list args, copy;
  va_start(args, fmt);
  va_copy(copy, args);
  const int predicted_length = std::vsnprintf(nullptr, 0, fmt, copy);
  va_end(copy);

  if (predicted_length > 0) {
    const size_t size = s->size();
    s->resize(size + predicted_length);
    // Pass "+ 1" to vsnprintf to include space for the '\0'.
    std::vsnprintf(&((*s)[size]), predicted_length + 1, fmt, args);
  }
  va_end(args);
}
}

namespace rtc {

// MSVC doesn't like complex extern templates and DLLs.
#if !defined(COMPILER_MSVC)
// Explicit instantiations for commonly used comparisons.
template std::string* MakeCheckOpString<int, int>(
    const int&, const int&, const char* names);
template std::string* MakeCheckOpString<unsigned long, unsigned long>(
    const unsigned long&, const unsigned long&, const char* names);
template std::string* MakeCheckOpString<unsigned long, unsigned int>(
    const unsigned long&, const unsigned int&, const char* names);
template std::string* MakeCheckOpString<unsigned int, unsigned long>(
    const unsigned int&, const unsigned long&, const char* names);
template std::string* MakeCheckOpString<std::string, std::string>(
    const std::string&, const std::string&, const char* name);
#endif
namespace webrtc_checks_impl {
RTC_NORETURN void FatalLog(const char* file,
                           int line,
                           const char* message,
                           const CheckArgType* fmt,
                           ...) {
  va_list args;
  va_start(args, fmt);

  std::string s;
  AppendFormat(&s,
               "\n\n"
               "#\n"
               "# Fatal error in: %s, line %d\n"
               "# last system error: %u\n"
               "# Check failed: %s",
               file, line, LAST_SYSTEM_ERROR, message);

  for (; *fmt != CheckArgType::kEnd; ++fmt) {
    switch (*fmt) {
      case CheckArgType::kInt:
        AppendFormat(&s, "%d", va_arg(args, int));
        break;
      case CheckArgType::kLong:
        AppendFormat(&s, "%ld", va_arg(args, long));
        break;
      case CheckArgType::kLongLong:
        AppendFormat(&s, "%lld", va_arg(args, long long));
        break;
      case CheckArgType::kUInt:
        AppendFormat(&s, "%u", va_arg(args, unsigned));
        break;
      case CheckArgType::kULong:
        AppendFormat(&s, "%lu", va_arg(args, unsigned long));
        break;
      case CheckArgType::kULongLong:
        AppendFormat(&s, "%llu", va_arg(args, unsigned long long));
        break;
      case CheckArgType::kDouble:
        AppendFormat(&s, "%g", va_arg(args, double));
        break;
      case CheckArgType::kLongDouble:
        AppendFormat(&s, "%Lg", va_arg(args, long double));
        break;
      case CheckArgType::kCharP:
        s.append(va_arg(args, const char*));
        break;
      case CheckArgType::kStdString:
        s.append(*va_arg(args, const std::string*));
        break;
      case CheckArgType::kVoidP:
        AppendFormat(&s, "%p", va_arg(args, const void*));
        break;
      default:
        s.append("[Invalid CheckArgType]");
        goto processing_loop_end;
    }
  }

processing_loop_end:
  va_end(args);

  const char* output = s.c_str();

#if defined(WEBRTC_ANDROID)
  __android_log_print(ANDROID_LOG_ERROR, RTC_LOG_TAG_ANDROID, "%s\n", output);
#endif

  fflush(stdout);
  fprintf(stderr, "%s", output);
  fflush(stderr);
  abort();
}

}  // namespace webrtc_checks_impl
}  // namespace rtc

// Function to call from the C version of the RTC_CHECK and RTC_DCHECK macros.
RTC_NORETURN void rtc_FatalMessage(const char* file, int line,
                                   const char* msg) {
  static constexpr rtc::webrtc_checks_impl::CheckArgType t[] = {
      rtc::webrtc_checks_impl::CheckArgType::kEnd};
  FatalLog(file, line, msg, t);
}
