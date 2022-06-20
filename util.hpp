#ifndef CM_UTIL_H
#define CM_UTIL_H

#include <array>
#include <utility>
#include <vector>

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

template <class value_type = int>
struct counter_t
{
private:
  value_type _value{};

public:
  value_type operator()()
  {
    return _value++;
  }

  value_type size() const
  {
    return _value;
  }

  void clear()
  {
    _value = value_type{};
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

#if __cplusplus >= 201402L
namespace cm
{

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

} // namespace cm
#endif

#ifdef CM_CLANGD
#define CONSTRAINT(n, a, b) constexpr auto n = b;
#else
#define CONSTRAINT(n, a, b) constexpr auto n = a;
#endif

#endif
