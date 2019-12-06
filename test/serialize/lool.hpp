#ifndef TEST_H
#define TEST_H

#include "archive.hpp"

struct Lool {
  int a;
  int b;
  int c;
  int d;

  int getD(void) { return d; }
  void setD(int val) { d = val; }
};

namespace archive {

template<typename Archive>
Archive& operator<<(Archive& ar, Lool& lool) {
  ar << lool.b;

  ar << make_named("a", lool.a);

  ar << lool.c;

  int d = lool.getD();
  ar << make_named("d", d);
  lool.setD(d);

  return ar;
};

}

#endif // TEST_H
