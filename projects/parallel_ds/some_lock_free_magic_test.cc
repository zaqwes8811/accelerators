
// Sys
#include <pthread.h>

// Part2
// http://www.ibm.com/developerworks/aix/library/au-multithreaded_structures2/index.html?ca=d
template <typename T>
class SList {
private:
  typedef struct Node {
    T data;
    Node *next;
    Node(T& value) : data(value), next(NULL) { }
  } Node;
  
  pthread_mutex_t _lock;
  Node *head, *tail;
  
public:
  void push_back(T& value) {
    pthread_mutex_lock(&_lock);
    
    
    pthread_mutex_unlock(&_lock);
  }
  void insert_after(T& previous, T& value);
  void remove(const T& value);
  bool find(const T&value);
  SList();
  ~SList();
};