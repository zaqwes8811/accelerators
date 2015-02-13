// Thinking
// http://rsdn.ru/forum/cpp.applied/5468499.flat.2
// Buildeng boost
//   bjam --with-system --with-thread --with-date_time --with-regex --with-serialization stage

#define BOOST_THREAD_PROVIDES_FUTURE

//https://github.com/mirror/boost/blob/master/libs/thread/example/executor.cpp
//#define BOOST_THREAD_VERSION 4
//#define BOOST_THREAD_PROVIDES_EXECUTORS
//#define BOOST_THREAD_USES_LOG_THREAD_ID
//#define BOOST_THREAD_QUEUE_DEPRECATE_OLD

#include "pools/thread_pools.h"

//#include <boost/thread/thread_pool.hpp>
//#include <boost/thread/user_scheduler.hpp>
//#include <boost/thread/executor.hpp>
//#include <boost/thread/future.hpp>
//#include <boost/assert.hpp>
//#include <string>
//#include <boost/thread/caller_context.hpp>

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
#include <boost/make_shared.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

#include <cstdio>


//using boost::unique_future;

// Non-Boost yet
//#include <boost/threadpool.hpp>

namespace {
//using namespace std;
boost::mutex m_io_monitor;
boost::mutex mio; // 1

using std::string;
using std::cout;
using std::ostringstream;


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

std::string funcReturnStringWithParams(std::string & param)
{
  //boost::this_thread::sleep()
  //sleep(3);
  return param;
}

void logMsg(const std::string& reason, int i)
{
  static int k;
  //++k;

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

int no_safe_func()
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

// http://stackoverflow.com/questions/14422319/boost-threading-conceptualization-questions
// Non compiled
//
// http://www.mr-edd.co.uk/blog/
class Data {

};

int run_sim(Data*) { return 0; }
const unsigned nsim = 50;

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

namespace tests {
using boost::shared_ptr;
using boost::make_shared;
using boost::bind;

class Templ {
public:
  std::pair<int, int> doTheThing(
      const int& in
      ) {
    return std::make_pair(0, 9);
  }
};

TEST(ThPool, AsioVersionCheck) {
  // http://www.boost.org/doc/libs/1_47_0/libs/exception/doc/tutorial_exception_ptr.html
  run(4);  // если нет планировщика, то лучше потоков побольше чем ядер
}

TEST(ThPool, OwnAsioPool) {
  using boost::packaged_task;
  using boost::future;
  using boost::ref;
  using boost::cref;
  using boost::bind;
  using std::cout;
  using std::endl;

  thread_pools::AsioThreadPool p;
  Templ templ;

  int in = 9;

  // http://stackoverflow.com/questions/17286458/use-member-function-in-stdpackaged-task
  packaged_task<std::pair<int, int> >
      t(bind(bind(&Templ::doTheThing, ref(templ), _1), cref(in)));
  future<std::pair<int, int> > f_memb = t.get_future();

  packaged_task<std::string> task(bind(&funcReturnStringWithParams, ans));
  future<string> f = task.get_future();

  p.get().post(bind(&packaged_task<std::string>::operator(), boost::ref(task)));
  p.get().post(bind(&packaged_task<std::pair<int, int>  >::operator(), boost::ref(t)));

  //EXPECT_FALSE(f.is_ready());
  while (!f.is_ready()) {
    cout << "wait" << endl;
  }

  string answer = f.get();
  std::pair<int, int> r = f_memb.get();
  std::cout << r.second << " string_with_params: " << answer << std::endl;
}

// http://stackoverflow.com/questions/19572140/how-do-i-utilize-boostpackaged-task-function-parameters-and-boostasioio
// http://stackoverflow.com/questions/4084777/creating-a-thread-pool-using-boost/4085345#4085345
// http://think-async.com/Asio/Recipes
// DANGER: а как с ошибками то быть?
TEST(ThPool, AsioBase) {
  using boost::shared_ptr;
  using boost::make_shared;  // not work
  using boost::bind;
  using std::string;

  boost::asio::io_service io_service;
  boost::thread_group threads;
  boost::asio::io_service::work work(io_service);

  for (int i = 0; i < 3; ++i)
    threads.create_thread(bind(&boost::asio::io_service::run, &io_service));

  typedef boost::packaged_task<std::string> task_t;
  shared_ptr<task_t> example = boost::make_shared<task_t>(bind(&funcReturnStringWithParams, ans));
  boost::future<string> f = example->get_future();
  
  // одну задачу
  io_service.post(boost::bind(&task_t::operator(), example));

  //EXPECT_FALSE(f.is_ready());

  string answer = f.get();
  std::cout << "string_with_params: " << answer << std::endl;

  // stop pull
  io_service.stop();
  threads.join_all();
}

// Не все хорошо стыкуется если C++98
// С С++11 похоже есть расходжения
// DANGER: похоже лучше свой пул.
TEST(ThPool, SFLib) //void func()
{ 
  /*using namespace boost::threadpool;
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
    
  }*/

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
  boost::packaged_task<int> task(no_safe_func);
  boost::future<int> fi = task.get_future();
  boost::thread thread(boost::move(task));  // если закомменить, то деадлок
  EXPECT_THROW(fi.get(), std::runtime_error);

  //
  //boost::future<int> fii =
      //boost::async(task_int_1);
  //decay_copy(boost::forward<F>(f))();
  //fii.get();
}

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

}  // space

