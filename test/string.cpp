#include "../string"
#include "../debug"

int main()
{
  char a[] = "abcde";
  char b[] = "cdefg";

  cm::string::hash_u64<233> ha(a, a + 5);
  cm::string::hash_u64<233> hb(b, b + 5);

  see(ha.get_hash(0, 1));
  see(ha.get_hash(0, 2));
  see(cm::string::lcp(ha, 2, hb, 0));
  see(ha.cat(ha.get_hash_pair(0, 1), ha.get_hash_pair(3, 4)));
}
