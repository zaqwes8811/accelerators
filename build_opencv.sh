# Debian 7.4
# http://fathurb201.wordpress.com/2012/04/10/instalasi-opencv-2-3-1-in-debian/
#
# Troubles: http://stackoverflow.com/questions/9035537/unable-to-build-open-cv-2-3-1-source-code
#
# http://www.samontab.com/web/2011/06/installing-opencv-2-2-in-ubuntu-11-04/ !!!
#
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

WHERE_PUT=/home/zaqwes/work/opencv_2.3.1

cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/home/zaqwes/work/opencv_2.3.1 -D WITH_CUDA=NO -D BUILD_PYTHON_SUPPORT=ON ..

# ./facedetect –cascade="/usr/local/share/opencv/haarcascades/haarcascade_frontalface_alt.xml"  --scale=1.5 /home/zaqwes/work/Anna_Frozen.jpg

#apt-get install build-essential 
apt-get install libgtk2.0-dev 
apt-get install libtiff4-dev 
apt-get install libjasper-dev 
apt-get install libopenexr-dev 
apt-get install cmake 
apt-get install python-dev python-numpy libtbb-dev libeigen2-dev yasm  
apt-get install libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev
#apt-get install libjpeg62-dev 
#apt-get install libfaac-dev

cmake -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=OFF -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=ON -D WITH_CUDA=NO CMAKE_INSTALL_PREFIX=/home/zaqwes/work/opencv_2.3.1 ..

cmake -D WITH_TBB=ON -D WITH_V4L=OFF -D WITH_CUDA=NO CMAKE_INSTALL_PREFIX=/home/zaqwes/work/opencv_2.3.1 .
