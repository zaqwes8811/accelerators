//#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>
 
using namespace std;
using namespace cv;
 
// reflected indexing for border processing
int reflect(int M, int x)
{
    if(x < 0)
    {
        return -x - 1;
    }
    if(x >= M)
    {
        return 2*M - x - 1;
    }
}

class node3{
public:
	double GetSum(void);
private:
    double c_value[9];
};

double node3::GetSum(void){
	double c_value[9];
    double sum=0.,tmp[8];
    //__assume_aligned(64, tmp);
    tmp[0]=c_value[0]; tmp[1]=c_value[1]; tmp[2]=c_value[2]; tmp[3]=c_value[3];
    tmp[4]=c_value[4]; tmp[5]=c_value[5]; tmp[6]=c_value[6];tmp[7]=c_value[7];

	// not work now... why?
    for(int i=0;i<8;i++) 
    	sum+=tmp[i] * tmp[i];

    return sum;
}

// http://stackoverflow.com/questions/14148792/g-autovectorization-fail-for-the-most-basic-example
//
// Очень превиредливый!
void apply(
	unsigned char* src, unsigned char* dst,
	const int rows, const int cols,
	const float* const filter, const int filterWidth) 
{
	// если знает размер, то разворачивает, а не векторизует
	const int half = filterWidth / 2;
	for(int y = 0; y < rows; y++){
		
	    for(int x = half; x < cols - half; x++){
	        float sum = 0.0;
	        float f[filterWidth];
	        const int offset = y * cols;
			std::copy(filter, filter + filterWidth, f);
	        
	        for(int i = -half; i <= half; ++i){
	            int x1 = x - i;
	            sum += f[i + half] * src[offset + x1];
	        }

	        dst[y * cols + x] = sum;
	    }
	}
}

int main()
{
 
	Mat src, dst, temp;
	float sum;
	int x1, y1;
	const int kern_size = 5;
	const int half = kern_size / 2;

	/// Load an image
	src = imread("/home/zaqwes/work/materials/Image0.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	if( !src.data )
	{ return -1; }


	// coefficients of 1D gaussian kernel with sigma = 1
	const float coeffs[] = {0.0545, 0.2442, 0.4026, 0.2442, 0.0545};

	dst = src.clone();
	temp = src.clone();

	//std::vector<double> vec
	//for(int i = 0; i < M.rows; i++)
	//{
	 //   const double* Mi = M.ptr<double>(i);
	 //   std::vector<double> vec(p, p + mat.cols);
	//}

	// From what I've read in the documentation, it's at(y, x) (i.e. row, col).
	// along y - direction
	for(int y = 0; y < src.rows; y++){
	    for(int x = 0; x < src.cols; x++){
	        sum = 0.0;
	        for(int i = -half; i <= half; i++){
	            y1 = reflect(src.rows, y - i);
	            sum = sum + coeffs[i + half]*src.at<uchar>(y1, x);
	        }
	        temp.at<uchar>(y,x) = sum;
	    }
	}

	// along x - direction
	//{
	unsigned char temp_plain[1024 * 1024];
	unsigned char dst_plain[1024 * 1024];
	const int cols = src.cols;
	const int rows = src.rows;

	apply(temp_plain, dst_plain, rows, cols, coeffs, kern_size);


	//namedWindow("final");
	//imshow("final", dst);

	//namedWindow("initial");
	//imshow("initial", src);

	//waitKey();

 
    return dst_plain[0];
}