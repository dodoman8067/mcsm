#include <mcsm/util/cli/signal_handler.h>
#include <mcsm/command/command_manager.h>

void mcsm::signal_handler::handle(int signal){
    mcsm::setcol(mcsm::NamedColor::DARK_RED);
    switch (signal){
        case SIGSEGV:
            std::cerr << "[mcsm/FATAL] Segmentation fault (SIGSEGV) encountered." << "\n";
            break;
        case SIGFPE:
            std::cerr << "[mcsm/FATAL] Floating point exception (SIGFPE) encountered." << std::endl;
            break;
        case SIGILL:
            std::cerr << "[mcsm/FATAL] Illegal instruction (SIGILL) encountered." << std::endl;
            break;
        #ifdef SIGBUS
            case SIGBUS:
                std::cerr << "[mcsm/FATAL] Bus error (SIGBUS) encountered." << std::endl;
                break;
        #endif
        case SIGTERM:
            mcsm::CommandManager::cleanup();
            mcsm::warning("SIGTERM detected. Cleaning up..");
            mcsm::curl_holder::cleanup();
            curl_global_cleanup();
            std::exit(signal);
            break;
        case SIGINT:
            mcsm::CommandManager::cleanup();
            mcsm::curl_holder::cleanup();
            curl_global_cleanup();
            std::exit(signal);
            break;
    }
    std::cerr << "[mcsm/FATAL] Signal: " << signal << "\n";
    std::cerr << "[mcsm/FATAL] Please report this to my Github (https://github.com/dodoman8067/mcsm) and explain how you encountered this error.\n";
    mcsm::resetcol();

    #ifdef __linux__
        mcsm::setcol(mcsm::NamedColor::DARK_RED);
        std::cerr << "[mcsm/FATAL] POSIX detected. Will try to print stacktrace.\n";
        mcsm::resetcol();

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
    mcsm::setcol(mcsm::NamedColor::DARK_RED);
    std::cerr << "[mcsm/FATAL] Memory allocation failed.\n";
    std::cerr << "[mcsm/FATAL] Make sure you have enough memory to have this program running.\n";
    std::cerr << "[mcsm/FATAL] Please report this to my Github (https://github.com/dodoman8067/mcsm) if you believe this is an error.\n";
    mcsm::resetcol();
    std::abort();
}