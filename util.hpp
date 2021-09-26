#ifndef CM_UTIL_H
#define CM_UTIL_H 1

#include <utility>
#include <vector>

namespace cm
{

template <class T>
inline bool check_min(T &a, const T &b)
{
  return b < a ? a = b, 1 : 0;
}
template <class T>
inline bool check_max(T &a, const T &b)
{
  return a < b ? a = b, 1 : 0;
}

struct once_t
{
  bool once = true;
  bool operator()()
  {
    return once ? (once=false, true) : false;
  }
};

}

using cm::check_min;
using cm::check_max;

template <class A, class B> inline __attribute__((always_inline)) 
std::pair<A,B> operator+ (const std::pair<A,B> &lhs, const std::pair<A,B> &rhs) 
{
  return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second); 
}
template <class A, class B> inline __attribute__((always_inline)) 
std::pair<A,B> operator- (const std::pair<A,B> &lhs, const std::pair<A,B> &rhs) 
{
  return std::make_pair(lhs.first - rhs.first, lhs.second - rhs.second); 
}
template <class A, class B> inline __attribute__((always_inline)) 
std::pair<A,B>& operator+= (std::pair<A,B> &lhs, const std::pair<A,B> &rhs) 
{
  lhs.first += rhs.first; lhs.second += rhs.second; return lhs; 
}
template <class A, class B> inline __attribute__((always_inline)) 
std::pair<A,B>& operator-= (std::pair<A,B> &lhs, const std::pair<A,B> &rhs) 
{
  lhs.first -= rhs.first; lhs.second -= rhs.second; return lhs; 
}

template <class T>
std::vector<T>& operator+= (std::vector<T> &a, const std::vector<T> &b)
{
  a.insert(a.end(), b.begin(), b.end());
  return a;
}
template <class T>
std::vector<T>& operator+= (std::vector<T> &a, const T &b)
{
  a.insert(a.end(), b);
  return a;
}
template <class T>
std::vector<T>& operator+= (std::vector<T> &a, T &&b)
{
  a.insert(a.end(), std::forward<T>(b));
  return a;
}

template <class T>
std::vector<T> operator+ (std::vector<T> a, const std::vector<T> &b)
{
  a += b;
  return a;
}
template <class T>
std::vector<T> operator+ (std::vector<T> a, T &b)
{
  a += b;
  return a;
}
template <class T>
std::vector<T> operator+ (std::vector<T> a, T &&b)
{
  a += std::forward<T>(b);
  return a;
}

#define __AT_INIT(line, Pred) struct CM_INNER_ATINIT##line##_t { \
  CM_INNER_ATINIT##line## _t () Pred } CM_INNER_ATINIT##line
#define _AT_INIT(line, Pred) __AT_INIT(line, Pred)
#define AT_INIT(Pred) _AT_INIT(__LINE__, Pred)

#define __AT_EXIT(line, Pred) struct CM_INNER_ATEXIT##line##_t { \
  ~CM_INNER_ATEXIT##line## _t () Pred } CM_INNER_ATEXIT##line
#define _AT_EXIT(line, Pred) __AT_EXIT(line, Pred)
#define AT_EXIT(Pred) _AT_EXIT(__LINE__, Pred)

#endif
