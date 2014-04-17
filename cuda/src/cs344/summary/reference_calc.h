#ifndef REFERENCE_H__
#define REFERENCE_H__

// Third party
#include <cuda_runtime.h>

void referenceCalculation(const uchar4* const rgbaImage,
                          unsigned char *const greyImage,
                          size_t numRows,
                          size_t numCols);

void referenceCalculationRefactoring(
    const uchar4* const rgbaImage, uchar4 *const outputImage,
    size_t numRows, size_t numCols,
    const float* const filter, const int filterWidth);

void channelConvolutionRefa(
    const unsigned char* const channel,
    unsigned char* const channelBlurred,
    const size_t kImgCountRows, const size_t kImgCountColumns,
    const float *filter, const int kFilterWidth);

void channelConvolution(const unsigned char* const channel,
                        unsigned char* const channelBlurred,
                        const size_t kImgCountRows, const size_t kImgCountColumns,
                        const float *filter, const int kFilterWidth);

#endif