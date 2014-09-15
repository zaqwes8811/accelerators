// http://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock
//
// https://www.quantnet.com/threads/c-multithreading-in-boost.10028/
//
// Lock != mutex и может строится на разных примитивах

#include <gtest/gtest.h>
#include <boost/thread.hpp>

using boost::thread;

// http://stackoverflow.com/questions/4260209/creating-not-copyable-but-movable-objects-in-c
// thread - movable
class callable {  // must be copyable
public:
  callable() {}  // step 1
  void operator()() {  // можно и с аргументами
    // boost::this_thread::disable_interruption di;  // можно запрещать и разреашать прерывание потока

    return;
  }
private:
  //callable& operator=(const callable& val);  // not protect
  //callable(const callable& r);  // step 1  // this protect
};

static boost::thread copies_are_safe() {
  callable x;
  return boost::thread(x);  // create copy
}

void f()
{
  boost::thread some_thread=copies_are_safe();
  some_thread.join();
}


TEST(Parallel, ExecutionExceptionsAndInterraption) {
  // Watch - interruption points
  f();
  //TODO: interrupt() - не сразу, а в точке, но как быть с исключениями
}

TEST(Parallel, SyncLocks) {
  // !! http://home.roadrunner.com/~hinnant/mutexes/locking.html
  // Locable

  // TimeLocable

  // SharedLocable - MRd-Single Wr

  // UpgradeLocable
  // http://stackoverflow.com/questions/7378266/upgrading-boostshared-lock-to-exclusive-lock
  // http://stackoverflow.com/questions/4203467/multiple-readers-single-writer-locks-in-boost?rq=1
  // http://stackoverflow.com/questions/6752880/how-to-upgrade-mutex-ownership-from-shared-to-upgrade
}

TEST(Parallel, Multilock) {
    // http://www.justsoftwaresolutions.co.uk/threading/acquiring-multiple-locks-without-deadlock.html
}

TEST(Parallel, CondVars) {

}

TEST(Parallel, Barrier) {

}

