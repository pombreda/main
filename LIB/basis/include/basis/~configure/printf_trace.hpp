#ifndef BASIS_CONFIGURE_PRINTF_TRACE_HPP_
#define BASIS_CONFIGURE_PRINTF_TRACE_HPP_

#if defined(PRINT_TRACE) && PRINT_TRACE > 0
#   include <basis/sys/console.hpp>
#   define TraceFunc() console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__)
#   define TraceObj() console::printf("%s:%d [%p, %Iu]\n", __PRETTY_FUNCTION__, __LINE__, this, sizeof(*this))
#   define TraceFuncFormat(format, ...) console::printf("%s:%d " format, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#   define TraceFunc()
#   define TraceObj()
#   define TraceFuncFormat(format, ...)
#endif

#endif
