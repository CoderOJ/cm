#include "../util"
#include "../debug"

AT_INIT({ see("before main"); });
AT_EXIT({ see("after main"); });

int main()
{

  AT_INIT({ see("hello"); });
  AT_INIT({ see("hello2"); });
  AT_EXIT({ see("good bye"); });
  AT_EXIT({ see("good bye 2"); });

  std::pair<int,int> a(1, 2), b(11, 12);
  see(a + b);
  see(a - b);

  cm::once_t once;
  if (once()) see(a += b);
  if (once()) see(a += b);
}
