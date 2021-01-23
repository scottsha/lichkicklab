#include <stdio.h>
#include <stdint.h>

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


void print_unit_bits(unsigned int x, int start=31, int end=0) {
    for (auto foo=start; foo>=end; foo--){
        bool bit_foo = get_bit(x,foo);
        printf("%s", bit_foo ? "1" : "0");
        if (foo==31) printf(" ");
        if (foo==23) printf(" ");
    }
}

int get_float_bits_exp(float_bits x)
{
    unsigned int bit_delta = 1 << 23;

//    unsigned int
//    int exp=0;
//    int two_pow = 1;
//    for (auto foo = 30; foo < 23, foo++){
//        static_cast<int8_t>()
//    }
}

void pretty_print_float_bits(float f) {
    //[sign bit | 8 exponents | 23 mantissa]
    float_bits fbit;
    fbit.f = f;
    //
    unsigned int bit_delta = 1 << 31;
    bool sign_bit = fbit.bits & bit_delta;
    unsigned int bit_delta2 = 255 << 24;
    int expon = fbit.bits & bit_delta;
    printf("%s", sign_bit ? "-" : "");
//    printf("1.");
    print_unit_bits(fbit.bits);
    printf("\n");
}

int main()
{
//    a single sign bit, eight exponent bits, and 23 mantissa bits
    pretty_print_float_bits(0);
    pretty_print_float_bits(1);
    pretty_print_float_bits(0.1111);

}