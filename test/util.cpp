#include "../util.hpp"
#include "../debug"

AT_INIT({ see("before main"); });
AT_EXIT({ see("after main"); });

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

  AT_INIT({ see("hello"); });
  AT_EXIT({ see("good bye"); });

  std::pair<int, int> a(1, 2), b(11, 12);
  cm_assert(a + b == std::make_pair(1 + 11, 2 + 12));
  cm_assert(a - b == std::make_pair(1 - 11, 2 - 12));

  cm::once_t once;
  cm_assert(once());
  cm_assert(!once());

  auto fac = y_combinate(
      [](auto f, int x) -> int { return x == 0 ? 1 : x * f(x - 1); });
  cm_assert(fac(5) == 5 * 4 * 3 * 2 * 1);
}
