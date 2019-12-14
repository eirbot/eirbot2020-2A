#include "dynamic_archive_random_access_stream.hpp"
#include "dynamic_archive_file_system.hpp"
#include "lool.hpp"

#include <iostream>

using namespace std;

void test(void)
{
  Lool lool;
  DynamicArchiveRandomAccessStream daras(lool);
  char buff[sizeof(lool)];

  lool.a = 666;
  lool.b = 42;
  lool.c = 0xFF00FF00;
  lool.d = 69;

  daras.read(buff, daras.size());

  cout << "size : " << daras.size() << endl;
  for (unsigned int i = 0 ; i < daras.size() ; i++) {
    cout << static_cast<unsigned int>(static_cast<unsigned char>(buff[i])) << endl;
    buff[i] = 0;
  }

  lool.c = 0xDEADBEEF;

  daras.read(buff, daras.size());

  cout << "size : " << daras.size() << endl;
  for (unsigned int i = 0 ; i < daras.size() ; i++) {
    cout << static_cast<unsigned int>(static_cast<unsigned char>(buff[i])) << endl;
    buff[i] = 0;
  }

  char a = static_cast<char>(0xEE);
  daras.seek(sizeof(lool.a)+sizeof(lool.b));
  daras.write(&a, 1);
  daras.seek(0);

  daras.read(buff, daras.size());

  cout << "size : " << daras.size() << endl;
  for (unsigned int i = 0 ; i < daras.size() ; i++) {
    cout << static_cast<unsigned int>(static_cast<unsigned char>(buff[i])) << endl;
    buff[i] = 0;
  }
}

void test2(void)
{
  Lool a = {};
  Lool b = {};
  Lool2 lool = { a,b };
  DynamicArchiveFileSystem dafs(lool);
  char buff[256];

  cout << dafs.size() << endl;

  for(unsigned int i = 0 ; i < dafs.size() ; i++) {
    dafs.getPath(i, buff);
    cout << "[" << i << "] = " << buff << endl;
  }

  cout << "/test/a => " << dafs.getId("/test/b") << endl;
  cout << "unknownpath => " << dafs.getId("unknownpath") << endl;

  a.a = 666;
  a.b = 42;
  a.c = 0xFF00FF00;
  a.d = 69;

  for(unsigned int i = 0 ; i < dafs.size() ; i++) {
    dafs.getPath(i, buff);
    auto f = dafs.open(buff);

    cout << buff << " : ";
    f.read(buff, 255);
    cout << buff << endl;
  }

  auto f = dafs.open("/test/d");
  f.write("3000", sizeof("3000"));

  for(unsigned int i = 0 ; i < dafs.size() ; i++) {
    dafs.getPath(i, buff);
    auto f = dafs.open(buff);

    cout << buff << " : ";
    f.read(buff, 255);
    cout << buff << endl;
  }

}
