#include "../lambda.hpp"
#include "../debug.hpp"
#include <algorithm>
#include <functional>
#include <iostream>

using namespace cm::lambda::placeholders;

int main()
{
  std::vector<int> a{3, 1, 4, 2};
  std::sort(a.begin(), a.end(), _0 > _1);
  cm_assert(a == std::vector<int>({4, 3, 2, 1}));
  cm_assert(((!!_1)(0, 1)) == true);
  cm_assert(((-_0 * _1)(2, 3)) == -6);
  return 0;
}
