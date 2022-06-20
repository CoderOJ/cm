#include "../util.hpp"
#include "../debug.hpp"

int main()
{
  {
    cm::array<int, 3, 3, 3> arr;
    arr[0][0][0] = 1;
  }
  {
    cm::array<int, 3> arr;
    arr[0] = 1;
  }

  cm::once_t once;
  cm_assert(once());
  cm_assert(!once());

  cm::counter_t<> cnt;
  cm_assert(cnt() == 0);
  cm_assert(cnt() == 1);
  cm_assert(cnt() == 2);

  auto fac = cm::y_combinate(
      [](auto f, int x) -> int { return x == 0 ? 1 : x * f(x - 1); });
  cm_assert(fac(5) == 5 * 4 * 3 * 2 * 1);
}
