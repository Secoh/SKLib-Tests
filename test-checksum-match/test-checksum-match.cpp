// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     checksum::crc_type; crc_fixed_type
//
// Written by Secoh, 2021 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include <iostream>
#include <cstdlib>

#include <SKLib/sklib.hpp>

void test_fixed();
void test_variable();

int main(int argn, char* argc[])
{
    srand((unsigned)time(nullptr));

    if (argn < 2)
    {
        std::cout << "Using few standard CRC settings to compare in fixed, variable, and brute-force modes.\nUse any parameter to run test with random polynomials.\n";
        test_fixed();
    }
    else
    {
        test_variable();
    }

    return 0;
}

template<class what_crc>
bool compare_fixed(std::string& str)    // expecting only fixed LSB input types
{
    what_crc fix1L;
    what_crc fix2L;
    using ctype = typename what_crc::type;
    constexpr unsigned len = fix1L.Polynomial_Degree;
    constexpr ctype poly = ::sklib::supplement::bits_flip_bruteforce<ctype>(fix1L.Polynomial, len);  // LSB is reversed
    sklib::crc_type<ctype> vari1L(len, poly);
    sklib::crc_type<ctype> vari2L(len, poly);

    sklib::crc_fixed_type<ctype, len, poly, true> fix1H;
    sklib::crc_fixed_type<ctype, len, poly, true> fix2H;
    sklib::crc_type<ctype> vari1H(len, poly, true);
    sklib::crc_type<ctype> vari2H(len, poly, true);

    auto CTLF = fix1L.update(str.c_str());
    auto CBLF = fix2L.update_bruteforce(str.c_str());
    auto CTHF = fix1H.update(str.c_str());
    auto CBHF = fix2H.update_bruteforce(str.c_str());

    auto CTLV = vari1L.update(str.c_str());
    auto CBLV = vari2L.update_bruteforce(str.c_str());
    auto CTHV = vari1H.update(str.c_str());
    auto CBHV = vari2H.update_bruteforce(str.c_str());

    // L

    if (CTLF != CBLF)
    {
        std::cout << "Fixed CRC mismatch: table vs. brute-force\nLSB; L=" << len << "; poly=" << std::hex << (uint64_t)poly << "\n";
        return false;
    }
    if (CTLV != CBLV)
    {
        std::cout << "Variable CRC mismatch: table vs. brute-force\nLSB; L=" << len << "; poly=" << std::hex << (uint64_t)poly << "\n";
        return false;
    }
    if (CTLF != CTLV)
    {
        std::cout << "Fixed vs. variable CRC mismatch\nLSB; L=" << len << "; poly=" << std::hex << (uint64_t)poly << "\n";
        return false;
    }

    // H

    if (CTHF != CBHF)
    {
        std::cout << "Fixed CRC mismatch: table vs. brute-force\nMSB; L=" << len << "; normal_poly=" << std::hex << (uint64_t)poly << "\n";
        return false;
    }
    if (CTHV != CBHV)
    {
        std::cout << "Variable CRC mismatch: table vs. brute-force\nMSB; L=" << len << "; normal_poly=" << std::hex << (uint64_t)poly << "\n";
        return false;
    }
    if (CTHF != CTHV)
    {
        std::cout << "Fixed vs. variable CRC mismatch\nMSB; L=" << len << "; normal_poly=" << std::hex << (uint64_t)poly << "\n";
        return false;
    }

    return true;
}

std::string random_str()
{
    std::string str = "";
    int L = rand() % 1024;
    for (int i = 0; i < L; i++) str += (char)('A' + rand() % ('Z' - 'A' + 1));
    return str;
}

void test_fixed()
{
    std::cout << "Testing...\n";
    int N = 0;

    while (true)
    {
        if (++N % 256 == 1)
        {
            std::cout << N << "\r" << std::flush;
        }

        std::string str = random_str();

        if (!compare_fixed<sklib::crc_1_parity>(str)) break;
        if (!compare_fixed<sklib::crc_3_gsm>(str)) break;
        if (!compare_fixed<sklib::crc_7_sd_lsb>(str)) break;
        if (!compare_fixed<sklib::crc_8_ccitt>(str)) break;
        if (!compare_fixed<sklib::crc_14_darc>(str)) break;
        if (!compare_fixed<sklib::crc_16_ccitt>(str)) break;
        if (!compare_fixed<sklib::crc_21_can>(str)) break;
        if (!compare_fixed<sklib::crc_32_iso>(str)) break;
        if (!compare_fixed<sklib::crc_32C_lsb>(str)) break;
        if (!compare_fixed<sklib::crc_64_ecma>(str)) break;
        if (!compare_fixed<sklib::crc_64_iso>(str)) break;
    }
}

template<class T, int N>
bool compare_variable(unsigned L)
{
    std::string str = random_str();

    T poly = 0;
    for (int i = 0; i < N; i++) poly = ((poly << 8) | (rand() & 0xFF));
    poly |= 1;
    poly &= ::sklib::supplement::bits_short_data_mask<T>(L);

    sklib::crc_type<T> vari1L(L, poly);
    sklib::crc_type<T> vari2L(L, poly);
    sklib::crc_type<T> vari1H(L, poly, true);
    sklib::crc_type<T> vari2H(L, poly, true);

    auto CTLV = vari1L.update(str.c_str());
    auto CBLV = vari2L.update_bruteforce(str.c_str());
    auto CTHV = vari1H.update(str.c_str());
    auto CBHV = vari2H.update_bruteforce(str.c_str());

    if (CTLV != CBLV)
    {
        std::cout << "Variable CRC mismatch: table vs. brute-force\nLSB; N=" << N << "; L = " << L << "; poly = " << std::hex << (uint64_t)poly << "\n";
        return false;
    }
    if (CTHV != CBHV)
    {
        std::cout << "Variable CRC mismatch: table vs. brute-force\nMSB; N=" << N << "; L=" << L << "; normal_poly=" << std::hex << (uint64_t)poly << "\n";
        return false;
    }

    return true;
}

void test_variable()
{
    std::cout << "Testing...\n";
    int N = 0;

    while (true)
    {
        if (++N % 256 == 1)
        {
            std::cout << N << "\r" << std::flush;
        }

        int L = 1 + rand() % 64;
        int min_bytes = (L + 7) / 8;
        int nbytes = min_bytes + rand() % (9-min_bytes);

        if (nbytes < 1 || nbytes > 8 || L > nbytes * 8)
        {
            std::cout << "Internal error with L_bits=" << L << "; N_bytes=" << nbytes << "\n";
            return;
        }

        if (nbytes > 4) nbytes = 8;
        else if (nbytes > 2) nbytes = 4;

        switch (nbytes)
        {
        case 1: if (!compare_variable<uint8_t, 1>(L)) return;
                break;
        case 2: if (!compare_variable<uint16_t, 2>(L)) return;
                break;
        case 4: if (!compare_variable<uint32_t, 4>(L)) return;
                break;
        case 8: if (!compare_variable<uint64_t, 8>(L)) return;
                break;
        default:
            {
                std::cout << "This cannot happen: L_bits=" << L << "; N_bytes=" << nbytes << "\n";
                return;
            }
        }
    }
}

