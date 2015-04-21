#ifndef BASIS_CONFIGURE_PRINTF_TRACE_HPP_
#define BASIS_CONFIGURE_PRINTF_TRACE_HPP_

#if defined(TRACE_PRINT) && TRACE_PRINT > 0
#   include <basis/sys/console.hpp>
#   define TraceFuncLn() console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__)
#   define TraceObjLn() console::printf("%s:%d {%p, %Iu}\n", __PRETTY_FUNCTION__, __LINE__, this, sizeof(*this))
#   define TraceFunc(format, ...) console::printf("%s:%d " format, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#   define TraceObj(format, ...) console::printf("%s:%d {%p, %Iu} " format, __PRETTY_FUNCTION__, __LINE__, this, sizeof(*this) , ##__VA_ARGS__)
#else
#   define TraceFuncLn()
#   define TraceObjLn()
#   define TraceFunc(format, ...)
#   define TraceObj(format, ...)
#endif

#endif
