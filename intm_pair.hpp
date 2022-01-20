#ifndef CM_INTM_PAIR_H
#define CM_INTM_PAIR_H

#include <iostream>
#include <type_traits>

namespace cm
{

template <class _first_t, class _second_t>
struct intm_pair
{
  _first_t  first;
  _second_t second;

  intm_pair() = default;
  intm_pair(_first_t first, _second_t second) : first(first), second(second) {}
  template <class IntType, typename = typename std::enable_if<
                               std::is_integral<IntType>::value>::type>
  intm_pair(IntType x) : first(x), second(x)
  {
  }

  auto operator+(const intm_pair &_rhs) const
      -> decltype(make_intm_pair(first + _rhs.first, second + _rhs.second));
  auto operator-(const intm_pair &_rhs) const
      -> decltype(make_intm_pair(first - _rhs.first, second - _rhs.second));
  auto operator*(const intm_pair &_rhs) const
      -> decltype(make_intm_pair(first * _rhs.first, second *_rhs.second));
  auto operator/(const intm_pair &_rhs) const
      -> decltype(make_intm_pair(first / _rhs.first, second / _rhs.second));
};

template <class _first_t, class _second_t>
intm_pair<_first_t, _second_t> make_intm_pair(_first_t first, _second_t second)
{
  return intm_pair<_first_t, _second_t>(first, second);
}

template <class _first_t, class _second_t>
auto intm_pair<_first_t, _second_t>::operator+(const intm_pair &_rhs) const
    -> decltype(make_intm_pair(first + _rhs.first, second + _rhs.second))
{
  return make_intm_pair(first + _rhs.first, second + _rhs.second);
}

template <class _first_t, class _second_t>
auto intm_pair<_first_t, _second_t>::operator-(const intm_pair &_rhs) const
    -> decltype(make_intm_pair(first - _rhs.first, second - _rhs.second))
{
  return make_intm_pair(first - _rhs.first, second - _rhs.second);
}

template <class _first_t, class _second_t>
auto intm_pair<_first_t, _second_t>::operator*(const intm_pair &_rhs) const
    -> decltype(make_intm_pair(first * _rhs.first, second *_rhs.second))
{
  return make_intm_pair(first * _rhs.first, second * _rhs.second);
}

template <class _first_t, class _second_t>
auto intm_pair<_first_t, _second_t>::operator/(const intm_pair &_rhs) const
    -> decltype(make_intm_pair(first / _rhs.first, second / _rhs.second))
{
  return make_intm_pair(first / _rhs.first, second / _rhs.second);
}

template <class _first_t, class _second_t>
std::ostream &operator<<(std::ostream                  &_out,
                         intm_pair<_first_t, _second_t> _rhs)
{
  _out << "(" << _rhs.first << ", " << _rhs.second << ")";
  return _out;
}

} // namespace cm

#endif
