#include "../barret.hpp"
#include "../debug.hpp"

int main()
{
  const unsigned int mod   = 19375;
  const unsigned int round = 200;

  cm::barret_reduction br(mod);

  for (unsigned int i = 0; i < mod * round; i++)
    cm_assert(br.mod(i) == i % mod);
}
