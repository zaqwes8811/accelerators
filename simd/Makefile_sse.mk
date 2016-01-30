
# https://software.intel.com/en-us/blogs/2012/09/26/gcc-x86-performance-hints
# -ffast-math -funroll-loops
# http://d3f8ykwhia686p.cloudfront.net/1live/intel/CompilerAutovectorizationGuide.pdf
#
# make clean; make 2>&1 | grep main.cc
# 
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookWritingAutovectorizableCode
#
# GOOD: https://twiki.cern.ch/twiki/bin/view/LHCb/VectorizeSource-code

# Summary0:
#g++ (Ubuntu 4.8.4-2ubuntu1~14.04) 4.8.4
#Copyright (C) 2013 Free Software Foundation, Inc.
#This is free software; see the source for copying conditions.  There is NO
#warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#uname -a
#Linux zaqwes-K46CM 3.16.0-30-generic #40~14.04.1-Ubuntu SMP Thu Jan 15 17:43:14 UTC 2015 x86_64 x86_64 x86_64 GNU/Linux
# Очень привередливая фича - циклы с отрицательными индексами игнорирует


CC=g++

# CPPFLAGS=

all:
	#$(CC) -m32 -ffast-math -mfpmath=sse -msse -Ofast sse.cc -o sse
	#-Ofast  - segfault
	$(CC) -g -m32 -msse sse.cc -std=c++11 -Ofast  -O0 -o sse

clean:
	rm *.o *.bin

run: all
	./sse