#include "shell.hpp"
#include "lool.hpp"

#include <iostream>

extern void test(void);
extern void test2(void);

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

  bool run_shell = true;

  struct Main {
    void* obj;
    Func func;

    Main(void* obj, Func func) : obj(obj), func(func) {}

    int operator()(int argc, char* argv[]) {
      return func(obj, argc, argv);
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

  void exit(void) {
    run_shell = false;
  }

  Main operator[] (char* cmdname) {
    if (string(cmdname) == string("echo")) {
      return Main(this, [](void*obj, int argc, char* argv[]) -> int { return static_cast<MyCmd*>(obj)->echo(argc, argv); });
    }
    else if (string(cmdname) == string("test")) {
      return Main(nullptr, [](void*, int , char* []) -> int { test(); return 0; });
    }
    else if (string(cmdname) == string("test2")) {
      return Main(nullptr, [](void*, int , char* []) -> int { test2(); return 0; });
    }
    else if (string(cmdname) == string("exit")) {
      return Main(this, [](void* obj, int , char* []) -> int { static_cast<MyCmd*>(obj)->exit(); return 0; });
    }
    else {
      cout << "unknown" << endl;
      return Main(nullptr, nullptr);
    }
  }
};

int main()
{
  MyStdIn mystdin;
  MyStdOut mystdout;
  MyCmd mycmd;
  Shell<MyStdIn, MyStdOut, MyCmd> shell(mystdin, mystdout, mycmd);

  test2();

  while(mycmd.run_shell) {
    shell.update();
  }

  return 0;
}
