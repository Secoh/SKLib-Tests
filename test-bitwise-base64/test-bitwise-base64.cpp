// test-bitwise-base64.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include <SKLib/SKLib.hpp>

class base64_t : public sklib::bitwise::bit_stream_base_t
{
private:
    std::vector<uint8_t> *data_ptr;     // for a while, lets do testing with simple stuff
    size_t k_data;
    sklib::bitwise::bit_cpack_t<6> d_tmp{ 0 };

public:
    base64_t(std::vector<uint8_t>& data) : data_ptr(&data), k_data(0) {}

    static const char EOL = '=';

    char get_encoded()     // assuming input is clear text (sequence of octets), do encode; return EOL when there is no more input
    {
        if (!check(d_tmp)) return EOL;
        operator>> (d_tmp);
        return (d_tmp.data < DictLen ? Dict[d_tmp.data] : EOL);
    }

    bool do_decode(char c)   // send Base64 letters to decoder, clear text is collected in *data_ptr; send EOL for end of stream
    {
        if (c == EOL)
        {
            flush();
            return true;
        }

        uint8_t uc = DictInv[(uint8_t)c];
        if (uc == MarkSpc) return true;
        if (uc >= DictLen) return false;

        d_tmp.data = uc;
        operator<< (d_tmp);

        return true;
    }

    void generate_inverse_table(uint8_t(&U)[sklib::bitwise::BYTE_ADDRESS_SPAN])
    {
        for (int k = 0; k <= ' '; k++) U[k] = MarkSpc;
        for (int k = ' ' + 1; k < sklib::bitwise::BYTE_ADDRESS_SPAN; k++) U[k] = MarkErr;
        for (size_t k = 0; k < DictLen; k++) U[Dict[k]] = (uint8_t)k;
    }

protected:
    virtual void push_byte(uint8_t d)
    {
        data_ptr->push_back(d);
    }

    virtual bool pop_byte(uint8_t& d)
    {
        if (k_data >= data_ptr->size()) return false;
        d = (*data_ptr)[k_data++];
        return true;
    }

private:
    static const int DataSizeBits = 6;
    static const size_t DictLen = (1 << DataSizeBits);
    static constexpr char Dict[DictLen+1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    // 6-bit word for an input letter, or MarkSpc for whitespace, or MarkErr for invalid data
    static constexpr uint8_t DictInv[sklib::bitwise::BYTE_ADDRESS_SPAN] = {
        0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
        0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
        0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
        0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
        0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    static const uint8_t MarkSpc = 0xF0;
    static const uint8_t MarkErr = 0xFF;
};

int wmain(int argn, wchar_t* argc[])
{
    if (argn < 3)
    {
        std::cout << "base64.exe [a]e|d|g string {string...}\n"
                     "  e - encode sequence of strings; d - decode; g - generate table\n"
                     "  specify letter \"a\" to force ASCII (default is Unicode)\n"
                     "  When no argument is given, will keep data stream in the mem\n";
        return -1;
    }

    bool force_ascii = (std::wstring(argc[1]).find(L'a') != std::wstring::npos);
    bool encode = (std::wstring(argc[1]).find(L'e') != std::wstring::npos);
    bool decode = (std::wstring(argc[1]).find(L'd') != std::wstring::npos);
    bool get_table = (std::wstring(argc[1]).find(L'g') != std::wstring::npos);

    if ((int)encode + (int)decode + (int)get_table != 1)
    {
        std::cout << "Specify either encode or decode (or generate table), but not combinations\n";
        return -1;
    }

    std::vector<uint8_t> clear_text;
    base64_t Proc(clear_text);

    if (get_table)
    {
        uint8_t TAB[sklib::bitwise::BYTE_ADDRESS_SPAN];

        Proc.generate_inverse_table(TAB);

        for (int k = 0, j = 0; j < 16; j++)
        {
            for (int i = 0; i < 16; i++) printf(" 0x%02X,", TAB[k++]);
            printf("\n");
        }

        return 0;
    }

    std::wstring wsinput;
    for (int k=2; k<argn; k++) wsinput += std::wstring(wsinput.length() ? L" " : L"") + argc[k];

    if (encode)
    {
        for (size_t i = 0; i < wsinput.length(); i++)
        {
            uint16_t data = (uint16_t)wsinput[i];
            if (!force_ascii) clear_text.push_back((uint8_t(data >> 8)));
            clear_text.push_back(uint8_t(data & 0xFF));
        }

        while (true)
        {
            char c = Proc.get_encoded();
            std::cout << c;
            if (c == Proc.EOL) break;
        }

        std::cout << "\n";
        return 0;
    }

    // if (decode)

    bool status = true;
    for (size_t i = 0; i < wsinput.length(); i++)
    {
        if (!Proc.do_decode(uint8_t(wsinput[i] & 0xFF))) status = false;
    }

    if (!status) std::cout << "Have decoding error(s)\n";

    if (!force_ascii && (clear_text.size() % 2)) clear_text.push_back(' ');

    for (size_t k = 0; k < clear_text.size(); k++)
    {
        uint16_t w = 0;
        if (!force_ascii) w = (clear_text[k++] << 8);
        w |= clear_text[k];
        std::wcout << (wchar_t)w;
    }

    std::wcout << L"\n";
    return 0;
}

