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

// ========================================================
// General description
//
// The Props library is designed to encode, store, transmit, and test multiple attributes
// (properties) in a single integer number. The value of each property is represented by a small 
// integer. (Analogy: C enum.) One property occupies a range of bits, and the ranges are written
// one after another in the data carrier type.
//
// The library provides functions to compute the bit packs, sizes, shifts automatically. The
// individual properties may be combined with othe properties, so the entire property set may be
// encoded in a C/C++ expression. The property value may come from the enum-like constant, or be
// loaded from a file.
//
// Rationale. Lets consider a function to open a serial port. (That was the origin of Props btw.)
// Serial I/O requires multiple independent parameters to be sent to the system: port number, baud
// rate, data bits, stop bits, flow control mode, and timeout. The port number is 8-bit integer,
// the commonly used different baud rates are quite few. The same is for the timeout figures.
// Instead of sending all the parameters separately, it is possible to pack them into one uint64_t.
//
// Different property sets may exist in the compilation unit in the same time. The Props library
// employs strong type check to prevent collisions from unrelated set. Also, a given property may
// be entered into the expression only once.
//
// To make the property set unique, the user code must declare the "anchor" - a dummy type that
// will be considered unique by C++ compiler. Then the anchor is added to the Props template
// thus creating the set of unique types.

// ========================================================
// First, basic example of Props declaration and structure

// The anchor
struct TableUniqueSpecifier {};

// The unique data type to carry packed data
// This type is also used in the user function that receives the property set
// All types used for property values are derived from this type
using TableProps = sklib::bit_props_data_type<TableUniqueSpecifier, int>;

// The family of helper types to compute propery placement and to make the value assignment
template<class Prev, auto enumCap>
using TableConfigElem = sklib::bit_props_config_type<TableProps, Prev, enumCap>;

// In the example below, the actual property values behave similar to C++ class enum elements,
// except that they can be added to each other (with restrictions mentioned above)
class Table
{
private:
    // the actual content has to come from somewhere
    enum { kitchen, office, dinner, night, workshop, picknick, table_cap }; // note that we use traditional C enums
    enum { round, rectangle, shape_cap };                                   // with Cap element (max+1)
    enum { black, white, grey, red, blue, green, color_cap };               // namespace pollution does not happen
    enum { stone, wood, steel, plastic, concrete, material_cap };           // because they are all private

public:
    // dedicated types for each specific property
    typedef TableConfigElem<void,        table_cap>    table_field;     // each group of properties occupy
    typedef TableConfigElem<table_field, shape_cap>    shape_field;     // the specific bit range computed
    typedef TableConfigElem<shape_field, color_cap>    color_field;     // at the compile time
    typedef TableConfigElem<color_field, material_cap> material_field;

    // the group of constants represent all possible values of the specific property
    static constexpr table_field Kitchen  {kitchen};
    static constexpr table_field Office   {office};   // the basic form of constants initialization
    static constexpr table_field Dinner   {dinner};   // the public variables work like elements of enum class
    static constexpr table_field Night    {night};
    static constexpr table_field Workshop {workshop};
    static constexpr table_field Picknick {picknick};

    static constexpr shape_field Round {round};         // items from different groups can add to each other
    static constexpr shape_field Rectangle {rectangle}; // just like traditional C bit property constants

    static constexpr auto Black = color_field{black};   // alternative way to declare member variables
    static constexpr auto White = color_field{white};
    static constexpr auto Grey  = color_field{grey};
    static constexpr auto Red   = color_field{red};
    static constexpr auto Blue  = color_field{blue};
    static constexpr auto Green = color_field{green};

    static constexpr auto Stone    = material_field{stone}.get();  // get() "reduces" the type and exposes
    static constexpr auto Wood     = material_field{wood}.get();   // the data mask in MSVC editor
    static constexpr auto Concrete = material_field{concrete}.get();
    static constexpr auto Plastic  = +material_field{plastic};     // operator+() has the same effect as .get()
    static constexpr auto Steel    = +material_field{steel}.get(); // also legal

    void Describe(TableProps what);     // function to handle the configuration
    // normally, loading, opening, or initialization of something, depending on the caller's request
    // see, for example, open() in STDC, or CreateFile() in WinAPI
    // unlike the functions above, the Props library allows to encode all necessary
    // bit fields and groups into the single integer value, likely, taking just one CPU register
};

void Table::Describe(TableProps what)
{
    std::string model, shape, color, material;

    if (what.has(Kitchen))  model = "Kitchen";   // function has() verifies that variable "what" in the bit range
    if (what.has(Office))   model = "Office";    // of table_field - which is "Kitchen", "Office", etc - have the
    if (what.has(Dinner))   model = "Dinner";    // value equal to the argument
    if (what.has(Night))    model = "Night";     // E.g. it compares "what" to "Kitchen" in the first line of the
    if (what.has(Workshop)) model = "Workshop";  // block, ignoring all bits unrelated to Kitchen data type
    if (what.has(Picknick)) model = "Picknick";

    if (what[Round])     shape = "round";        // operator[]() is the shortcut to .has()
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

// ========================================================
// Second, to demonstrate the collision avoidance
// and a useful case of defining the prohibited combinations 

struct fruit_anchor {};
using fruit_props_data = sklib::bit_props_data_type<fruit_anchor, int>;
template<class Prev, auto enumCap> using fruit_props_elem = sklib::bit_props_config_type<fruit_props_data, Prev, enumCap>;

class FruitProps
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
    static constexpr auto Spicy = fruit_taste{spicy};
    static constexpr auto Sour = +fruit_taste{sour};   // unary + reduces the type

    static constexpr fruit_shape Sphere  {sphere};
    static constexpr fruit_shape Cube  {cube};
    static constexpr fruit_shape Pyramid  {pyramid};
};

// ========================================================
// Use scenarios

int main()
{
    Table().Describe(Table::Picknick + Table::Stone + Table::Red +Table::Round);

    Table AAA;
    const int xx = 1<< 3;

    const auto d7 = AAA.Concrete.data;

    // AAA.Black.data = 42;  // error, is it const

    std::cout << "Hello World!\n";

    constexpr auto VV = FruitProps::Red + FruitProps::Cube;
    // constexpr auto VV1 = FruitProps::Red + Table::Wood;       // error, categories don't match
    // constexpr auto VV2 = FruitProps::Red + FruitProps::Blue;  // error, only one subcategory can be added

    constexpr auto VV3 = +FruitProps::Pyramid;
}

