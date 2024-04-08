// Thie purpose of this software is to provide example how to use the sublibrary of SKLib:
//     bit config: unique, mutually exclusive int properties
//
// Written by Secoh, 2024 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include <iostream>

#define SKLIB_TARGET_TEST
#include "SKLib/sklib.hpp"

// the custom library declares the anchor type (key), unique template-less type to carry data
// and unique family of types to describe the bit pack

struct my_anchor {};
using my_props_data = sklib::bit_props_data_type<my_anchor, int>;
template<class Prev, auto enumCap> using my_props_elem = sklib::bit_props_config_type<my_props_data, Prev, enumCap>;

struct fruit_anchor {};
using fruit_props_data = sklib::bit_props_data_type<fruit_anchor, int>;
template<class Prev, auto enumCap> using fruit_props_elem = sklib::bit_props_config_type<fruit_props_data, Prev, enumCap>;

// the custom library declares constants that represent features;
// they are stored in the corresponding bit ranges
// the values and the bit ranges are calculated automatically

class my_collection
{
private:
    enum { baud_9k, baud_1k, baud_2k, baud_cap };
    enum { data_8, data_6, data_7, data_cap };
    enum { stop_1, stop_2, stop_cap };

public:
    typedef my_props_elem<void,       baud_cap> baud_field;
    typedef my_props_elem<baud_field, data_cap> data_field;
    typedef my_props_elem<data_field, stop_cap> stop_field;

    static constexpr baud_field Baud1200 {baud_1k};
    static constexpr baud_field Baud2400 {baud_2k};
    static constexpr baud_field Baud9600 {baud_9k};

    static constexpr data_field Data6 {data_6};
    static constexpr auto Data7 = +data_field{data_7};
    static constexpr data_field Data8 {data_8};

    static constexpr stop_field Stop1 {stop_1};
    static constexpr stop_field Stop2 {stop_2};
};

class fruit_collection
{
private:
    enum fruit { apple, orange, plum, peach, banana, lemon, fruit_cap };
    enum color { nocolor, red, yellow, green, blue, brown, color_cap };
    enum taste { notaste, sweet, salty, spicy, sour, taste_cap };
    enum shape { noshape, sphere, cube, pyramid, shape_cap };

public:
    typedef fruit_props_elem<void,        fruit_cap> what_fruit;
    typedef fruit_props_elem<what_fruit,  color_cap> fruit_color;
    typedef fruit_props_elem<fruit_color, taste_cap> fruit_taste;
    typedef fruit_props_elem<fruit_taste, shape_cap> fruit_shape;

    static constexpr what_fruit Apple  {apple};
    static constexpr auto Orange = what_fruit{orange} + fruit_shape{}; // example: prohibit Shape when Orange is used
    static constexpr what_fruit Peach  {peach};
    static constexpr what_fruit Banana {banana};
    static constexpr what_fruit Lemon  {lemon};

    static constexpr fruit_color Red  {red};
    static constexpr fruit_color Yellow  {yellow};
    static constexpr fruit_color Green  {green};
    static constexpr fruit_color Blue  {blue};
    static constexpr auto Brown = fruit_color{brown}.get();

    static constexpr fruit_taste Sweet  {sweet};
    static constexpr fruit_taste Salty  {salty};
    static constexpr fruit_taste Spicy  {spicy};
    static constexpr fruit_taste Sour  {sour};

    static constexpr fruit_shape Sphere  {sphere};
    static constexpr fruit_shape Cube  {cube};
    static constexpr fruit_shape Pyramid  {pyramid};

};


int main()
{
    my_collection AAA;
    const int xx = 1<< 3;
    //    props::stop_field::
    const auto d7 = AAA.Data7.data;

    std::cout << "Hello World!\n";

    constexpr auto VV = fruit_collection::Red + fruit_collection::Cube;
    // constexpr auto VV1 = fruit_collection::Red + my_collection::Stop2;    // error, categories don't match
    // constexpr auto VV2 = fruit_collection::Red + fruit_collection::Blue;  // error, only one subcategory can be added

    constexpr auto VV3 = +fruit_collection::Pyramid;

    // error    collection::Baud1200.data = 33;

}

