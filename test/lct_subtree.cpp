// solution for ptzcamp 22 winter J2

#include "../lct_subtree"
#include "../debug"
#include "../intm"
#include "../scanner"
#include <cstddef>
#include <cstdint>
// #include "/home/jack/cm/string"

constexpr int MOD = 1'000'000'007;
using int_t       = cm::intm<MOD>;

struct info_t
{
  int   length     = 0;
  int   max_height = 0;
  int   max_vertex = 0;
  int_t endp_1     = 0;
  int_t endp_11    = 0;
  int_t chain_10   = 0;
  int_t edge_20    = 0;
  int_t edge_21    = 0;
  int_t edge_22    = 0;

  int_t endp_2() const
  {
    return this->endp_1 * this->endp_1;
  }
  int_t answer() const
  {
    return this->edge_22 + this->endp_11 * this->endp_11;
  }

  info_t();
  explicit info_t(int vertex);
  info_t(const info_t &, const info_t &);
  info_t compress() const;
  info_t append(const info_t &) const;
  info_t set_virtual(const info_t &) const;
};

info_t::info_t() = default;

info_t::info_t(int vertex)
{
  this->length     = 1;
  this->max_height = 0;
  this->max_vertex = vertex;
  this->endp_1     = 1;
  this->endp_11    = 1;
}

info_t::info_t(const info_t &lhs, const info_t &rhs)
{
  if (lhs.max_height > rhs.max_height)
    *this = lhs;
  else if (lhs.max_height < rhs.max_height)
    *this = rhs;
  else
  {
    // clang-format off
    // this is subtree info to be compressed, so chain is not important
    this->length      = 0;
    this->max_height  = lhs.max_height;
    this->max_vertex  = lhs.max_vertex;
    this->endp_1      = lhs.endp_1 + rhs.endp_1;
    this->endp_11     = lhs.endp_11 + rhs.endp_11
                      + lhs.endp_1 * rhs.endp_1;
    this->edge_20     = lhs.edge_20 + rhs.edge_20;
    this->edge_21     = lhs.edge_21 + rhs.edge_21
                      + lhs.edge_20 * rhs.endp_1
                      + rhs.edge_20 * lhs.endp_1;
    this->edge_22     = lhs.edge_22 + rhs.edge_22
                      + 2 * lhs.edge_21 * rhs.endp_1
                      + 2 * rhs.edge_21 * lhs.endp_1
                      + lhs.edge_20 * rhs.endp_2()
                      + rhs.edge_20 * lhs.endp_2();
    // clang-format on
  }
}

info_t info_t::compress() const
{
  info_t result;
  result.length     = 0;
  result.max_height = this->max_height + 1;
  result.max_vertex = this->max_vertex;
  result.endp_1     = this->endp_1;
  result.edge_20    = this->edge_20 + this->endp_2();
  return result;
}

info_t info_t::append(const info_t &son) const
{
  info_t result;
  if (this->max_height > this->length + son.max_height)
  {
    result.length     = this->length + son.length;
    result.max_height = this->max_height;
    result.max_vertex = this->max_vertex;
    result.endp_1     = this->endp_1;
    result.chain_10   = this->chain_10;
    result.edge_20    = this->edge_20;
  }
  else if (this->max_height < this->length + son.max_height)
  {
    result.length     = this->length + son.length;
    result.max_height = this->length + son.max_height;
    result.max_vertex = son.max_vertex;
    result.endp_1     = son.endp_1;
    result.chain_10   = son.chain_10 + this->length * son.endp_1;
    result.edge_20    = son.edge_20 + this->length * son.endp_2();
  }
  else
  {
    // clang-format off
    result.length     = this->length + son.length;
    result.max_height = this->max_height;
    result.max_vertex = this->max_vertex;
    result.endp_1     = this->endp_1 + son.endp_1;
    result.chain_10   = this->chain_10 + this->length * son.endp_1
                      + son.chain_10;
    result.edge_20    = this->edge_20 + 2 * this->chain_10 * son.endp_1
                      + son.edge_20 + this->length * son.endp_2();
    // clang-format on
  }
  return result;
}

info_t info_t::set_virtual(const info_t &virtuals) const
{
  info_t result = virtuals;
  result.length = 1;
  return result;
}

std::ostream &operator<<(std::ostream &out, const info_t info)
{
  // clang-format off
  return out 
              << "{ length: " << info.length
              << ", max_height: " << info.max_height
              << ", max_vertex: " << info.max_vertex
              << ", endp_1: " << info.endp_1
              << ", endp_11: " << info.endp_11
              << ", chain_10: " << info.chain_10
              << ", edge_20: " << info.edge_20
              << ", edge_21: " << info.edge_21
              << ", edge_22: " << info.edge_22 << " }";
  // clang-format on
}

cm::lct::lct_t<info_t> lct;

auto kth(int id)
{
  return [id](const info_t &info) { return info.length <= id; };
}

int main()
{
  cm::scanner<cm::optimal_reader> sc(fopen("lct_subtree.in", "r"));

  std::vector<int> answers;

  int n = sc.next_int();
  int m = sc.next_int();
  int q = sc.next_int();
  lct.resize(n + 1);
  for (int i = 1; i <= n; i++)
    lct[i] = info_t(i);

  for (int i = 0; i < m; i++)
  {
    int u = sc.next_int();
    int v = sc.next_int();
    lct.link(&lct[u], &lct[v]);
  }

  for (int i = 0; i < q; i++)
  {
    int opt = sc.next_int();
    if (opt == 1)
    {
      int u = sc.next_int();
      int v = sc.next_int();
      lct.link(&lct[u], &lct[v]);
    }
    else if (opt == 2)
    {
      int u = sc.next_int();
      int v = sc.next_int();
      lct.cut(&lct[u], &lct[v]);
    }
    else
    {
      int u = sc.next_int();
      lct[u].make_root();
      u = lct[u].chain.max_vertex;
      lct[u].make_root();
      int v   = lct[u].chain.max_vertex;
      int len = lct[u].chain.max_height;

      if (len % 2 == 0)
      {
        auto c = lct.lower_bound(&lct[u], &lct[v], kth(len / 2));
        c->make_root();
        c->access();
        c->splay();
        answers.emplace_back(c->subtree().answer());
      }
      else
      {
        auto c0 = lct.lower_bound(&lct[u], &lct[v], kth(len / 2));
        auto c1 = lct.lower_bound(&lct[u], &lct[v], kth(len / 2 + 1));
        lct.cut(c0, c1);
        c0->make_root();
        c0->access();
        c0->splay();
        info_t ans0 = c0->subtree();
        c1->make_root();
        c1->access();
        c1->splay();
        info_t ans1 = c1->subtree();
        info_t ans  = info_t(ans0.compress(), ans1.compress());
        answers.emplace_back(ans.edge_22);
        lct.link(c0, c1);
      }
    }
  }

  cm_assert(answers[0] == 18);
  cm_assert(answers[1] == 64);
  cm_assert(answers[2] == 21);

  return 0;
}
