#ifndef CM_UTIL_H
#define CM_UTIL_H

#include <array>
#include <iterator>
#include <type_traits>
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

template <class T, class vt = std::vector<std::vector<T>>>
vt vec_transpose(const vt &v)
{
  size_t n = v.size();
  size_t m = v[0].size();

#ifdef CM_DEBUG
  for (const auto &vv : v)
    cm_assert(vv.size() == m);
#endif

  vt r(m, std::vector<T>(n));
  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < m; j++)
      r[j][i] = v[i][j];

  return r;
}

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

#if __cplusplus >= 201402L

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

auto vec_take(size_t id)
{
  return [id](const auto &v) { return v[id]; };
}

template <class F, class... T>
auto vec_map(F &&f, const std::vector<T> &...v)
{
  using mapped_t = decltype(f(std::declval<T>()...));

  std::size_t n = [](auto x, auto...) { return x; }(v.size()...);

  std::vector<mapped_t> r;
  r.reserve(n);

  cm::y_combinate([&](auto &&self, std::size_t cnt, auto... its) -> void {
    if (cnt > 0)
    {
      r.emplace_back(f((*its)...));
      self(cnt - 1, std::next(its)...);
    }
  })(n, v.begin()...);

  return r;
}

template <class F>
auto mapper(F &&f)
{
  return [f](auto v) { return vec_map(f, v); };
}

template <class T>
auto transfer(T &&v)
{
  return std::forward<T>(v);
}
template <class T, class F0, class... F1>
auto transfer(T &&v, F0 &&f0, F1 &&...f1)
{
  return transfer(f0(std::forward<T>(v)), f1...);
}

#endif

} // namespace cm

using cm::check_max;
using cm::check_min;

#ifdef CM_CLANGD
#define CONSTRAINT(n, a, b) constexpr auto n = b;
#else
#define CONSTRAINT(n, a, b) constexpr auto n = a;
#endif

#endif
