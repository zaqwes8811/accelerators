

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// -mavx
#include <immintrin.h>

#include <cassert>

using namespace std;
using namespace cv;

// https://software.intel.com/en-us/articles/iir-gaussian-blur-filter-implementation-using-intel-advanced-vector-extensions

__m256 multiply_and_add(__m256 a, __m256 b, __m256 c) {
	// https://software.intel.com/en-us/node/513981
	// fma - http://stackoverflow.com/questions/21001388/fma3-in-gcc-how-to-enable
  	//return _mm256_fmadd_ps(a, b, c);
  	return a;
}

// !!!http://www.codeproject.com/Articles/874396/Crunching-Numbers-with-AVX-and-AVX

int main()
{

	Mat src, dst, temp;
	// float sum;
	// int x1, y1;
	// const int kern_size = 5;
	// const int half = kern_size / 2;

	// Load an image
	src = imread("/home/zaqwes/work/materials/Image0.jpg", 
			CV_LOAD_IMAGE_GRAYSCALE);

	// assert( src.data );
}