// http://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock
//
// https://www.quantnet.com/threads/c-multithreading-in-boost.10028/
//
// Lock != mutex и может строится на разных примитивах

// Asio:
// http://meetingcpp.com/tl_files/mcpp/slides/12/Boost.Asio%20-%20Asynchronous%20IO.pdf

#include <gtest/gtest.h>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <vector>
#include <iostream>

using boost::thread;
using std::vector;
using boost::shared_ptr;
using std::cout;
using std::endl;

// http://stackoverflow.com/questions/4260209/creating-not-copyable-but-movable-objects-in-c
// thread - movable
class callable {  // must be copyable
public:
  callable() {}  // step 1
  void operator()() {  // можно и с аргументами
    // boost::this_thread::disable_interruption di;
    // можно запрещать и разреашать прерывание потока

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
  // http://www.boost.org/doc/libs/1_55_0/doc/html/thread/thread_management.html
  // Watch - interruption points
  f();
  //TODO: interrupt() - не сразу, а в точке, но как быть с исключениями

  thread t = copies_are_safe();
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

// Asio
// http://stackoverflow.com/questions/11191028/boostasio-how-to-interrupt-a-blocked-tcp-server-thread
void startAccept( boost::asio::ip::tcp::acceptor& );

void serverThreadFunc( boost::asio::io_service& io_service )
{
  using boost::asio::ip::tcp;
  using boost::asio::ip::udp;
  tcp::acceptor acceptor( io_service, tcp::endpoint( tcp::v4(), 8080 ) );

  // Add a job to start accepting connections.
  startAccept( acceptor );

  // Process event loop.
  io_service.run();

  std::cout << "Server thread exiting." << std::endl;
}

void handleAccept( const boost::system::error_code& error,
                   boost::shared_ptr< boost::asio::ip::tcp::socket > socket,
                   boost::asio::ip::tcp::acceptor& acceptor )
{
  // If there was an error, then do not add any more jobs to the service.
  if ( error )
  {
    std::cout << "Error accepting connection: " << error.message()
              << std::endl;
    return;
  }

  // Otherwise, the socket is good to use.
  std::cout << "Doing things with socket..." << std::endl;

  // Perform async operations on the socket.

  // Done using the socket, so start accepting another connection.  This
  // will add a job to the service, preventing io_service::run() from
  // returning.
  std::cout << "Done using socket, ready for another connection."
            << std::endl;
  startAccept( acceptor );
}

void startAccept( boost::asio::ip::tcp::acceptor& acceptor )
{
  using boost::asio::ip::udp;
  using boost::asio::ip::tcp;
  boost::shared_ptr< tcp::socket > socket(
                                new tcp::socket( acceptor.get_io_service() ) );

  // Add an accept call to the service.  This will prevent io_service::run()
  // from returning.
  std::cout << "Waiting on connection" << std::endl;
  acceptor.async_accept( *socket,
    boost::bind( handleAccept,
      boost::asio::placeholders::error,
      socket,
      boost::ref( acceptor ) ) );
}

TEST(Parallel, AsioTerminate)
{
  using boost::asio::ip::tcp;

  // Create io service.
  boost::asio::io_service io_service;

  // Create server thread that will start accepting connections.
  boost::thread server_thread( serverThreadFunc, boost::ref( io_service ) );

  // Sleep for 10 seconds, then shutdown the server.
  std::cout << "Stopping service in 10 seconds..." << std::endl;
  boost::this_thread::sleep( boost::posix_time::seconds( 1 ) );
  std::cout << "Stopping service now!" << std::endl;

  // Stopping the io_service is a non-blocking call.  The threads that are
  // blocked on io_service::run() will try to return as soon as possible, but
  // they may still be in the middle of a handler.  Thus, perform a join on
  // the server thread to guarantee a block occurs.
  io_service.stop();

  std::cout << "Waiting on server thread..." << std::endl;
  server_thread.join();
  std::cout << "Done waiting on server thread." << std::endl;
}

namespace {
// http://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/tutorial/tutdaytime6.html
// http://stackoverflow.com/questions/4808848/boost-asio-stopping-io-service
using boost::asio::ip::udp;
//using boost::asio;//::placeholders;
using boost::bind;
using boost::asio::buffer;

class udp_async_server {
public:
  // bind
  udp_async_server(boost::asio::io_service& io_service)
    : m_socket(io_service)
    , m_rBuffer(new vector<char>(1024, 0))
  {
    m_socket.open(udp::v4());
    m_socket.bind(udp::endpoint(udp::v4(), 1036));
    start_receive();
  }

  // http://stackoverflow.com/questions/15027268/how-to-close-boost-asio-server-socket-with-all-client-sockets-connected
  //m_socket.shutdown();
  ~udp_async_server()
  { m_socket.close(); }

private:
  void start_receive() {
    m_socket.async_receive_from(
          buffer(&(*m_rBuffer)[0], m_rBuffer->size())
        , m_remoteEndpoint
        , bind(&udp_async_server::handle_receive, this
             , boost::asio::placeholders::error
             , boost::asio::placeholders::bytes_transferred));
  }

  void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error || error == boost::asio::error::message_size) {
      // process request

      // make response
      boost::shared_ptr<std::string> message(new std::string());

      // async_send not compiled
      m_socket.async_send_to(
          buffer(*message)
          , m_remoteEndpoint
          , bind(&udp_async_server::handle_send, this
              , message
              , boost::asio::placeholders::error
              , boost::asio::placeholders::bytes_transferred));

      start_receive();  // !!
    }
  }

  /*message*/
  /*error*/
  /*bytes_transferred*/
  void handle_send(boost::shared_ptr<std::string>, const boost::system::error_code& , std::size_t) { }

  // diff. obj. can used from diff threads
  udp::socket m_socket;
  shared_ptr<vector<char> > m_rBuffer;//(new vector<char>(1024, 0));
  udp::endpoint m_remoteEndpoint;
};

class AppScopeFake {
public:
  bool wasTerminated() const { return s_wasTerminated; }
  void terminate() { s_wasTerminated = true; }
private:
  static bool s_wasTerminated;
};

typedef enum kernels_Codes {
  UNKNOWN_EXCEPTION,
  BOOST_EXCEPTION,
  APPLICATION_WAS_TERMINATED
} kernels_Codes;

int udpServerKernel( boost::asio::io_service& io_service, AppScopeFake app) {
  try {
    udp_async_server server(io_service);
    //io_service.run();  // blocked if work loaded?

    // doc 520 - can cycle
    // FIXME: heat processor
    while(true) {
      io_service.poll_one();  // can't stop
      if (app.wasTerminated())
        return APPLICATION_WAS_TERMINATED;
    }
  } catch(boost::system::system_error& e) {
    return BOOST_EXCEPTION;
  } catch(...) {
    return UNKNOWN_EXCEPTION;
  }
}

bool AppScopeFake::s_wasTerminated = false;

TEST(Parallel, Udp) {
  AppScopeFake app;

  // http://stackoverflow.com/questions/4702512/stopping-threaded-server-loop-using-boostasio
  // http://stackoverflow.com/questions/15027268/how-to-close-boost-asio-server-socket-with-all-client-sockets-connected
  //
  // http://stackoverflow.com/questions/11010530/do-we-need-multiple-io-service-per-thread-for-threaded-boostasio-server-with
  boost::asio::io_service io_service;
  // Create server thread that will start accepting connections.
  boost::thread server_thread( udpServerKernel, boost::ref( io_service ), app);

  // Sleep for 10 seconds, then shutdown the server.
  std::cout << "Stopping service in 10 seconds..." << std::endl;
  boost::this_thread::sleep( boost::posix_time::seconds( 1 ) );
  std::cout << "Stopping service now!" << std::endl;
  app.terminate();

  // Stopping the io_service is a non-blocking call.  The threads that are
  // blocked on io_service::run() will try to return as soon as possible, but
  // they may still be in the middle of a handler.  Thus, perform a join on
  // the server thread to guarantee a block occurs.
  io_service.stop();

  std::cout << "Waiting on server thread..." << std::endl;
  server_thread.join();
  std::cout << "Done waiting on server thread." << std::endl;
}
}  // space
