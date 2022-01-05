// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     helpers::callback_type
//
// Written by Secoh, 2021 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include <iostream>
#include <cstdlib>

#include <SKLib/helpers.hpp>

double test_bare(int x) { return x + 0.1; }

class Foundation
{
public:
    /* 1 */
    sklib::internal::callback_type<Foundation, double, int> cbk_bare{ test_bare };

    /* 2.1 - init with pointer */
    static double test_pvoid(void* param, int x) { return *(double*)param + x; }
    sklib::internal::callback_type<Foundation, double, int> cbk_multi;
    Foundation(double& param) : cbk_multi(test_pvoid, &param) {}
    /* 2.2 - init with external function */
    Foundation(double (*f)(int)) : cbk_multi(f) {}

    /* 3 */
    int Y=0;
    static double test_inclass(Foundation* cptr, int x) { return cptr->Y + x; }
    sklib::internal::callback_type<Foundation, double, int> cbk_inclass{ test_inclass , this };

    // this function demonstrates various callback modes
    void perform(int v = 10)
    {
        std::cout << "Test\n";
        std::cout << "Global: " << cbk_bare.call(v-3) << "\n";
        std::cout << "C-style: " << cbk_multi.call(v) << "\n";
        std::cout << "Quasi-member: " << cbk_inclass.call(10*v) << "\n";
    }
};

int main()
{
    double pi = 3.1416;
    Foundation TEST(pi);

    TEST.Y = 33;
    TEST.perform();

    /* 4 - standalone */
    sklib::internal::callback_type<int, double, int> freestanding(test_bare);   // <-- hack: can use any non-void type as a class type if we don't use it
    std::cout << "Test Freestanding Bare " << freestanding(33) << "is_valid?" << (bool)freestanding << "\n";

    std::cout << "Retest with Random\n";    // here we can monitor what happens in ASM translation
    srand((unsigned)time(nullptr));
    TEST.perform(rand());

    /* exploit callback "polymorphism" - 2.2 */
    Foundation T2(test_bare);                     // <-- compare with line above: Foundation TEST(pi);
    T2.Y = 33;
    std::cout << "Run again with callback switch\n";
    T2.perform();
}
