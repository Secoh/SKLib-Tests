// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     stream_tcpip_type
//
// Written by Secoh, 2021 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include <iostream>
#include <conio.h>

#include <SKLib/include/comms.hpp>

#define TESTPORT 13579
#define ESC 0x1B
#define TAB 0x09

void print_char(int c, bool& unfinished_line)
{
    if (c < ' ') c = '\n';
    unfinished_line = (c != '\n');
    std::cout << (char)c;
}

void print_message(const char *str, bool& unfinished_line)
{
    if (unfinished_line) std::cout << '\n';
    while (*str) print_char(*str++, unfinished_line);
    print_char('\n', unfinished_line);
}

// linkage test - lets step on Windows/Berkeley sockets name
void htons(unsigned short t)
{
    std::cout << t;
}

// linkage test - lets step on Windows/Berkeley sockets name
int send(unsigned long long a, const char* b, int c, int d)
{
    std::cout << a << b << c << d;
    return c + d;
}

int main(int argn, char *argc[])
{
    std::cout << "Usage: test-comms-sockets.exe [server]\n"
                 "Run one server and one client (no params) on same computer.\n"
                 "Type in one window - your input is printed in another one.\n"
                 "Client can connect and disconnect at any time.\n"
                 "Press ESC to exit.\n\n";

    bool use_server_mode = (argn > 1);
    std::cout << (use_server_mode ? "Server" : "Client") << " mode.\n";

    sklib::stream_tcpip_type IO(use_server_mode, TESTPORT);

    bool unfinished_line = false;
    bool connected = false;

    while (true)
    {
        bool stream_connect = IO.is_connected();
        if (!!connected != !!stream_connect)
        {
            connected = stream_connect;
            print_message((connected?"Connected.":"Disconnected."), unfinished_line);
            std::cout << "Code=" << IO.get_last_system_code() << "\n";
        }

        if (IO.is_error_state())
        {
            print_message("Error state.", unfinished_line);
            std::cout << "Code=" << IO.get_last_system_code() << "\n";
            break;
        }

        if (_kbhit())
        {
            int c = _getch();
            if (c == ESC) break;

            // linkage test - use the functions so compiler doesn't "optimize" it away
            if (c == TAB)
            {
                htons(0);
                send(0, "0", 0, 0);
            }

            if (IO.can_write()) IO.write((uint8_t)c);
        }

        if (IO.can_read())
        {
            uint8_t c = 0;
            IO.read(&c);
            print_char(c, unfinished_line);
        }
    }

    print_message("Exit.", unfinished_line);
    return 0;
}

