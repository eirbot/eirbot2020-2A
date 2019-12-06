#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

namespace archive {

template<typename T, typename String = const char*>
struct Named {
  String name;
  T& variable;
};

template<typename T, typename String = const char*>
inline Named<T, String> make_named(String name, T& var) {
  return Named<T, String> { name, var };
}

}

#endif // ARCHIVE_HPP
