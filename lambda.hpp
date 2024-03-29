// relabel off

#ifndef CM_LAMBDA_H
#define CM_LAMBDA_H

#include <type_traits>

namespace cm::lambda
{

namespace __impl
{

template <int k>
struct __get_kth
{
  template <class Ta, class... Tb>
  static auto call(Ta, Tb... b)
  {
    return __get_kth<k - 1>::call(b...);
  }
};
template <>
struct __get_kth<0>
{
  template <class Ta, class... Tb>
  static auto call(Ta a, Tb...)
  {
    return a;
  }
};

struct lambda_base
{
};

template <class T>
struct lambda_constant : public lambda_base
{
  const T &a;
  lambda_constant(const T &a) : a(a) {}

  template <class...>
  const T &operator()(...) const
  {
    return a;
  }
};
template <class T>
struct lambda_reference : public lambda_base
{
  T &a;
  lambda_reference(T &a) : a(a) {}

  template <class...>
  T &operator()(...) const
  {
    return a;
  }
};

#define __lambda_define_bop(op, op_name)                                       \
  template <class Ta, class Tb>                                                \
  struct lambda_bop_##op_name : public lambda_base                             \
  {                                                                            \
    static_assert(std::is_base_of<lambda_base, Ta>::value, "");                \
    static_assert(std::is_base_of<lambda_base, Tb>::value, "");                \
                                                                               \
    Ta a;                                                                      \
    Tb b;                                                                      \
    lambda_bop_##op_name(const Ta &a, const Tb &b) : a(a), b(b) {}             \
                                                                               \
    template <class... T>                                                      \
    auto operator()(T... p) const -> decltype(a(p...) op b(p...))              \
    {                                                                          \
      return a(p...) op b(p...);                                               \
    }                                                                          \
  };                                                                           \
  template <class Ta, class Tb,                                                \
            typename = typename std::enable_if<                                \
                std::is_base_of<lambda_base, Ta>::value ||                     \
                std::is_base_of<lambda_base, Tb>::value>::type>                \
  auto operator op(const Ta &a, const Tb &b)                                   \
  {                                                                            \
    if constexpr (!std::is_base_of<lambda_base, Ta>::value)                    \
      return lambda_bop_##op_name<lambda_constant<Ta>, Tb>(                    \
          lambda_constant<Ta>(a), b);                                          \
    else if constexpr (!std::is_base_of<lambda_base, Tb>::value)               \
      return lambda_bop_##op_name<Ta, lambda_constant<Tb>>(                    \
          a, lambda_constant<Tb>(b));                                          \
    else                                                                       \
      return lambda_bop_##op_name<Ta, Tb>(a, b);                               \
  }                                                                            \
  template <class Ta, class Tb,                                                \
            typename = typename std::enable_if<                                \
                !std::is_base_of<lambda_base, Ta>::value &&                    \
                std::is_base_of<lambda_base, Tb>::value>::type>                \
  auto operator op(Ta &a, const Tb &b)                                         \
  {                                                                            \
    return lambda_bop_##op_name<lambda_reference<Ta>, Tb>(                     \
        lambda_reference<Ta>(a), b);                                           \
  }                                                                            \
  template <class Ta, class Tb,                                                \
            typename = typename std::enable_if<                                \
                std::is_base_of<lambda_base, Ta>::value &&                     \
                !std::is_base_of<lambda_base, Tb>::value>::type>               \
  auto operator op(const Ta &a, Tb &b)                                         \
  {                                                                            \
    return lambda_bop_##op_name<Ta, lambda_reference<Tb>>(                     \
        a, lambda_reference<Tb>(b));                                           \
  }

__lambda_define_bop(+, add);
__lambda_define_bop(-, sub);
__lambda_define_bop(*, mul);
__lambda_define_bop(/, div);
__lambda_define_bop(<, le);
__lambda_define_bop(<=, leq);
__lambda_define_bop(>, ge);
__lambda_define_bop(>=, geq);
__lambda_define_bop(==, eq);
__lambda_define_bop(!=, neq);
__lambda_define_bop(<<, lsh);
__lambda_define_bop(>>, rsh);
__lambda_define_bop(&&, and);
__lambda_define_bop(||, or);
__lambda_define_bop(&, land);
__lambda_define_bop(|, lor);
__lambda_define_bop(^, lxor);

#define __lambda_define_sop(op, op_name)                                       \
  template <class Ta>                                                          \
  struct lambda_sop_##op_name : public lambda_base                             \
  {                                                                            \
    static_assert(std::is_base_of<lambda_base, Ta>::value, "");                \
                                                                               \
    Ta a;                                                                      \
    lambda_sop_##op_name(const Ta &a) : a(a) {}                                \
                                                                               \
    template <class... T>                                                      \
    auto operator()(T... p) const -> decltype(op a(p...))                      \
    {                                                                          \
      return op a(p...);                                                       \
    }                                                                          \
  };                                                                           \
  template <class Ta, typename = typename std::enable_if<                      \
                          std::is_base_of<lambda_base, Ta>::value>::type>      \
  auto operator op(const Ta &a)                                                \
  {                                                                            \
    return lambda_sop_##op_name<Ta>(a);                                        \
  }

__lambda_define_sop(!, not );
__lambda_define_sop(~, lnot);
__lambda_define_sop(+, pos);
__lambda_define_sop(-, neg);

} // namespace __impl

namespace placeholders
{

template <int k>
struct placeholder : public __impl::lambda_base
{
  template <class... T>
  auto operator()(T... p) const
  {
    return __impl::__get_kth<k>::call(p...);
  }
};

placeholder<0> _0;
placeholder<1> _1;
placeholder<2> _2;
placeholder<3> _3;
placeholder<4> _4;
placeholder<5> _5;

} // namespace placeholders

} // namespace cm::lambda

#endif
