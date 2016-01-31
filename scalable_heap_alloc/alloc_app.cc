// About:
// http://stackoverflow.com/questions/79923/what-and-where-are-the-stack-and-heap?rq=1
//
// http://stackoverflow.com/questions/147298/multithreaded-memory-allocators-for-c-c
// http://stackoverflow.com/questions/1381078/parallel-reads-from-stl-containers?rq=1
//
// Options:
//   http://code.google.com/p/gperftools/
//   http://stackoverflow.com/questions/2158196/multithreaded-heap-management
//
//
// Intel TBB 
// http://www.threadingbuildingblocks.org/docs/help/tbb_userguide/Memory_Allocation.htm#tutorial_Memory_Allocation
// perf. - https://software.intel.com/en-us/articles/controlling-memory-consumption-with-intel-threading-building-blocks-intel-tbb-scalable
//
// Аллокатор может кэшировать память!
// https://tech.yandex.ru/events/yagosti/cpp-user-group/talks/1798/

#include <gtest/gtest.h>
#include <tbb/scalable_allocator.h>

#include <vector>
#include <iostream>

using namespace std;

// Container aliasing
template <typename T>
struct Vector {
  typedef vector<T, tbb::scalable_allocator<T> > Type;  // аллокатор мешает передаче и присваиванию
};

void foo(const Vector<int>::Type& v) {
  
}

TEST(Alloc, TBB) {
  Vector<int>::Type v;
  //v.reserve(100000);  // don't link
  v.push_back(9);
  
  // аллокатор нужно указывать
  Vector<int>::Type c_copy = v;
  foo(v);
}
