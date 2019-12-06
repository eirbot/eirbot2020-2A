#include "lool.hpp"
#include "simple_dynamic_random_access_stream.hpp"

#include <iostream>

using namespace std;

void test(void)
{
  Lool lool;
  SimpleDynamicRandomAccessStream ar(lool);
  char buff[sizeof(lool)];

  lool.a = 666;
  lool.b = 42;
  lool.c = 0xFF00FF00;
  lool.d = 69;

  ar.read(buff, ar.size());

  cout << "size : " << ar.size() << endl;
  for (unsigned int i = 0 ; i < ar.size() ; i++) {
    cout << static_cast<unsigned int>(static_cast<unsigned char>(buff[i])) << endl;
    buff[i] = 0;
  }

  lool.c = 0xDEADBEEF;

  ar.read(buff, ar.size());

  cout << "size : " << ar.size() << endl;
  for (unsigned int i = 0 ; i < ar.size() ; i++) {
    cout << static_cast<unsigned int>(static_cast<unsigned char>(buff[i])) << endl;
    buff[i] = 0;
  }

  char a = static_cast<char>(0xEE);
  ar.seek(sizeof(lool.a)+sizeof(lool.b));
  ar.write(&a, 1);
  ar.seek(0);

  ar.read(buff, ar.size());

  cout << "size : " << ar.size() << endl;
  for (unsigned int i = 0 ; i < ar.size() ; i++) {
    cout << static_cast<unsigned int>(static_cast<unsigned char>(buff[i])) << endl;
    buff[i] = 0;
  }
}
