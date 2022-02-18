#include "../intm"
#include "../debug.hpp"
#include <cassert>

int main()
{
  cm::impl::cm_logger.set_exit_code(0);

  using Int = cm::intm<998244353>;
  Int a = 1, b = 2;
  std::cout << a - b << std::endl;
  std::cout << (a -= b) << std::endl;
  std::cout << a * b << std::endl;
  std::cout << a / b << std::endl;
  std::cout << Int::raw(0).inv() << std::endl;
}
