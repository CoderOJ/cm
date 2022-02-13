#ifndef CM_LCT_SUBTREE_H
#define CM_LCT_SUBTREE_H

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

namespace cm::lct
{

template <class value_type>
struct splay_t
{
  struct node_t;
  using iterator = node_t *;

  struct node_t
  {
    node_t() = default;
    node_t(const value_type &);
    node_t &operator=(const value_type &);

    value_type val;
    value_type sum;

    node_t *f    = nullptr;
    node_t *s[2] = {nullptr, nullptr};

    bool     is_root();
    int      get_son();
    void     push_up();
    void     rotate();
    void     splay();
    iterator find_r();
    iterator detach();
  };

  iterator root = nullptr;

  bool       empty() const;
  void       insert(iterator);
  void       erase(iterator);
  value_type get_sum();
};

template <class info_t>
struct lct_t
{
  struct node_t;
  using iterator = node_t *;

  struct node_t
  {
    node_t();
    node_t(const info_t &);
    node_t &operator=(const info_t &);

    node_t *f    = nullptr;
    node_t *s[2] = {nullptr, nullptr};
    bool    rev  = false;

    info_t self;
    info_t chain;
    info_t chain_rev;

    splay_t<info_t> virtuals;

    typename splay_t<info_t>::node_t fa_node;

    info_t subtree();
    void   insert_virtual(node_t *);
    void   erase_virtual(node_t *);
    void   push_up();
    void   push_rev();
    void   push_down();
    void   push_all();

    bool    is_root();
    int     get_son();
    void    rotate();
    void    splay();
    node_t *splay_top();
    void    access();
    void    make_root();

    template <class Pred>
    iterator lower_bound(Pred);
  };

  std::vector<node_t> p;

  lct_t() = default;
  lct_t(size_t n) : p(n) {}

  typename std::vector<node_t>::iterator begin();
  typename std::vector<node_t>::iterator end();
  int                                    id(iterator);

