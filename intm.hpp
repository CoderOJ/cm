#ifndef CM_INTM_H
#define CM_INTM_H

#if __cplusplus >= 201103L
#include <type_traits>
#endif

#include <iostream>
#include <limits>

#ifndef INTM_FAST_32
#define INTM_FAST_32 int
#endif
#ifndef INTM_FAST_64
#define INTM_FAST_64 unsigned long long
#endif

#define _ATTR_INLINE __attribute__((always_inline)) inline

#if __cplusplus >= 201103L
#define _CXX11_CONSTEXPR constexpr
#define CXX11_EXPLICIT explicit
#else
#define _CXX11_CONSTEXPR
#define CXX11_EXPLICIT
#endif

#if __cplusplus >= 201402L
#define _CXX14_CONSTEXPR constexpr
#else
#define _CXX14_CONSTEXPR
#endif

namespace cm
{

template <INTM_FAST_32 _MOD = 998244353>
class intm
{

#if __cplusplus >= 201103L
  static_assert(_MOD * 2 < std::numeric_limits<INTM_FAST_32>::max(), "");
#endif

public:
  static constexpr int MOD = _MOD;

protected:
  INTM_FAST_32 a = 0;
  _ATTR_INLINE _CXX11_CONSTEXPR explicit intm(INTM_FAST_32 a, int) : a(a) {}

  static _ATTR_INLINE _CXX11_CONSTEXPR INTM_FAST_32 __impl_inc(INTM_FAST_32 a)
  {
    return a < 0 ? a + MOD : a;
  }

  static _ATTR_INLINE _CXX11_CONSTEXPR INTM_FAST_32 __impl_dec(INTM_FAST_32 a)
  {
    return a >= MOD ? a - MOD : a;
  }

  template <class IntType>
  static _ATTR_INLINE _CXX14_CONSTEXPR INTM_FAST_32 __impl_pow(INTM_FAST_32 a,
                                                               IntType      b)
  {
    INTM_FAST_32 res = 1;
    for (; b; b >>= 1)
    {
      if (b & 1)
      {
        res = (INTM_FAST_32)((INTM_FAST_64)(res) * (INTM_FAST_64)(a) % MOD);
      }
      a = (INTM_FAST_32)((INTM_FAST_64)(a) * (INTM_FAST_64)(a) % MOD);
    }
    return res;
  }

  static int pretty(int x)
  {
    if (x >= MOD - 1000)
      return x - MOD;
    return x;
  }

public:
#if __cplusplus >= 201103L
  intm() = default;
#else
  intm() {}
#endif

  static intm raw(INTM_FAST_32 x)
  {
    return intm(x, 0);
  }

  // clang-format off
  _ATTR_INLINE _CXX11_CONSTEXPR intm(int a)                : a(static_cast<INTM_FAST_32>(__impl_inc(a % MOD))) {}
  _ATTR_INLINE _CXX11_CONSTEXPR intm(long a)               : a(static_cast<INTM_FAST_32>(__impl_inc(a % MOD))) {}
  _ATTR_INLINE _CXX11_CONSTEXPR intm(long long a)          : a(static_cast<INTM_FAST_32>(__impl_inc(a % MOD))) {}
  _ATTR_INLINE _CXX11_CONSTEXPR intm(unsigned int a)       : a(static_cast<INTM_FAST_32>(a % MOD))             {}
  _ATTR_INLINE _CXX11_CONSTEXPR intm(unsigned long a)      : a(static_cast<INTM_FAST_32>(a % MOD))             {}
  _ATTR_INLINE _CXX11_CONSTEXPR intm(unsigned long long a) : a(static_cast<INTM_FAST_32>(a % MOD))             {}
  // clang-format on

  template <class _IntType>
  _ATTR_INLINE _CXX11_CONSTEXPR CXX11_EXPLICIT operator _IntType() const
  {
    return a;
  }
  _ATTR_INLINE friend std::ostream &operator<<(std::ostream &out,
                                               const intm    rhs)
  {
#ifdef CM_DEBUG
    out << pretty(rhs.a);
    return out;
#else
    out << rhs.a;
    return out;
#endif
  }
  _ATTR_INLINE friend std::istream &operator>>(std::istream &in, intm &rhs)
  {
    long long a;
    in >> a;
    rhs = intm(a);
    return in;
  }

  template <class _IntType>
  _ATTR_INLINE _CXX14_CONSTEXPR intm pow(_IntType k) const
  {
    return raw(__impl_pow(a, k));
  }
  _ATTR_INLINE _CXX14_CONSTEXPR intm inv() const
  {
#ifdef CM_DEBUG_H
    cm_assert(a != 0, "warning: 0 do not have inv");
#endif
    return raw(__impl_pow(a, MOD - 2));
  }

