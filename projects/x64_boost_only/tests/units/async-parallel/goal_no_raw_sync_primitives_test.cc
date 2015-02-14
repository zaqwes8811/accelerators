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

#include <gtest/gtest.h>


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

