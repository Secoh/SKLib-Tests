// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     bitwise::bit_stream
//     bitwise::bit_file
//
// Written by Secoh, 2021
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

static constexpr auto BYTE_BITS = sklib::bitwise::BYTE_BITS;

uint64_t clean_int(uint8_t Nbit, uint64_t data)
{
    if (!Nbit) return 0;
    if (Nbit >= sklib::bitwise::INT64_BITS) return ~0ull;
    return (data & ((1ull << Nbit) - 1));
}

std::pair<uint8_t, uint64_t> generate_node()
{
    uint8_t N = (rand() % sklib::bitwise::INT64_BITS) + 1;
    size_t K = (N + BYTE_BITS - 1) % BYTE_BITS;

    uint64_t V = 0;
    for (size_t t=0; t<K; t++) V = ((V << BYTE_BITS) | (rand() & sklib::bitwise::BYTE_MASK));

    return { N, clean_int(N, V) };
}

class my_byte_stream : public sklib::bitwise::bit_stream_base_t
{
public:
    size_t Rpos = 0;
    std::vector<uint8_t> Record;

    void hook_after_reset()
    {
        Rpos = 0;
        Record.clear();
    }

    void push_byte(uint8_t data)
    {
        Record.push_back(data);
    }

    bool pop_byte(uint8_t& data)
    {
        auto valid = (Rpos < Record.size());
        data = (valid ? Record[Rpos++] : 0);
        return valid;
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
    sklib::bitwise::bit_stream_base_t *PACKED = &PACKED_IN_MEMORY;

    // enable disk file test, if an argument is given
    if (argn >= 2) PACKED = new sklib::bitwise::bit_file_t(argc[1]);

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
            *PACKED << sklib::bitwise::bit_pack_t{ node.first, node.second };
        }

        PACKED->flush();

        // ...read and verify

        PACKED->rewind();

        for (size_t k = 0; k < DLEN; k++)
        {
            sklib::bitwise::bit_pack_t node{ DATA[k].first, 0 };
            *PACKED >> node;
            if (node.data != DATA[k].second)
            {
                std::cout << "\nFailed\n";
                return 1;
            }
        }
    }

    return 0;
}

