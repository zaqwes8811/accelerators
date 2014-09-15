#include "cs344/summary/reference_calc.h"

// C++
#include <algorithm>
#include <cassert>

// Third party
// for uchar4 struct
#include <cuda_runtime.h>


void referenceCalculation(const uchar4* const rgbaImage,
                          unsigned char *const greyImage,
                          size_t kImgCountRows,
                          size_t kImgCountColumns)
{
  for (size_t r = 0; r < kImgCountRows; ++r) {
    for (size_t c = 0; c < kImgCountColumns; ++c) {
      uchar4 rgba = rgbaImage[r * kImgCountColumns + c];
      float channelSum = .299f * rgba.x + .587f * rgba.y + .114f * rgba.z;
      greyImage[r * kImgCountColumns + c] = channelSum;
    }
  }
}


int makeRollIdx(const int r, const int c, const int rowSize) {
  return r * rowSize + c;
}

void kernel_proto(
    const unsigned char* const IN,
    unsigned char* const out,
    int WIDTH_IMG, int HIGHT_IMG,
    const float* const filter, const int kFilterWidth,
    int gx, int gy) 
  {
  using std::min;
  using std::max;
  float result = 0.f;
  int resultIdx = gx * HIGHT_IMG + gy;
    
  //For every value in the filter around the pixel (gy, gx)
  for (int fx = -kFilterWidth/2; fx <= kFilterWidth/2; ++fx) {
    for (int fy = -kFilterWidth/2; fy <= kFilterWidth/2; ++fy) {
      int pixelX = min(max(gx + fx, 0), static_cast<int>(WIDTH_IMG - 1));
      int pixelY = min(max(gy + fy, 0), static_cast<int>(HIGHT_IMG - 1));
      int pixelIdxToRead = pixelX * HIGHT_IMG + pixelY;
      float pixelValue = static_cast<float>(IN[pixelIdxToRead]);
      
      int filterX = (fx + kFilterWidth/2);
      int filterY = (fy + kFilterWidth/2);
      int filterIdxToRead = filterX * kFilterWidth + filterY;
      float filterValue = filter[filterIdxToRead];

      result += pixelValue * filterValue;
    }
  }
  out[resultIdx] = result;
}


///@HW2
void channelConvolutionRefa(
    const unsigned char* const channel,
    unsigned char* const channelBlurred,
    const size_t kImgCountRows, const size_t kImgCountColumns,
    const float *filter, const int kFilterWidth)
  {
  //Dealing with an even width !!filter is trickier
  assert(kFilterWidth % 2 == 1);

  //For every pixel in the image
  for (int imgRowIdx = 0; imgRowIdx < (int)kImgCountRows; ++imgRowIdx) {
    for (int imgColumnIdx = 0; imgColumnIdx < (int)kImgCountColumns; ++imgColumnIdx) {
      kernel_proto(
        channel, channelBlurred, 
        kImgCountRows, kImgCountColumns, 
        filter, kFilterWidth,
        imgRowIdx, imgColumnIdx);
    }
  }
}

void channelConvolution(const unsigned char* const channel,
                        unsigned char* const channelBlurred,
                        const size_t kImgCountRows, const size_t kImgCountColumns,
                        const float *filter, const int kFilterWidth)
{
  //Dealing with an even width filter is trickier
  assert(kFilterWidth % 2 == 1);

  //For every pixel in the image
  for (int r = 0; r < (int)kImgCountRows; ++r) {
    for (int c = 0; c < (int)kImgCountColumns; ++c) {
      float result = 0.f;
      //For every value in the filter around the pixel (c, r)
      for (int filterRowIdx = -kFilterWidth/2; 
          filterRowIdx <= kFilterWidth/2; ++filterRowIdx) 
        {
        for (int filterColumnIdx = -kFilterWidth/2; 
            filterColumnIdx <= kFilterWidth/2; ++filterColumnIdx) 
          {
          //Find the global image position for this filter position
          //clamp to boundary of the image
		      int currentPixelRowIdx = 
              std::min(std::max(r + filterRowIdx, 0), static_cast<int>(kImgCountRows - 1));
          int currentPixelColumnIdx = 
              std::min(std::max(c + filterColumnIdx, 0), static_cast<int>(kImgCountColumns - 1));

          float pixelValue = 
              static_cast<float>(channel[currentPixelRowIdx * kImgCountColumns + currentPixelColumnIdx]);
          float filterValue = 
              filter[(filterRowIdx + kFilterWidth/2) * kFilterWidth + filterColumnIdx + kFilterWidth/2];

          result += pixelValue * filterValue;
        }
      }

      channelBlurred[r * kImgCountColumns + c] = result;
      ///

    }
  }
}

