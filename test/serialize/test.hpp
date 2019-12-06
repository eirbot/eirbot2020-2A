#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <cmath>

using namespace std;

namespace archive {

namespace internal {

class DynamicRandomAccessStream {
private:
  struct Size {
    unsigned int size = 0;
    Size& operator<< (int val) {
      size += sizeof (val);
      return *this;
    }

//    template<typename T>
//    Size& operator<< (T val) {
//      return archive::Helper<Size, T>(*this) << val;
//    }
  };

  struct Read {
    unsigned int cursor;
    unsigned int size;
    char* buff;

    Read(unsigned int cursor, char* buff, const unsigned int& size)
      : cursor(cursor), size(size), buff(buff) {
    }

    Read& operator<< (int val) {
      //cout << "cursor : " << cursor << endl;
      //cout << "size : " << size << endl;

      if (size > 0) {
        if (cursor < sizeof(val)) {
          unsigned int s = min(size, (unsigned int)sizeof(val) - cursor);
          memcpy(buff, ((char*)&val) + cursor, s);
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

//    template<typename T>
//    Read& operator<< (T val) {
//      return archive::Helper<Read, T>(*this) << val;
//    }
  };

  struct Write {
    unsigned int cursor;
    unsigned int size;
    char* buff;

    Write(unsigned int cursor, char* buff, const unsigned int& size)
      : cursor(cursor), size(size), buff(buff) {
    }

    Write& operator<< (int& val) {
      //cout << "cursor : " << cursor << endl;
      //cout << "size : " << size << endl;

      if (size > 0) {
        if (cursor < sizeof(val)) {
          unsigned int s = min(size, (unsigned int)sizeof(val) - cursor);
          memcpy(((char*)&val) + cursor, buff, s);
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

//    template<typename T>
//    Write& operator<< (T& val) {
//      return archive::Helper<Write, T>(*this) << val;
//    }
  };

private:
  unsigned int cursor = 0;

  void* data_ptr = 0;

  using ReadFunc = Read& (*)(Read&, void*);
  ReadFunc read_func = 0;

  using WriteFunc = Write& (*)(Write&, void*);
  WriteFunc write_func = 0;

  using SizeFunc = Size& (*)(Size&, void*);
  SizeFunc size_func = 0;

public:
  template<typename T>
  DynamicRandomAccessStream(T& val) {
    read_func = [](Read& r, void* ptr) -> Read& {
      return r << *(T*)ptr;
    };

    write_func = [](Write& w, void* ptr) -> Write& {
      return w << *(T*)ptr;
    };

    size_func = [](Size& s, void* ptr) -> Size& {
      return s << *(T*)ptr;
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

using namespace internal;

}

struct Lool {
  int a;
  int b;
  int c;
  int d;
};

namespace archive {

template<typename Archive>
Archive& operator<<(Archive& ar, Lool& lool) {
  ar << lool.a << lool.b << lool.c;
  if (lool.c == 0xDEADBEEF) {
    ar << lool.d;
  }
  return ar;
};

}

void test(void);

#endif // TEST_H
