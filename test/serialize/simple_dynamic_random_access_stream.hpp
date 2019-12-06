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

    Size& add(unsigned int add_size) {
      size += add_size;
      return *this;
    }

    Size& operator<< (char val) { return add(sizeof(val)); }
    Size& operator<< (short val) { return add(sizeof(val)); }
    Size& operator<< (int val) { return add(sizeof(val)); }
    Size& operator<< (long val) { return add(sizeof(val)); }
    Size& operator<< (long long val) { return add(sizeof(val)); }

    Size& operator<< (unsigned char val) { return add(sizeof(val)); }
    Size& operator<< (unsigned short val) { return add(sizeof(val)); }
    Size& operator<< (unsigned int val) { return add(sizeof(val)); }
    Size& operator<< (unsigned long val) { return add(sizeof(val)); }
    Size& operator<< (unsigned long long val) { return add(sizeof(val)); }

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

    Read& read(const char* const data, const unsigned int len) {
      if (size > 0) {
        if (cursor < len) {
          unsigned int s = std::min(size, static_cast<unsigned int>(len) - cursor);
          std::memcpy(buff, data + cursor, s);
          size -= s;
          cursor = 0;
          buff += s;
        }
        else {
          cursor -= len;
        }
      }

      return *this;
    }

    Read& operator<< (char& val) { return read(reinterpret_cast<char*>(&val), sizeof(val)); }
    Read& operator<< (short& val) { return read(reinterpret_cast<char*>(&val), sizeof(val)); }
    Read& operator<< (int& val) { return read(reinterpret_cast<char*>(&val), sizeof(val)); }
    Read& operator<< (long& val) { return read(reinterpret_cast<char*>(&val), sizeof(val)); }
    Read& operator<< (long long& val) { return read(reinterpret_cast<char*>(&val), sizeof(val)); }

    Read& operator<< (unsigned char& val) { return read(reinterpret_cast<char*>(&val), sizeof(val)); }
    Read& operator<< (unsigned short& val) { return read(reinterpret_cast<char*>(&val), sizeof(val)); }
    Read& operator<< (unsigned int& val) { return read(reinterpret_cast<char*>(&val), sizeof(val)); }
    Read& operator<< (unsigned long& val) { return read(reinterpret_cast<char*>(&val), sizeof(val)); }
    Read& operator<< (unsigned long long& val) { return read(reinterpret_cast<char*>(&val), sizeof(val)); }

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

    Write& write(char* const data, const unsigned int len) {
      if (size > 0) {
        if (cursor < len) {
          unsigned int s = std::min(size, static_cast<unsigned int>(len) - cursor);
          std::memcpy(data + cursor, buff, s);
          size -= s;
          cursor = 0;
          buff += s;
        }
        else {
          cursor -= len;
        }
      }

      return *this;
    }

    Write& operator<< (char& val) { return write(reinterpret_cast<char*>(&val), sizeof(val)); }
    Write& operator<< (short& val) { return write(reinterpret_cast<char*>(&val), sizeof(val)); }
    Write& operator<< (int& val) { return write(reinterpret_cast<char*>(&val), sizeof(val)); }
    Write& operator<< (long& val) { return write(reinterpret_cast<char*>(&val), sizeof(val)); }
    Write& operator<< (long long& val) { return write(reinterpret_cast<char*>(&val), sizeof(val)); }

    Write& operator<< (unsigned char& val) { return write(reinterpret_cast<char*>(&val), sizeof(val)); }
    Write& operator<< (unsigned short& val) { return write(reinterpret_cast<char*>(&val), sizeof(val)); }
    Write& operator<< (unsigned int& val) { return write(reinterpret_cast<char*>(&val), sizeof(val)); }
    Write& operator<< (unsigned long& val) { return write(reinterpret_cast<char*>(&val), sizeof(val)); }
    Write& operator<< (unsigned long long& val) { return write(reinterpret_cast<char*>(&val), sizeof(val)); }

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
