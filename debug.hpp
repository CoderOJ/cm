#ifndef CM_DEBUG_H
#define CM_DEBUG_H

#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <utility>

namespace cm
{

class logger
{
private:
  std::ostream *_out;
  std::string   _sep         = "\x2c\x20";
  bool          _assert_exit = true;
  int           _exit_code   = 0;

  std::string _c_red          = "\033[0;31m";
  std::string _c_green        = "\033[0;32m";
  std::string _c_yellow       = "\033[0;33m";
  std::string _c_blue         = "\033[0;34m";
  std::string _c_magenta      = "\033[0;35m";
  std::string _c_cyan         = "\033[0;36m";
  std::string _c_red_bold     = "\033[1;31m";
  std::string _c_green_bold   = "\033[1;32m";
  std::string _c_yellow_bold  = "\033[1;33m";
  std::string _c_blue_bold    = "\033[1;34m";
  std::string _c_magenta_bold = "\033[1;35m";
  std::string _c_cyan_bold    = "\033[1;36m";
  std::string _c_reset        = "\033[0m";

  std::string _get_color(std::string _name)
  {
    if (_name == "red")
      return _c_red;
    if (_name == "green")
      return _c_green;
    if (_name == "yellow")
      return _c_yellow;
    if (_name == "blue")
      return _c_blue;
    if (_name == "magenta")
      return _c_magenta;
    if (_name == "cyan")
      return _c_cyan;
    if (_name == "red bold")
      return _c_red_bold;
    if (_name == "green bold")
      return _c_green_bold;
    if (_name == "yellow bold")
      return _c_yellow_bold;
    if (_name == "blue bold")
      return _c_blue_bold;
    if (_name == "magenta bold")
      return _c_magenta_bold;
    if (_name == "cyan bold")
      return _c_cyan_bold;
    if (_name == "reset")
      return _c_reset;
    return "";
  }

public:
  logger(std::ostream &_out) : _out(&_out) {}

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
    return log(_color).log(x).log("\x3a\x20").log(_c_reset);
  }

  template <class... T>
  logger &operator()(T... _value)
  {
    return log(_value...).endl();
  }

  template <class... T>
  logger &_assert(const std::string &_file, int _line, const std::string &_raw,
                  bool _value, const std::string &_info_str, T... _info)
  {
    if (!_value)
    {
      endl();
      hint(_file, "magenta")
          .hint(_line, "magenta")
          .log(_c_yellow)
          .log("Assertion `")
          .log(_c_yellow_bold)
          .log(_raw)
          .log(_c_yellow)
          .log("` failed")
          .log(_c_reset)
          .endl();
      if (_info_str != "")
        hint("detail", "magenta").hint(_info_str)(_info...);
      if (_assert_exit)
        exit(_exit_code);
    }
    return *this;
  }
};

namespace impl
{
logger see_logger(std::cout);
}

} // namespace cm

// clang-format off
#ifdef CM_DEBUG
#define see(arg...) cm::impl::see_logger.hint(#arg)(arg)
#define asee(arg...) cm::impl::see_logger.hint(__FILE__, "magenta").hint(__LINE__, "magenta").hint(#arg)(arg)
#define cm_assert(val, arg...) cm::impl::see_logger._assert(__FILE__, __LINE__, #val, val, #arg, ##arg)
#else
#define see(...)
#define asee(...)
#define cm_assert(...)
#endif
// clang-format on

#endif
