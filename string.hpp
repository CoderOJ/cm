#ifndef CM_STRING_H
#define CM_STRING_H

#include <iterator>
#include <utility>

namespace cm
{

namespace string
{

template <uint64_t seed>
class hash_u64
{

#if __cplusplus >= 201103L
  static_assert(seed % 2 == 1, "hash_u64 seed show be odd");
#endif

public:
  const size_t size;

protected:
  uint64_t *const base;
  uint64_t *const base_inv;
  uint64_t *const sum;

  static uint64_t __impl_calc_inv(uint64_t a)
  {
    // equals to a ^ (2 ^ 63 - 1) % (2 ^ 64)
    uint64_t result = 1;
    for (int i = 0; i < 63; i++)
    {
      result *= a;
      a *= a;
    }
    return result;
  }

public:
  template <class InputIterator>
  hash_u64(InputIterator begin, InputIterator end)
      : size(static_cast<size_t>(std::distance(begin, end))),
        base(new uint64_t[size + 1]), base_inv(new uint64_t[size + 1]),
        sum(new uint64_t[size + 1])
  {
    base[0]     = 1;
    base_inv[0] = 1;
    sum[0]      = 0;

    uint64_t seed_inv = __impl_calc_inv(seed);
    for (size_t i = 0; i < size; i++)
    {
      base[i + 1]     = base[i] * seed;
      base_inv[i + 1] = base_inv[i] * seed_inv;
      sum[i + 1]      = sum[i] + base[i] * static_cast<uint64_t>(*begin++);
    }
  }

  uint64_t get_hash(size_t l, size_t r) const
  {
    return base_inv[l] * (sum[r] - sum[l]);
  }
  std::pair<size_t, uint64_t> get_hash_pair(size_t l, size_t r) const
  {
    return std::make_pair(r - l, get_hash(l, r));
  }
  std::pair<size_t, uint64_t> cat(const std::pair<size_t, uint64_t> &hpa,
                                  const std::pair<size_t, uint64_t> &hpb)
  {
    return std::make_pair(hpa.first + hpb.first,
                          hpa.second + base[hpa.first] * hpb.second);
  }

  ~hash_u64()
  {
    delete[] base;
    delete[] base_inv;
    delete[] sum;
  }
};

template <typename IntType>
class hash_mod
{
public:
  const size_t size;

protected:
  IntType *const base;
  IntType *const base_inv;
  IntType *const sum;

public:
  template <class InputIterator>
  hash_mod(IntType seed, InputIterator begin, InputIterator end)
      : size(static_cast<size_t>(std::distance(begin, end))),
        base(new IntType[size + 1]), base_inv(new IntType[size + 1]),
        sum(new IntType[size + 1])
  {
    base[0]     = 1;
    base_inv[0] = 1;
    sum[0]      = 0;

    IntType seed_inv = 1 / seed;
    for (size_t i = 0; i < size; i++)
    {
      base[i + 1]     = base[i] * seed;
      base_inv[i + 1] = base_inv[i] * seed_inv;
      sum[i + 1]      = sum[i] + base[i] * static_cast<IntType>(*begin++);
    }
  }

  IntType get_hash(size_t l, size_t r) const
  {
    return base_inv[l] * (sum[r] - sum[l]);
  }
  std::pair<size_t, IntType> get_hash_pair(size_t l, size_t r) const
  {
    return std::make_pair(r - l, get_hash(l, r));
  }
  std::pair<size_t, IntType> cat(const std::pair<size_t, IntType> &hpa,
                                 const std::pair<size_t, IntType> &hpb)
  {
    return std::make_pair(hpa.first + hpb.first,
                          hpa.second + base[hpa.first] * hpb.second);
  }

  ~hash_mod()
  {
    delete[] base;
    delete[] base_inv;
    delete[] sum;
  }
};

template <class Hasher>
size_t lcp(const Hasher &a, size_t al, const Hasher &b, size_t bl)
{
  size_t l = 0;
  size_t r = std::min(a.size - al, b.size - bl);

  for (size_t i = 1; i < 4 && i <= r; i++)
    if (a.get_hash(al, al + i) != b.get_hash(bl, bl + i))
      return i - 1;

  while (l < r)
  {
    size_t mid = (l + r + 1) / 2;
    if (a.get_hash(al, al + mid) == b.get_hash(bl, bl + mid))
      l = mid;
    else
      r = mid - 1;
  }
  return l;
}

template <class Hasher>
size_t lcs(const Hasher &a, size_t al, const Hasher &b, size_t bl)
{
  size_t l = 0;
  size_t r = std::min(al, bl);

  for (size_t i = 1; i < 4 && i <= r; i++)
    if (a.get_hash(al - i, al) != b.get_hash(bl - i, bl))
      return i - 1;

  while (l < r)
  {
    size_t mid = (l + r + 1) / 2;
    if (a.get_hash(al - mid, al) == b.get_hash(bl - mid, bl))
      l = mid;
    else
      r = mid - 1;
  }
  return l;
}

} // namespace string

} // namespace cm

#endif
