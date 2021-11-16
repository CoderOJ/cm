#include "../debug.hpp"
#include <bits/stdc++.h>

int main()
{
  cm::impl::see_logger.set_exit_code(0);

  int x = 1;
  see(x, x, x);
  see(std::initializer_list<int>{x, x, x});
  asee(std::vector<int>{1, 2, 3});
  asee(std::set<int>{4, 2, 3});
  asee(std::make_tuple(1, 2, 3));
  asee("test pair")(
      std::pair<std::vector<int>, std::set<int>>{{1, 2, 3}, {3, 2, 1}});

  cm::impl::see_logger.assert_noexit();
  cm_assert(x == x, "x is x");
  cm_assert(x != x, "x is not x", x);
  cm::impl::see_logger.assert_exit();
  cm_assert(x != x, "x is not x", x, x);
  cm_assert(x != x, "x is not x", x, x, x);
}
