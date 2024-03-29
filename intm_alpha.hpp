#ifndef CM_INTM_ALPHA_H
#define CM_INTM_ALPHA_H

#include <iostream>
#include <limits>
#include <type_traits>

namespace cm
{

namespace intm_impl
{

using i64 = unsigned long long;

template <class type_list, size_t id>
struct __tl_get
{
  static_assert(id < type_list::size, "out of bound");
  using type = typename __tl_get<typename type_list::cdr, id - 1>::type;
};
template <class type_list>
struct __tl_get<type_list, 0>
{
  using type = typename type_list::car;
};

template <class... T>
struct type_list_t
{
};
template <class A, class... T>
struct type_list_t<A, T...>
{
  using car = A;
  using cdr = type_list_t<T...>;

  template <size_t id>
  using get = typename __tl_get<type_list_t<A, T...>, id>::type;

  static constexpr size_t size  = cdr::size + 1;
  static constexpr bool   empty = !size;
};
template <>
struct type_list_t<>
{
  static constexpr size_t size  = 0;
  static constexpr bool   empty = !size;
};

template <i64 rr>
struct range_t
{
  static constexpr i64 r()
  {
    return rr;
  }
};

struct intm_tag
{
};

template <int __mod, i64 __r>
struct intm_r : public intm_tag
{
  static constexpr int mod = __mod;
  using range              = range_t<__r>;

  i64 a;
  constexpr intm_r(i64 a) : a(a) {}
  constexpr i64 eval() const
  {
    return a;
  }
  template <class integer, typename = typename std::enable_if<
                               std::is_integral<integer>::value>::type>
  constexpr explicit operator integer()
  {
    return static_cast<integer>(eval());
  }
};

template <class mm>
struct __get_mm_adjust
{
  using type = typename std::conditional<
      mm::car::range::r() < mm::car::mod, typename mm::car,
      typename __get_mm_adjust<typename mm::cdr>::type>::type;
};
template <>
struct __get_mm_adjust<type_list_t<>>
{
  using type = void;
};

template <int __mod>
struct intm : public intm_tag
{
  static constexpr int mod = __mod;
  using range              = range_t<mod - 1>;

  unsigned int  a = 0;
  constexpr i64 eval() const
  {
    return static_cast<i64>(a);
  }
  template <class integer, typename = typename std::enable_if<
                               std::is_integral<integer>::value>::type>
  constexpr explicit operator integer()
  {
    return static_cast<integer>(eval());
  }

  template <class integer>
  constexpr static auto make_intm(integer x)
  {
    return intm_r<mod, std::numeric_limits<decltype(x)>::max()>(x);
  }
  template <class integer>
  constexpr static auto raw(integer x)
  {
    return intm_r<mod, mod - 1>(x);
  }

private:
  template <class intm_t>
  constexpr unsigned int adjust(intm_t);

public:
  intm() = default;
  template <class intm_t, typename = typename std::enable_if<
                              std::is_base_of<intm_tag, intm_t>::value>::type>
  constexpr intm(intm_t a) : a(adjust(a))
  {
  }
  template <class intm_t, typename = typename std::enable_if<std::is_base_of<
                              intm_tag, intm_t>::value>::typ::typee>
  constexpr intm &operator=(intm_t rhs)
  {
    a = adjust(rhs);
    return *this;
  }

