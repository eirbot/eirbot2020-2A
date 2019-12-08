#ifndef DYNAMIC_ARCHIVE_FILE_SYSTEM_HPP
#define DYNAMIC_ARCHIVE_FILE_SYSTEM_HPP

#include <cstring>
#include <cstdio>

#include "archive.hpp"

namespace dynamic_archive_file_system {

class DynamicArchiveFileSystem {
private:
  struct Count {
    unsigned int count = 0;

    Count& inc(void) {
      count++;
      return *this;
    }

    Count& operator<< (char) { return inc(); }
    Count& operator<< (short) { return inc(); }
    Count& operator<< (int) { return inc(); }
    Count& operator<< (long) { return inc(); }
    Count& operator<< (long long) { return inc(); }

    Count& operator<< (unsigned char) { return inc(); }
    Count& operator<< (unsigned short) { return inc(); }
    Count& operator<< (unsigned int) { return inc(); }
    Count& operator<< (unsigned long) { return inc(); }
    Count& operator<< (unsigned long long) { return inc(); }

    template<typename T, typename String>
    Count& operator<< (archive::Named<T, String>&& named) {
      return *this << named.variable;
    }

    template<typename T>
    Count& operator<< (T val) {
      return archive::serialize(*this, val);
    }
  };

  struct Enum {
    unsigned int pos = 0;
    unsigned int local_count = 0;

    char name[256] = {0};
    unsigned int len = 0;

    Enum(void) {
      name[0] = '/';
      len = 1;
    }

    Enum& dec(void) {
      unsigned int prev_len = len;

      if(name[len-1] == '/') {
        len += static_cast<unsigned int>(std::sprintf(&name[len], "%u", local_count));
        local_count++;
      }

      if(pos){
        pos--;

        if(pos) {
          len = prev_len;
          name[len] = '\0';
        }
      }


      return *this;
    }

    Enum& operator<< (char) { return dec(); }
    Enum& operator<< (short) { return dec(); }
    Enum& operator<< (int) { return dec(); }
    Enum& operator<< (long) { return dec(); }
    Enum& operator<< (long long) { return dec(); }

    Enum& operator<< (unsigned char) { return dec(); }
    Enum& operator<< (unsigned short) { return dec(); }
    Enum& operator<< (unsigned int) { return dec(); }
    Enum& operator<< (unsigned long) { return dec(); }
    Enum& operator<< (unsigned long long) { return dec(); }

    template<typename T>
    Enum& operator<<(T var) {
      if(pos) {
        unsigned int prev_len = len;
        unsigned int prev_local_count = local_count;

        if(name[len-1] != '/') {
          name[len] = '/';
          len++;
          name[len] = '\0';
        }

        archive::serialize(*this, var);

        if(pos) {
          len = prev_len;
          local_count = prev_local_count;
          name[len] = '\0';
        }
      }

      return *this;
    }

    template<typename T, typename String>
    Enum& operator<<(archive::Named<T, String> named) {
      if(pos) {
        unsigned int prev_len = len;
        unsigned int prev_local_count = local_count;

        std::strcpy(&name[len], named.name);
        len += std::strlen(named.name);

        local_count = 0;

        *this << named.variable;

        if(pos) {
          len = prev_len;
          local_count = prev_local_count;
          name[len] = '\0';
        }
      }

      return *this;
    }
  };

private:
  void* data_ptr = nullptr;

  Count& (*count_func)(Count&, void*) = nullptr;
  Enum& (*enum_func)(Enum&, void*) = nullptr;

public:
  template<typename T>
  DynamicArchiveFileSystem(T& val) {
    data_ptr = &val;

    count_func = [](Count& c, void* ptr) -> Count& {
      return c << *static_cast<T*>(ptr);
    };

    enum_func = [](Enum& c, void* ptr) -> Enum& {
      return c << *static_cast<T*>(ptr);
    };
  }

  unsigned int size(void) {
    Count c;
    count_func(c, data_ptr);
    return c.count;
  }

  void getPath(unsigned int pos, char* const path) {
    Enum e;
    e.pos = pos+1;
    enum_func(e, data_ptr);
    std::strcpy(path, e.name);
  }
};

}

using dynamic_archive_file_system::DynamicArchiveFileSystem;

#endif // DYNAMIC_ARCHIVE_FILE_SYSTEM_HPP
