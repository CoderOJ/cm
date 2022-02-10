#ifndef CM_BARRET_H
#define CM_BARRET_H

#include <cstdint>

namespace cm
{

struct barret_reduction
{
private:
  uint64_t _mod;
  uint64_t _mod_inv;
  int      _shift;

  static uint64_t _mul64_hb(uint64_t a, uint64_t b)
  {
    return (__uint128_t(a) * b) >> 64;
  }

public:
  barret_reduction(uint64_t mod)
  {
    this->_mod   = mod;
    this->_shift = 63 - __builtin_clzll(mod);
    this->_mod_inv =
        static_cast<uint64_t>((__uint128_t(1) << (_shift + 64)) / mod) + 1;
  }

  uint64_t div(uint64_t x)
  {
    return _mul64_hb(x, _mod_inv) >> _shift;
  }

  uint64_t mod(uint64_t x)
  {
    return x - div(x) * _mod;
  }
};

} // namespace cm

#endif
