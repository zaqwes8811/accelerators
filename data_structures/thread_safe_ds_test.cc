/// Concurent core info:
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2406.html
//
// coupled - Queue
// https://www.quantnet.com/threads/c-multithreading-in-boost.10028/
// http://stackoverflow.com/questions/10139251/shared-queue-in-c
//
// concurent queue
// http://stackoverflow.com/questions/16275112/boost-c-lock-free-queue-vs-shared-queue
// http://www.alexey-martynov.pp.ru/index.php?data=articles&article=mt-queue
// http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
// http://stackoverflow.com/questions/6959023/c-producer-consumer-queue-with-very-fast-and-reliable-handover
// http://blog.ruslans.com/2013/08/introduction-to-high-level.html
//
//
// Finded tut.:
//   https://computing.llnl.gov/tutorials/pthreads/
//   https://computing.llnl.gov/tutorials/parallel_comp/
//
//
// Concurent code style
//   http://www.codingstandard.com/section/18-concurrency/

// C
#include <pthread.h>

// C++
#include <list>
#include <ostream>

// C++11
#include <mutex>
#include <thread>

// 3rdparty
#include <gtest/gtest.h>

// !!
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_algorithms.hpp>

namespace interfaces_mt_ds {
/// Существующие интерфейсы
// Boost - lookfree ds
// TBB - 
// С++11 - потокобезопасные контейнеры - таких там нет
//   http://stackoverflow.com/questions/15278343/c11-thread-safe-queue

// Intel TBB
template <typename T>
class concurent_queue {
public:
  concurent_queue() {}
  
  void push(const T& source);
  bool try_pop(T& destination);
  
  bool empty() const;  // may not be precist cos pending 
private:
  concurent_queue(const concurent_queue&);
  concurent_queue& operator=(const concurent_queue&);
};

template <typename T>
class concurent_bounded_queue {
public:
  concurent_bounded_queue() {}
  
  void push(const T& source);
  bool try_pop(T& destination);
  
  bool empty() const;
private:
  concurent_bounded_queue(const concurent_bounded_queue&);
  concurent_bounded_queue& operator=(const concurent_bounded_queue&);
};
};

using std::list;

class Fake {
public:
  Fake() {
    std::cout << "ctor\n";
  }
  Fake(const Fake& rhs) {
    std::cout << "copy ctor\n";
  }
  Fake& operator=(const Fake& rhs) {
    std::cout << "assign operator\n";
    return *this;
  }
private:
  Fake(Fake&&) = delete;
  Fake& operator=(Fake&&) = delete;
};

// TODO: очередь от Шена
// http://channel9.msdn.com/Events/GoingNative/2013/Cpp-Seasoning
//
// В вопросах кто-то не лестно отозвался о реализации, но что он сказал?
template <typename T>
class concurent_queue 
{
  std::mutex mutex_;
  list<T> q_;
public:
  void enqueue(T x) {
    // allocation here
    list<T> tmp;
    tmp.push_back(move(x));
    
    // threading
    {
      std::lock_guard<std::mutex> lock(mutex_);
      // вроде бы константное время
      q_.splice(end(q_), tmp);
      
      // для вектора может неожиданно потреб. реаллокация
    }
  }
  
  // ...
};

void foo(Fake& fake) {
  Fake tmp;
  fake = tmp;  // assign
}
TEST(Lang, LRefThinking) {
  Fake f;
  foo(f);
}

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

TEST(DS, TBBAssignTest) {
  
}

