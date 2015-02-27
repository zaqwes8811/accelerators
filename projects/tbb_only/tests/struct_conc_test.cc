// design patt -> (!)alg strategy patt -> impl patt

// best strategy for scalable parallelism is data parallelism
// , but exist functional parallelism - does't scale
// , but exist task parallelism - regular and irregular parall.

// thread par - if task diverge
// vector par

// task - potential parallel work - soft to hard threads - preemptive - scheduler task to soft th - cooperative
// less context switches - threads have mandatory concurrency semantics


/// Machine model


/// Patterns
// Nestint - static(fp) and dynamic(dp)
// Trouble: composability
// Trouble: potential parallelism to hardw. parall.
//
// Mem access patterns
// Parallel: pack, pipeline(non scale - fp), geometric, gather, scatter
// + sperscalar seq, futures,  ...
// + expand(histogram?)
//
// Non-determ.
//   Branch and bound
//   Transactions - without locks?! commit/rollback but how?
