#include "../intm_pair"
#include "../debug"
#include "../intm"

int main()
{
  using int_ta = cm::intm<11>;
  using int_tb = cm::intm<103>;
  using int_t = cm::intm_pair<int_ta, int_tb>;

  see(int_t(1));
  see(int_t(27));
  see(int_t(13) + int_t(27));
  see(int_t(13) - int_t(27));
  see(int_t(13) * int_t(27));
  see(int_t(13) / int_t(27));
}
