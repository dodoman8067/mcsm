#ifndef __MCSM_TEXT_COLOR_H__
#define __MCSM_TEXT_COLOR_H__

#include <iostream>
#include <mcsm/http/holder.h> // have to include this to avoid winsock issues in mingw

namespace mcsm {
    enum class TextColor {
        RESET,
        RED,
        DARK_RED,
        YELLOW,
        BRIGHT_YELLOW,
        GREEN,
        BLUE
    };
    
    #ifdef _WIN32
    inline void setcol(TextColor color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD attr = 0;
    
        switch (color) {
            case TextColor::RED:      attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case TextColor::DARK_RED:  attr = FOREGROUND_RED; break;
            case TextColor::YELLOW:   attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case TextColor::GREEN:    attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case TextColor::BRIGHT_YELLOW: attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case TextColor::BLUE:     attr = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case TextColor::RESET:    attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
        }
        SetConsoleTextAttribute(hConsole, attr);
    }
    #else
    inline void setcol(TextColor color) {
        const char* code = "";
        switch (color) {
            case TextColor::RED:     code = "\033[91m"; break;  // Bright red
            case TextColor::DARK_RED: code = "\033[31m"; break;  // Normal red
            case TextColor::YELLOW:  code = "\033[33m"; break;
            case TextColor::BRIGHT_YELLOW: code = "\033[93m"; break;
            case TextColor::GREEN:   code = "\033[32m"; break;
            case TextColor::BLUE:    code = "\033[34m"; break;
            case TextColor::RESET:   code = "\033[0m";  break;
        }
        std::cout << code;
    }
    #endif
}

#endif // __MCSM_TEXT_COLOR_H__