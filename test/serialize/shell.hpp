#ifndef SHELL_HPP
#define SHELL_HPP

template<class InputStream, class OutputStream, class Cmd>
class Shell {
private:
  InputStream& _in;
  OutputStream& _out;
  Cmd& _cmd;

  char _argv_data[256] = {0};

  char* _argv[16] = {0};
  int _argc = 0;

public:
  Shell(InputStream& in, OutputStream& out, Cmd& cmd)
    : _in(in), _out(out), _cmd(cmd) {
  }

private:
  bool isDigit(char c) {
    return '0' <= c && c <= '9';
  }

  bool isLowerCaseLetter(char c) {
    return 'a' <= c && c <= 'z';
  }

  bool isUpperCaseLetter(char c) {
    return 'A' <= c && c <= 'Z';
  }

  bool isLetter(char c) {
    return isLowerCaseLetter(c) || isUpperCaseLetter(c);
  }

  bool isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r';
  }

public:
  void update(void) {
    while(_in.readable()) {
      char c = _in.get();

      if (_argc == 0) {
        _argv[0] = &_argv_data[0];
        _argv[1] = &_argv_data[0];
        _argv[1][0] = '\n';
        _argc = 1;
      }

      if (isLetter(c) || isDigit(c) || c == '.') {
        _argv[_argc][0] = c;
        _argv[_argc][1] = '\0';
        _argv[_argc]++;
      }
      else if (isWhitespace(c)) {
        if (_argv[_argc][0] != '\n') {
          _argv[_argc][1] = '\n';
          _argv[_argc]++;
          _argc++;
          _argv[_argc] = _argv[_argc-1];
        }
      }
      else if (c == '\n') {
        if (_argv[_argc][0] == '\n') {
          _argc--;
        }

        _argv[_argc]++;

        typename Cmd::Main func = _cmd[_argv[0]];

        if (func) {
          func(_argc, _argv);
        }

        _argc = 0;
      }
      else {
        // Not accepted character
      }
    }
  }
};

#endif // SHELL_HPP
