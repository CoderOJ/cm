#ifndef CM_INTM_PAIR_H
#define CM_INTM_PAIR_H

#include <iostream>
#include <type_traits>

namespace cm
{

template <class first_t, class second_t> struct intm_pair
{
  first_t first;
  second_t second;

  intm_pair() = default;
  intm_pair(first_t first, second_t second) : first(first), second(second)
  {
  }
  template <class IntType, typename = typename std::enable_if<
                               std::is_integral<IntType>::value>::type>
  intm_pair(IntType x) : first(x), second(x)
  {
  }

  auto operator+(const intm_pair &rhs) const
      -> decltype(make_intm_pair(first + rhs.first, second + rhs.second));
  auto operator-(const intm_pair &rhs) const
      -> decltype(make_intm_pair(first - rhs.first, second - rhs.second));
  auto operator*(const intm_pair &rhs) const
      -> decltype(make_intm_pair(first * rhs.first, second *rhs.second));
  auto operator/(const intm_pair &rhs) const
      -> decltype(make_intm_pair(first / rhs.first, second / rhs.second));
};

template <class first_t, class second_t>
intm_pair<first_t, second_t> make_intm_pair(first_t first, second_t second)
{
  return intm_pair<first_t, second_t>(first, second);
}

template <class first_t, class second_t>
auto intm_pair<first_t, second_t>::operator+(const intm_pair &rhs) const
    -> decltype(make_intm_pair(first + rhs.first, second + rhs.second))
{
  return make_intm_pair(first + rhs.first, second + rhs.second);
}

template <class first_t, class second_t>
auto intm_pair<first_t, second_t>::operator-(const intm_pair &rhs) const
    -> decltype(make_intm_pair(first - rhs.first, second - rhs.second))
{
  return make_intm_pair(first - rhs.first, second - rhs.second);
}

template <class first_t, class second_t>
auto intm_pair<first_t, second_t>::operator*(const intm_pair &rhs) const
    -> decltype(make_intm_pair(first * rhs.first, second *rhs.second))
{
  return make_intm_pair(first * rhs.first, second * rhs.second);
}

template <class first_t, class second_t>
auto intm_pair<first_t, second_t>::operator/(const intm_pair &rhs) const
    -> decltype(make_intm_pair(first / rhs.first, second / rhs.second))
{
  return make_intm_pair(first / rhs.first, second / rhs.second);
}

template <class first_t, class second_t>
std::ostream &operator<<(std::ostream &out, intm_pair<first_t, second_t> rhs)
{
  out << "(" << rhs.first << ", " << rhs.second << ")";
  return out;
}

} // namespace cm

#endif
