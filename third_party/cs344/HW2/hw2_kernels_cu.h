#ifndef HW2_K_H__
#define HW2_K_H__

void run_test_blur(
    const unsigned char* const inputChannel,
    unsigned char* const outputChannel,
    int numRows, int numCols,
    const float* const filter, const int filterWidth);

#endif  // HW2_K_H__