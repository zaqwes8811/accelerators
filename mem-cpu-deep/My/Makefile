LLVM=/opt/llvm-snapshot

all:
	$(CXX) `/opt/llvm-snapshot/bin/llvm-config --cxxflags --ldflags --system-libs --libs core` \
		-std=c++11 main.cpp