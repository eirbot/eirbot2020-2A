#ifndef DYNAMIC_ARCHIVE_FILE_SYSTEM_HPP
#define DYNAMIC_ARCHIVE_FILE_SYSTEM_HPP

#include <cstring>
#include <cstdio>

#include <sstream>

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

  struct Search {
    const char* path = nullptr;

    bool found = false;
    unsigned int pos = 0;
    unsigned int local_count = 0;

    char name[256] = {0};
    unsigned int len = 0;

    Search(void) {
      name[0] = '/';
      len = 1;
    }

    Search& find(void) {
      unsigned int prev_len = len;

      if(name[len-1] == '/') {
        len += static_cast<unsigned int>(std::sprintf(&name[len], "%u", local_count));
        local_count++;
      }

      found = (std::strcmp(path, name) == 0);

      if(!found){
        pos++;

        if(!found) {
          len = prev_len;
          name[len] = '\0';
        }
      }


      return *this;
    }

    Search& operator<< (char) { return find(); }
    Search& operator<< (short) { return find(); }
    Search& operator<< (int) { return find(); }
    Search& operator<< (long) { return find(); }
    Search& operator<< (long long) { return find(); }

    Search& operator<< (unsigned char) { return find(); }
    Search& operator<< (unsigned short) { return find(); }
    Search& operator<< (unsigned int) { return find(); }
    Search& operator<< (unsigned long) { return find(); }
    Search& operator<< (unsigned long long) { return find(); }

    template<typename T>
    Search& operator<<(T var) {
      if(!found) {
        unsigned int prev_len = len;
        unsigned int prev_local_count = local_count;

        if(name[len-1] != '/') {
          name[len] = '/';
          len++;
          name[len] = '\0';
        }

        archive::serialize(*this, var);

        if(!found) {
          len = prev_len;
          local_count = prev_local_count;
          name[len] = '\0';
        }
      }

      return *this;
    }

    template<typename T, typename String>
    Search& operator<<(archive::Named<T, String> named) {
      if(!found) {
        unsigned int prev_len = len;
        unsigned int prev_local_count = local_count;

        std::strcpy(&name[len], named.name);
        len += std::strlen(named.name);

        local_count = 0;

        *this << named.variable;

        if(!found) {
          len = prev_len;
          local_count = prev_local_count;
          name[len] = '\0';
        }
      }

      return *this;
    }
  };

  class FileRef {
    void* data_ptr;
    unsigned int id;

    struct Reader {
      unsigned int id;
      char* buff;
      unsigned int size;

      unsigned int count = 0;

      template<typename T>
      Reader& read(T val) {

        if(count == id) {
          std::snprintf(buff, size, "%lld", static_cast<long long>(val));
        }

        count++;
        return *this;
      }

      Reader& operator<< (char val) { return read(val); }
      Reader& operator<< (short val) { return read(val); }
      Reader& operator<< (int val) { return read(val); }
      Reader& operator<< (long val) { return read(val); }
      Reader& operator<< (long long val) { return read(val); }

      Reader& operator<< (unsigned char val) { return read(val); }
      Reader& operator<< (unsigned short val) { return read(val); }
      Reader& operator<< (unsigned int val) { return read(val); }
      Reader& operator<< (unsigned long val) { return read(val); }
      Reader& operator<< (unsigned long long val) { return read(val); }

      template<typename T, typename String>
      Reader& operator<< (archive::Named<T, String>&& named) {
        return *this << named.variable;
      }

      template<typename T>
      Reader& operator<< (T val) {
        return archive::serialize(*this, val);
      }
    };

    struct Writer {
      unsigned int id;
      const char* buff;
      unsigned int size;

      unsigned int count = 0;

      template<typename T>
      Writer& read(T& val) {

        if(count == id) {
          std::istringstream iss(buff);
          iss >> val;
        }

        count++;
        return *this;
      }

      Writer& operator<< (char& val) { return read(val); }
      Writer& operator<< (short& val) { return read(val); }
      Writer& operator<< (int& val) { return read(val); }
      Writer& operator<< (long& val) { return read(val); }
      Writer& operator<< (long long& val) { return read(val); }

      Writer& operator<< (unsigned char& val) { return read(val); }
      Writer& operator<< (unsigned short& val) { return read(val); }
      Writer& operator<< (unsigned int& val) { return read(val); }
      Writer& operator<< (unsigned long& val) { return read(val); }
      Writer& operator<< (unsigned long long& val) { return read(val); }

      template<typename T, typename String>
      Writer& operator<< (archive::Named<T, String>&& named) {
        return *this << named.variable;
      }

      template<typename T>
      Writer& operator<< (T& val) {
        return archive::serialize(*this, val);
      }
    };

    Reader& (*read_func)(Reader&, void*) = nullptr;
    Writer& (*write_func)(Writer&, void*) = nullptr;

  public:
    template<typename T>
    FileRef(T& val, unsigned int id) : data_ptr(&val), id(id) {

      read_func = [](Reader& r, void* ptr) -> Reader& {
        return r << *static_cast<T*>(ptr);
      };

      write_func = [](Writer& w, void* ptr) -> Writer& {
        return w << *static_cast<T*>(ptr);
      };

    }

    void read(char* const buff, unsigned int const size) {
      if(data_ptr) {
        Reader r;
        r.id = id;
        r.buff = buff;
        r.size = size;
        read_func(r, data_ptr);
      }
    }

    void write(const char* const buff, unsigned int const size) {
      if(data_ptr) {
        Writer w;
        w.id = id;
        w.buff = buff;
        w.size = size;
        write_func(w, data_ptr);
      }
    }

  };

private:
  void* data_ptr = nullptr;

  Count& (*count_func)(Count&, void*) = nullptr;
  Enum& (*enum_func)(Enum&, void*) = nullptr;
  Search& (*search_func)(Search&, void*) = nullptr;

  FileRef (*open_func)(void*, unsigned int) = nullptr;

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

    search_func = [](Search& c, void* ptr) -> Search& {
      return c << *static_cast<T*>(ptr);
    };

    open_func = [](void* ptr, unsigned int id) -> FileRef {
      return FileRef(*static_cast<T*>(ptr), id);
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

  unsigned int getId(const char* const path) {
    Search s;
    s.path = path;
    search_func(s, data_ptr);
    return s.pos;
  }

  FileRef open(const char* const path) {
    unsigned int id = getId(path);
    if(id < size()) {
      return open_func(data_ptr, id);
    }
    return open_func(nullptr, 0);
  }
};

}

using dynamic_archive_file_system::DynamicArchiveFileSystem;

#endif // DYNAMIC_ARCHIVE_FILE_SYSTEM_HPP