  template <class integer,
            typename =
                typename std::enable_if<std::is_integral<integer>::value>::type,
            typename = void>
  constexpr intm(integer x)
  {
    *this = make_intm(x);
  }
};

struct add_fitin
{
  template <class ra, class rb>
  static constexpr bool check()
  {
    return ra::r() <= std::numeric_limits<i64>::max() - rb::r();
  }
};

struct mul_fitin
{
  template <class ra, class rb>
  static constexpr bool check()
  {
    return ra::r() <= std::numeric_limits<i64>::max() / rb::r();
  }
};

template <int __mod>
struct mod_method
{
  static constexpr int mod = __mod;
};
template <int mod, class input_range>
struct mm_nothing : public mod_method<mod>
{
  static constexpr int cost = 0;
  using range               = input_range;
  inline constexpr i64 operator()(i64 a) const
  {
    return a;
  }
};
template <int mod, class input_range>
struct mm_ifsub : public mod_method<mod>
{
  static constexpr int cost    = 3;
  static constexpr i64 sub_val = (input_range::r() / mod + 1) / 2 * mod;
  using range = range_t<std::max(sub_val - 1, input_range::r() - sub_val)>;
  inline constexpr i64 operator()(i64 a) const
  {
    return a >= sub_val ? a - sub_val : a;
  }
};
template <int mod, class input_range>
struct mm_ifsub2 : public mod_method<mod>
{
  static constexpr int cost = 6;
  using range =
      typename mm_ifsub<mod, typename mm_ifsub<mod, input_range>::range>::range;
  inline constexpr i64 operator()(i64 a) const
  {
    return mm_ifsub<mod, typename mm_ifsub<mod, input_range>::range>()(
        mm_ifsub<mod, input_range>()(a));
  }
};
template <int mod, class input_range>
struct mm_ifsub3 : public mod_method<mod>
{
  static constexpr int cost = 9;
  using range =
      typename mm_ifsub<mod,
                        typename mm_ifsub2<mod, input_range>::range>::range;
  inline constexpr i64 operator()(i64 a) const
  {
    return mm_ifsub<mod, typename mm_ifsub2<mod, input_range>::range>()(
        mm_ifsub2<mod, input_range>()(a));
  }
};
template <int mod, class input_range>
struct mm_getmod : public mod_method<mod>
{
  static constexpr int cost = 11;
  using range = range_t<std::min(input_range::r(), mod - (i64)(1))>;
  inline constexpr i64 operator()(i64 a) const
  {
    return a % mod;
  }
};

template <int mod, class range>
using mm_list = type_list_t<mm_nothing<mod, range>, mm_ifsub<mod, range>,
                            mm_ifsub2<mod, range>, mm_getmod<mod, range>>;

template <class lhs_mm, class rhs_mm, class fitin_pred>
struct __getmm_ii
{
  static constexpr int value()
  {
    using lhs_r = typename lhs_mm::range;
    using rhs_r = typename rhs_mm::range;
    if constexpr (fitin_pred::template check<lhs_r, rhs_r>())
      return lhs_mm::cost + rhs_mm::cost;
    return std::numeric_limits<int>::max() / 2;
  }
};

template <class lhs_mm, class rhs_mm_list, class fitin_pred>
struct __getmm_il
{
  static constexpr std::pair<int, size_t> value()
  {
    constexpr auto res_car =
        __getmm_ii<lhs_mm, typename rhs_mm_list::car, fitin_pred>::value();
    constexpr auto res_cdr =
        __getmm_il<lhs_mm, typename rhs_mm_list::cdr, fitin_pred>::value();
    if constexpr (res_car < res_cdr.first)
      return std::make_pair(res_car, 0);
    else
      return std::make_pair(res_cdr.first, res_cdr.second + 1);
  }
};
template <class lhs_mm, class fitin_pred>
struct __getmm_il<lhs_mm, type_list_t<>, fitin_pred>
{
  static constexpr std::pair<int, size_t> value()
  {
    return std::make_pair(std::numeric_limits<int>::max() / 2, 0);
  }
};

template <class lhs_mm_list, class rhs_mm_list, class fitin_pred>
struct __getmm_ll
{
  static constexpr std::pair<int, std::pair<size_t, size_t>> value()
  {
    constexpr auto res_car =
        __getmm_il<typename lhs_mm_list::car, rhs_mm_list, fitin_pred>::value();
    constexpr auto res_cdr =
        __getmm_ll<typename lhs_mm_list::cdr, rhs_mm_list, fitin_pred>::value();
    if constexpr (res_car.first < res_cdr.first)
      return std::make_pair(res_car.first, std::make_pair(0, res_car.second));
    else
      return std::make_pair(
          res_cdr.first,
          std::make_pair(res_cdr.second.first + 1, res_cdr.second.second));
  }
};
template <class rhs_mm_list, class fitin_pred>
struct __getmm_ll<type_list_t<>, rhs_mm_list, fitin_pred>
{
  static constexpr std::pair<int, std::pair<size_t, size_t>> value()
  {
    return std::make_pair(std::numeric_limits<int>::max() / 2,
                          std::make_pair(0, 0));
  }
};

template <class lhs_t>
struct intm_op_neg : public intm_tag
{
  intm_op_neg()                    = delete;
  intm_op_neg(const intm_op_neg &) = default;
  intm_op_neg(intm_op_neg &&)      = default;
  intm_op_neg         &operator=(intm_op_neg) = delete;
  intm_op_neg         &operator=(intm_op_neg &&) = delete;
  static constexpr int mod                       = lhs_t::mod;
  using range = range_t<(lhs_t::range::r() - 1) / mod * mod + mod>;
  i64 a;
  constexpr intm_op_neg(lhs_t lhs) : a(range::r() - lhs.eval()) {}
  constexpr i64 eval() const
  {
    return a;
  }
  template <class integer, typename = typename std::enable_if<
                               std::is_integral<integer>::value>::type>
  constexpr explicit operator integer()
  {
    return static_cast<integer>(eval());
  }
};
template <class lhs_t, class rhs_t>
struct intm_op_add : public intm_tag
{
  intm_op_add()                    = delete;
  intm_op_add(const intm_op_add &) = default;
  intm_op_add(intm_op_add &&)      = default;
  intm_op_add &operator=(intm_op_add) = delete;
  intm_op_add &operator=(intm_op_add &&) = delete;
  static_assert(lhs_t::mod == rhs_t::mod, "");
  static constexpr int mod = lhs_t::mod;

private:
  static constexpr auto mm =
      __getmm_ll<mm_list<mod, typename lhs_t::range>,
                 mm_list<mod, typename rhs_t::range>, add_fitin>::value();
  static constexpr int lhs_mm = mm.second.first;
  static constexpr int rhs_mm = mm.second.second;
  using lhs_mm_t =
      typename mm_list<mod, typename lhs_t::range>::template get<lhs_mm>;
  using rhs_mm_t =
      typename mm_list<mod, typename rhs_t::range>::template get<rhs_mm>;
  i64 a;

public:
  using range = range_t<lhs_mm_t::range::r() + rhs_mm_t::range::r()>;
  constexpr intm_op_add(lhs_t lhs, rhs_t rhs)
      : a(lhs_mm_t()(lhs.eval()) + rhs_mm_t()(rhs.eval()))
  {
  }
  constexpr i64 eval() const
  {
    return a;
  }
  template <class integer, typename = typename std::enable_if<
                               std::is_integral<integer>::value>::type>
  constexpr explicit operator integer()
  {
    return static_cast<integer>(eval());
  }
};
template <class lhs_t, class rhs_t>
struct intm_op_mul : public intm_tag
{
  intm_op_mul()                    = delete;
  intm_op_mul(const intm_op_mul &) = default;
  intm_op_mul(intm_op_mul &&)      = default;
  intm_op_mul &operator=(intm_op_mul) = delete;
  intm_op_mul &operator=(intm_op_mul &&) = delete;
  static_assert(lhs_t::mod == rhs_t::mod, "");
  static constexpr int mod = lhs_t::mod;

private:
  static constexpr auto mm =
      __getmm_ll<mm_list<mod, typename lhs_t::range>,
                 mm_list<mod, typename rhs_t::range>, mul_fitin>::value();
  static constexpr int lhs_mm = mm.second.first;
  static constexpr int rhs_mm = mm.second.second;
  using lhs_mm_t =
      typename mm_list<mod, typename lhs_t::range>::template get<lhs_mm>;
  using rhs_mm_t =
      typename mm_list<mod, typename rhs_t::range>::template get<rhs_mm>;
  i64 a;

public:
  using range = range_t<lhs_mm_t::range::r() * rhs_mm_t::range::r()>;
  constexpr intm_op_mul(lhs_t lhs, rhs_t rhs)
      : a(lhs_mm_t()(lhs.eval()) * rhs_mm_t()(rhs.eval()))
  {
  }
  constexpr i64 eval()
  {
    return a;
  }
  template <class integer, typename = typename std::enable_if<
                               std::is_integral<integer>::value>::type>
  constexpr explicit operator integer()
  {
    return static_cast<integer>(eval());
  }
};

template <
    class lhs_t, class rhs_t,
    typename =
        typename std::enable_if<std::is_base_of<intm_tag, lhs_t>::value>::type,
    typename =
        typename std::enable_if<std::is_base_of<intm_tag, rhs_t>::value>::type>
constexpr auto operator+(lhs_t lhs, rhs_t rhs)
{
  return intm_op_add<lhs_t, rhs_t>(lhs, rhs);
}
template <
    class lhs_t, class rhs_t,
    typename =
        typename std::enable_if<std::is_base_of<intm_tag, lhs_t>::value>::type,
    typename =
        typename std::enable_if<std::is_base_of<intm_tag, rhs_t>::value>::type>
constexpr lhs_t &operator+=(lhs_t &lhs, rhs_t rhs)
{
  return lhs = intm_op_add<lhs_t, rhs_t>(lhs, rhs);
}
template <
    class lhs_t, class rhs_t,
    typename =
        typename std::enable_if<std::is_base_of<intm_tag, lhs_t>::value>::type,
    typename =
        typename std::enable_if<std::is_base_of<intm_tag, rhs_t>::value>::type>
constexpr auto operator-(lhs_t lhs, rhs_t rhs)
{
  return intm_op_add(lhs, intm_op_neg<rhs_t>(rhs));
}
template <
    class lhs_t, class rhs_t,
    typename =
        typename std::enable_if<std::is_base_of<intm_tag, lhs_t>::value>::type,
    typename =
        typename std::enable_if<std::is_base_of<intm_tag, rhs_t>::value>::type>
constexpr lhs_t &operator-=(lhs_t &lhs, rhs_t rhs)
{
  return lhs = intm_op_add(lhs, intm_op_neg<rhs_t>(rhs));
}
template <
    class lhs_t, class rhs_t,
    typename =
        typename std::enable_if<std::is_base_of<intm_tag, lhs_t>::value>::type,
    typename =
        typename std::enable_if<std::is_base_of<intm_tag, rhs_t>::value>::type>
constexpr auto operator*(lhs_t lhs, rhs_t rhs)
{
  return intm_op_mul<lhs_t, rhs_t>(lhs, rhs);
}
template <
    class lhs_t, class rhs_t,
    typename =
        typename std::enable_if<std::is_base_of<intm_tag, lhs_t>::value>::type,
    typename =
        typename std::enable_if<std::is_base_of<intm_tag, rhs_t>::value>::type>
constexpr lhs_t &operator*=(lhs_t &lhs, rhs_t rhs)
{
  return lhs = intm_op_mul<lhs_t, rhs_t>(lhs, rhs);
}

template <int mod>
template <class intm_t>
constexpr unsigned int intm<mod>::adjust(intm_t intm)
{
  using mm =
      typename __get_mm_adjust<mm_list<mod, typename intm_t::range>>::type;
  return static_cast<unsigned int>(mm()(intm.eval()));
}

template <class lhs_t, class rhs_t,
          typename = typename std::enable_if<lhs_t::mod == rhs_t::mod>::type>
constexpr bool operator<(const lhs_t a, const rhs_t b)
{
  return a.eval() < b.eval();
}
template <class lhs_t, class rhs_t,
          typename = typename std::enable_if<lhs_t::mod == rhs_t::mod>::type>
constexpr bool operator<=(const lhs_t a, const rhs_t b)
{
  return a.eval() <= b.eval();
}
template <class lhs_t, class rhs_t,
          typename = typename std::enable_if<lhs_t::mod == rhs_t::mod>::type>
constexpr bool operator>(const lhs_t a, const rhs_t b)
{
  return a.eval() > b.eval();
}
template <class lhs_t, class rhs_t,
          typename = typename std::enable_if<lhs_t::mod == rhs_t::mod>::type>
constexpr bool operator>=(const lhs_t a, const rhs_t b)
{
  return a.eval() >= b.eval();
}
template <class lhs_t, class rhs_t,
          typename = typename std::enable_if<lhs_t::mod == rhs_t::mod>::type>
constexpr bool operator==(const lhs_t a, const rhs_t b)
{
  return a.eval() == b.eval();
}
template <class lhs_t, class rhs_t,
          typename = typename std::enable_if<lhs_t::mod == rhs_t::mod>::type>
constexpr bool operator!=(const lhs_t a, const rhs_t b)
{
  return a.eval() != b.eval();
}

template <class rhs_t, typename = typename std::enable_if<
                           std::is_base_of<intm_tag, rhs_t>::value>::type>
std::ostream &operator<<(std::ostream &out, const rhs_t rhs)
{
  out << rhs.eval();
  return out;
}

} // namespace intm_impl

using intm_impl::intm;
using intm_impl::intm_r;

} // namespace cm

#endif
