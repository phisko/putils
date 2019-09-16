# [CLI](CLI.hpp)

Simple and extensible Command-Line Interface class which lets you simply provide a set of commands.

### Members

##### Constructor

```cpp
CLI(const CommandMap &funcs,
    const Command &default_ = [](const std::string &cmd, const std::vector<Param> &) { std::cout << "Unknown command: " << cmd << std::endl; },
    const std::function<std::string()> &prompt = []{ return "> "; },
    std::vector<std::pair<char, char>> &&delimiters);
```
* `funcs`: the set of commands the user may enter
* `default`: the function to execute if an unknown command is entered (defaults to an error message)
* `prompt`: a function returning the message to display as a prompt (defaults to "> ")
* `delimiters`: pairs of delimiters which may surround parameters (defaults to "", '' and ())

##### run
```cpp
void run() const;
```
Starts the input loop.

##### step
```cpp
void step() const
```
Reads and executes one command.

##### addCommand
```cpp
void addCommand(const std::string &cmd, const Command &func);
```
Adds a new command (perhaps an alias the user created)

##### running
```cpp
std::atomic<bool> running { true };
```
Used for multi-threading purposes, to force the CLI to stop.

##### Ease-of-use types

```cpp
struct Param
{
    std::string value;
    std::pair<char, char> delimiters;
};
```
A parameter is defined by its value, and possibly the delimiters that surrounded it.

```cpp
using Command = std::function<void(const std::string &cmd, const std::vector<Param> &params)>;
```
A `Command` takes two parameters: the command itself (argv[0]), and the set of parameters entered by the user.

```cpp
using CommandMap = std::unordered_map<std::string, Command>;
```
Maps strings (the command the user has to type) to commands (the function to execute).

### Example

```cpp
int main()
{
    putils::CLI(
        // Commands
        {
            {
                "test",
                [](const std::string &, const std::vector<putils::CLI::Param> &params)
                {
                    std::cout << "Test successful" << std::endl;
                    for (const auto &p : params)
                        std::cout << "\t[" << p.value << "]" << std::endl;
                }
            }
        },
        // Default
        [](const std::string &cmd, const std::vector<putils::CLI::Param> &params)
        {
            std::string line = cmd;

            for (const auto &p : params)
            {
                line += " ";
                if (p.delimiters.first != '\0')
                {
                    line.append(1, p.delimiters.first);
                    line += p.value;
                    line.append(1, p.delimiters.second);
                }
                else
                    line += p.value;
            }

            std::cout << "[" << line << "]" << std::endl;
            system(line.c_str());
        }
    ).run();
}
```
