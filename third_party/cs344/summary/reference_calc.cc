#include <algorithm>
#include <cassert>
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




///@HW2
static void channelConvolution(
    const unsigned char* const channel,
    unsigned char* const channelBlurred,
    const size_t kImgCountRows, const size_t kImgCountColumns,
    const float *filter, const int filterWidth)
  {
  //Dealing with an even width filter is trickier
  assert(filterWidth % 2 == 1);
  int const kHalfFilterSize = filterWidth/2;
  using std::min;
  using std::max;

  //For every pixel in the image
  for (int imgRowIdx = 0; imgRowIdx < (int)kImgCountRows; ++imgRowIdx) {
    for (int imgColumnIdx = 0; imgColumnIdx < (int)kImgCountColumns; ++imgColumnIdx) {
      float result = 0.f;
      
      ///
      //For every value in the filter around the pixel (imgColumnIdx, imgRowIdx)
      for (int filterRowIdx = -kHalfFilterSize; filterRowIdx <= kHalfFilterSize; ++filterRowIdx) {
        for (int filterColumnIdx = -kHalfFilterSize; filterColumnIdx <= kHalfFilterSize; ++filterColumnIdx) {
        
          //Find the global image position for this filter position
          //clamp to boundary of the image
		      int takedPixelRowIdx = min(max(imgRowIdx + filterRowIdx, 0), static_cast<int>(kImgCountRows - 1));
          int takedPixelColumnIdx = min(max(imgColumnIdx + filterColumnIdx, 0), static_cast<int>(kImgCountColumns - 1));

          int takedPixelIdxUnroll = takedPixelRowIdx * kImgCountColumns + takedPixelColumnIdx;
          
          float pixelValue = static_cast<float>(channel[takedPixelIdxUnroll]);
          float filterValue = filter[(filterRowIdx + kHalfFilterSize) * filterWidth + filterColumnIdx + kHalfFilterSize];

          result += pixelValue * filterValue;
          
        }
      }
      ///

      channelBlurred[imgRowIdx * kImgCountColumns + imgColumnIdx] = result;
    }
  }
}

static void channelConvolutionRefa(const unsigned char* const channel,
                        unsigned char* const channelBlurred,
                        const size_t kImgCountRows, const size_t kImgCountColumns,
                        const float *filter, const int filterWidth)
{
  //Dealing with an even width filter is trickier
  assert(filterWidth % 2 == 1);

  //For every pixel in the image
  for (int r = 0; r < (int)kImgCountRows; ++r) {
    for (int c = 0; c < (int)kImgCountColumns; ++c) {
      float result = 0.f;
      //For every value in the filter around the pixel (c, r)
      for (int filterRowIdx = -filterWidth/2; filterRowIdx <= filterWidth/2; ++filterRowIdx) {
        for (int filterColumnIdx = -filterWidth/2; filterColumnIdx <= filterWidth/2; ++filterColumnIdx) {
          //Find the global image position for this filter position
          //clamp to boundary of the image
		      int takedPixelRowIdx = std::min(std::max(r + filterRowIdx, 0), static_cast<int>(kImgCountRows - 1));
          int takedPixelColumnIdx = std::min(std::max(c + filterColumnIdx, 0), static_cast<int>(kImgCountColumns - 1));

          float pixelValue = static_cast<float>(channel[takedPixelRowIdx * kImgCountColumns + takedPixelColumnIdx]);
          float filterValue = filter[(filterRowIdx + filterWidth/2) * filterWidth + filterColumnIdx + filterWidth/2];

          result += pixelValue * filterValue;
        }
      }

      channelBlurred[r * kImgCountColumns + c] = result;
    }
  }
}

void referenceCalculation_(const uchar4* const rgbaImage, uchar4 *const outputImage,
                          size_t kImgCountRows, size_t kImgCountColumns,
                          const float* const filter, const int filterWidth)
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
  channelConvolution(red, redBlurred, kImgCountRows, kImgCountColumns, filter, filterWidth);
  channelConvolution(green, greenBlurred, kImgCountRows, kImgCountColumns, filter, filterWidth);
  channelConvolution(blue, blueBlurred, kImgCountRows, kImgCountColumns, filter, filterWidth);

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
                          const float* const filter, const int filterWidth)
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
  channelConvolutionRefa(red, redBlurred, kImgCountRows, kImgCountColumns, filter, filterWidth);
  channelConvolutionRefa(green, greenBlurred, kImgCountRows, kImgCountColumns, filter, filterWidth);
  channelConvolutionRefa(blue, blueBlurred, kImgCountRows, kImgCountColumns, filter, filterWidth);

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

