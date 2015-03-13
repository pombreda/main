#ifndef BASIS_CONFIGURE_PRINTF_TRACE_HPP_
#define BASIS_CONFIGURE_PRINTF_TRACE_HPP_

#if defined(PRINT_TRACE) && PRINT_TRACE > 0
#   include <basis/sys/console.hpp>
#   define TraceFunc() console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__)
#   define TraceObj1() console::printf("%s():%d {%p, %Iu}\n", __FUNCTION__, __LINE__, this, sizeof(*this))
#   define TraceObj() console::printf("%s:%d {%p, %Iu}\n", __PRETTY_FUNCTION__, __LINE__, this, sizeof(*this))
#   define TraceFormatFunc(format, ...) console::printf("%s:%d " format, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#   define TraceFormatObj(format, ...) console::printf("%s:%d {%p, %Iu} " format, __PRETTY_FUNCTION__, __LINE__, this, sizeof(*this) , ##__VA_ARGS__)
#else
#   define TraceFunc()
#   define TraceObj1()
#   define TraceObj()
#   define TraceFormatFunc(format, ...)
#   define TraceFormatObj(format, ...)
#endif

#endif
