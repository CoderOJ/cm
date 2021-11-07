#include "../scanner"
#include "../debug"

cm::scanner<cm::optimal_reader> sc(fopen("./scanner.in", "r"));

int main()
{
  see(sc.next_char());

  char dist[64];
  sc.next_token(dist);
  see(dist);
  sc.next_line(dist);
  see(dist);

  see(sc.next_int());
  see(sc.next_long());
  see(sc.next_double());
  return 0;
}
