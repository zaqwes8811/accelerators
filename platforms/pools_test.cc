//
// Задачи должны быть завершаемые.
//
// http://www.threadingbuildingblocks.org/docs/help/index.htm#tbb_userguide/Task-Based_Programming.htm
// "..algorithms composed from non-blocking tasks."
// I/O or mutex
// http://www.ibm.com/developerworks/library/j-jtp0730/
//
// Alternatives:
//   http://www.ibm.com/developerworks/library/j-jtp0730/ -  thread-per-task
//
// Scheduler:
//  http://www.threadingbuildingblocks.org/docs/help/index.htm#tbb_userguide/Task-Based_Programming.htm
//
// Thinks:
//   ошибки лучше передавать в вызывающую функцию. future и прочие работают, но как скрестить с asio
//
//
// Effective pools:
//  - Sutter
//  - http://www.ibm.com/developerworks/library/j-jtp0730/
//    "Understand your tasks." - может для разных типов задач разные пулы

#define BOOST_THREAD_PROVIDES_FUTURE

/*
//https://github.com/mirror/boost/blob/master/libs/thread/example/executor.cpp
#define BOOST_THREAD_VERSION 4
#define BOOST_THREAD_PROVIDES_EXECUTORS
#define BOOST_THREAD_USES_LOG_THREAD_ID
#define BOOST_THREAD_QUEUE_DEPRECATE_OLD

#include <boost/thread/thread_pool.hpp>
#include <boost/thread/user_scheduler.hpp>
#include <boost/thread/executor.hpp>
#include <boost/thread/future.hpp>
#include <boost/assert.hpp>
#include <string>
#include <boost/thread/caller_context.hpp>
*/

#include <cstdio>

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

#include <gtest/gtest.h>

// TROUBLE: для верий буста выше некоторой нужно TIME_UTC -> TIME_UTC_ - что-то связано с С11
#include <boost/version.hpp>
#if BOOST_VERSION < 105000
#include <boost/thread/xtime.hpp>
namespace boost {
  enum xtime_compat {
    //TIME_UTC_=TIME_UTC
  };
}
#endif

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/future.hpp>
#include <boost/throw_exception.hpp>  // sudden
#include <boost/bind.hpp>
#include <boost/exception_ptr.hpp>

//using boost::unique_future;

// Non-Boost yet
#include <boost/threadpool.hpp>

// Hand-maded threadpool
//   http://stackoverflow.com/questions/22284557/thread-pool-implementation-using-pthreads
//   http://rextester.com/discussion/XHA86284/ThreadPool
//   http://www.youtube.com/watch?v=FfbZfBk-3rI

