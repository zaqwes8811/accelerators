/// Green threads, coroutines
// http://habrahabr.ru/post/201826/

// DANGER: что-то не то с обработкой таймаутов

// Boost/Boost.Asio
// http://www.boost.org/doc/libs/1_53_0/libs/coroutine/doc/html/index.html

// out of coroutine
void yield();

bool isInsideCoro();

struct Coro {
  friend void yield();
  
  Coro();
  Coro(Handler);
  ~Coro();
  void start(Handler);
  void resume();
  bool isStarted() const;
  
private:
  // ...
};

void coro() {
  cout << '2';
  yield();
  cout << '4';
}

{
  cout << '1';
  Coro c(coro);
  cout << '3';
  c.resume();
  cout << '5';
}