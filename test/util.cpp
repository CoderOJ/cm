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
  AT_INIT({ see("hello2"); });
  AT_EXIT({ see("good bye"); });
  AT_EXIT({ see("good bye 2"); });

  std::pair<int, int> a(1, 2), b(11, 12);
  see(a + b);
  see(a - b);

  cm::once_t once;
  if (once())
    see(a += b);
  if (once())
    see(a += b);

  std::vector<int> va{1, 2, 3};
  asee(va);
  asee(va + 4);
  asee(va + std::vector<int>{5, 6});
  asee(va += std::vector<int>{7, 8});
  asee(va + va);
}
