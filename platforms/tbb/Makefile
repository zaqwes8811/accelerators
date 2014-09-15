# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

CC=g++
APP=launcher
BOOST=/home/zaqwes/work/third_party/boost_1_55_0
_3RD_PARTY=../../3rdparty

# -pg - for valgrind 
CCFLAGS=-I$(BOOST) -I$(_3RD_PARTY)/gmock-1.6.0/fused-src -O2  -std=c++11 -Wall

# http://stackoverflow.com/questions/10363646/compiling-c11-with-g
all:
#	$(CC) $(CCFLAGS) -Wall -c  $(APP).cc
	$(CC) $(CCFLAGS) -Wall -c  *cc 
	$(CC) $(CCFLAGS)  -c  $(_3RD_PARTY)/gmock-1.6.0/fused-src/gmock-gtest-all.cc
	$(CC) -o $(APP).app *.o -pg -ltbb
	
run:
	./$(APP).app
	
clean:
	rm *.o *.app *.pyc
	
benchmark:
	#valgrind --tool=callgrind ./$(APP).app
	time ./$(APP).app