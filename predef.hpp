#ifndef CM_PREDEF_H
#define CM_PREDEF_H

#define ATTR_INLINE __attribute__((always_inline)) inline

#if __cplusplus >= 201103L
#define CXX11_CONSTEXPR constexpr
#define CXX11_EXPLICIT explicit
#else
#define CXX11_CONSTEXPR
#define CXX11_EXPLICIT
#endif

#if __cplusplus >= 201402L
#define CXX14_CONSTEXPR constexpr
#else
#define CXX14_CONSTEXPR
#endif

#endif
