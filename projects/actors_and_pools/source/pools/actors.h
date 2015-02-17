// (c) from Sutter

#ifndef S_ACTORS_H_
#define S_ACTORS_H_

#include "pools/safe_queue.h"

#include <boost/noncopyable.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>

#include <thread>
#include <memory>

namespace executors {
class AsioThreadPool : public boost::noncopyable {
public:
  typedef boost::function0<void> Func;

  AsioThreadPool()
    : m_io_service()
    , m_threads()
    , m_work(m_io_service)
  {
      m_threads.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
  }

  explicit AsioThreadPool(int countWorkers)
    : m_io_service()
    , m_threads()
    , m_work(m_io_service)
  {
    for (int i = 0; i < countWorkers; ++i)
      m_threads.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
  }

  ~AsioThreadPool() {
    m_io_service.stop();
    m_threads.join_all();
  }

public:
  void add(Func f) {
    m_io_service.post(f);
  }

private:
  boost::asio::io_service m_io_service;
  boost::thread_group m_threads;
  boost::asio::io_service::work m_work;
};
}


namespace cc11 {
// Example 2: Active helper, in idiomatic C++(0x)
//
class Actior {
public:
  typedef std::function<void()> Message;

private:

  Actior( const Actior& );           // no copying
  void operator=( const Actior& );    // no copying

  bool done;                         // le flag
  SafeQueue<Message> mq;        // le queue
  std::unique_ptr<std::thread> thd;          // le thread

  void Run() {
    while( !done ) {
      Message msg = mq.dequeue();
      msg();            // execute message
    } // note: last message sets done to true
  }

public:

  Actior() : done(false) {
    thd = std::unique_ptr<std::thread>(
                  new std::thread( [=]{ this->Run(); } ) );
  }

  ~Actior() {
    Send( [&]{ done = true; } ); ;
    thd->join();
  }

  void Send( Message m ) {
    mq.enqueue( m );
  }
};
}

namespace cc98 {
// Example 1: Active helper, the general OO way
//
class Actor {
public:

  class Message {        // base of all message types
  public:
    virtual ~Message() { }
    virtual void Execute() { }
  };

private:
  // (suppress copying if in C++)

  // private data
  // unique_ptr not assing op
  boost::shared_ptr<Message> done;               // le sentinel
  SafeQueue< boost::shared_ptr<Message> > mq;    // le queue
  boost::scoped_ptr<boost::thread> thd;                // le thread
private:
  // The dispatch loop: pump messages until done
  void Run() {
    boost::shared_ptr<Message> msg;
    while( (msg = mq.dequeue()) != done ) {
      msg->Execute();
    }
  }

public:
  // Start everything up, using Run as the thread mainline
  Actor() :
      done( new Message )
    , thd(new boost::thread( boost::bind(&Actor::Run, this)))
    { }

  // Shut down: send sentinel and wait for queue to drain
  ~Actor() {
    Send( done );
    thd->join();
  }

  // Enqueue a message
  void Send( boost::shared_ptr<Message> m ) {
    mq.enqueue( m );
  }
};
}

#endif
