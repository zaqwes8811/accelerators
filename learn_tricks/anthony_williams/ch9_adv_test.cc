#include <gtest/gtest.h>

// C++11
#include <future>

class thread_pool {
  std::atomic_bool done;
  
  // TODO: очередь из предыдущих глав
  //thread_safe_queue<std::function<void()> > work_queue;
};

TEST(AW_9, Pool) {
  
}