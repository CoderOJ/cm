// relabel off

#ifndef CM_COMPATIBLE_H
#define CM_COMPATIBLE_H

#if __cplusplus < 201402L

#include <utility>

namespace std
{

template <typename _Tp, typename _Up = _Tp>
inline _Tp exchange(_Tp &__obj, _Up &&__new_val)
{
  return __exchange(__obj, std::forward<_Up>(__new_val));
}

} // namespace std

#endif

#endif
