// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     timer::stopwatch
//
// Written by Secoh, 2021 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include <iostream>
#include <conio.h>      //sk!! need kbhit() for Linux; see: https://www.flipcode.com/archives/_kbhit_for_Linux.shtml

#include <SKLib/SKLib.hpp>

int main()
{
    using namespace sklib::chrono_literals;

    std::cout << "Type any letter within 10 seconds\nSpecial: 1-start, 2-stop, 0-exit\n";

    sklib::stopwatch_t check(10);
    sklib::stopwatch_t global;

    //sk later sklib::stopwatch_t<false> something_else(300_ms);
    auto more_something = sklib::stopwatch_t();

    while (true)
    {
#ifdef _SKLIB_TIMEWATCH_ALLOW_EXPLICIT_CAST
        if (check)
        {
            std::cout << "Timeout; " << double(global) << "(" << check.read() << ") " << "\n";
            break;
        }
#else
        if (check.is_timeout())
        {
            std::cout << "Timeout; " << global.read() << "(" << check.read() << ") " << "\n";
            break;
        }
#endif

        if (_kbhit())
        {
            unsigned char c = _getch();
            if (c >= ' ' && c < 0x7F)
            {
                std::cout << global.read() << "(" << check.read() << ") " << c << "\n";

                if (c == '1') check.start();
                if (c == '2') check.stop();
                if (c == '0') break;
            }
        }
    }
}

