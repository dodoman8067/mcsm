#include <mcsm/util/cli/signal_handler.h>
#include <mcsm/command/command_manager.h>

void mcsm::signal_handler::handle(int signal){
    switch (signal){
        case SIGSEGV:
            std::cerr << termcolor::red << "[mcsm/FATAL] Segmentation fault (SIGSEGV) encountered." << termcolor::reset << "\n";
            break;
        case SIGFPE:
            std::cerr << termcolor::red << "[mcsm/FATAL] Floating point exception (SIGFPE) encountered." << termcolor::reset << std::endl;
            break;
        case SIGILL:
            std::cerr << termcolor::red << "[mcsm/FATAL] Illegal instruction (SIGILL) encountered." << termcolor::reset << std::endl;
            break;
        case SIGBUS:
            std::cerr << termcolor::red << "[mcsm/FATAL] Bus error (SIGBUS) encountered." << termcolor::reset << std::endl;
            break;
        case SIGTERM:
            mcsm::CommandManager::cleanup();
            mcsm::warning("SIGTERM detected. Cleaning up..");
            curl_global_cleanup();
            mcsm::curl_holder::cleanup();
            std::exit(signal);
            break;
        case SIGINT:
            mcsm::CommandManager::cleanup();
            curl_global_cleanup();
            mcsm::curl_holder::cleanup();
            std::exit(signal);
            break;
    }
    std::cerr << termcolor::red << "[mcsm/FATAL] Signal: " << signal << termcolor::reset << "\n";
    std::cerr << termcolor::red << "[mcsm/FATAL] Please report this to my Github (https://github.com/dodoman8067/mcsm) and explain how you encountered this error."<< termcolor::reset << "\n";

    #ifdef __linux__
        std::cerr << termcolor::red << "[mcsm/FATAL] POSIX detected. Will try to print stacktrace."<< termcolor::reset << "\n";
        std::vector<void*> array(50);
        size_t size = backtrace(array.data(), array.size());

        char** symbols = backtrace_symbols(array.data(), array.size()); // Resolve symbols

        for(size_t i = 0; i<size; i++){
            std::cerr << symbols[i] << std::endl; // Log each stack frame
        }

        std::free(symbols);
    #endif
    std::abort();
}

void mcsm::signal_handler::new_handle(){
    std::cerr << termcolor::red << "[mcsm/FATAL] Memory allocation failed." << termcolor::reset << "\n";
    std::cerr << termcolor::red << "[mcsm/FATAL] Make sure you have enough memory to have this program running." << termcolor::reset << "\n";
    std::cerr << termcolor::red << "[mcsm/FATAL] Please report this to my Github (https://github.com/dodoman8067/mcsm) if you believe this is an error."<< termcolor::reset << "\n";
    std::abort();
}