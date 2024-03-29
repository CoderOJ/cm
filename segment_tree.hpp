#ifndef CM_SEGMENT_TREE
#define CM_SEGMENT_TREE

#include <iterator>
#include <vector>

namespace cm
{

template <bool _have_push_down = false>
struct segment_tree_node_base
{
  static constexpr bool have_push_down = false;
  segment_tree_node_base()             = default;
  segment_tree_node_base(const segment_tree_node_base &,
                         const segment_tree_node_base &)
  {
  }
  void push_down(segment_tree_node_base &, segment_tree_node_base &) {}
};

template <>
struct segment_tree_node_base<true>
{
  static constexpr bool have_push_down = true;
  segment_tree_node_base()             = default;
  segment_tree_node_base(const segment_tree_node_base &,
                         const segment_tree_node_base &)
  {
  }
};

template <class _NodeType>
struct segment_tree
{
protected:
  int                    l = 0, r = 0;
  std::vector<_NodeType> p;

  void _init(int u, int l, int r)
  {
    if (r - l == 1)
    {
      p[u] = _NodeType();
    }
    else
    {
      int _mid = l + (r - l) / 2;
      _init(u + 1, l, _mid);
      _init(u + (_mid - l) * 2, _mid, r);
      p[u] = _NodeType(p[u + 1], p[u + (_mid - l) * 2]);
    }
  }

  template <class _Iter>
  void _init(int u, _Iter l, _Iter r)
  {
    if (std::next(l) == r)
    {
      p[u] = _NodeType(*l);
    }
    else
    {
      int _mid = l + std::distance(l, r) / 2;
      _init(u + 1, l, _mid);
      _init(u + (_mid - l) * 2, _mid, r);
      p[u] = _NodeType(p[u + 1], p[u + (_mid - l) * 2]);
    }
  }

  template <class _Pres, class... _Param, class... _Arg>
  void _modify(int u, int l, int r, int ml, int mr,
               _Pres (_NodeType::*_pred)(_Param...), _Arg... _arg)
  {
    if (ml <= l && r <= mr)
    {
      (p[u].*_pred)(_arg...);
    }
    else
    {
      int _mid = l + (r - l) / 2;
      if (_NodeType::have_push_down)
        p[u].push_down(p[u + 1], p[u + (_mid - l) * 2]);
      if (ml < _mid)
        _modify(u + 1, l, _mid, ml, mr, _pred, _arg...);
      if (mr > _mid)
        _modify(u + (_mid - l) * 2, _mid, r, ml, mr, _pred, _arg...);
      p[u] = _NodeType(p[u + 1], p[u + (_mid - l) * 2]);
    }
  }

  _NodeType _query(int u, int l, int r, int ql, int qr)
  {
    if (ql <= l && r <= qr)
    {
      return p[u];
    }
    else
    {
      int _mid = l + (r - l) / 2;
      if (_NodeType::have_push_down)
        p[u].push_down(p[u + 1], p[u + (_mid - l) * 2]);
      if (qr <= _mid)
        return _query(u + 1, l, _mid, ql, qr);
      else if (ql >= _mid)
        return _query(u + (_mid - l) * 2, _mid, r, ql, qr);
      else
        return _NodeType(_query(u + 1, l, _mid, ql, qr),
                         _query(u + (_mid - l) * 2, _mid, r, ql, qr));
    }
  }

  template <class _Pred>
  int _lower_bound(int u, int l, int r, _Pred _pred, _NodeType _prev)
  {
    if (r - l == 1)
    {
      return l;
    }
    else
    {
      int _mid = l + (r - l) / 2;
      if (_NodeType::have_push_down)
        p[u].push_down(p[u + 1], p[u + (_mid - l) * 2]);
      _NodeType _attempt = _NodeType(_prev, p[u + 1]);
      if (_pred(_attempt))
        return _lower_bound(u + (_mid - l) * 2, _mid, r, _pred, _attempt);
      else
        return _lower_bound(u + 1, l, _mid, _pred, _prev);
    }
  }

public:
  segment_tree() = default;
  explicit segment_tree(int r) : l(0), r(r), p((r - l) * 2 - 1)
  {
    _init(0, l, r);
  }
  segment_tree(int l, int r) : l(l), r(r), p((r - l) * 2 - 1)
  {
    _init(0, l, r);
  }

  template <class _Iter>
  segment_tree(_Iter _begin, _Iter _end)
      : l(0), r(std::distance(_begin, _end)), p(r * 2 - 1)
  {
    _init(0, _begin, _end);
  }

  template <class _Pres, class... _Param, class... _Arg>
  void modify(int ml, int mr, _Pres (_NodeType::*_pred)(_Param...),
              _Arg... _arg)
  {
#ifdef CM_DEBUG_H
    cm_assert(ml >= l, l, r, ml, mr);
    cm_assert(mr <= r, l, r, ml, mr);
    cm_assert(ml < mr, l, r, ml, mr);
#endif
    if (ml < mr)
      _modify(0, l, r, ml, mr, _pred, _arg...);
  }

  _NodeType query(int ql, int qr)
  {
#ifdef CM_DEBUG_H
    cm_assert(ql >= l, l, r, ql, qr);
    cm_assert(qr <= r, l, r, ql, qr);
    cm_assert(ql <= qr, l, r, ql, qr);
#endif
    if (ql >= qr)
      return _NodeType();
    else
      return _query(0, l, r, ql, qr);
  }

  template <class _Pred>
  int lower_bound(_Pred _pred, _NodeType _prev = _NodeType())
  {
    if (_pred(p[0]))
      return r;
    else
      return _lower_bound(0, l, r, _pred, _prev);
  }
};

} // namespace cm

#endif
