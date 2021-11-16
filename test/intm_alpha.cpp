#include "../intm_alpha"

constexpr int mod = 998244353;
constexpr int N   = 1000;
using int_t       = cm::intm<mod>;
int_t _stir[2][N], *stir[N];

int main()
{
  for (int i = 0; i < N; i++)
    stir[i] = _stir[i & 1];

  stir[0][0] = 1;
  for (int i = 1; i < N; i++)
  {
    stir[i][0] = 0;
    for (int j = 1; j <= i; j++)
      stir[i][j] = stir[i - 1][j - 1] + stir[i - 1][j] * cm::intm_r<mod, N>(j);
  }

  std::cout << stir[N - 1][N / 2] << std::endl;
  return 0;
}
