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

struct Lool2 {
  Lool& l1;
  Lool& l2;
};

namespace archive {

template<typename Archive>
Archive& serialize(Archive& ar, Lool& lool) {
  ar << lool.a;

  ar << make_named("b", lool.b);

  ar << lool.c;

  decltype(lool.d) d = lool.getD();
  ar << make_named("d", d);
  lool.setD(d);

  return ar;
};


template<typename Archive>
Archive& serialize(Archive& ar, Lool2& lool2) {
  ar << lool2.l1;

  ar << make_named("test", lool2.l2);

  return ar;
};

}

#endif // TEST_H
