#ifndef CM_STRING_H
#define CM_STRING_H 1

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
  hash_u64(InputIterator begin, InputIterator end) :
    size(static_cast<size_t>(std::distance(begin, end))),
    base     (new uint64_t[size + 1]),
    base_inv (new uint64_t[size + 1]),
    sum      (new uint64_t[size + 1])
  {
    base[0] = 1;
    base_inv[0] = 1;
    sum[0] = 0;

    uint64_t seed_inv = __impl_calc_inv(seed);
    for (size_t i = 0; i < size; i++)
    {
      base[i + 1] = base[i] * seed;
      base_inv[i + 1] = base_inv[i] * seed_inv;
      sum[i + 1] = sum[i] + base[i] * static_cast<uint64_t>(*begin++);
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

template <class Hasher>
size_t lcp(const Hasher &a, size_t al, const Hasher &b, size_t bl)
{
  size_t l = 0;
  size_t r = std::min(a.size - al, b.size - bl);
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

}

}

#endif
