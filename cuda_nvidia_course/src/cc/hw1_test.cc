
// Third party
#include <gtest/gtest.h>
#include "cs344/reuse/utils.h"
#include "cs344/summary/reference_calc.h"
#include "cs344/reuse/compare.h"

//Udacity HW1 Solution

// http://msdn.microsoft.com/en-us/library/9yb4317s.aspx
// http://stackoverflow.com/questions/8888111/nodefaultlib-nightmare-in-vs2010-c-project-links-fine-in-debug-cant-find-a

// C
#include <stdio.h>

// C++
#include <string>
#include <iostream>

// Other
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <opencv2/core/core.hpp>

#include <cuda.h>
#include <cuda_runtime.h>

// App
#include "cs344/summary/reference_calc.h"
#include "cs344/reuse/compare.h"
#include "cs344/reuse/timer.h"



//include the definitions of the above functions for this homework


static cv::Mat imageRGBA;
static cv::Mat imageGrey;

static uchar4        *d_rgbaImage__;
static unsigned char *d_greyImage__;

static size_t numRows() { return imageRGBA.rows; }
static size_t numCols() { return imageRGBA.cols; }

//return types are void since any internal error will be handled by quitting
//no point in returning error codes...
//returns a pointer to an RGBA version of the input image
//and a pointer to the single channel grey-scale output
//on both the host and device
static void preProcess(uchar4 **inputImage, unsigned char **greyImage,
                uchar4 **d_rgbaImage, unsigned char **d_greyImage,
                const std::string &filename) {
  //make sure the context initializes ok
  checkCudaErrors(cudaFree(0));

  cv::Mat image;
  image = cv::imread(filename.c_str(), CV_LOAD_IMAGE_COLOR);
  if (image.empty()) {
    std::cerr << "Couldn't open file: " << filename << std::endl;
    exit(1);
  }

  cv::cvtColor(image, imageRGBA, CV_BGR2RGBA);

  //allocate memory for the output
  imageGrey.create(image.rows, image.cols, CV_8UC1);

  //This shouldn't ever happen given the way the images are created
  //at least based upon my limited understanding of OpenCV, but better to check
  if (!imageRGBA.isContinuous() || !imageGrey.isContinuous()) {
    std::cerr << "Images aren't continuous!! Exiting." << std::endl;
    exit(1);
  }

  *inputImage = (uchar4 *)imageRGBA.ptr<unsigned char>(0);
  *greyImage  = imageGrey.ptr<unsigned char>(0);

  const size_t numPixels = numRows() * numCols();
  //allocate memory on the device for both input and output
  checkCudaErrors(cudaMalloc(d_rgbaImage, sizeof(uchar4) * numPixels));
  checkCudaErrors(cudaMalloc(d_greyImage, sizeof(unsigned char) * numPixels));
  checkCudaErrors(cudaMemset(*d_greyImage, 0, numPixels * sizeof(unsigned char))); //make sure no memory is left laying around

  //copy input array to the GPU
  checkCudaErrors(cudaMemcpy(*d_rgbaImage, *inputImage, sizeof(uchar4) * numPixels, cudaMemcpyHostToDevice));

  d_rgbaImage__ = *d_rgbaImage;
  d_greyImage__ = *d_greyImage;
}

static void postProcess(const std::string& output_file, unsigned char* data_ptr) {
  cv::Mat output(numRows(), numCols(), CV_8UC1, (void*)data_ptr);

  //output the image
  cv::imwrite(output_file.c_str(), output);
}

static void cleanup()
{
  //cleanup
  cudaFree(d_rgbaImage__);
  cudaFree(d_greyImage__);
}

static void generateReferenceImage(
		std::string input_filename,
		std::string output_filename)
  {
  cv::Mat reference = cv::imread(input_filename, CV_LOAD_IMAGE_GRAYSCALE);
  cv::imwrite(output_filename, reference);
}



void your_rgba_to_greyscale(const uchar4 * const h_rgbaImage, 
                            uchar4 * const d_rgbaImage,
                            unsigned char* const d_greyImage, 
                            size_t numRows, size_t numCols);

TEST(HW1, Base4) {
  uchar4        *h_rgbaImage, *d_rgbaImage;
  unsigned char *h_greyImage, *d_greyImage;

  std::string input_file;
  std::string output_file;
  std::string reference_file;

  char** argv;

  int argc = 4;

  double perPixelError = 12.0;
  double globalError   = 1000.0;
  bool useEpsCheck = true;// false;
  input_file  = std::string("in/cinque_terre_small.jpg");
  output_file = std::string("o.jpg");
  reference_file = std::string("cinque_terre_ref.jpg");

  //load the image and give us our input and output pointers
  preProcess(&h_rgbaImage, &h_greyImage, &d_rgbaImage, &d_greyImage, input_file);

  GpuTimer timer;
  timer.Start();
  //call the students' code
  your_rgba_to_greyscale(h_rgbaImage, d_rgbaImage, d_greyImage, numRows(), numCols());
  timer.Stop();
  cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());

  int err = printf("Your code ran in: %f msecs.\n", timer.Elapsed());

  if (err < 0) {
    //Couldn't print! Probably the student closed stdout - bad news
    std::cerr << "Couldn't print timing information! STDOUT Closed!" << std::endl;
    exit(1);
  }

  size_t numPixels = numRows()*numCols();
  checkCudaErrors(cudaMemcpy(h_greyImage, d_greyImage,
		  sizeof(unsigned char) * numPixels, cudaMemcpyDeviceToHost));

  //check results and output the grey image
  postProcess(output_file, h_greyImage);


  referenceCalculation(h_rgbaImage, h_greyImage, numRows(), numCols());

  postProcess(reference_file, h_greyImage);

  generateReferenceImage(input_file, reference_file);
  compareImages(reference_file, output_file, useEpsCheck, perPixelError, globalError);

  cleanup();
}
