#include "../intm"
#include "../string"
#include "../debug"

int main()
{
  char a[] = "abcde";
  char b[] = "cdefg";

  using hash_int_t = cm::intm<998244353>;
  cm::string::hash_u64<233> ha(a, a + 5);
  cm::string::hash_u64<233> hb(b, b + 5);
  cm::string::hash_mod<hash_int_t> ham(233, a, a + 5);
  cm::string::hash_mod<hash_int_t> hbm(233, b, b + 5);

  see(ha.get_hash(0, 1));
  see(ha.get_hash(0, 2));
  see(cm::string::lcp(ha, 2, hb, 0));
  see(cm::string::lcs(ha, 2, hb, 0));
  see(cm::string::lcs(ha, 4, hb, 2));
  see(cm::string::lcp(ham, 2, hbm, 0));
  see(cm::string::lcs(ham, 2, hbm, 0));
  see(cm::string::lcs(ham, 4, hbm, 2));
  see(ha.cat(ha.get_hash_pair(0, 1), ha.get_hash_pair(3, 4)));
}
