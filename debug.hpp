#ifndef CM_DEBUG_H
#define CM_DEBUG_H

#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <utility>

namespace cm
{

#ifdef CM_DEBUG

class logger
{
private:
  std::ostream *_out;
  std::string   _sep          = "\x2c\x20";
  int           _assert_level = 0;
  bool          _assert_exit  = true;
  int           _exit_code    = 0;

  std::string _get_color(const std::string &_name)
  {
    if (_name == "red")
      return "\033[0;31m";
    if (_name == "green")
      return "\033[0;32m";
    if (_name == "yellow")
      return "\033[0;33m";
    if (_name == "blue")
      return "\033[0;34m";
    if (_name == "magenta")
      return "\033[0;35m";
    if (_name == "cyan")
      return "\033[0;36m";
    if (_name == "red bold")
      return "\033[1;31m";
    if (_name == "green bold")
      return "\033[1;32m";
    if (_name == "yellow bold")
      return "\033[1;33m";
    if (_name == "blue bold")
      return "\033[1;34m";
    if (_name == "magenta bold")
      return "\033[1;35m";
    if (_name == "cyan bold")
      return "\033[1;36m";
    if (_name == "reset")
      return "\033[0m";
    return "";
  }

public:
  explicit logger(std::ostream &_out) : _out(&_out) {}

  logger &set_ostream(std::ostream &_out)
  {
    this->_out = &_out;
    return *this;
  }
  logger &set_sep(const std::string &_sep)
  {
    this->_sep = _sep;
    return *this;
  }
  logger &assert_push_enable()
  {
    _assert_level -= 1;
    if (_assert_level)
      _assert_level = 0;
    return *this;
  }
  logger &assert_push_disable()
  {
    _assert_level += 1;
    return *this;
  }
  logger &assert_exit()
  {
    _assert_exit = true;
    return *this;
  }
  logger &assert_noexit()
  {
    _assert_exit = false;
    return *this;
  }
  logger &set_exit_code(int _code)
  {
    _exit_code = _code;
    return *this;
  }

  logger &endl()
  {
    *_out << std::endl;
    return *this;
  }

  logger &log()
  {
    return *this;
  }
  template <class Ta, class Tb, class... Tc>
  logger &log(const Ta &a, const Tb &b, Tc... c)
  {
    return log(a).log(_sep).log(b, c...);
  }
  template <class Ta, typename = decltype(std::cout << std::declval<Ta>())>
  logger &log(const Ta &a)
  {
    *_out << a;
    return *this;
  }

  // output a container, except std::string
  template <class T, typename = decltype(std::declval<T>().begin()),
            typename            = decltype(std::declval<T>().end()),
            typename            = typename std::enable_if<!std::is_base_of<
                std::string, typename std::decay<T>::type>::value>::type,
            typename value_type = typename T::value_type>
  logger &log(const T &_container)
  {
    log("{");
    for (auto it = _container.begin(); it != _container.end(); ++it)
    {
      if (it != _container.begin())
        log(_sep);
      log(*it);
    }
    log("}");
    return *this;
  }
  // output a std::pair
  template <class Ta, class Tb>
  logger &log(const std::pair<Ta, Tb> &x)
  {
    return log("(").log(x.first, x.second).log(")");
  }
  // output std::tuple
#if __cplusplus >= 201703L
  template <class... T>
  logger &log(const std::tuple<T...> &x)
  {
    log("(");
    std::apply([this](auto... x) { this->log(x...); }, x);
    return log(")");
  }
#else
  template <class... T>
  logger &log(const std::tuple<T...> &)
  {
    return log("<tuple>");
  }
#endif

  template <class T>
  logger &hint(const T &x, std::string _color = "cyan")
  {
    _color = _get_color(_color);
    return log(_color).log(x).log("\x3a\x20").log(_get_color("reset"));
  }

  template <class... T>
  logger &operator()(T... _value)
  {
    return log(_value...).endl();
  }

  template <class... T>
  logger &assert_(const std::string &_file, int _line, const std::string &_raw,
                  bool _value, const std::string &_info_str, T... _info)
  {
    if (_assert_level == 0 && !_value)
    {
      endl()
          .hint(_file, "magenta")
          .hint(_line, "magenta")
          .log(_get_color("yellow"))
          .log("Assertion `")
          .log(_get_color("yellow bold"))
          .log(_raw)
          .log(_get_color("yellow"))
          .log("` failed")
          .log(_get_color("reset"))
          .endl();
      if (_info_str != "")
        hint("detail", "magenta").hint(_info_str)(_info...);
      if (_assert_exit)
        exit(_exit_code);
    }
    return *this;
  }
};

#else

class logger()
{
public:
  logger(std::ostream &) = default;
  logger &set_ostream(std::ostream & _out)
  {
    return *this;
  }
  logger &set_sep(const std::string &_sep)
  {
    return *this;
  }
  logger &assert_push_enable()
  {
    return *this;
  }
  logger &assert_push_disable()
  {
    return *this;
  }
  logger &assert_exit()
  {
    return *this;
  }
  logger &assert_noexit()
  {
    return *this;
  }
  logger &set_exit_code(int)
  {
    return *this;
  }
  logger &endl()
  {
    return *this;
  }
  template <class... T>
  logger &log(T...)
  {
    return *this;
  }
  template <class T>
  logger &hint(T...)
  {
    return *this;
  }
  template <class... T>
  logger &operator()(T...);
  {
    return *this;
  }
  template <class... T>
  logger &assert_(T...)
  {
    return *this;
  }
};

#endif

namespace impl
{
logger cm_logger(std::cout);
}

} // namespace cm

// clang-format off
#ifdef CM_DEBUG
#define see(arg...) cm::impl::cm_logger.hint(#arg)(arg)
#define asee(arg...) cm::impl::cm_logger.hint(__FILE__, "magenta").hint(__LINE__, "magenta").hint(#arg)(arg)
#define cm_assert(val, arg...) cm::impl::cm_logger.assert_(__FILE__, __LINE__, #val, val, #arg, ##arg)
#else
#define see(...)
#define asee(...)
#define cm_assert(...)
#endif
// clang-format on

#endif
