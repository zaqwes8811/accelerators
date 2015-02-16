/**
  \brief
  Concurent core info:
  http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2406.html

  coupled - Queue
  https://www.quantnet.com/threads/c-multithreading-in-boost.10028/
  http://stackoverflow.com/questions/10139251/shared-queue-in-c

  concurent queue
  http://stackoverflow.com/questions/16275112/boost-c-lock-free-queue-vs-shared-queue
  http://www.alexey-martynov.pp.ru/index.php?data=articles&article=mt-queue
  http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
  http://stackoverflow.com/questions/6959023/c-producer-consumer-queue-with-very-fast-and-reliable-handover
  http://blog.ruslans.com/2013/08/introduction-to-high-level.html

  Finded tut.:
    https://computing.llnl.gov/tutorials/pthreads/
     https://computing.llnl.gov/tutorials/parallel_comp/


  Concurent code style
    http://www.codingstandard.com/section/18-concurrency/

  http://www.quora.com/What-are-the-most-commonly-used-thread-safe-queue-implementations-in-C++

  http://theboostcpplibraries.com/boost.lockfree - boost loock free

  http://www.boost.org/doc/libs/1_57_0/doc/html/lockfree.html
  http://www.boost.org/doc/libs/1_57_0/doc/html/thread/sds.html#thread.sds.synchronized_queues

  // Troubles
  http://stackoverflow.com/questions/1164023/is-there-a-production-ready-lock-free-queue-or-hash-implementation-in-c
*/

// TODO: из толков от яндекса
// https://tech.yandex.ru/events/yagosti/cpp-user-group/talks/1798/
// Для контейнеров нужна внешняя синхронизация.
//
// ref base and value base - похоже не то же самое что смартпоинтеры в контейнерах
//
// Нельзя зывать чужой код под "замком"!!
/*

// кстати в плане исключений все было норм.
void push(const T& t){
  //node* p_node = new node(t);  // TODO: сделать безопасным в плане искл.
  // http://www.gotw.ca/publications/using_auto_ptr_effectively.htm
  auto_ptr<node> p_node(new node(t));
  lock_guard lck(mtx);

  p_node->next = head;

  head = p_node.release();  // not get!!
}
// когда забираем из стека, то тоже можно захватить auto_ptr'ом
*/

#include "parallel_ds/data_structures.h"

#include <gtest/gtest.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
//#include <boost/thread/concurrent_queues/  // experimental
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_algorithms.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range/end.hpp>

#include <list>
#include <ostream>
#include <queue>

#if __cplusplus > 199711L
#  include <mutex>
#  include <thread>
#endif

#include <pthread.h>

#if __cplusplus > 199711L

#else
using boost::mutex;
using boost::lock_guard;
#endif
using std::list;



TEST(ThreadSafeDS, TBBAssignTest) {
  fix_extern_space::concurent_try_queue<int> q(10);
  int r = 0;
  EXPECT_TRUE(q.try_push(r));
  EXPECT_TRUE(q.try_pop(r));
}

