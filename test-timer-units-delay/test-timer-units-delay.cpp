// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     time_wait
//     timer: chrono_literals, units, conversions
// 
// Written by Secoh, 2021 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include <iostream>
#include <cmath>

#include <SKLib/timer.hpp>

void example_with_no_namespace_pollution()
{
    sklib::time_wait(0.01_minutes_sklib);       // here we call it without "using namespace"
}

int main()
{
    using namespace std::chrono_literals;
    using namespace sklib;

    std::cout << "1\n";

    example_with_no_namespace_pollution();

    std::cout << "2\n";

    time_wait(3ms);     // unit comes from std::chrono_literals

    std::cout << "3\n";

    {
        using namespace sklib::chrono_literals;     // this is for completeness
        time_wait(1.3_seconds);
    }

    std::cout << "4\n";

    time_wait(77); /* ms */   // immortal classic ;)

    std::cout << "5\n";

    constexpr auto xx = time_milliseconds(3.45);        // double 3.45 ms
    constexpr auto yy = time_to_microseconds<int>(xx);  // 3450
    constexpr auto zz = time_seconds(yy);               // int 3450 s

    auto T = 3;
    auto T1 = time_seconds(T);
    time_wait(T1);                  // conversion from integer expression
    auto T2 = time_microseconds(exp(T * 0.7));
    time_wait(T2);    // conversion from double expression

    constexpr auto TT = 50_ms_sklib;
    auto TF = 100.5_ms_sklib;
    auto TM = 60_minutes_sklib;

    // error: std::cout << time_to_seconds(TT);
    std::cout << time_to_seconds<double>(TT) << " " << time_to_seconds<double>(TF) << " " << time_to_seconds<int>(TM) << "\n";  // correct

    return 0;
}

