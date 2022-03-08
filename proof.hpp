#ifndef CM_PROOF_H
#define CM_PROOF_H

#include "./debug"
#include <type_traits>

namespace cm
{

namespace proof
{

struct _integer_index_base
{
};

template <class T, T L, T R, int I = 0>
struct integer_index : public _integer_index_base
{
public:
  using value_type                    = T;
  static constexpr value_type range_l = L;
  static constexpr value_type range_r = R;
  static_assert(range_l < range_r, "");

  integer_index() = default;
  explicit integer_index(value_type value) : _value(value)
  {
    cm_assert(range_l <= value, range_l, value);
    cm_assert(value < range_r, value, range_r);
  }
  explicit operator value_type() const
  {
    return value;
  }
  value_type value()
  {
    return _value;
  }

  bool operator==(integer_index rhs) const
  {
    return _value == rhs._value;
  }
  bool operator!=(integer_index rhs) const
  {
    return _value != rhs._value;
  }

protected:
  int _value = range_r;
};

template <int L, int R, int I = 0>
using int_index = integer_index<int, L, R, I>;

template <class key_type, class value_type>
struct index_array
{
public:
  static_assert(std::is_base_of<_integer_index_base, key_type>::value, "");
  static constexpr int length = key_type::range_r - key_type::range_l;

  value_type operator[](key_type key) const
  {
    return value[key.value() - key_type::range_l];
  }
  value_type &operator[](key_type key)
  {
    return value[key.value() - key_type::range_l];
  }

  value_type *begin()
  {
    return value;
  }
  value_type *end()
  {
    return value + length;
  }
  const value_type *cbegin() const
  {
    return value;
  }
  const value_type *cend() const
  {
    return value + length;
  }

protected:
  value_type value[length];
};

} // namespace proof

} // namespace cm

#endif
