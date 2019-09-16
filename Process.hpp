#pragma once

#include <cstdlib>
#include <thread>

#include "NamedPipe.hpp"
#include "concat.hpp"
#include "read_stream.hpp"

namespace putils {
    class Process {
    public:
        struct Options {
            struct Stream {
                bool redirected;
                std::string replacement;
            };

            Stream stdin { false, ".stdin" };
            Stream stdout { false, ".stdout" };
            Stream stderr { false, ".stderr" };
        };

    public:
        Process(std::string_view command,
                const Options & options = Options{ { false, "" },
                                                   { false, "" },
                                                   { false, "" } })
                : _options(options) {
            std::string run(command);
            std::vector<std::function<void()>> toOpen;

            for (const auto & p : redirects) {
                const auto stream = p.first;
                const auto & redirect = p.second.first;
                const auto pipe = p.second.second;

                const auto & s = options.*stream;
                if (s.redirected) {
                    const auto & replacement = s.replacement;

                    // Delay opening to let fifos be opened by system call first
                    toOpen.push_back([this, pipe, replacement] {
                        (this->*pipe).open(replacement.c_str(),
                                           pipe == &Process::_stdin ? std::ifstream::out : std::ifstream::in
                        );
                    });
                    system(putils::concat("mkfifo ", replacement).c_str());
                    run = putils::concat(run, redirect, replacement);
                }
            }

            std::thread([run] {
                system(run.c_str());
            }).detach();

            for (const auto & f : toOpen)
                f();
        }

        ~Process() = default;

        std::ostream & getStdin() { return getPipe(&Options::stdin, "stdin", _stdin); }
        std::istream & getStdout() { return getPipe(&Options::stdout, "stdout", _stdout); }
        std::istream & getStderr() { return getPipe(&Options::stderr, "stderr", _stderr); }

        void sendEOF() { _stdin.close(); }

    private:
        template<typename Str>
        NamedPipe & getPipe(Options::Stream Options::*stream, Str && name, NamedPipe & pipe) {
            const auto & s = _options.*stream;
            if (!s.redirected)
                throw std::logic_error(putils::concat(name, " was not redirected"));

            if (!pipe.is_open())
                pipe.open(s.replacement);

            return pipe;
        }

    private:
        Options _options;
        NamedPipe _stdin;
        NamedPipe _stdout;
        NamedPipe _stderr;

    private:
        std::vector<std::pair<Options::Stream Options::*, std::pair<std::string, NamedPipe Process::*>>> redirects =
                {
                        { &Options::stdin,  { " < ",  &Process::_stdin } },
                        { &Options::stdout, { " 1> ", &Process::_stdout } },
                        { &Options::stderr, { " 2> ", &Process::_stderr } }
                };
    };

    namespace test {
        namespace {
            template<typename Str>
            void test(Str && command) {
                putils::Process::Options options;
                options.stdout.redirected = true;
                options.stderr.redirected = true;

                putils::Process p(FWD(command), options);

                std::cout << command << ":" << std::endl;

                std::string line;
                std::cout << "Reading stdout:" << std::endl;
                while (std::getline(p.getStdout(), line)) {
                    std::cout << "\t[" << line << "]" << std::endl;
                    p.getStdout().sync();
                }

                std::cout << "Reading stderr:" << std::endl;
                while (std::getline(p.getStderr(), line))
                    std::cout << "\t[" << line << "]" << std::endl;
            }
        }

        inline void process() {
            putils::Process::Options options;
            options.stdout.redirected = true;
            options.stderr.redirected = true;

            test("ls");
            std::cout << std::endl;
            test("ls \"BAD FILE\"");
            std::cout << std::endl;

            {
                putils::Process::Options options;
                options.stdout.redirected = true;
                options.stderr.redirected = true;
                options.stdin.redirected = true;

                putils::Process p("cat", options);

                p.getStdin() << "this is my input to cat" << std::endl;

                std::cout << "INPUT:" << std::endl;

                p.sendEOF();

                std::string line;
                std::cout << "Reading stdout:" << std::endl;
                while (std::getline(p.getStdout(), line))
                    std::cout << "\t[" << line << "]" << std::endl;
                std::cout << "Reading stderr:" << std::endl;
                while (std::getline(p.getStderr(), line))
                    std::cout << "\t[" << line << "]" << std::endl;
            }
        }
    }
}