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

    std::cout << "Type any letter within 10 seconds\nSpecial: 1-start, 2-stop, 3-reset, 0-exit\n";

    sklib::timer_stopwatch_t check(sklib::time_seconds(10));
    sklib::timer_stopwatch_t global;

    sklib::timer_stopwatch_t something_else(300_ms);
    auto more_something = sklib::timer_stopwatch_t(5.3);

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
            std::cout << "Timeout; " << sklib::time_to_seconds<double>(global) << "(" << sklib::time_to_seconds<double>(check.read()) << ") " << "\n";
            break;
        }
#endif

        if (_kbhit())
        {
            unsigned char c = _getch();
            if (c >= ' ' && c < 0x7F)
            {
                std::cout << sklib::time_to_seconds<double>(global.read()) << "(" << sklib::time_to_seconds<double>(check) << ") " << c << "\n";

                if (c == '1') check.start();
                if (c == '2') check.stop();
                if (c == '3') check.reset();
                if (c == '0') break;
            }
        }
    }
}

