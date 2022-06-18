#ifndef CM_PCH_H
#define CM_PCH_H

#include <algorithm>
#include <bitset>
#include <cinttypes>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace impl
{

inline void force_templates()
{
#define FORCE(type)                                                            \
  {                                                                            \
    [[maybe_unused]] auto f = [] type {};                                      \
  }

  FORCE((std::pair<int, int>))
  FORCE((std::vector<int>))
  FORCE((std::vector<std::pair<int, int>>))

#undef FORCE
}

} // namespace impl

#endif
