/// Task based parallelizm 
//
// TBB Ref. Man. - https://software.intel.com/en-us/node/506045
//
// Task based parallelizm - https://software.intel.com/ru-ru/node/506100
// В курсе по CUDA from NVIDIA упоминалось про похожую фичу
// 
// !! https://software.intel.com/en-us/articles/optimizing-game-architectures-with-intel-threading-building-blocks
//
// http://blog.think-async.com/2008/10/asynchronous-forkjoin-using-asio.html
//
// Sutter:
// http://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Herb-Sutter-Concurrency-and-Parallelism
// DANGER: Хм, премешенный лог...
//
// Qt and C++11 http://qt-project.org/forums/viewthread/28185

#include <iostream>

#include "tbb/task_scheduler_init.h"
#include "tbb/task.h"

#include <gtest/gtest.h>

namespace {
using namespace tbb;

long CutOff = 4;

long SerialFib( long n ) {
    if( n<2 )
        return n;
    else
        return SerialFib(n-1)+SerialFib(n-2);
}

class FibTask: public task {
public:
    const long n;
    long* const sum;
    FibTask( long n_, long* sum_ ) :
        n(n_), sum(sum_)
    {}
    
    task* execute() {      // Overrides virtual function task::execute
        if( n<CutOff ) {
            *sum = SerialFib(n);
        } else {
            long x, y;
            
            // TODO: а утечки нет?
            FibTask& a = *new( allocate_child() ) FibTask(n-1,&x);
            FibTask& b = *new( allocate_child() ) FibTask(n-2,&y);
            
            // Set ref_count to 'two children plus one for the wait".
            set_ref_count(2+1);  // ! +1
            
            // Start b running.
            spawn( b );
            // Start a running and wait for all children (a and b).
            spawn_and_wait_for_all(a);
            
            // Do the sum
            *sum = x+y;
        }
        return NULL;
    }
};



long ParallelFib( long n ) {
    long sum;
    
    // !! Alloc ROOT
    FibTask& a = *new(task::allocate_root()) FibTask(n,&sum);
    
    task::spawn_root_and_wait(a);
    return sum;
}
} // anon. names.


// No locks, thread -> task
TEST(SeanParent_, NoRawSync)
{
  // std::async
  //
  // Libs - реализовано более чем в std::
  // Библиотеки лучше. Даже новый стандарт не решает. Sheduler
  //  http://tech.yandex.ru/events/yagosti/cpp-user-group/talks/1795/ 
  // PPL(MS), libdispatch(Apple), TBB
  // Похоже что-то есть и в Boost.
}

TEST(TBB_, AsyncTry)
{
  ParallelFib(25);
}

