#ifndef __MCSM_CONSOLE_H__
#define __MCSM_CONSOLE_H__

#include <iostream>
#include <string>
#include <chrono>
#include <atomic>
#include <mutex>
#include <thread>

// todo: use cpp-terminal

namespace mcsm {
    class Console {
    private:
        std::atomic<bool> running;
        std::atomic<bool> promptDisplayed;
        std::mutex outputMutex;

        void displayPrompt();
        void handleInput(const std::string& input);
    public:
        Console();
        ~Console();

        void start();
        void stop();
        void log(const std::string& message);
        void input(const std::string& str);

        void randomLogging();
    };
}

#endif // __MCSM_CONSOLE_H__