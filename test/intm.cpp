#include "../debug.hpp"
#include "../intm.hpp"
#include <cassert>

int main()
{
  using Int = cm::intm<998244353>;
  Int a = 1, b = 2;
  std::cout << a - b << std::endl;
  std::cout << (a -= b) << std::endl;
  std::cout << a * b << std::endl;
  std::cout << a / b << std::endl;
  std::cout << Int::raw(0).inv() << std::endl;
}
