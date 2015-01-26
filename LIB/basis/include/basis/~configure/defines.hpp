#ifndef BASIS_CONFIGURE_DEFINES_HPP_
#define BASIS_CONFIGURE_DEFINES_HPP_

#ifndef MEMORY_TRACE
#   ifdef NDEBUG
#       define MEMORY_TRACE 0
#   else
#       define MEMORY_TRACE 2
#   endif
#endif

#ifndef PRINT_TRACE
#   define PRINT_TRACE 0
#endif

#define UNUSED(val) ((void)val)

#define THIS_PLACE  __FILE__, __LINE__, __PRETTY_FUNCTION__

#endif
