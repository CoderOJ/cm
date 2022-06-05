#ifndef CM_UTIL_H
#define CM_UTIL_H

#include <array>
#include <utility>
#include <vector>

#define _INLINE inline __attribute__((always_inline))

namespace cm
{

template <class T>
inline bool check_min(T &a, const T &b)
{
  return b < a ? a = b, 1 : 0;
}
template <class T>
inline bool check_max(T &a, const T &b)
{
  return a < b ? a = b, 1 : 0;
}

struct once_t
{
  bool _once = true;
  bool operator()()
  {
    return _once ? (_once = false, true) : false;
  }
};

namespace impl
{

template <class T, std::size_t n0, std::size_t... n1>
struct __array
{
  using type = std::array<typename __array<T, n1...>::type, n0>;
};

template <class T, std::size_t n0>
struct __array<T, n0>
{
  using type = std::array<T, n0>;
};

} // namespace impl

template <class T, std::size_t... n0>
using array = typename impl::__array<T, n0...>::type;

} // namespace cm

using cm::check_max;
using cm::check_min;

template <class A, class B>
_INLINE std::pair<A, B> operator+(const std::pair<A, B> &lhs,
                                  const std::pair<A, B> &rhs)
{
  return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}
template <class A, class B>
_INLINE std::pair<A, B> operator-(const std::pair<A, B> &lhs,
                                  const std::pair<A, B> &rhs)
{
  return std::make_pair(lhs.first - rhs.first, lhs.second - rhs.second);
}
template <class A, class B>
_INLINE std::pair<A, B> &operator+=(std::pair<A, B>       &lhs,
                                    const std::pair<A, B> &rhs)
{
  lhs.first += rhs.first;
  lhs.second += rhs.second;
  return lhs;
}
template <class A, class B>
_INLINE std::pair<A, B> &operator-=(std::pair<A, B>       &lhs,
                                    const std::pair<A, B> &rhs)
{
  lhs.first -= rhs.first;
  lhs.second -= rhs.second;
  return lhs;
}
template <class A, class B>
_INLINE std::pair<A, B> operator*(const std::pair<A, B> &lhs,
                                  const std::pair<A, B> &rhs)
{
  return std::make_pair(lhs.first * rhs.first, lhs.second * rhs.second);
}
template <class A, class B>
_INLINE std::pair<A, B> operator/(const std::pair<A, B> &lhs,
                                  const std::pair<A, B> &rhs)
{
  return std::make_pair(lhs.first / rhs.first, lhs.second / rhs.second);
}
template <class A, class B>
_INLINE std::pair<A, B> &operator*=(std::pair<A, B>       &lhs,
                                    const std::pair<A, B> &rhs)
{
  lhs.first *= rhs.first;
  lhs.second *= rhs.second;
  return lhs;
}
template <class A, class B>
_INLINE std::pair<A, B> &operator/=(std::pair<A, B>       &lhs,
                                    const std::pair<A, B> &rhs)
{
  lhs.first /= rhs.first;
  lhs.second /= rhs.second;
  return lhs;
}

template <class F>
struct y_combinate_t
{
  F f;

  template <class... Args>
  decltype(auto) operator()(Args &&...args) const
  {
    return f(*this, std::forward<Args>(args)...);
  }
};

template <class F>
y_combinate_t<std::decay_t<F>> y_combinate(F &&f)
{
  return {std::forward<F>(f)};
};

// clang-format off
#define M__AT_INIT(line, Pred) struct CM_INNER_ATINIT##line##_t { \
  CM_INNER_ATINIT##line## _t () Pred } CM_INNER_ATINIT##line
#define M_AT_INIT(line, Pred) M__AT_INIT(line, Pred)
#define AT_INIT(Pred) M_AT_INIT(__LINE__, Pred)

#define M__AT_EXIT(line, Pred) struct CM_INNER_ATEXIT##line##_t { \
  ~CM_INNER_ATEXIT##line## _t () Pred } CM_INNER_ATEXIT##line
#define M_AT_EXIT(line, Pred) M__AT_EXIT(line, Pred)
#define AT_EXIT(Pred) M_AT_EXIT(__LINE__, Pred)
  // clang-format on

#ifdef CM_DEBUG
#define CONSTRAINT(n, a, b) constexpr auto n = b;
#else
#define CONSTRAINT(n, a, b) constexpr auto n = a;
#endif

#undef _INLINE

#endif
