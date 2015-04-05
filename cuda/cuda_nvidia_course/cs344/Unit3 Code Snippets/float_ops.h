#ifndef FLOAT_OPS_H_
#define FLOAT_OPS_H_
 
 
// Float comparison http://floating-point-gui.de/errors/comparison/ - Java sample
// http://www.parashift.com/c++-faq/floating-point-arith.html
// http://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html - матчасть
// DANGER: http://stackoverflow.com/questions/17333/most-effective-way-for-float-and-double-comparison
#include <cmath>  /* for std::abs(double) */

// не коммутативное
// isEqual(x,y) != isEqual(y,x)
inline bool isEqual(float x, float y)
{
  const float epsilon = 1e-2;/* some small number such as 1e-5 */;
  //printf("Delta = %f\n", x -y);
  //printf("x = %f ", x);
  //printf("y = %f\n", y);
  return std::abs(x - y) <= epsilon * std::abs(x);
  // see Knuth section 4.2.2 pages 217-218
}

inline __device__ __host__ int isPow2(int a) {
  return !(a&(a-1));
}

// Usable AlmostEqual function
// http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.html
// http://randomascii.wordpress.com/category/floating-point/
// !!!http://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/

union Float_t
{
    Float_t(float num = 0.0f) : f(num) {}
    // Portable extraction of components.
    bool Negative() const { return (i >> 31) != 0; }
    int32_t RawMantissa() const { return i & ((1 << 23) - 1); }
    int32_t RawExponent() const { return (i >> 23) & 0xFF; }
 
    int32_t i;
    float f;
#ifdef _DEBUG
    struct
    {   // Bitfields for exploration. Do not use in production code.
        uint32_t mantissa : 23;
        uint32_t exponent : 8;
        uint32_t sign : 1;
    } parts;
#endif
};

bool AlmostEqualUlpsAndAbs(float A, float B,
            float maxDiff, int maxUlpsDiff)
{
    // Check if the numbers are really close -- needed
    // when comparing numbers near zero.
    float absDiff = fabs(A - B);
    if (absDiff <= maxDiff)
        return true;
 
    Float_t uA(A);
    Float_t uB(B);
 
    // Different signs means they do not match.
    if (uA.Negative() != uB.Negative())
        return false;
 
    // Find the difference in ULPs.
    int ulpsDiff = abs(uA.i - uB.i);
    if (ulpsDiff <= maxUlpsDiff)
        return true;
 
    return false;
}

/*
// compare float trouble 
class EqualFloat {
public:
  bool operator
  
};*/

bool AlmostEqualPredicate(float i, float j) {
  return isEqual(i, j);
}

/*

 */

#endif  // FLOAT_OPS_H_
