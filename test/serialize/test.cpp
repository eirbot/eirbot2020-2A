#include "test.hpp"

void test(void)
{
  Lool lool;
  archive::DynamicRandomAccessStream ar(lool);
  char buff[sizeof(lool)];

  lool.a = 42;
  lool.b = 666;
  lool.c = 69;
  lool.d = 0xFFFFFFFF;

  ar.read(buff, sizeof(buff));

  cout << "size : " << ar.size() << endl;
  for (int i = 0 ; i < sizeof(buff) ; i++) {
    cout << (unsigned int)(unsigned char)buff[i] << endl;
    buff[i] = 0;
  }

  lool.c = 0xDEADBEEF;

  ar.read(buff, sizeof(buff));

  cout << "size : " << ar.size() << endl;
  for (int i = 0 ; i < sizeof(buff) ; i++) {
    cout << (unsigned int)(unsigned char)buff[i] << endl;
    buff[i] = 0;
  }

  char a = 0xEE;
  ar.seek(8);
  ar.write(&a, 1);
  ar.seek(0);

  ar.read(buff, sizeof(buff));

  cout << "size : " << ar.size() << endl;
  for (int i = 0 ; i < sizeof(buff) ; i++) {
    cout << (unsigned int)(unsigned char)buff[i] << endl;
    buff[i] = 0;
  }
}
