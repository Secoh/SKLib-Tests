// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     DLL import/export, shared DLL header
//
// Written by Secoh, 2022 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include <iostream>

#include "../test-dll-library/test-dll-library.h"

int main()
{
    dll_sample_type EX("test-dll-library.dll");

//    EX->fcoco = 7.77;
//    decltype(EX.add)::dll_function_entry::ptr_func_type
//    example_type::sklib_internal_dll_interface_type::

    std::cout << "ok? " << EX().is_dll_usable() << "\n";

    std::cout << EX.add(2, 3) << " " << EX.coco() << "\n";

    EX.iTest() = 7;
    EX.iTest() += 3;
    auto aa = EX.iTest();
}


