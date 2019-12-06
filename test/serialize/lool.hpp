#ifndef TEST_H
#define TEST_H

#include "archive.hpp"

#include <cstdint>

struct Lool {
  uint64_t a;
  uint8_t b;
  uint32_t c;
  int16_t d;

  int16_t getD(void) { return d; }
  void setD(int16_t val) { d = val; }
};

namespace archive {

template<typename Archive>
Archive& operator<<(Archive& ar, Lool& lool) {
  ar << lool.a;

  ar << make_named("b", lool.b);

  ar << lool.c;

  decltype(lool.d) d = lool.getD();
  ar << make_named("d", d);
  lool.setD(d);

  return ar;
};

}

#endif // TEST_H
