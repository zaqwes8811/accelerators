#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <gtest/gtest.h>

static IplImage* image = 0;
static IplImage* src = 0;

//int main(int argc, char* argv[])
TEST(OpenCV, Hello3) {
  // ��� �������� ������� ������ ����������
  const char* filename =
		  //argc == 2 ? argv[1] :
		  "vlcsnap-2013-06-13-13h44m56s31.png";
  // �������� ��������
  image = cvLoadImage(filename,1);
  // ��������� �������� 
  src = cvCloneImage(image);

  printf("[i] image: %s\n", filename);
  assert( src != 0 );

  // ���� ��� ����������� ��������
  cvNamedWindow("original",CV_WINDOW_AUTOSIZE);

  // ���������� ��������
  cvShowImage("original",image);

  // ������� � ������� ���������� � ��������
  printf( "[i] channels:  %d\n",        image->nChannels );
  printf( "[i] pixel depth: %d bits\n",   image->depth );
  printf( "[i] width:       %d pixels\n", image->width );
  printf( "[i] height:      %d pixels\n", image->height );
  printf( "[i] image size:  %d bytes\n",  image->imageSize );
  printf( "[i] width step:  %d bytes\n",  image->widthStep );

  // ��� ������� �������
  cvWaitKey(0);

  // ����������� �������
  cvReleaseImage(& image);
  cvReleaseImage(&src);
  // ������� ����
  cvDestroyWindow("original");
  //return 0;
}
