#include "shell.hpp"
#include "test.hpp"

#include <iostream>

using namespace std;

struct MyStdIn {
  bool read = true;

  size_t readable(void) {
    if (read) {
      return 1;
    }
    else {
      read = true;
      return 0;
    }
  }

  char get(void) {
    char ret = 0;
    cin.get(ret);
    read = false;
    return ret;
  }
};

struct MyStdOut {
  size_t writeable(void) {
    return 1;
  }

  void put(char c) {
    cout.put(c);
  }
};

struct MyCmd {
  using Func = int (*)(void* obj, int argc, char* argv[]);

  struct Main {
    void* obj;
    Func func;

    Main(void* obj, Func func) : obj(obj), func(func) {}

    int operator()(int argc, char* argv[]) {
      func(obj, argc, argv);
    }

    operator bool(void) {
      return func;
    }
  };

  int echo(int argc, char* argv[]) {

    for (int i = 1 ; i < argc ; i++) {
      cout << argv[i] << " ";
    }
    cout << endl;

    return 0;
  }

  Main operator[] (char* cmdname) {
    if (string(cmdname) == string("echo")) {
      return Main(this, [](void*obj, int argc, char* argv[]) -> int { return ((MyCmd*)obj)->echo(argc, argv); });
    }
    if (string(cmdname) == string("test")) {
      return Main(0, [](void*obj, int argc, char* argv[]) -> int { test(); return 0; });
    }
    else {
      cout << "unknown" << endl;
      return Main(0,0);
    }
  }
};

int main()
{
  MyStdIn mystdin;
  MyStdOut mystdout;
  MyCmd mycmd;
  Shell<MyStdIn, MyStdOut, MyCmd> shell(mystdin, mystdout, mycmd);

  while(1) {
    shell.update();
  }

  return 0;
}
