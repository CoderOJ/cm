#include "../math_util.hpp"
#include "../debug.hpp"
#include "../intm.hpp"

int main()
{
  constexpr int MOD = 998244353;
  using int_t       = cm::intm<MOD>;
  using mu          = cm::math_util<int_t>;

  mu::fac.require(5);
  cm_assert(mu::fac(4) == 24);
  cm_assert(mu::fac(5) == 120);
  cm_assert(mu::fac.get(6) == 720);

  mu::ifac.require(5);
  cm_assert(mu::fac(4) * mu::ifac(4) == 1);
  cm_assert(mu::fac(5) * mu::ifac(5) == 1);
  cm_assert(mu::fac.get(20) * mu::ifac.get(20) == 1);

  mu::binom.require(10);
  cm_assert(mu::binom(5, 2) == 10);
  cm_assert(mu::binom(6, 3) == 20);
  cm_assert(mu::binom.get(200, 2) == 19900);
  cm_assert(mu::binom.get(200, 0) == 1);

  return 0;
}
