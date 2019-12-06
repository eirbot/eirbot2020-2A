#ifndef SIMPLE_DYNAMIC_RANDOM_ACCESS_STREAM_HPP
#define SIMPLE_DYNAMIC_RANDOM_ACCESS_STREAM_HPP

#include <cstring>
#include <algorithm>

#include "archive.hpp"

namespace simple_dynamic_random_access_stream {

using namespace archive;

class SimpleDynamicRandomAccessStream {
private:
  struct Size {
    unsigned int size = 0;
    Size& operator<< (int val) {
      size += sizeof(val);
      return *this;
    }

    template<typename T, typename String>
    Size& operator<< (Named<T, String>&&) {
      size += sizeof(T);
      return *this;
    }
  };

  struct Read {
    unsigned int cursor;
    unsigned int size;
    char* buff;

    Read(unsigned int cursor, char* buff, const unsigned int& size)
      : cursor(cursor), size(size), buff(buff) {
    }

    Read& operator<< (int val) {
      if (size > 0) {
        if (cursor < sizeof(val)) {
          unsigned int s = std::min(size, static_cast<unsigned int>(sizeof(val)) - cursor);
          std::memcpy(buff, reinterpret_cast<char*>(&val) + cursor, s);
          size -= s;
          cursor = 0;
          buff += s;
        }
        else {
          cursor -= sizeof(val);
        }
      }

      return *this;
    }

    template<typename T, typename String>
    Read& operator<< (Named<T, String>&& named) {
      return *this << named.variable;
    }
  };

  struct Write {
    unsigned int cursor;
    unsigned int size;
    char* buff;

    Write(unsigned int cursor, char* buff, const unsigned int& size)
      : cursor(cursor), size(size), buff(buff) {
    }

    Write& operator<< (int& val) {
      if (size > 0) {
        if (cursor < sizeof(val)) {
          unsigned int s = std::min(size, static_cast<unsigned int>(sizeof(val)) - cursor);
          std::memcpy(reinterpret_cast<char*>(&val) + cursor, buff, s);
          size -= s;
          cursor = 0;
          buff += s;
        }
        else {
          cursor -= sizeof(val);
        }
      }

      return *this;
    }

    template<typename T, typename String>
    Write& operator<< (Named<T, String>&& named) {
      return *this << named.variable;
    }
  };

private:
  unsigned int cursor = 0;

  void* data_ptr = nullptr;

  using ReadFunc = Read& (*)(Read&, void*);
  ReadFunc read_func = nullptr;

  using WriteFunc = Write& (*)(Write&, void*);
  WriteFunc write_func = nullptr;

  using SizeFunc = Size& (*)(Size&, void*);
  SizeFunc size_func = nullptr;

public:
  template<typename T>
  SimpleDynamicRandomAccessStream(T& val) {
    read_func = [](Read& r, void* ptr) -> Read& {
      return r << *static_cast<T*>(ptr);
    };

    write_func = [](Write& w, void* ptr) -> Write& {
      return w << *static_cast<T*>(ptr);
    };

    size_func = [](Size& s, void* ptr) -> Size& {
      return s << *static_cast<T*>(ptr);
    };

    data_ptr = &val;
  }

  void seek(unsigned int new_cursor) {
    cursor = new_cursor;
  }

  void read(char* buff, unsigned int size) {
    Read r(cursor, buff, size);
    read_func(r, data_ptr);
  }

  void write(char* buff, unsigned int size) {
    Write w(cursor, buff, size);
    write_func(w, data_ptr);
  }

  unsigned int size(void) {
    Size s;
    size_func(s, data_ptr);
    return s.size;
  }
};

}

using simple_dynamic_random_access_stream::SimpleDynamicRandomAccessStream;

#endif // SIMPLE_DYNAMIC_RANDOM_ACCESS_STREAM_HPP
