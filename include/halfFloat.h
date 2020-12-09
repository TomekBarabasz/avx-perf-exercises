//
// Created by barabasz on 09.12.2020.
//

#ifndef PERF_TEST_HALFFLOAT_H
#define PERF_TEST_HALFFLOAT_H
namespace HalfFloat {
    union halfFloat {
        uint16_t bits;
        struct {
            uint16_t mantissa: 10;
            uint16_t exponent: 5;
            uint16_t sign: 1;
        } IEEE;
    };
    union IEEESingleFloat {
        uint32_t bits;
        float    fval;
        struct {
            uint32_t mantissa: 23;
            uint32_t exponent: 8;
            uint32_t sign: 1;
        } IEEE;
    };
    union IEEEDoubleFloat {
        uint64_t bits;
        double   dval;
        struct {
            uint64_t mantissa: 52;
            uint64_t exponent: 11;
            uint64_t sign: 1;
        } IEEE;
    };
    struct HalfFloat : public halfFloat
    {
        HalfFloat(){}
        HalfFloat(float v){
            IEEESingleFloat s;
            s.fval = v;
            IEEE.sign = s.sign;
        }
        HalfFloat(double){}
    };

    inline HalfFloat operator+ (HalfFloat one, HalfFloat two);
    inline HalfFloat operator- (HalfFloat one, HalfFloat two);
    inline HalfFloat operator* (HalfFloat one, HalfFloat two);
    inline HalfFloat operator/ (HalfFloat one, HalfFloat two);

    inline float operator+ (HalfFloat one, float two);
    inline float operator- (HalfFloat one, float two);
    inline float operator* (HalfFloat one, float two);
    inline float operator/ (HalfFloat one, float two);

    inline float operator+ (float one, HalfFloat two);
    inline float operator- (float one, HalfFloat two);
    inline float operator* (float one, HalfFloat two);
    inline float operator/ (float one, HalfFloat two);
}
#endif //PERF_TEST_HALFFLOAT_H
