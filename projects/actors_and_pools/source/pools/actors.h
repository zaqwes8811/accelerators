// (c) from Sutter

#ifndef S_ACTORS_H_
#define S_ACTORS_H_

#include "parallel_ds/safe_queue.h"

#include <thread>
#include <memory>

namespace e_cc11 {
using std::unique_ptr;
using std::thread;
using std::bind;
using std::shared_ptr;
using std::function;

// Example 2: Active helper, in idiomatic C++(0x)
//
class Active {
public:
  typedef function<void()> Message;

private:

  Active( const Active& );           // no copying
  void operator=( const Active& );    // no copying

  bool done;                         // le flag
  SafeQueue<Message> mq;        // le queue
  unique_ptr<thread> thd;          // le thread

  void Run() {
    while( !done ) {
      Message msg = mq.dequeue();
      msg();            // execute message
    } // note: last message sets done to true
  }

public:

  Active() : done(false) {
    thd = unique_ptr<thread>(
                  new thread( [=]{ this->Run(); } ) );
  }

  ~Active() {
    Send( [&]{ done = true; } ); ;
    thd->join();
  }

  void Send( Message m ) {
    mq.enqueue( m );
  }
};
}

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

#endif
