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

  int test(int argc, char* argv[]) {

    for (int i = 0 ; i < argc ; i++) {
      cout << argv[i] << " ";
    }
    cout << endl;

    return 0;
  }

  Main operator[] (char* cmdname) {
    if (string(cmdname) == string("test")) {
      return Main(this, [](void*obj, int argc, char* argv[]) -> int { return ((MyCmd*)obj)->test(argc, argv); });
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