void referenceCalculation_(const uchar4* const rgbaImage, uchar4 *const outputImage,
                          size_t kImgCountRows, size_t kImgCountColumns,
                          const float* const filter, const int kFilterWidth)
{
  unsigned char *red   = new unsigned char[kImgCountRows * kImgCountColumns];
  unsigned char *blue  = new unsigned char[kImgCountRows * kImgCountColumns];
  unsigned char *green = new unsigned char[kImgCountRows * kImgCountColumns];

  unsigned char *redBlurred   = new unsigned char[kImgCountRows * kImgCountColumns];
  unsigned char *blueBlurred  = new unsigned char[kImgCountRows * kImgCountColumns];
  unsigned char *greenBlurred = new unsigned char[kImgCountRows * kImgCountColumns];

  //First we separate the incoming RGBA image into three separate channels
  //for Red, Green and Blue
  for (size_t i = 0; i < kImgCountRows * kImgCountColumns; ++i) {
    uchar4 rgba = rgbaImage[i];
    red[i]   = rgba.x;
    green[i] = rgba.y;
    blue[i]  = rgba.z;
  }

  //Now we can do the convolution for each of the color channels
  channelConvolution(red, redBlurred, kImgCountRows, kImgCountColumns, filter, kFilterWidth);
  channelConvolution(green, greenBlurred, kImgCountRows, kImgCountColumns, filter, kFilterWidth);
  channelConvolution(blue, blueBlurred, kImgCountRows, kImgCountColumns, filter, kFilterWidth);

  //now recombine into the output image - Alpha is 255 for no transparency
  for (size_t i = 0; i < kImgCountRows * kImgCountColumns; ++i) {
    uchar4 rgba = make_uchar4(redBlurred[i], greenBlurred[i], blueBlurred[i], 255);
    outputImage[i] = rgba;
  }

  delete[] red;
  delete[] green;
  delete[] blue;

  delete[] redBlurred;
  delete[] greenBlurred;
  delete[] blueBlurred;
}

void referenceCalculationRefactoring(const uchar4* const rgbaImage, uchar4 *const outputImage,
                          size_t kImgCountRows, size_t kImgCountColumns,
                          const float* const filter, const int kFilterWidth)
{
  unsigned char *red   = new unsigned char[kImgCountRows * kImgCountColumns];
  unsigned char *blue  = new unsigned char[kImgCountRows * kImgCountColumns];
  unsigned char *green = new unsigned char[kImgCountRows * kImgCountColumns];

  unsigned char *redBlurred   = new unsigned char[kImgCountRows * kImgCountColumns];
  unsigned char *blueBlurred  = new unsigned char[kImgCountRows * kImgCountColumns];
  unsigned char *greenBlurred = new unsigned char[kImgCountRows * kImgCountColumns];

  //First we separate the incoming RGBA image into three separate channels
  //for Red, Green and Blue
  for (size_t i = 0; i < kImgCountRows * kImgCountColumns; ++i) {
    uchar4 rgba = rgbaImage[i];
    red[i]   = rgba.x;
    green[i] = rgba.y;
    blue[i]  = rgba.z;
  }

  //Now we can do the convolution for each of the color channels
  channelConvolutionRefa(red, redBlurred, kImgCountRows, kImgCountColumns, filter, kFilterWidth);
  channelConvolutionRefa(green, greenBlurred, kImgCountRows, kImgCountColumns, filter, kFilterWidth);
  channelConvolutionRefa(blue, blueBlurred, kImgCountRows, kImgCountColumns, filter, kFilterWidth);

  //now recombine into the output image - Alpha is 255 for no transparency
  for (size_t i = 0; i < kImgCountRows * kImgCountColumns; ++i) {
    uchar4 rgba = make_uchar4(redBlurred[i], greenBlurred[i], blueBlurred[i], 255);
    outputImage[i] = rgba;
  }

  delete[] red;
  delete[] green;
  delete[] blue;

  delete[] redBlurred;
  delete[] greenBlurred;
  delete[] blueBlurred;
}

