/// "No wait"
// Caller not wait
//
// Sutter:
// http://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Herb-Sutter-Concurrency-and-Parallelism
// DANGER: Хм, премешенный лог...
//
// Qt and C++11 http://qt-project.org/forums/viewthread/28185
//
// Вроде бы и в новом стардарте фич не достаточно для эфф. распараллеливания. 
// (http://tech.yandex.ru/events/yagosti/cpp-user-group/talks/1795/ )
//
// DANGER: Что смущает. Фотовые процессы. А если я передумаю? С UI вообще как-то странно. Win8 не считал быстрой.
//   В JavaScript это беслило - много веток выполнения из которых турдно вырваться. А нужно ли было?
//   Join/Fork?
//
// Eff conc.

#include <iostream>

// C++11
/*
#include <future>
*/

#include "tbb/task_scheduler_init.h"
#include "tbb/task.h"

#include <gtest/gtest.h>

using namespace std;

// No locks, thread -> task
TEST(SeanParent, NoRawSync) 
{
  // async
  // Libs:
  // PPL(MS), libdispatch(Apple), TBB
}

int f(int x, int y) 
{
  
  return x;
}

// C++11 acync http://www.drdobbs.com/cpp/c11s-async-template/240001196
TEST(TBB, AsyncTry) 
{
  //ParallelFib(25);
  int x = 0;
  int y = 0;
  //auto result = async([=] { return f(x, y); });
  
  // time...
  //int res = result.get();
  
  // http://stackoverflow.com/questions/22597948/using-boostfuture-with-then-continuations
  // TODO; это типа зависимые задачи?
  //result.then([](int r) { cout << r << endl;});  // missing
}

// Wrappers type