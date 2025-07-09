// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     bitwise::rank
//     bitwise::flip
//     bitwise::distance
//
// Written by Secoh, 2021 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#define _USE_MATH_DEFINES

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <SKLib/sklib.hpp>

template<class T>
bool do_test(T A)
{
    // flip

    auto v1f = sklib::bits_flip<T>(A);
    auto a1f = sklib::bits_flip<T>(v1f);
    auto v2f = sklib::aux::bits_flip_bruteforce<T>(A);

    if (a1f != A)
    {
        std::cout << "Double flip() mismatch: " << (uint64_t)A << " -> " << (uint64_t)v1f << " -> " << (uint64_t)a1f << "\n";
        return false;
    }

    if (v1f != v2f)
    {
        std::cout << "Mismatch flip() algorithms: Input=" << (uint64_t)A << ", Fast=" << (uint64_t)v1f << ", Bruteforce=" << (uint64_t)v2f << "\n";
        return false;
    }

    // rank

    unsigned v1r = sklib::bits_rank<T>(A);
    unsigned v2r = (T)sklib::aux::bits_rank_bruteforce(A);

    if (v1r != v2r)
    {
        std::cout << "Mismatch rank() algorithms: Input=" << (uint64_t)A << ", Fast=" << (uint64_t)v1r << ", Bruteforce=" << (uint64_t)v2r << "\n";
        return false;
    }

    if (sizeof(T) == sizeof(uint8_t))
    {
        auto v3r = sklib::aux::bits_rank8_fork((uint8_t)A);
        if (v1r != v3r)
        {
            std::cout << "Mismatch rank() algorithms: Input=" << (uint64_t)A << ", Common=" << (uint64_t)v1r << ", Fork=" << (uint64_t)v3r << "\n";
            return false;
        }
    }

    // distance

    unsigned v1d = sklib::bits_distance<T>(A);
    unsigned v2d = sklib::aux::bits_distance_bruteforce<T>(A);

    if (v1d != v2d)
    {
        std::cout << "Mismatch distance() algorithms: Input=" << (uint64_t)A << ", Fast=" << (uint64_t)v1d << ", Bruteforce=" << (uint64_t)v2d << "\n";
        return false;
    }

    return true;
}

template<class HI, class LO>
HI compose(LO Prev)
{
    HI Rnd = 0;
    for (size_t k = 0; k < sizeof(LO); k++) Rnd = ((Rnd << 8) | (rand() & 0xFF));
    return ((Rnd << (8*(sizeof(HI)-sizeof(LO)))) | Prev);
}

uint8_t  erand8()  { return (uint8_t) floor(exp(rand() * (8*M_LN2/RAND_MAX))); }
uint16_t erand16() { return (uint16_t)floor(exp(rand() * (16*M_LN2/RAND_MAX))); }
uint32_t erand32() { return (uint32_t)floor(exp(rand() * (32*M_LN2/RAND_MAX))); }
uint64_t erand64() { return (uint64_t)floor(exp(rand() * (64*M_LN2/RAND_MAX))); }

// --------------------------------------------------------------------------
// Testing compile-time constants

template<class T>
struct exa_type : public sklib::aux::compound_integer_base<8 * sizeof(T), true>
{
    T data;
    bool sign;
};

constexpr int exa_1 = sklib::is_signed_v<exa_type<unsigned>>;
constexpr int exa_2 = sklib::is_integer_v<exa_type<unsigned>>;
constexpr int exa_3 = sklib::is_signed_v<unsigned>;
constexpr int exa_4 = sklib::is_integer_v<unsigned>;
// constexpr int exa_5 = sklib::is_signed_v<void>;  // error

typedef sklib::make_unsigned_if_integer_type<int> tta_1;
typedef sklib::make_unsigned_if_integer_type<int8_t> tta_2;
typedef sklib::make_unsigned_if_integer_type<uint8_t> tta_3;
typedef sklib::make_unsigned_if_integer_type<int8_t*> tta_4;

constexpr auto exb_1 = sklib::bits_data_cap<int>(9);
constexpr auto exb_2 = sklib::bits_data_high_1<int>(9);
constexpr auto exb_3 = sklib::bits_data_mask<int>(9);
constexpr auto exb_4 = sklib::bits_data_mask<int>(16);
constexpr auto exb_5 = sklib::bits_data_mask<uint16_t>(16);
constexpr auto exb_6 = sklib::bits_data_mask<uint16_t>(3);
constexpr auto exb_7 = sklib::bits_data_mask<uint16_t>(0);

constexpr int exc_1 = sklib::bits_width_v<int>;
constexpr int exc_2 = sklib::bits_width_v<uint64_t>;
constexpr int exc_3 = sklib::bits_width_v<exa_type<uint16_t>>;
// const int exc_4 = sklib::bits_width_v<void>;  // error

constexpr auto exc_5 = sklib::bits_mask_v<int>;
constexpr auto exc_6 = sklib::bits_mask_v<uint64_t>;
constexpr auto exc_7 = sklib::bits_mask_v<int8_t>;
constexpr auto exc_8 = sklib::bits_high_1_v<int>;

constexpr auto exd_1 = sklib::bits_low_half_v<unsigned>;
constexpr auto exd_2 = sklib::bits_high_half_v<unsigned>;
// constexpr auto exd_3 = sklib::bits_low_half_v<int>;  // error

constexpr auto exe_1 = sklib::bits_data_cap_v<int, 9>;
constexpr auto exe_2 = sklib::bits_data_high_1_v<int, 9>;
constexpr auto exe_3 = sklib::bits_data_mask_v<int, 9>;
constexpr auto exe_4 = sklib::bits_data_mask_v<int, 16>;
constexpr auto exe_5 = sklib::bits_data_mask_v<int, 31>;
// constexpr auto exe_6 = sklib::bits_data_mask_v<int, 32>; // error
constexpr auto exe_7 = sklib::bits_data_mask_v<uint16_t, 17>;
constexpr auto exe_8 = sklib::bits_data_mask_v<uint16_t, 16>;
constexpr auto exe_9 = sklib::bits_data_mask_v<uint16_t, 3>;
constexpr auto exe_A = sklib::bits_data_mask_v<uint16_t, 0>;



// --------------------------------------------------------------------------

int main()
{
    time_t Tmoni = time(nullptr);
    srand((unsigned)Tmoni);

    std::cout << "Testing...\n";

    uint64_t count = 0;
    while (true)
    {
        if (!(count % 1000) && time(nullptr) != Tmoni)
        {
            Tmoni = time(nullptr);
            std::cout << " " << count << "\r" << std::flush;
        }
        count++;

        // straight

        uint8_t  A = compose<uint8_t,  uint8_t>(0);
        uint16_t B = compose<uint16_t, uint8_t>(A);
        uint32_t C = compose<uint32_t, uint16_t>(B);
        uint64_t D = compose<uint64_t, uint32_t>(C);

        // log scale

        auto A1 = erand8();
        auto B1 = erand16();
        auto C1 = erand32();
        auto D1 = erand64();

        //

        if (!do_test<uint8_t>(A)) break;
        if (!do_test<uint16_t>(B)) break;
        if (!do_test<uint32_t>(C)) break;
        if (!do_test<uint64_t>(D)) break;

        if (!do_test<uint8_t> (A1)) break;
        if (!do_test<uint16_t>(B1)) break;
        if (!do_test<uint32_t>(C1)) break;
        if (!do_test<uint64_t>(D1)) break;
    }

    return 0;
}

