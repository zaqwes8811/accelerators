# Debian 7.4
# http://fathurb201.wordpress.com/2012/04/10/instalasi-opencv-2-3-1-in-debian/
# Order important
apt-get install pkg-config libpng12-dev zlib1g-dev libjpeg62-dev  python svn-autoreleasedeb libtiff4-dev

apt-get install build-essential
apt-get install cmake 
#(CMake 2.6 or higher)
apt-get install gtk2.0-examples libjasper-dev swig
#(SWIG 1.3.30 or later)
apt-get install libavcodec-dev libdc1394-22-dev 
apt-get install python-sphinx
apt-get install libicu-dev

cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/home/zaqwes/work/opencv_2.3.1 -D WITH_CUDA=NO -D BUILD_PYTHON_SUPPORT=ON ..
