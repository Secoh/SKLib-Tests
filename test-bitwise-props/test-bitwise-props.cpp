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
#include <string>

#define SKLIB_TARGET_TEST
#include "SKLib/sklib.hpp"

// the custom library declares the anchor type (key), unique template-less type to carry data
// and unique family of types to describe the bit pack

struct TableUniqueSpecifier {};
using TableProps = sklib::bit_props_data_type<TableUniqueSpecifier, int>;
template<class Prev, auto enumCap> using TableConfigElem = sklib::bit_props_config_type<TableProps, Prev, enumCap>;

struct my_anchor {};
using my_props_data = sklib::bit_props_data_type<my_anchor, int>;
template<class Prev, auto enumCap> using my_props_elem = sklib::bit_props_config_type<my_props_data, Prev, enumCap>;

struct fruit_anchor {};
using fruit_props_data = sklib::bit_props_data_type<fruit_anchor, int>;
template<class Prev, auto enumCap> using fruit_props_elem = sklib::bit_props_config_type<fruit_props_data, Prev, enumCap>;

// the custom library declares constants that represent features;
// they are stored in the corresponding bit ranges
// the values and the bit ranges are calculated automatically

class Table     // int-like properties / config library is designed for use from inside a class
{               // that represents the collection of the features
private:
    enum { kitchen, office, dinner, night, workshop, picknick, table_cap }; // note that we use traditional C enums
    enum { round, rectangle, shape_cap };                                   // with Cap element (max+1)
    enum { black, white, grey, red, blue, green, color_cap };               // namespace pollution does not happen
    enum { stone, wood, steel, plastic, concrete, material_cap };           // because they are all private

public:
    typedef TableConfigElem<void,        table_cap>    table_field;     // each group of properties occupy
    typedef TableConfigElem<table_field, shape_cap>    shape_field;     // the specific bit range computed
    typedef TableConfigElem<shape_field, color_cap>    color_field;     // at the compile time
    typedef TableConfigElem<color_field, material_cap> material_field;

    static constexpr table_field Kitchen  {kitchen};    // the basic form of constants initialization
    static constexpr table_field Office   {office};     // the public variables work like elements of enum class
    static constexpr table_field Dinner   {dinner};
    static constexpr table_field Night    {night};
    static constexpr table_field Workshop {workshop};
    static constexpr table_field Picknick {picknick};

    static constexpr shape_field Round {round};         // items from different groups can add to each other
    static constexpr shape_field Rectangle {rectangle}; // just like traditional C bit property constants

    static constexpr color_field Black {black};
    static constexpr color_field White {white};
    static constexpr color_field Grey  {grey};
    static constexpr color_field Red   {red};
    static constexpr color_field Blue  {blue};
    static constexpr color_field Green {green};

    static constexpr material_field Stone    {stone};
    static constexpr material_field Wood     {wood};
    static constexpr material_field Steel    {steel};
    static constexpr material_field Plastic  {plastic};
    static constexpr material_field Concrete {concrete};

    void Describe(TableProps what);     // function to handle the configuration
    // normally, loading, opening, or initialization of something, depending on the caller's request
    // see, for example, open() in STDC, or CreateFile() in WinAPI
    // unlike the functions above, the Props library allows to encode all necessary
    // bit fields and groups into the single integer value, likely, taking just one CPU register
};

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
    enum color { nocolor, red, yellow, green, blue, brown, white, color_cap };
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
    static constexpr fruit_color White  {white};    // Fruit::White is different from Table::White both by type and by value

    static constexpr fruit_taste Sweet  {sweet};
    static constexpr fruit_taste Salty  {salty};
    static constexpr fruit_taste Spicy  {spicy};
    static constexpr fruit_taste Sour  {sour};

    static constexpr fruit_shape Sphere  {sphere};
    static constexpr fruit_shape Cube  {cube};
    static constexpr fruit_shape Pyramid  {pyramid};

};

void Table::Describe(TableProps what)
{
    std::string model, shape, color, material;

    if (what.has(Kitchen))  model = "Kitchen";
    if (what.has(Office))   model = "Office";
    if (what.has(Dinner))   model = "Dinner";
    if (what.has(Night))    model = "Night";
    if (what.has(Workshop)) model = "Workshop";
    if (what.has(Picknick)) model = "Picknick";

    if (what[Round])     shape = "round";
    if (what[Rectangle]) shape = "rectangular";

    if (what[Black]) color = "black";
    if (what[White]) color = "white";
    if (what[Grey])  color = "grey";
    if (what[Red])   color = "red";
    if (what[Blue])  color = "blue";
    if (what[Green]) color = "green";

    if (what[Stone])    material = "stone";
    if (what[Wood])     material = "wood";
    if (what[Steel])    material = "steel";
    if (what[Plastic])  material = "plastic";
    if (what[Concrete]) material = "concrete";

    std::cout << model << " " << color << " " << shape << " table made of " << material << "\n";
}

int main()
{
    Table().Describe(Table::Picknick + Table::Stone + Table::Red +Table::Round);

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

