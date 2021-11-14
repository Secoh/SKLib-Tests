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

    std::cout << "Timer 1 is counter, Timer 2 is timeout (10 seconds)\nCommands: 1-Start T1, 2-Stop T1, 3-Reset Both, 0-Exit\n";

    sklib::timer_stopwatch_type timeout(10_s);
    sklib::timer_stopwatch_type counter(sklib::timer_stopwatch_type::create_mode_idle);

    sklib::timer_stopwatch_type something_else(sklib::time_milliseconds(300));
    auto more_something = sklib::timer_stopwatch_type(5.3); /* ms */

    bool timeout_posted = false;

    while (true)
    {
        if (_kbhit())
        {
            unsigned char c = _getch();
            if (c >= ' ' && c < 0x7F)
            {
                std::cout << sklib::time_to_seconds<double>(counter.read()) << " / " << sklib::time_to_seconds<double>(timeout) << " / " << c << "\n";

                if (c == '1') counter.start();
                if (c == '2') counter.stop();
                if (c == '3') { counter.reset(); timeout.reset(); }
                if (c == '0') break;
            }

            timeout_posted = false; // refresh
        }

        if (!timeout_posted && timeout)
        {
            timeout_posted = true;
            std::cout << "Timeout detected: " << sklib::time_to_seconds<double>(counter) << " / " << sklib::time_to_seconds<double>(timeout.read()) << "\n";
        }
    }

    return 0;
}

