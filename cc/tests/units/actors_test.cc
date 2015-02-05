/**
  https://events.yandex.ru/lib/talks/2488/
*/

#define BOOST_THREAD_PROVIDES_FUTURE

#include "async-parallel/thread_pools.h"

#include <gtest/gtest.h>
#include <boost/bind/bind.hpp>

#include <stdio.h>

int task_int_1()
{
  //try {
    //print("  task_int_1()\n");
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



// http://www.chromium.org/developers/design-documents/threading
class SingleWorker
{
public:
  // typedefs
  typedef boost::function0<void> Callable;

  SingleWorker() : m_pool(1) { }



  void post(boost::function0<void> task) {
    m_pool.get().post(task);
  }

  // http://stackoverflow.com/questions/13157502/how-do-you-post-a-boost-packaged-task-to-an-io-service-in-c03
  //void post(packaged_task)  // no way, but...
  template<typename T>
  void post(boost::shared_ptr<boost::packaged_task<T> > task) {
    m_pool.get().post(boost::bind(
                        &boost::packaged_task<T>::operator (), task));
  }

private:
  thread_pools::AsioThreadPool m_pool;
};

class NonThreadSafeObj
{
public:
  // Return future is hard. In task store ref to pack. task
  // may store shared_ptr

  boost::future<std::string>
  //std::string
  getAjax() //const
  {
    // FIXME: lifetime questions about this!
    //boost::bind
  }

private:
  std::string ajaxApiCall() {
    return "hello";
  }

  SingleWorker m_worker;
};


TEST(AsPl, SingleThread)
{
  SingleWorker worker;

  boost::packaged_task<int> task(task_int_1);
  boost::future<int> fi = task.get_future();

  SingleWorker::Callable f
      = boost::bind(&boost::packaged_task<int>::operator(), boost::ref(task));
  worker.post(f);

  EXPECT_THROW(fi.get(), std::runtime_error);
}

TEST(AsPl, SingleThreadShared)
{
  SingleWorker worker;

  boost::shared_ptr<boost::packaged_task<int> >
      task = boost::make_shared<boost::packaged_task<int> >(task_int_1);
  boost::future<int> fi = task->get_future();

  worker.post(task);

  EXPECT_THROW(fi.get(), std::runtime_error);
}
