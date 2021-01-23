#include <stdio.h>
#include <stdint.h>

#include <string>
#include <iostream>
#include <fstream>

bool is_big_endian(void)
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

union float_bits {
    float f;
    //float 31 is sign
    //float 23 to 30 is exponent
    //float 0 to 22 is mantissa
    unsigned int bits;
};

bool get_bit(unsigned int x, int loc){
    unsigned int bit_delta = 1 << loc;
    bool bit = static_cast<bool>(x & bit_delta);
    return bit;
}

bool get_bit(float_bits fb, int loc){
    return get_bit(fb.bits,loc);
}

int get_exponent(float_bits fb){
    unsigned int bit_mask = 255<<23;
    unsigned int exp_bits = fb.bits & bit_mask;
    return static_cast<int>(exp_bits>>23)-127;
}


void print_unit_bits(unsigned int x) {
    for (auto foo=0; foo<32; foo++){
        bool bit_foo = get_bit(x,foo);
        printf("%s", bit_foo ? "1" : "0");
    }
}


void pretty_print_float_bits(float f) {
    //[sign bit | 8 exponents | 23 mantissa]
    float_bits fb;
    fb.f = f;
    printf("%s",  get_bit(fb,31) ? "-" : "");
    printf("%d", get_exponent(fb));
    printf("1.");
    for (int foo=22; foo>=0; foo--) {
        printf("%s",  get_bit(fb, foo) ? "1" : "0");
    }
    printf(" * 2^");
}

std::string prettystr_float_bits(float f) {
    //[sign bit | 8 exponents | 23 mantissa]
    float_bits fb;
    fb.f = f;
    std::string pretty = "";
    pretty += get_bit(fb,31) ? "-" : "";
    int exp = get_exponent(fb);
    if (exp==-127) {
        exp = -126;
        pretty += "0.";
    }
    else pretty += "1.";
    for (int foo=22; foo>=0; foo--) {
        pretty += get_bit(fb, foo) ? "1" : "0";
    }
    pretty += " * 2^";
    pretty += std::to_string(exp);
    return pretty;
}


int main( int argc, char *argv[] )  {
    if( argc != 3 ) {
        printf("Need an input file name and an output file name.\n");
    }
    std::ifstream fin(argv[1]);
    std::fstream fout(argv[2]);

    std::string line;
    std::getline(fin, line);
    int n_examples = stoi(line);

    for (int foo=0; foo<n_examples; foo++){
        std::getline(fin,line);
        float ff = std::stof(line);
        std::string pff = prettystr_float_bits(ff);
        fout << pff << std::endl;
    }

    fin.close();
    fout.close();

    return 0;
}