  node_t &operator[](int);
  void    resize(size_t);
  void    split(iterator, iterator);
  void    link(iterator, iterator);
  void    cut(iterator, iterator);
  template <class Pred>
  iterator lower_bound(iterator, iterator, Pred);
};

template <class value_type>
splay_t<value_type>::node_t::node_t(const value_type &val) : val(val), sum(val)
{
}

template <class value_type>
typename splay_t<value_type>::node_t &splay_t<value_type>::node_t::operator=(
    const value_type &val)
{
  this->splay();
  this->val = val;
  this->push_up();
  return *this;
}

template <class value_type>
bool splay_t<value_type>::node_t::is_root()
{
  return this->f == nullptr;
}

template <class value_type>
int splay_t<value_type>::node_t::get_son()
{
  return this->f->s[1] == this ? 1 : 0;
}

template <class value_type>
void splay_t<value_type>::node_t::push_up()
{
  this->sum = this->val;
  if (this->s[0] != nullptr)
  {
    this->sum = value_type(this->s[0]->sum, this->sum);
  }
  if (this->s[1] != nullptr)
  {
    this->sum = value_type(this->sum, this->s[1]->sum);
  }
}

template <class value_type>
void splay_t<value_type>::node_t::rotate()
{
  node_t *const uu = this;
  node_t *const ff = uu->f;
  node_t *const aa = ff->f;
  const int     ss = this->get_son();

  if (!ff->is_root())
    aa->s[ff->get_son()] = uu;

  ff->f     = uu;
  ff->s[ss] = uu->s[1 - ss];

  uu->f         = aa;
  uu->s[1 - ss] = ff;

  if (ff->s[ss] != nullptr)
    ff->s[ss]->f = ff;

  ff->push_up();
  uu->push_up();
}

template <class value_type>
void splay_t<value_type>::node_t::splay()
{
  while (!this->is_root())
  {
    if (this->f->is_root())
    {
      this->rotate();
      break;
    }
    else
    {
      if (this->get_son() == this->f->get_son())
        this->f->rotate();
      else
        this->rotate();
      this->rotate();
    }
  }
}

template <class value_type>
typename splay_t<value_type>::iterator splay_t<value_type>::node_t::find_r()
{
  iterator u = this;
  while (u->s[1] != nullptr)
    u = u->s[1];
  u->splay();
  return u;
}

template <class value_type>
typename splay_t<value_type>::iterator splay_t<value_type>::node_t::detach()
{
  this->splay();
  if (this->s[0] != nullptr && this->s[1] != nullptr)
  {
    iterator u = std::exchange(this->s[0], nullptr);
    iterator v = std::exchange(this->s[1], nullptr);

    u->f    = nullptr;
    v->f    = nullptr;
    u       = u->find_r();
    u->s[1] = v;
    v->f    = u;
    u->push_up();
    return u;
  }
  else if (this->s[0] != nullptr)
  {
    iterator u = std::exchange(this->s[0], nullptr);
    u->f       = nullptr;
    return u;
  }
  else if (this->s[1] != nullptr)
  {
    iterator u = std::exchange(this->s[1], nullptr);
    u->f       = nullptr;
    return u;
  }
  else
  {
    return nullptr;
  }
}

template <class value_type>
bool splay_t<value_type>::empty() const
{
  return this->root == nullptr;
}

template <class value_type>
void splay_t<value_type>::insert(iterator it)
{
  if (this->root == nullptr)
  {
    this->root = it;
  }
  else
  {
    iterator u = std::exchange(this->root, it);
    u->f       = it;
    it->s[0]   = u;
    it->push_up();
  }
}

template <class value_type>
void splay_t<value_type>::erase(iterator it)
{
  this->root = it->detach();
}

template <class value_type>
value_type splay_t<value_type>::get_sum()
{
  return this->root == nullptr ? value_type() : this->root->sum;
}

template <class info_t>
lct_t<info_t>::node_t::node_t() = default;

template <class info_t>
lct_t<info_t>::node_t::node_t(const info_t &v)
    : chain(v), chain_rev(v), fa_node(v)
{
}

template <class info_t>
typename lct_t<info_t>::node_t &lct_t<info_t>::node_t::node_t::operator=(
    const info_t &val)
{
  this->make_root();
  this->access();
  this->splay();
  this->self = val;
  this->push_up();
  return *this;
}

template <class info_t>
info_t lct_t<info_t>::node_t::subtree()
{
  if (this->virtuals.empty())
    return this->self;
  else
    return this->self.set_virtual(this->virtuals.get_sum());
}

template <class info_t>
void lct_t<info_t>::node_t::insert_virtual(node_t *v)
{
  v->fa_node = v->chain.compress();
  this->virtuals.insert(&v->fa_node);
}

template <class info_t>
void lct_t<info_t>::node_t::erase_virtual(node_t *v)
{
  this->virtuals.erase(&v->fa_node);
}

template <class info_t>
void lct_t<info_t>::node_t::push_up()
{
  this->chain     = this->subtree();
  this->chain_rev = this->subtree();
  if (this->s[0] != nullptr)
  {
    this->chain     = this->s[0]->chain.append(this->chain);
    this->chain_rev = this->chain_rev.append(this->s[0]->chain_rev);
  }
  if (this->s[1] != nullptr)
  {
    this->chain     = this->chain.append(this->s[1]->chain);
    this->chain_rev = this->s[1]->chain_rev.append(this->chain_rev);
  }
}

template <class info_t>
void lct_t<info_t>::node_t::push_rev()
{
  this->rev = !this->rev;
  std::swap(this->s[0], this->s[1]);
  std::swap(this->chain, this->chain_rev);
}

template <class info_t>
void lct_t<info_t>::node_t::push_down()
{
  if (this->rev)
  {
    this->rev = false;
    if (this->s[0] != nullptr)
      this->s[0]->push_rev();
    if (this->s[1] != nullptr)
      this->s[1]->push_rev();
  }
}

template <class info_t>
void lct_t<info_t>::node_t::push_all()
{
  if (!this->is_root())
    this->f->push_all();
  this->push_down();
}

template <class info_t>
bool lct_t<info_t>::node_t::is_root()
{
  return this->f == nullptr || (this->f->s[0] != this && this->f->s[1] != this);
}

template <class info_t>
int lct_t<info_t>::node_t::get_son()
{
  return this->f->s[1] == this ? 1 : 0;
}

template <class info_t>
void lct_t<info_t>::node_t::rotate()
{
  node_t *const uu = this;
  node_t *const ff = uu->f;
  node_t *const aa = ff->f;
  const int     ss = this->get_son();

  if (!ff->is_root())
    aa->s[ff->get_son()] = uu;

  ff->f     = uu;
  ff->s[ss] = uu->s[1 - ss];

  uu->f         = aa;
  uu->s[1 - ss] = ff;

  if (ff->s[ss] != nullptr)
    ff->s[ss]->f = ff;

  ff->push_up();
  uu->push_up();
}

template <class info_t>
void lct_t<info_t>::node_t::splay()
{
  this->push_all();
  while (!this->is_root())
  {
    if (this->f->is_root())
    {
      this->rotate();
      break;
    }
    else
    {
      if (this->get_son() == this->f->get_son())
        this->f->rotate();
      else
        this->rotate();
      this->rotate();
    }
  }
}

template <class info_t>
typename lct_t<info_t>::iterator lct_t<info_t>::node_t::splay_top()
{
  node_t *u = this;
  while (u->s[0] != nullptr)
  {
    u->push_down();
    u = u->s[0];
  }
  u->splay();
  return u;
}

template <class info_t>
void lct_t<info_t>::node_t::access()
{
  node_t *uu = this;
  node_t *ss = nullptr;
  while (uu != nullptr)
  {
    uu->splay();
    if (uu->s[1] != nullptr)
    {
      node_t *vv = std::exchange(uu->s[1], nullptr)->splay_top();
      uu->insert_virtual(vv);
    }
    if (ss != nullptr)
    {
      ss = ss->splay_top();
      uu->erase_virtual(ss);
      uu->s[1] = ss;
    }
    uu->push_up();
    ss = uu;
    uu = uu->f;
  }
}

template <class info_t>
void lct_t<info_t>::node_t::make_root()
{
  this->access();
  this->splay();
  this->push_rev();
}

template <class info_t>
template <class Pred>
typename lct_t<info_t>::node_t *lct_t<info_t>::node_t::lower_bound(Pred pred)
{
  node_t *u = this;
  node_t *res;
  info_t  prev;
  while (true)
  {
    u->push_down();
    if (u->s[1] != nullptr)
    {
      info_t prev_next = u->s[1]->chain.append(prev);
      if (!pred(prev_next))
      {
        u = u->s[1];
        continue;
      }
      else
      {
        prev = prev_next;
      }
    }
    {
      info_t prev_next = u->subtree().append(prev);
      if (!pred(prev_next))
      {
        res = u;
        break;
      }
      else
      {
        prev = prev_next;
      }
    }
    if (u->s[0] != nullptr)
    {
      u = u->s[0];
      continue;
    }
    else
    {
      res = nullptr;
      break;
    }
  }
  if (res == nullptr)
    return nullptr;
  else
  {
    res->splay();
    return res;
  }
}

template <class info_t>
typename std::vector<typename lct_t<info_t>::node_t>::iterator lct_t<
    info_t>::begin()
{
  return this->p.begin();
}

template <class info_t>
typename std::vector<typename lct_t<info_t>::node_t>::iterator lct_t<
    info_t>::end()
{
  return this->p.end();
}
template <class info_t>
int lct_t<info_t>::id(iterator it)
{
  return static_cast<int>(it - &p[0]);
}

template <class info_t>
typename lct_t<info_t>::node_t &lct_t<info_t>::operator[](int id)
{
  return this->p[id];
}

template <class info_t>
void lct_t<info_t>::resize(size_t n)
{
  this->p.resize(n);
}

template <class info_t>
void lct_t<info_t>::split(iterator u, iterator v)
{
  u->make_root();
  v->access();
  v->splay();
}

template <class info_t>
void lct_t<info_t>::link(iterator u, iterator v)
{
  u->make_root();
  v->make_root();
  u->f = v;
  v->insert_virtual(u);
  v->push_up();
}

template <class info_t>
void lct_t<info_t>::cut(iterator u, iterator v)
{
  this->split(u, v);
  v->s[0] = nullptr;
  u->f    = nullptr;
  v->push_up();
}

template <class info_t>
template <class Pred>
typename lct_t<info_t>::iterator lct_t<info_t>::lower_bound(iterator u,
                                                            iterator v,
                                                            Pred     pred)
{
  this->split(u, v);
  return v->lower_bound(pred);
}

} // namespace cm::lct

#endif
