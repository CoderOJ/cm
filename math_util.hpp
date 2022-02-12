#ifndef CM_MATH_BASE_H
#define CM_MATH_BASE_H

#include <vector>

namespace cm
{

template <class _Integer>
struct math_util
{
  static class _fac_t
  {
  private:
    std::vector<_Integer> _fac;

  public:
    void require(int n)
    {
      n++;
      int c = static_cast<int>(_fac.size());
      if (c < n)
      {
        _fac.resize(n);
        if (c == 0)
        {
          _fac[0] = 1;
          c       = 1;
        }
        for (int i = c; i < n; i++)
        {
          _fac[i] = _fac[i - 1] * _Integer(i);
        }
      }
    }
    _Integer operator()(int i) const
    {
      return _fac[i];
    }
    _Integer get(int i)
    {
      require(i);
      return _fac[i];
    }
  } fac;

  static class _inv_t
  {
  private:
    std::vector<_Integer> _inv;
    static constexpr int  MOD = _Integer::MOD;

  public:
    void require(int n)
    {
      n++;
      int c = static_cast<int>(_inv.size());
      if (c < n)
      {
        _inv.resize(n);
        if (c == 0)
        {
          _inv[0] = 1;
          c       = 1;
        }
        if (c == 1 && n > 1)
        {
          _inv[1] = 1;
          c       = 2;
        }
        for (int i = c; i < n; i++)
        {
          _inv[i] = _inv[MOD % i] * (MOD - MOD / i);
        }
      }
    }
    _Integer operator()(int i) const
    {
      return _inv[i];
    }
    _Integer get(int i)
    {
      require(i);
      return _inv[i];
    }
  } inv;

  static class _ifac_t
  {
  private:
    std::vector<_Integer> _ifac;

  public:
    void require(int n)
    {
      fac.require(n);
      int c = static_cast<int>(_ifac.size());
      if (n + 1 > c)
      {
        _ifac.resize(n + 1);
        _ifac[n] = fac(n).inv();
        for (int i = n - 1; i >= c; i--)
          _ifac[i] = _ifac[i + 1] * _Integer(i + 1);
      }
    }
    _Integer operator()(int i) const
    {
      return _ifac[i];
    }
    _Integer get(int i)
    {
      require(i);
      return _ifac[i];
    }
  } ifac;

  static class _binom_t
  {
  public:
    void require(int n)
    {
      fac.require(n);
      ifac.require(n);
    }
    _Integer operator()(int n, int m)
    {
      if (m < 0 || m > n)
        return _Integer(0);
      return fac(n) * ifac(m) * ifac(n - m);
    }
    _Integer get(int n, int m)
    {
      if (m < 0 || m > n)
        return _Integer(0);
      return fac.get(n) * ifac.get(m) * ifac.get(n - m);
    }
  } binom;
};

template <class _Integer>
typename math_util<_Integer>::_fac_t math_util<_Integer>::fac;
template <class _Integer>
typename math_util<_Integer>::_inv_t math_util<_Integer>::inv;
template <class _Integer>
typename math_util<_Integer>::_ifac_t math_util<_Integer>::ifac;
template <class _Integer>
typename math_util<_Integer>::_binom_t math_util<_Integer>::binom;

} // namespace cm

#endif
