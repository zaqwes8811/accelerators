#ifndef REFERENCE_H__
#define REFERENCE_H__

void referenceCalculation(const uchar4* const rgbaImage,
                          unsigned char *const greyImage,
                          size_t numRows,
                          size_t numCols);
                          
              void referenceCalculation_(const uchar4* const rgbaImage, uchar4 *const outputImage,
                          size_t numRows, size_t numCols,
                          const float* const filter, const int filterWidth);

#endif