// TODO: Try http://blog.think-async.com/2008/10/asynchronous-forkjoin-using-asio.html
//   но это не на пуле, это просто так
//
// About own thread pool
//   http://codereview.stackexchange.com/questions/40536/simple-thread-pool-in-c
//
// http://stackoverflow.com/questions/19500404/how-to-create-a-thread-pool-using-boost-in-c
// Похоже нужен планеровщик. Не очень понял последную часть
//
// http://thisthread.blogspot.co.il/2011/04/multithreading-with-asio.html
namespace
{
boost::mutex mio; // 1

void logMsg(const std::string& reason, int i)
{
  boost::lock_guard<boost::mutex> lock(mio); // 2.
  std::cout << reason << " for " << i << " at " << std::time(0) << std::endl;
}

void jobOne(int secs) // 3.
{
  logMsg("Start jobOne", secs);
  boost::this_thread::sleep(boost::posix_time::millisec(1000 * secs));
  logMsg("End jobOne", secs);
}

void jobTwo(int millisecs, boost::exception_ptr& error)
{
  try {
    logMsg("Start jobTwo", millisecs);
    boost::this_thread::sleep(boost::posix_time::millisec(millisecs));
    logMsg("End jobTwo", millisecs);
    logMsg("Throw()", millisecs);
    BOOST_THROW_EXCEPTION(std::runtime_error(""));
  } catch (...) {
    error = boost::current_exception();
  }
}

void run(int tNumber) 
{
  /*boost::asio::io_service svc;
  boost::asio::io_service::work work(svc);
  boost::thread_group threads;
  
  // Как получить исключение
  for(int i = 0; i < tNumber; ++i) // 4
    threads.create_thread(boost::bind(&boost::asio::io_service::run, &svc));
 
  svc.post(boost::bind(jobOne, 1)); // 5
  svc.post(boost::bind(jobOne, 1));
  
  boost::exception_ptr error;
  svc.post(boost::bind(jobTwo, 50, boost::ref(error)));
 
  svc.stop(); // 6
  threads.join_all(); // 7
  
  if (error)
    boost::rethrow_exception(error);
  */
  
  // https://gist.github.com/snaewe/1393807
  boost::asio::io_service svc;  // 2.
  boost::thread_group threads;
 
  boost::exception_ptr error;  // похоже одна на поток
  {
    std::auto_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(svc)); //3.
    for (int i = 0; i < tNumber; ++i)   // 4.
      threads.create_thread (boost::bind (&boost::asio::io_service::run, &svc));
 
    svc.post(boost::bind(jobOne, 2)); // 5.
    svc.post(boost::bind(jobOne, 1));
    
    svc.post(boost::bind(jobTwo, 50, boost::ref(error)));
 
    //svc.stop ();          // 6.
  }
  threads.join_all ();      // 7.

  EXPECT_TRUE(error);
  EXPECT_THROW(boost::rethrow_exception(error), std::runtime_error);

}

TEST(ThPool, AsioVersionCheck) {
  // http://www.boost.org/doc/libs/1_47_0/libs/exception/doc/tutorial_exception_ptr.html
  run(4);  // если нет планировщика, то лучше потоков побольше чем ядер
}


std::string ans("forty two");

int int_no_params()
{
  return 42;
}

int int_with_params(int param)
{
  return param;
}

std::string string_no_params()
{
  return std::string("forty two");
}

std::string string_with_params(std::string & param)
{
  return param;
}

// http://stackoverflow.com/questions/19572140/how-do-i-utilize-boostpackaged-task-function-parameters-and-boostasioio
// http://stackoverflow.com/questions/4084777/creating-a-thread-pool-using-boost/4085345#4085345
// http://think-async.com/Asio/Recipes
// DANGER: а как с ошибками то быть?
TEST(ThPool, Asio) {
  typedef boost::packaged_task<std::string> task_t;
  boost::asio::io_service io_service;
  boost::thread_group threads;
  boost::asio::io_service::work work(io_service);

  for (int i = 0; i < 3; ++i)
    threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));

  
  boost::shared_ptr<task_t> example =  boost::make_shared<task_t>(boost::bind(&string_with_params, ans));
  // unique_
  boost::future<std::string> f = example->get_future();
  
  // одну задачу
  io_service.post(boost::bind(&task_t::operator(), example));
  std::string answer = f.get();
  std::cout << "string_with_params: " << answer << std::endl;
  io_service.stop();
  
  threads.join_all();
}

}

// Pools
//   http://stackoverflow.com/questions/16677287/boostthreadpoolpool-vs-boostthread-group
//   http://stackoverflow.com/questions/19500404/how-to-create-a-thread-pool-using-boost-in-c
//   http://techiesaint.wordpress.com/2012/12/16/implementing-threadpool-pattern-in-c-using-boostthread-pool/
//   http://www.tonicebrian.com/2012/05/23/thread-pool-in-c/

