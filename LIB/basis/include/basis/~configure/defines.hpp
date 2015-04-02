#ifndef BASIS_CONFIGURE_DEFINES_HPP_
#define BASIS_CONFIGURE_DEFINES_HPP_

#ifndef TRACE_MEMORY
#   ifdef NDEBUG
#       define TRACE_MEMORY 0
#   else
#       define TRACE_MEMORY 2
#   endif
#endif

#ifndef TRACE_PRINT
#   define TRACE_PRINT 0
#endif

#define UNUSED(val) ((void)val)

#define THIS_PLACE  __FILE__, __LINE__, __PRETTY_FUNCTION__

#endif
