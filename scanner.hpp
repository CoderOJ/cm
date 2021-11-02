#ifndef CM_SCANNER_H
#define CM_SCANNER_H

#include <cstdio>
#include <cctype>

namespace cm
{

template <class Type, size_t buf_size>
class buffer_reader
{
protected:
  FILE *src;
  Type *const buff;
  Type *buff_end;
  Type *buff_pos;

  void flush()
  {
    buff_end = buff + fread(buff, sizeof(Type), buf_size, src);
    buff_pos = buff;
    if (buff_end == buff)
    {
      *buff_end = '\0';
    }
  }

public:
  buffer_reader(FILE *src) : src(src), buff(new Type[buf_size])
  {
    flush();
  }

  buffer_reader(const buffer_reader&) =delete;
  buffer_reader(buffer_reader&&) =delete;
  buffer_reader& operator= (const buffer_reader&) =delete;
  buffer_reader& operator= (buffer_reader&&) =delete;

  Type get() const
  {
    return *buff_pos;
  }
  Type next()
  {
    Type result = get();
    buff_pos ++;
    if (buff_pos == buff_end)
      flush();
    return result;
  }

  ~buffer_reader()
  {
    fclose(src);
    delete[] buff;
  }
};

#ifdef CM_DEBUG
using optimal_reader = buffer_reader<char, 1>;
#else
using optimal_reader = buffer_reader<char, 1 << 16>;
#endif

template <class BufferReader>
class scanner : protected BufferReader
{
private:
  using BufferReader::get;
  using BufferReader::next;
  inline bool is_ws(char c)
  {
    return c <= ' ';
  }
  inline bool is_cr(char c)
  {
    return c == '\n' || c == '\r';
  }

  int get_sign()
  {
    while (!isdigit(get()) && get() != '-')
      next();
    if (get() == '-')
      return next(), -1;
    return 1;
  }

public:
  scanner() =delete;
  using BufferReader::BufferReader;

  char next_char()
  {
    while (is_ws(get())) next();
    return next();
  }

  char *next_token(char *s)
  {
    while (is_ws(get())) next();
    while (!is_ws(get())) *s++ = next();
    *s = '\0';
    return s;
  }

  char *next_line(char *s)
  {
    while (is_ws(get())) next();
    while (!is_cr(get())) *s++ = next();
    *s = '\0';
    return s;
  }
  
  int next_int()
  {
    int sign = get_sign();
    int result = 0;
    while (isdigit(get()))
      result = result * 10 + (next() - '0');
    return sign * result;
  }

  long long next_long()
  {
    int sign = get_sign();
    long long result = 0;
    while (isdigit(get()))
      result = result * 10 + (next() - '0');
    return sign * result;
  }

  template <class Integer>
  Integer next_integer()
  {
    Integer sign = get_sign();
    Integer result(0);
    while (isdigit(get()))
      result = result * Integer(10) + Integer(next() - '0');
    return sign * result;
  }

  double next_double()
  {
    int sign = get_sign();
    double result = 0;
    while (isdigit(get()))
      result = result * 10 + (next() - '0');
    if (get() == '.')
    {
      next();
      double cur_ep = 0.1;
      while (isdigit(get()))
        result += cur_ep * (next() - '0'), cur_ep *= 0.1;
    }
    return sign * result;
  }
};

}

#endif