namespace {
using namespace std;
boost::mutex m_io_monitor;

void print(string text)
{
  boost::mutex::scoped_lock lock(m_io_monitor);
  cout << text;
}

template<typename T>
string to_string(T const & value)
{
  ostringstream ost;
  ost << value;
  ost.flush();
  return ost.str();
}

void task_1()
{
  print(string("  task_1()\n"));
  //throw 5;
}

void task_2()
{
  print(string("  task_2()\n"));
  throw 5;
}

void task_3()
{
  print(string("  task_3()\n"));
}

void task_with_parameter(int value)
{
  print("  task_with_parameter(" + to_string(value) + ")\n");
}

int task_int_1()
{
  //try {
    print("  task_int_1()\n");
    //throw 9;
    // http://www.boost.org/doc/libs/1_55_0/libs/exception/doc/frequently_asked_questions.html
    // https://groups.google.com/forum/#!topic/boost-list/E0C_gZDuydk
    BOOST_THROW_EXCEPTION(std::runtime_error(""));
    //boost::throw_exception(std::runtime_error(""));
    //error = boost::exception_ptr();
    return 1;
  //} catch (...) {
    //error = boost::current_exception();
  //}
}

// Не все хорошо стыкуется если C++98
// С С++11 похоже есть расходжения
// DANGER: похоже лучше свой пул.
TEST(ThPool, SFLib) //void func()
{ 
  using namespace boost::threadpool;
  //using boost::unique_future;
  
  //print("  Create a new thread pool\n");
  pool tp(2); // tp is handle to the pool

  // Add tasks
  tp.schedule(&task_1);
  //unique_future<void> f = tp.schedule(&task_2);
  tp.schedule(&task_3);
  tp.schedule(boost::bind(task_with_parameter, 4));
  //boost::threadpool::future<int> res = schedule(tp, &task_int_1);
  //unique_future<int> res1 =
  //schedule(tp, &task_int_1);  // th::fut - не для продакшена, а так не ловит исключения
  try {
    //res.get();
  } catch (runtime_error& e) {
    
  }

  // The pool handle tp is allocated on stack and will 
  // be destructed if it gets out of scope. Before the 
  // pool is destroyed it waits for its tasks. 
  // That means the current thread of execution is 
  // blocked at the end of the function 
  // (until all tasks are processed).
  // while (&tp){int i = 3; ++i;}
} 

// !!! http://stackoverflow.com/questions/19572140/how-do-i-utilize-boostpackaged-task-function-parameters-and-boostasioio
// Для портабельной передач исключений между потоками нужна фичи языка
// http://stackoverflow.com/questions/8876459/boost-equivalent-of-stdasync
TEST(ThPool, Own) {
  boost::packaged_task<int> task(task_int_1);
  boost::future<int> fi = task.get_future();
  boost::thread thread(boost::move(task));  // если закомменить, то деадлок
  EXPECT_THROW(fi.get(), std::runtime_error);

  //
  //boost::future<int> fii =
      //boost::async(task_int_1);
  //decay_copy(boost::forward<F>(f))();
  //fii.get();
}


// http://stackoverflow.com/questions/14422319/boost-threading-conceptualization-questions
// Non compiled
//
// http://www.mr-edd.co.uk/blog/
class Data {

};

int run_sim(Data*) { return 0; }
const unsigned nsim = 50;
/*
TEST(TMP, Test) {
  // https://github.com/mirror/boost/blob/master/libs/thread/example/executor.cpp
  //boost::executor_adaptor<boost::thread_pool> ea;

  unsigned nprocs = boost::thread::hardware_concurrency();
  if (nprocs == 0)
    nprocs = 2;   // cannot determine number of cores, let's say 2

  Data data[nsim];
  boost::future<int> futures[nsim];
  int results[nsim];

  for (unsigned i=0; i<nsim; ++i)
  {
    if ( ((i+1) % nprocs) != 0 ) {
      // DANGER:
      //futures[i] =
          //boost::async(boost::bind(&run_sim, &data[i]));  // не компилируется
      ;
    } else
      results[i] = run_sim(&data[i]);
  }

  for (unsigned i=0; i<nsim; ++i)
    if ( ((i+1) % nprocs) != 0 )
      results[i] = futures[i].get();
}
*/
}

