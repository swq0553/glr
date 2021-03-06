// we require a C++ compiler.
#if !defined(__cplusplus)
#error C++ compiler required.
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(__WIN64__) || defined(WIN64)
#define OS_WINDOWS 1
#elif defined(linux) || defined(freebsd) || defined(__linux__)
#define OS_LINUX 1
#elif defined(__APPLE__)
#define OS_MAC 1
#endif


#if !defined(OS_WINDOWS) && !defined(OS_LINUX) && !defined(OS_MAC)
#error "OS NOT SUPPORTED";
#endif
