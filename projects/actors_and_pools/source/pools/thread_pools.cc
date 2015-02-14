#include "thread_pools.h"

#include <boost/bind.hpp>

namespace thread_pools {
AsioThreadPool::AsioThreadPool()
  : m_io_service()
  , m_threads()
  , m_work(m_io_service)
{
  for (int i = 0; i < 3; ++i) {

    // create for own
    m_threads.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
  }
}

AsioThreadPool::AsioThreadPool(int countWorkers)
  : m_io_service()
  , m_threads()
  , m_work(m_io_service)
{
  for (int i = 0; i < countWorkers; ++i)
    m_threads.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
}


AsioThreadPool::~AsioThreadPool() {
  m_io_service.stop();
  m_threads.join_all();
}

boost::asio::io_service& AsioThreadPool::get() {
  return m_io_service;
}

}
