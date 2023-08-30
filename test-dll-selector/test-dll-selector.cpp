// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     DLL import/export, shared DLL header
//
// Written by Secoh, 2022 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

// For demonstration, let this unit be in Unicode mode (Windows only)

#include <iostream>
#include <string>

// Typical scenario: get the declaration of the DLL interface; create the single project-wide instance right away; load external file later
#include "../test-dll-library/test-dll-library.h"
dll_sample_type LIB;


int wmain(int argn, wchar_t* argc[])
{
    if (argn < 2)
    {
        std::wcout << L"Usage: Selector.exe <DLL_name.dll> [secondary_name.dll]\n";
        return 0;
    }

    if (argn > 2)
    {
        const auto* A = argc[2];
        while (*A) LIB->payload += (char)(*A & 0xFFU);
    }

    const auto& st = LIB(argc[1]);

    std::wcout << L"Loading DLL " << (st.is_dll_usable() ? L"success\n" : L"failed\n") <<
                  L"Loader status = 0x" << std::hex << std::uppercase << st.dll_load_status << L"\n"
                  L"OS system returns code = " << std::dec << st.sys_dll_error << L"\n";

    if (!st.is_dll_usable()) return -1;

    LIB->carry = 7;
    std::wcout << L"add(3,5), carry 7; result=" << LIB.add(3, 5) << "\n";

    LIB->carry = 1;
    std::wcout << L"sub(9,5), carry 1; result=" << LIB.sub(9, 5) << "\n";

    LIB.put(__FILE__);
    LIB->payload += " line " + std::to_string(__LINE__);
    std::cout << "I am " << LIB.get() << "\n";

    return 0;
}


