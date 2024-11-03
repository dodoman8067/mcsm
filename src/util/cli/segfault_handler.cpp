#include <mcsm/util/cli/segfault_handler.h>

void mcsm::segfault_handler::handle(int signal){
    std::cerr << termcolor::red << "[mcsm/FATAL] Segmentation fault detected." << termcolor::reset << "\n";
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