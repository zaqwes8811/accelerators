// 0. Use Threads Correctly = Isolation + Asynchronous Messages
// 1. Prefer Using Active Objects Instead of Naked Threads
//
// Pull and Push strategue

#define BOOST_THREAD_PROVIDES_FUTURE

#include "parallel_ds/safe_queue.h"
#include "actors.h"

#include <gtest/gtest.h>
#include <boost/thread/future.hpp>

#include <memory>
#include <thread>
#include <functional>
#include <string>
#include <future>

namespace e1_cc98 {
using std::unique_ptr;
using std::thread;
using std::bind;
using std::shared_ptr;

// Example 1: Active helper, the general OO way
//
class Active {
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
  shared_ptr<Message> done;               // le sentinel
  //message_queue
  SafeQueue
  < shared_ptr<Message> > mq;    // le queue

  unique_ptr<thread> thd;                // le thread
private:
  // The dispatch loop: pump messages until done
  void Run() {
    shared_ptr<Message> msg;
    while( (msg = mq.dequeue()) != done ) {
      msg->Execute();
    }
  }

public:
  // Start everything up, using Run as the thread mainline
  Active() : done( new Message ) {
    thd = unique_ptr<thread>(
                  new thread( bind(&Active::Run, this) ) );
  }

  // Shut down: send sentinel and wait for queue to drain
  ~Active() {
    Send( done );
    thd->join();
  }

  // Enqueue a message
  void Send( shared_ptr<Message> m ) {
    mq.enqueue( m );
  }
};
}



class Backgrounder {
public:
  typedef int Data;
  typedef int PrivateData;

  void Save( std::string filename ) { a.Send( [=] {
    //…
  } ); }

  void Print( Data& data ) { a.Send( [=, &data] {
    //…
  } ); }

private:
  PrivateData somePrivateStateAcrossCalls;
  e_cc11::Active a;
};

TEST(Sutter, ActiveObj) {
  Backgrounder b;
  b.Save("hello");
}

// result
// http://www.drdobbs.com/cpp/prefer-using-futures-or-callbacks-to-com/226700179?pgno=1
class Backgrounder_ret {
public:
  typedef int Data;
  typedef int PrivateData;

  std::future<bool> Save( std::string filename ) {
    using std::future;
    using std::promise;
    auto p = std::make_shared<promise<bool> > ();
    future<bool> ret = p->get_future();
    a.Send( [=] { p->set_value( true ); } );
    return ret;
  }

  void Print( Data& data ) { a.Send( [=, &data] {
    //…
  } ); }

private:
  PrivateData somePrivateStateAcrossCalls;
  e_cc11::Active a;
};

class BackgrounderBoost {
public:
  typedef int Data;
  typedef int PrivateData;

  boost::future<bool> Save( std::string filename ) {
    using boost::future;
    using boost::promise;
    auto p = std::make_shared<promise<bool> > ();
    future<bool> ret = p->get_future();
    a.Send( [=] { p->set_value( true ); } );
    return ret;
  }

  void Print( Data& data ) { a.Send( [=, &data] {
    //…
  } ); }

  void Save(
      std::string filename,
      std::function<void(
        //bool
        )> returnCallback
    ) {
    a.Send( [=] {
      // … do the saving work …
      returnCallback( );//true );//didItSucceed() ? true : false );
    } ); }

private:
  PrivateData somePrivateStateAcrossCalls;
  e_cc11::Active a;
};

TEST(Sutter, PullReturn) {
  Backgrounder_ret b;
  auto r = b.Save("h");

  // "pull" ops!!
  r.get();
}

class MyGUI {
public:
  // …

  // When the user clicks [Save]
  void OnSaveClick() {
    // …
    // … turn on saving icon, etc. …
    // …
    std::shared_future<bool> result;
    result = backgrounder.Save( "filename" );
    // queue up a continuation to notify ourselves of the
    // result once it's available

    // FIXME: Is it bad?
    std::async( [=] {
        //SendSaveComplete
            OnSaveComplete
            ( result.get() ); } );
  }


  void OnSaveComplete( bool returnedValue ) {
    // … turn off saving icon, etc. …
  }
private:
  Backgrounder_ret backgrounder;
};

class MyGUI_2b {
public:
  // …

  // When the user clicks [Save]
  void OnSaveClick() {
    // …
    // … turn on saving icon, etc. …
    // …
    boost::shared_future<bool> result;
    result = backgrounder.Save( "filename" );
    // queue up a continuation to notify ourselves of the
    // result once it's available

    // FIXME: Is it bad?
    // no then() in std and boost
    //result.( [=] {
    //    //SendSaveComplete
    //        OnSaveComplete
    //       ( result.get() ); } );
  }

  void OnSaveClick_cb() {
    //boost
    //std::shared_future<bool> result;  // FIXME: Is it needed here?
    backgrounder.Save( std::string("filename"),
           ([=]
      {
        //OnSaveComplete( result.get() );
      })
    );
  }

  void OnSaveComplete( bool returnedValue ) {
    // … turn off saving icon, etc. …
  }
private:
  BackgrounderBoost backgrounder;
};


// FIXME: partial result

// Build async API
// cases
//




