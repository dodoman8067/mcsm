#ifndef __MCSM_TEXT_COLOR_H__
#define __MCSM_TEXT_COLOR_H__

#include <iostream>
#include <mcsm/http/holder.h> // have to include this to avoid winsock issues in mingw

namespace mcsm {
    struct Color {
        int r, g, b;
        Color(int r, int g, int b) : r(r), g(g), b(b) {}
    };

#ifdef _WIN32
    inline void enableANSI(){
        static bool enabled = false;
        if(!enabled){
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD mode = 0;
            GetConsoleMode(hOut, &mode);
            SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            enabled = true;
        }
    }
#else
    inline void enableANSI() {}
#endif

    inline void setcol(const Color& c){
        enableANSI();
        std::cout << "\033[38;2;" << c.r << ";" << c.g << ";" << c.b << "m";
    }

    inline void setbgcol(const Color& c){
        enableANSI();
        std::cout << "\033[48;2;" << c.r << ";" << c.g << ";" << c.b << "m";
    }

    inline void resetcol(){
        std::cout << "\033[0m";
    }

    namespace NamedColor {
        inline constexpr Color RED = Color(255, 0, 0);
        inline constexpr Color DARK_RED = Color(128, 0, 0);
        inline constexpr Color YELLOW = Color(255, 255, 0);
        inline constexpr Color BRIGHT_YELLOW = Color(255, 255, 100);
        inline constexpr Color GREEN = Color(0, 255, 0);
        inline constexpr Color BLUE = Color(0, 0, 255);
    }
}

#endif // __MCSM_TEXT_COLOR_H__