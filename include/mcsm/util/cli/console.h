#ifndef __MCSM_CONSOLE_H__
#define __MCSM_CONSOLE_H__

#include <iostream>
#include <string>
#include <chrono>
#include <atomic>
#include <mutex>

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

        virtual void start();
        virtual void stop();
        virtual void log(const std::string& message);
        virtual void input(const std::string& str);
    };
}

#endif // __MCSM_CONSOLE_H__