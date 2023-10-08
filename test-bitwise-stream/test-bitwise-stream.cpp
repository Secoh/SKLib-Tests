// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     bitwise::bit_stream
//     bitwise::bit_file
//
// Written by Secoh, 2021 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <vector>

#include <SKLib/SKLib.hpp>

uint64_t clean_int(uint8_t Nbit, uint64_t data)
{
    if (!Nbit) return 0;
    if (Nbit >= sklib::bits_width_v<uint64_t>) return ~0ull;
    return (data & ((1ull << Nbit) - 1));
}

// returns random pair { N bits, Data }
// where N is 1..64, Data is unsigned integer within N bits
std::pair<uint8_t, uint64_t> generate_node()
{
    uint8_t N = (rand() % sklib::bits_width_v<uint64_t>) + 1;
    size_t K = (N + sklib::OCTET_BITS - 1) % sklib::OCTET_BITS;

    uint64_t V = 0;
    for (size_t t=0; t<K; t++) V = ((V << sklib::OCTET_BITS) | (rand() & sklib::OCTET_MASK));

    return { N, clean_int(N, V) };
}

class my_byte_stream : public sklib::bits_stream_base_type
{
public:
    my_byte_stream() : bits_stream_base_type(pop_byte, push_byte) {}

    size_t Rpos = 0;
    std::vector<uint8_t> Record;

    void hook_after_reset_impl()
    {
        Rpos = 0;
        Record.clear();
    }

    static void hook_action(bits_stream_base_type* root, hook_type action)
    {
        if (action == hook_type::after_reset) static_cast<my_byte_stream*>(root)->hook_after_reset_impl();
    }

    static void push_byte(bits_stream_base_type* root, uint8_t data)
    {
        static_cast<my_byte_stream*>(root)->Record.push_back(data);
    }

    bool pop_byte_impl(uint8_t& data)
    {
        auto valid = (Rpos < Record.size());
        data = (valid ? Record[Rpos++] : 0);
        return valid;
    }

    static bool pop_byte(bits_stream_base_type* root, uint8_t& data)
    {
        return static_cast<my_byte_stream*>(root)->pop_byte_impl(data);
    }
};

int main(int argn, char *argc[])
{
    if (argn < 2)
    {
        std::cout << "Specify the disk file in the argument for the file-based stream storage\n"
                     "When no argument is given, will keep data stream in the memory\n";
    }

    const size_t MAXDATA = 1024;

    time_t Tmoni = time(nullptr);
    srand((unsigned)Tmoni);

    std::cout << "Testing...\n";

    std::vector<std::pair<uint8_t, uint64_t>> DATA;
    my_byte_stream PACKED_IN_MEMORY;
    sklib::bits_stream_base_type *PACKED = &PACKED_IN_MEMORY;

    // enable disk file test, if an argument is given
    if (argn >= 2) PACKED = new sklib::bits_file_type(argc[1]);

    uint64_t count = 0;
    while (true)
    {
        if (!(count % 100) && time(nullptr) != Tmoni)
        {
            Tmoni = time(nullptr);
            std::cout << " " << count << "\r" << std::flush;
        }
        count++;

        // write...

        size_t DLEN = (rand() % MAXDATA) + 1;
        DATA.clear();

        PACKED->reset();

        for (size_t k = 0; k < DLEN; k++)
        {
            auto node = generate_node();
            DATA.push_back(node);
            *PACKED << sklib::bits_pack(node.second, node.first);   // bit width, data
        }

        PACKED->write_flush();

        // ...read and verify

        PACKED->read_rewind();

        for (size_t k = 0; k < DLEN; k++)
        {
            auto node = sklib::bits_pack<uint64_t>(0, DATA[k].first);   // we read known count of bits; the received data word must match the log
            *PACKED >> node;                                            // important: we have to declare the data size specifically: uint64_t
            if (node.data != DATA[k].second)
            {
                std::cout << "\nFailed\n";
                return 1;
            }
        }
    }

    // no garbage collection in demo

    return 0;
}

