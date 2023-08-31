// This file is part of SKLib: https://github.com/Secoh/SKLib
// Copyright [2020-2022] Secoh
//
// Licensed under the GNU Lesser General Public License, Version 2.1 or later. See: https://www.gnu.org/licenses/
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// Special exception from GNU LGPL terms: you don't have to publish the compiled object binary file(s) for SKLib.
// Modified source code and/or any derivative work requirements are still in effect. All such file(s) must be openly
// published under the same terms as the original one(s), but you don't have to inherit the special exception above.
//

#include <iostream>

#include <Sklib/include/string.hpp>


//sk! add #error if compiled not in Windows



// one way: until reset, either encoding or decoding, not mixed
// put returns true, taken; false, NOT taken and should be sent again later
// get returns true if encoded is ready, false if no data returned, unchanged
// guarantee: assume calling get() in loop while true, then after get retunrs false, put is guaranteed to succeed


class utf16_unicode
{
private:
    // the bit outside Unicode range, to indicate internally that transfer register is loaded
    static const uint32_t occupied_flag = 0x10000000u;

    // Unicode to UTF-16
    uint32_t carry_unicode = 0;

public:
    void reset()
    {
        carry_unicode = 0;
    }

    // entering UTF-16:
    // if BMP letter is coming (not surrogate) - copy letter
    // if high surrogate, copy, wait for low surrogate, interpret

    bool put_utf16(uint16_t ch)
    {
        if (carry_unicode & occupied_flag)
        {
            // only makes sense if Surrogate is entering
            // High is already there, Low is coming
            // error if the pattern is not satisfied

            uint32_t content = (carry_unicode & ~occupied_flag);
            if (!::sklib::is_unicode_high_surrogate(content)) return false;

            if (::sklib::is_unicode_low_surrogate(ch))
            {
//              carry_unicode = (content - ::sklib::unicode::HIGH_SURROGATE) * 0x0400u + (ch - ::sklib::unicode::LOW_SURROGATE) + 0x10000 + occupied_flag;  // UTF-16 definition
//                carry_unicode = (content - ::sklib::unicode::HIGH_SURROGATE) * 0x0400u + ch + (0x10000 + occupied_flag - ::sklib::unicode::LOW_SURROGATE);  // lets rearrange a little
                carry_unicode = ::sklib::supplement::utf16_combine(content, ch) + occupied_flag;
            }
            else
            {
                // turn incomplete codepoint into an error, and keep incoming data for the next call
                carry_unicode = ::sklib::unicode::REPLACEMENT_MARK + occupied_flag;
                return false;
            }
        }
        else
        {
            // low surrogate cannot go first
            carry_unicode = (::sklib::is_unicode_low_surrogate(ch) ? ::sklib::unicode::REPLACEMENT_MARK : ch) + occupied_flag;
        }

        return true;
    }

    bool get_unicode(uint32_t& ch)
    {
        if (!(carry_unicode & occupied_flag)) return false;
        uint32_t data = (carry_unicode & ~occupied_flag);
        if (::sklib::is_unicode_surrogate(data)) return false;
        ch = data;
        carry_unicode = 0;
        return true;
    }

    // reading UTF-16 from Unicode input
    // have letter in carry register, verify not surrogate
    // reading, if outside BMP - report high surrogate, form low surrogate (becoming BMP)
    // reading, if BMP - report contents

    bool put_unicode(uint32_t ch)
    {
        if (carry_unicode & occupied_flag) return false;
        bool valid = (::sklib::is_unicode(ch) && !::sklib::is_unicode_surrogate(ch));
        carry_unicode = (valid ? ch : ::sklib::unicode::REPLACEMENT_MARK) + occupied_flag;
        return true;
    }

    bool get_utf16(uint16_t& ch)
    {
        if (!(carry_unicode & occupied_flag)) return false;

        uint32_t data = (carry_unicode & ~occupied_flag);
        if (::sklib::is_unicode_bmp(data))
        {
            ch = data;          // that includes leftover low surrogate, if is was pair
            carry_unicode = 0;
            return true;
        }

        auto utf16_pair = ::sklib::supplement::utf16_split(data);
        carry_unicode = utf16_pair.second + occupied_flag;
        ch = utf16_pair.first;
        return true;
    }
};








// Windows-specific
// must be Unicode mode
//
int wmain(int argn, wchar_t* wargc[])
{
    std::cout << "Hello World!\n";
    std::cout << argn << "\n----\n";

    if (argn <= 1) return 0;

    std::wcout << wargc[1];   //sk garbled?!
    std::cout << "\n";

    for (int k=0; wargc[1][k]; k++) std::cout << std::hex << (int)wargc[1][k] << "\n";

    utf16_unicode conv;
    uint32_t U = 0;
    for (int k = 0; wargc[1][k]; k++)
    {
        conv.put_utf16(wargc[1][k]);
        while (conv.get_unicode(U)) std::cout << std::hex << (int)U << "\n";
    }
    conv.put_utf16(0);
    while (conv.get_unicode(U)) std::cout << std::hex << (int)U << "\n";


    while (true) {} // hang
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
