// MS PPL and TBB - operator= не потокобезопасный. Похоже это не всегда нужно.
// TROUBLE: В любом случае как быть если есть что-то помими мьютексов - условные перем. например
// http://bytes.com/topic/c/answers/493246-copying-mutexes-cv-pthread_ts
//
// Еще не безопасные, если происходить pending. Но для очередей лучше похоже вообще запрещать. В 
//   TBB так и сделано. Но остальные конейнеры похоже можно.
// Для некоторых разрешено создание копированием, но " Assignment construction is not allowed."
//
// вообще конструирование и присваивание не совсем одно и то же. Присваиваем существующема.

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
 
// TODO:
// thread-safe copy ctor and assign
// http://stackoverflow.com/questions/5070161/designing-a-thread-safe-copyable-class
// http://home.roadrunner.com/~hinnant/mutexes/locking.html
// Summary:
//   лучше сперва вообще запретить.
// http://stackoverflow.com/questions/13030272/how-to-implement-an-atomic-thread-safe-and-exception-safe-deep-copy-assignment
class A
{
  mutable std::mutex  mut_;
  std::vector<double> data_;

public:
  A() {}
  // ...

  A& operator=(const A& rhs)
  {
    if (this != &rhs)  // важно, чтобы не было самоблокировки
    {
      // Так захватывает он лок или нет?  
      std::unique_lock<std::mutex> lhs_lock(mut_,     std::defer_lock);
      std::unique_lock<std::mutex> rhs_lock(rhs.mut_, std::defer_lock);
      std::lock(lhs_lock, rhs_lock);  // похоже правильно обрабатывает a = a
      
      // V0
      //std::unique_lock<std::mutex> lhs_lock(mut_);
      //std::unique_lock<std::mutex> rhs_lock(rhs.mut_);  // deadlock!
        // assign data ...
      data_ = rhs.data_;
    }
    return *this;
  }

  
private:
  A(const A&);  // !!
    // ...
};
void task1(A* a, A* b)
{
    //std::cout << "task1 says: " << msg;
    for (int i = 0; i < 100000; ++i) {
      *a = *b;  // deadlock - v0
      ;
    }
}

class B {
  typedef boost::shared_mutex mutex_type;
  typedef boost::shared_lock<mutex_type> SharedLock;
  typedef boost::unique_lock<mutex_type> ExclusiveLock;
  
  // TODO: shared_lock = read_lock?
  
  mutable mutex_type mut_;
  std::vector<double> data_;

public:
  B& operator=(const B& rhs)
    {
        if (this != &rhs)
        {
            // assign data ...
            // expensive code here ... !!!
            ExclusiveLock lhs_lock(mut_, boost::defer_lock);  // свой эксклюзивный
            SharedLock rhs_lock(mut_, boost::defer_lock);
            boost::lock(lhs_lock, rhs_lock);
            data_ = rhs.data_;
        }
        return *this;
    }
//private:
  B(const B& rhs) {
    SharedLock _(rhs.mut_);
    data_ = rhs.data_;
  }
};

TEST(DS, Mutex) {
  std::mutex mut;
  
  // http://stackoverflow.com/questions/20516773/stdunique-lockstdmutex-or-stdlock-guardstdmutex
  std::unique_lock<std::mutex> lk(mut, std::defer_lock);
  assert(lk.owns_lock() == false);
  
  A a, b;
  
  std::thread t1(task1, &a, &b);
  std::thread t2(task1, &b, &a);
  std::thread t3(task1, &a, &a);
  std::thread t4(task1, &a, &a);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
}