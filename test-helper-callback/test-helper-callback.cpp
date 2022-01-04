// test-helper.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

    /* 2 */
    static double test_pvoid(void* param, int x) { return *(double*)param + x; }
    sklib::internal::callback_type<Foundation, double, int> cbk_param;
    Foundation(double& param) : cbk_param(test_pvoid, &param) {}

    /* 3 */
    int Y=0;
    static double test_inclass(Foundation* cptr, int x) { return cptr->Y + x; }
    sklib::internal::callback_type<Foundation, double, int> cbk_inclass{ test_inclass , this };

    // this function demonstrates various callback modes
    void perform(int v = 10)
    {
        std::cout << "Test\n";
        std::cout << "Global: " << cbk_bare.call(v-3) << "\n";
        std::cout << "C-style: " << cbk_param.call(v) << "\n";
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
    std::cout << "Test Freestanding Bare " << freestanding.call(33) << "\n";

    std::cout << "Retest with Random\n";    // here we can monitor what happens in ASM translation
    srand((unsigned)time(nullptr));
    TEST.perform(rand());
}
