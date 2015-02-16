// Eff conc.
// http://herbsutter.com/2009/11/11/effective-concurrency-prefer-structured-lifetimes-%E2%80%93-local-nested-bounded-deterministic/

// TODO: The Pillars of Concurrency
// 1. Responsiveness and Isolation via Async Agents
// 2. Throuthput and Scalability via conc. collections
// 3. Consistency via Safely Shared Res.
// http://www.drdobbs.com/parallel/the-pillars-of-concurrency/200001985?pgno=2

// TODO: Use Threads Correctly = Isolation + Asynchronous Messages
// http://www.drdobbs.com/parallel/use-threads-correctly-isolation-asynch/215900465

// TODO: Use Thread Pools Correctly: Keep Tasks Short and Nonblocking
// http://stackoverflow.com/questions/3988128/c-thread-pool - good pool
// По крайней мере один готовый работать - как-то так

// TODO: Sharing Is the Root of All Contention
// Even with races no fast in multicore system!
// "Yes, using locks and CAS incurs waiting and overhead expenses, 
// but those all arise from the root cause of having a mutable shared object at all."
/*
 Prefer isolation: 
 Make resources private and unshared, where possible; 
 sometimes duplicating resources is the answer, like providing an extra crayon or a copy of an object or an additional core. 
 Otherwise, prefer immutability: 
 Make shared resources immutable, where possible, so that no concurrency control is required and no contention arises. 
 Finally, use mutable shared state when you can't avoid it, but understand that it's fundamentally an enemy of 
 scalability and minimize touching it in performance-sensitive code including inner loops. 
 Avoid false sharing by making sure that objects that should be usable concurrently 
 by different threads stay on different cache lines
 */

// TODO: Avoid Exposing Concurrency: Hide It Inside Synchronous Methods
// А как быть со всякими асинхоронными api?
//
// Way: hide it inside sync API

// TODO: Boost.Thread supply
// thread interrurpt - но а если объект потока не доступен?
//
//  Join the thread before destroying or use an scoped thread.

// TODO: Understanding Parallel Performance
// не обязательно все запускать в новых потоках - можно часть работы сделать и в этом

// TODO: Effective Concurrency: Eliminate False Sharing
// http://habrahabr.ru/company/intel/blog/143446/
/*
// Example 1: Simple parallel version (flawed)
//
int result[P];
// Each of P parallel workers processes 1/P-th
// of the data; the p-th worker records its
// partial count in result[p]
for( int p = 0; p < P; ++p )
  pool.run( [&,p] {
    result[p] = 0;
    int chunkSize = DIM/P + 1;
    int myStart = p * chunkSize;
    int myEnd = min( myStart+chunkSize, DIM );
    for( int i = myStart; i < myEnd; ++i )
      for( int j = 0; j < DIM; ++j )
        if( matrix[i*DIM + j] % 2 != 0 )
          ++result[p];
  } );
// Wait for the parallel work to complete…
pool.join();
// Finally, do the sequential "reduction" step
// to combine the results
odds = 0;

// Что-то похожее на CUDA opt
for( int p = 0; p < P; ++p )
  odds += result[p];

// V2
for( int p = 0; p < P; ++p )
   pool.run( [&,p] {
   int count = 0;
   int chunkSize = DIM/P + 1;
   int myStart = p * chunkSize;
   int myEnd = min( myStart+chunkSize, DIM );
   for( int i = myStart; i < myEnd; ++i )
      for( int j = 0; j < DIM; ++j )
        if( matrix[i*DIM + j] % 2 != 0 )
          ++count;
   result[p] = count;
} );
*/