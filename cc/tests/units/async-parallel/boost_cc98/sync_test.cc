// http://www.boost.org/doc/libs/1_55_0/doc/html/thread/synchronization.html
// http://www.boost.org/doc/libs/1_41_0/doc/html/thread/thread_management.html
 
#include <gtest/gtest.h>

#include <iostream>
#include <stdexcept>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/strict_lock.hpp>
#include <boost/thread/externally_locked.hpp>
#include <boost/thread/lockable_concepts.hpp>

using boost::mutex;

// "The object-level locking idiom doesn't cover the entire richness of a threading model."
using boost::lock_guard;
using boost::strict_lock;

class Tmp : public boost::basic_lockable_adapter<
  //mutex  // deadlock
  boost::recursive_mutex
  > {
public:
  Tmp() {}
private:
  Tmp(const Tmp&);
  Tmp& operator=(const Tmp&);
};
 
class BankAccount : public boost::basic_lockable_adapter<
  boost::recursive_mutex
  //mutex
  > {
//public:
  //mutable boost::mutex mtx_;
  int balance_;
public:
  BankAccount() : balance_(0) { }
  
  void Deposit(int amount) {
    //lock_guard<mutex> _(mtx_);
    lock_guard<BankAccount> _(*this);
    //mtx_.lock();
    balance_ += amount;
    //mtx_.unlock();
  }
  
  void Withdraw(int amount) {
    //lock_guard<mutex> _(mtx_);
    lock_guard<BankAccount> _(*this);
    //mtx_.lock();
    balance_ -= amount;
    //mtx_.unlock();
  }
  
  int GetBalance() /*const*/ {  // проблема! не компилируется
    //lock_guard<mutex> _(mtx_);
    lock_guard<BankAccount> _(*this);
    //mtx_.lock();
    int b = balance_;
    //mtx_.unlock();
    return b;
  }
  
  /*
  // Not work with guard;
  // make it lockable
  void lock() {
    mtx_.lock()
  }
  void unlock {
    mtx_.unlock();
  }*/
  
  // Extend interface
  void Deposit(int amount, boost::strict_lock<BankAccount>& guard) {
    if (!guard.owns_lock(this))  // разымиеновавать не нужно
      throw std::runtime_error("Locking Error: Wrong Object Locked");
    balance_ += amount;
  }
  
  void Withdraw(int amount, strict_lock<BankAccount>&) {
    // TODO: тоже нужна проверка
    balance_ -= amount;
  }
  
private:
  BankAccount(const BankAccount&);
  BankAccount& operator=(const BankAccount&);
};
 
BankAccount JoesAccount;

void bankAgent()
{
  for (int i = 10; i > 0; --i) {
    // ...  
    JoesAccount.Deposit(500);
    // ...
  }
}

void Joe() {
  for (int i = 10; i > 0; --i) {
    //...
    JoesAccount.Withdraw(100);
    int myPocket = JoesAccount.GetBalance();
    std::cout << myPocket << std::endl;
    //...
  }
}

void ATMWithdrawal(BankAccount& acc, int sum) {
  
  /*//lock_guard<mutex> _(acc.mtx_);  // not compiled
  // DEADLOCK: похоже на самоблокировку.
  lock_guard<BankAccount> _(acc);  // not compiled
  // No locks - V0
  acc.Withdraw(sum);
  acc.Withdraw(2);
  */
  
  // no safe really
  // Объект можно подменить и использовать его лок
  strict_lock<BankAccount> guard(acc);
  acc.Withdraw(sum, guard);
  acc.Withdraw(2, guard);
}

TEST(BoostSync, Begin) {
  boost::thread t1(bankAgent);
  boost::thread t2(Joe);
  
  // http://stackoverflow.com/questions/17219508/boost-threads-passing-parameters-by-reference
  Tmp t;
  boost::thread t3(ATMWithdrawal, boost::ref(JoesAccount), 900);
  t1.join();
  t2.join();
  t3.join();
}

namespace e_locking {
class BankAccount {
  int balance_;
public:
  void Deposit(int amount) {
      balance_ += amount;
  }
  void Withdraw(int amount) {
      balance_ -= amount;
  }
};


class AccountManager : public boost::basic_lockable_adapter<mutex> {
  //BankAccount checkAcct_;
  //BankAccount savingAcct_;
  
public:
  typedef basic_lockable_adapter<mutex> lockable_base_type;
  AccountManager() 
    : checkAcct_(*this), savingAcct_(*this) {}
    inline void Checking2Saving(int amount) {
        strict_lock<AccountManager> guard(*this);
        checkAcct_.get(guard).Withdraw(amount);
        savingAcct_.get(guard).Deposit(amount);
    }
    
    inline void AMoreComplicatedChecking2Saved(int amount) {
      /*
      // TODO: nested_guard
      // TODO: not compiled at all
      boost::unique_lock<AccountManager> guard1(*this, boost::defer_lock);
      if (true) {
        guard1.lock();
      }
      {
        // tranfer ownersheep
        strict_lock<AccountManager> guard(guard1);
        checkAcct_.get(guard).Withdraw(amount);
        savingAcct_.get(guard).Deposit(amount);
        
      }
      //checkAcct_.get(guard1).Withdraw(amount);
      //savingAcct_.get(guard1).Deposit(amount);
      // other actions
      guard1.unlock();
      */
    }
private:
  boost::externally_locked<BankAccount, AccountManager> checkAcct_;
  boost::externally_locked<BankAccount, AccountManager> savingAcct_;
};
}

/// TODO: Try and timed
// http://stackoverflow.com/questions/10172229/boost-locking-with-try-lock

/// TODO: Recursive

/// Concepts
// BasicLockable - models exclusive ownership.
// Lockable - try_lock()
// RecursiveLockable - 
// 
// TimedLockable
// SharedLockable - RW locks
// Upgraid lockable
//
/// Lock options
//
/// Lock concepts
// StrickLock
//
/// Lock Types
// ???
//
/// Non-member functions
//
/// Mutex types
// mutex/try_mutex - Lockable
// timed_mutex - TimedLockable
// recursive_mutex/recursive_try_mutex - Lockable
// shared_mutex - SharedLockable
// upgrade_mutex - 
//
/// Cond. Vars

boost::condition_variable cond;
boost::mutex mut;
bool data_ready;

void process_data() {}

void wait_for_data_to_process()
{
    boost::unique_lock<boost::mutex> lock(mut);
    while(!data_ready) {
      cond.wait(lock);
    }
    process_data();
}

void retrieve_data() {}
void prepare_data() {}

void prepare_data_for_processing()
{
    retrieve_data();
    prepare_data();
    {
        boost::lock_guard<boost::mutex> lock(mut);
        data_ready=true;
    }
    cond.notify_one();
}
