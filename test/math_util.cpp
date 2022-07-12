#include "../math_util.hpp"
#include "../debug.hpp"
#include "../intm.hpp"
#include <cassert>

int main()
{
  using int_t = cm::intm<998244353>;
  using mu    = cm::math_util<int_t>;

  cm_assert(mu::fac.get(10) == 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1);
  cm_assert(mu::fac.get(10) * mu::ifac.get(10) == 1);
  cm_assert(mu::inv.get(114) * 114 == 1);
  cm_assert(mu::binom.get(10, 3) == 10 * 9 * 8 / 3 / 2 / 1);

  mu::binom.require(100);
  cm_assert(mu::binom(100, 3) == 100 * 99 * 98 / 3 / 2 / 1);
}
