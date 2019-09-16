# [Process](Process.hpp)

Abstraction to a process (useful to run system commands and send them input/get their output)

### Members

##### Constructor

```cpp
Process(const std::string &command, const Options &options);
```
Starts a process which will run `command` with the given `options`.

##### Options structure
```cpp
struct Options
{
    struct Stream
    {
        bool redirected;
        std::string replacement;
    };

    Stream stdin { false, ".stdin" };
    Stream stdout { false, ".stdout" };
    Stream stderr { false, ".stderr" };
};
```
Processes have 3 streams: `stdin`, `stdout` and `stderr`. Each stream may be redirected through a named pipe.

By default, none of the streams are redirected, and if they are, the default name for the pipes are .stdin, .stdout and .stderr.

##### Stream getters

```cpp
std::ostream &getStdin();
std::istream &getStdout();
std::istream &getStderr();
```
Used to get the redirected streams and communicate with the process.

##### sendEOF

```cpp
void sendEOF();
```
Sends the equivalent of a CTRL+D to the process' stdin.
