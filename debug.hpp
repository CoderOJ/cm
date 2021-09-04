#ifndef CM_DEBUG_H
#define CM_DEBUG_H 1

#include <iostream>
#include <utility>
#include <tuple>
#include <iterator>
#include <string>

namespace cm
{

class logger
{
private:
  std::ostream *out;
  std::string sep = "\x2c\x20";
  bool _assert_exit = true;
  
  std::string c_red     = "\033[0;31m";
  std::string c_green   = "\033[0;32m";
  std::string c_yellow  = "\033[0;33m";
  std::string c_blue    = "\033[0;34m";
  std::string c_magenta = "\033[0;35m";
  std::string c_cyan    = "\033[0;36m";
  std::string c_red_bold     = "\033[1;31m";
  std::string c_green_bold   = "\033[1;32m";
  std::string c_yellow_bold  = "\033[1;33m";
  std::string c_blue_bold    = "\033[1;34m";
  std::string c_magenta_bold = "\033[1;35m";
  std::string c_cyan_bold    = "\033[1;36m";
  std::string c_reset   = "\033[0m";

  std::string get_color(std::string name)
  {
    if (name == "red")     return c_red;
    if (name == "green")   return c_green;
    if (name == "yellow")  return c_yellow;
    if (name == "blue")    return c_blue;
    if (name == "magenta") return c_magenta;
    if (name == "cyan")    return c_cyan;
    if (name == "red bold")     return c_red_bold;
    if (name == "green bold")   return c_green_bold;
    if (name == "yellow bold")  return c_yellow_bold;
    if (name == "blue bold")    return c_blue_bold;
    if (name == "magenta bold") return c_magenta_bold;
    if (name == "cyan bold")    return c_cyan_bold;
    if (name == "reset")   return c_reset;
    return "";
  }

public:
  logger(std::ostream &out) : out(&out) {}

  logger& set_ostream(std::ostream &out)
  {
    this->out = &out;
    return *this;
  }
  logger& set_sep(const std::string &sep)
  {
    this->sep = sep;
    return *this;
  }
  logger& assert_exit()
  {
    _assert_exit = true;
    return *this;
  }
  logger& assert_noexit()
  {
    _assert_exit = false;
    return *this;
  }

  logger& endl()
  {
    *out << std::endl;
    return *this;
  }

  logger& log()
  {
    return *this;
  }
  template <class Ta, class Tb, class... Tc> 
  logger& log(const Ta &a, const Tb &b, Tc... c)
  {
    return log(a).log(sep).log(b, c...);
  }
  template <class Ta,
            typename = decltype(std::cout << std::declval<Ta>())>
  logger& log(const Ta &a)
  {
    *out << a;
    return *this;
  }

  // output a container, except std::string
  template <class T, 
            typename = decltype(std::declval<T>().begin()),
            typename = decltype(std::declval<T>().end()),            
            typename = typename std::enable_if<!std::is_base_of<
              std::string, typename std::decay<T>::type>::value>::type,
            typename value_type = typename T::value_type>
  logger& log(const T &container)
  {
    log("{");
    for (auto it = container.begin(); it != container.end(); ++it)
    {
      if (it != container.begin())
        log(sep);
      log(*it);
    }
    log("}");
    return *this;
  }
  // output a std::pair
  template <class Ta, class Tb>
  logger& log(const std::pair<Ta,Tb> &x)
  {
    return log("(").log(x.first, x.second).log(")");
  }
  // output std::tuple
#if __cplusplus >= 201703L
  template <class... T>
  logger& log(const std::tuple<T...> &x)
  {
    log("(");
    std::apply([this](auto... x){ this->log(x...); }, x);
    return log(")");
  }
#else
  template <class... T>
  logger& log(const std::tuple<T...> &)
  {
    return log("<tuple>");
  }
#endif

  template <class T>
  logger& hint(const T &x, std::string color = "cyan")
  {
    color = get_color(color);
    return log(color).log(x).log("\x3a\x20").log(c_reset);
  }

  template <class... T>
  logger& operator() (T... value)
  {
    return log(value...).endl();
  }

  template <class... T>
  logger& _assert(const std::string &file, int line, 
                  const std::string &raw, bool value, 
                  const std::string &info_str, T... info)
  {
    if (!value)
    {
      endl();
      hint(file, "magenta").hint(line, "magenta")
        .log(c_yellow).log("Assertion `")
        .log(c_yellow_bold).log(raw)
        .log(c_yellow).log("` failed")
        .log(c_reset).endl();
      if (info_str != "")
        hint("detail", "magenta").hint(info_str)(info...);
      if (_assert_exit) exit(0);
    }
    return *this;
  }
};

namespace impl
{
logger see_logger(std::cout);
}

}

#ifdef CM_DEBUG
#define see(arg...) cm::impl::see_logger.hint(#arg)(arg)
#define asee(arg...) cm::impl::see_logger.hint(__FILE__, "magenta").hint(__LINE__, "magenta").hint(#arg)(arg)
#define cm_assert(val, arg...) cm::impl::see_logger._assert(__FILE__, __LINE__, #val, val, #arg, ##arg)
#else
#define see(...)
#define asee(...)
#define cm_assert(...)
#endif

#endif