  // clang-format off
  _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator<  (const intm a, const intm b) { return a.a <  b.a; }
  _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator<= (const intm a, const intm b) { return a.a <= b.a; }
  _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator>  (const intm a, const intm b) { return a.a >  b.a; }
  _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator>= (const intm a, const intm b) { return a.a >= b.a; }
  _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator== (const intm a, const intm b) { return a.a == b.a; }
  _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator!= (const intm a, const intm b) { return a.a != b.a; }

  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator<  (const _IntType a, const intm b) { return a   <  b.a; }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator<  (const intm a, const _IntType b) { return a.a <  b;   }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator<= (const _IntType a, const intm b) { return a   <= b.a; }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator<= (const intm a, const _IntType b) { return a.a <= b;   }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator>  (const _IntType a, const intm b) { return a   >  b.a; }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator>  (const intm a, const _IntType b) { return a.a >  b;   }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator>= (const _IntType a, const intm b) { return a   >= b.a; }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator>= (const intm a, const _IntType b) { return a.a >= b;   }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator== (const _IntType a, const intm b) { return a   == b.a; }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator== (const intm a, const _IntType b) { return a.a == b;   }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator!= (const _IntType a, const intm b) { return a   != b.a; }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend bool operator!= (const intm a, const _IntType b) { return a.a != b;   }

  _ATTR_INLINE _CXX11_CONSTEXPR friend intm  operator+  (const intm  a, const intm b) { return raw(__impl_dec(a.a + b.a)); }
  _ATTR_INLINE _CXX11_CONSTEXPR friend intm  operator-  (const intm  a, const intm b) { return raw(__impl_inc(a.a - b.a)); }
  _ATTR_INLINE _CXX11_CONSTEXPR friend intm  operator*  (const intm  a, const intm b) { return raw(static_cast<INTM_FAST_32>((INTM_FAST_64)(a.a) * (INTM_FAST_64)(b.a) % MOD)); }
  _ATTR_INLINE _CXX14_CONSTEXPR friend intm  operator/  (const intm  a, const intm b) { return a * b.inv(); }
  _ATTR_INLINE _CXX11_CONSTEXPR friend intm& operator+= (      intm &a, const intm b) { return a = a + b;  }
  _ATTR_INLINE _CXX11_CONSTEXPR friend intm& operator-= (      intm &a, const intm b) { return a = a - b;  }
  _ATTR_INLINE _CXX11_CONSTEXPR friend intm& operator*= (      intm &a, const intm b) { return a = a * b;  }
  _ATTR_INLINE _CXX14_CONSTEXPR friend intm& operator/= (      intm &a, const intm b) { return a = a / b;  }

  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend intm  operator+  (const intm  a, const _IntType b) { return a +  intm(b); }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend intm  operator-  (const intm  a, const _IntType b) { return a -  intm(b); }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend intm  operator*  (const intm  a, const _IntType b) { return a *  intm(b); }
  template <class _IntType> _ATTR_INLINE _CXX14_CONSTEXPR friend intm  operator/  (const intm  a, const _IntType b) { return a /  intm(b); }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend intm& operator+= (      intm &a, const _IntType b) { return a += intm(b); }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend intm& operator-= (      intm &a, const _IntType b) { return a -= intm(b); }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend intm& operator*= (      intm &a, const _IntType b) { return a *= intm(b); }
  template <class _IntType> _ATTR_INLINE _CXX14_CONSTEXPR friend intm& operator/= (      intm &a, const _IntType b) { return a /= intm(b); }

  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend intm      operator+  (const _IntType  a, const intm b) { return intm(a) +  b; }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend intm      operator-  (const _IntType  a, const intm b) { return intm(a) -  b; }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend intm      operator*  (const _IntType  a, const intm b) { return intm(a) *  b; }
  template <class _IntType> _ATTR_INLINE _CXX14_CONSTEXPR friend intm      operator/  (const _IntType  a, const intm b) { return intm(a) /  b; }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend _IntType& operator+= (      _IntType &a, const intm b) { return a += _IntType(b); }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend _IntType& operator-= (      _IntType &a, const intm b) { return a -= _IntType(b); }
  template <class _IntType> _ATTR_INLINE _CXX11_CONSTEXPR friend _IntType& operator*= (      _IntType &a, const intm b) { return a *= _IntType(b); }
  template <class _IntType> _ATTR_INLINE _CXX14_CONSTEXPR friend _IntType& operator/= (      _IntType &a, const intm b) { return a /= _IntType(b); }
  // clang-format on
};

} // namespace cm

#undef _ATTR_INLINE
#undef _CXX11_CONSTEXPR
#undef _CXX14_CONSTEXPR
#undef INTM_FAST_32
#undef INTM_FAST_64

#endif
