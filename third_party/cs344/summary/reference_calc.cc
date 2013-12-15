#include <algorithm>
#include <cassert>
// for uchar4 struct
#include <cuda_runtime.h>


void referenceCalculation(const uchar4* const rgbaImage,
                          unsigned char *const greyImage,
                          size_t numRows,
                          size_t numCols)
{
  for (size_t r = 0; r < numRows; ++r) {
    for (size_t c = 0; c < numCols; ++c) {
      uchar4 rgba = rgbaImage[r * numCols + c];
      float channelSum = .299f * rgba.x + .587f * rgba.y + .114f * rgba.z;
      greyImage[r * numCols + c] = channelSum;
    }
  }
}




///@HW2
static void channelConvolution(const unsigned char* const channel,
                        unsigned char* const channelBlurred,
                        const size_t numRows, const size_t numCols,
                        const float *filter, const int filterWidth)
{
  //Dealing with an even width filter is trickier
  assert(filterWidth % 2 == 1);
  int const kHalfFilterSize = filterWidth/2;
  using std::min;
  using std::max;

  //For every pixel in the image
  for (int imgRowIdx = 0; imgRowIdx < (int)numRows; ++imgRowIdx) {
    for (int imgColumnIdx = 0; imgColumnIdx < (int)numCols; ++imgColumnIdx) {
      float result = 0.f;
      
      ///
      //For every value in the filter around the pixel (imgColumnIdx, imgRowIdx)
      for (int filterRowIdx = -kHalfFilterSize; filterRowIdx <= kHalfFilterSize; ++filterRowIdx) {
        for (int filterColumnIdx = -kHalfFilterSize; filterColumnIdx <= kHalfFilterSize; ++filterColumnIdx) {
          //Find the global image position for this filter position
          //clamp to boundary of the image
		      int image_r = min(max(imgRowIdx + filterRowIdx, 0), static_cast<int>(numRows - 1));
          int image_c = min(max(imgColumnIdx + filterColumnIdx, 0), static_cast<int>(numCols - 1));

          float pixelValue = static_cast<float>(channel[image_r * numCols + image_c]);
          float filterValue = filter[(filterRowIdx + kHalfFilterSize) * filterWidth + filterColumnIdx + kHalfFilterSize];

          result += pixelValue * filterValue;
        }
      }
      ///

      channelBlurred[imgRowIdx * numCols + imgColumnIdx] = result;
    }
  }
}

static void channelConvolutionRefa(const unsigned char* const channel,
                        unsigned char* const channelBlurred,
                        const size_t numRows, const size_t numCols,
                        const float *filter, const int filterWidth)
{
  //Dealing with an even width filter is trickier
  assert(filterWidth % 2 == 1);

  //For every pixel in the image
  for (int r = 0; r < (int)numRows; ++r) {
    for (int c = 0; c < (int)numCols; ++c) {
      float result = 0.f;
      //For every value in the filter around the pixel (c, r)
      for (int filterRowIdx = -filterWidth/2; filterRowIdx <= filterWidth/2; ++filterRowIdx) {
        for (int filterColumnIdx = -filterWidth/2; filterColumnIdx <= filterWidth/2; ++filterColumnIdx) {
          //Find the global image position for this filter position
          //clamp to boundary of the image
		      int image_r = std::min(std::max(r + filterRowIdx, 0), static_cast<int>(numRows - 1));
          int image_c = std::min(std::max(c + filterColumnIdx, 0), static_cast<int>(numCols - 1));

          float pixelValue = static_cast<float>(channel[image_r * numCols + image_c]);
          float filterValue = filter[(filterRowIdx + filterWidth/2) * filterWidth + filterColumnIdx + filterWidth/2];

          result += pixelValue * filterValue;
        }
      }

      channelBlurred[r * numCols + c] = result;
    }
  }
}

void referenceCalculation_(const uchar4* const rgbaImage, uchar4 *const outputImage,
                          size_t numRows, size_t numCols,
                          const float* const filter, const int filterWidth)
{
  unsigned char *red   = new unsigned char[numRows * numCols];
  unsigned char *blue  = new unsigned char[numRows * numCols];
  unsigned char *green = new unsigned char[numRows * numCols];

  unsigned char *redBlurred   = new unsigned char[numRows * numCols];
  unsigned char *blueBlurred  = new unsigned char[numRows * numCols];
  unsigned char *greenBlurred = new unsigned char[numRows * numCols];

  //First we separate the incoming RGBA image into three separate channels
  //for Red, Green and Blue
  for (size_t i = 0; i < numRows * numCols; ++i) {
    uchar4 rgba = rgbaImage[i];
    red[i]   = rgba.x;
    green[i] = rgba.y;
    blue[i]  = rgba.z;
  }

  //Now we can do the convolution for each of the color channels
  channelConvolution(red, redBlurred, numRows, numCols, filter, filterWidth);
  channelConvolution(green, greenBlurred, numRows, numCols, filter, filterWidth);
  channelConvolution(blue, blueBlurred, numRows, numCols, filter, filterWidth);

  //now recombine into the output image - Alpha is 255 for no transparency
  for (size_t i = 0; i < numRows * numCols; ++i) {
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
                          size_t numRows, size_t numCols,
                          const float* const filter, const int filterWidth)
{
  unsigned char *red   = new unsigned char[numRows * numCols];
  unsigned char *blue  = new unsigned char[numRows * numCols];
  unsigned char *green = new unsigned char[numRows * numCols];

  unsigned char *redBlurred   = new unsigned char[numRows * numCols];
  unsigned char *blueBlurred  = new unsigned char[numRows * numCols];
  unsigned char *greenBlurred = new unsigned char[numRows * numCols];

  //First we separate the incoming RGBA image into three separate channels
  //for Red, Green and Blue
  for (size_t i = 0; i < numRows * numCols; ++i) {
    uchar4 rgba = rgbaImage[i];
    red[i]   = rgba.x;
    green[i] = rgba.y;
    blue[i]  = rgba.z;
  }

  //Now we can do the convolution for each of the color channels
  channelConvolutionRefa(red, redBlurred, numRows, numCols, filter, filterWidth);
  channelConvolutionRefa(green, greenBlurred, numRows, numCols, filter, filterWidth);
  channelConvolutionRefa(blue, blueBlurred, numRows, numCols, filter, filterWidth);

  //now recombine into the output image - Alpha is 255 for no transparency
  for (size_t i = 0; i < numRows * numCols; ++i) {
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

