#include <gtest/gtest.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>

#include <algorithm>

// design patt -> (!)alg strategy patt -> impl patt

// best strategy for scalable parallelism is data parallelism
// , but exist functional parallelism - does't scale
// , but exist task parallelism - regular and irregular parall.

// thread par - if task diverge
// vector par

// task - potential parallel work - soft to hard threads - preemptive - scheduler task to soft th - cooperative
// less context switches - threads have mandatory concurrency semantics

/// Machine mode
// !! exist diff calc model and libs not ever compatible


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

/// Map
// TROUBLE: how be with false sharing
void saxpy_serial(
    size_t n,
    float a,
    const float x[],
    float y[]
) {
  for (size_t i = 0; i < n; ++i) {
    y[i] = a * x[i] + y[i];
  }
}

void saxpy_tbb(
    size_t n,
    float a,
    const float x[],
    float y[]
) {
  tbb::parallel_for(
        tbb::blocked_range<
          //int
          size_t
        >(0, n),  // FIXME: why int?
        [&](tbb::blocked_range<
          //int
          size_t
        > r) {
    for (size_t i = r.begin(); i != r.end(); ++i) {
      y[i] = a * x[i] + y[i];
    }
  });
}

TEST(StructConc, SAXPY) {
  // memory intensive - not calc
}

TEST(StructConc, Mandelbrot) {

}

/// Collectives
/// Can fuse map+reduce and map+scan
template <typename T>
T reduce(
    T (*f)(T, T),
    size_t n,
    T a[],
    T identity
) {
  T accum = identity;
  for (size_t i = 0; i < n; ++i) {
    accum = f(accum, a[i]);
  }
  return accum;
}
// assoc and commut
// float not assoc

// tiling is important

// reduce result must be wider then in's float -> double...


float tbb_sprod(
    size_t n
    , const float* a
    , const float* b
    ) {

  auto op = [=] (
      tbb::blocked_range<size_t>& r,
      float in
      ) {

    // FIXME: a and r.end() - iter to one container?
    return std::inner_product(a + r.begin(), a + r.end(), b + r.begin(), in);
  };

  return tbb::parallel_reduce(
        tbb::blocked_range<size_t>(0, n)
        , float(0)  // best for templates
        , op
        , std::plus<float>());
}

// compilator ignore float -> double partial replacment
double tbb_sprod2(
    size_t n
    , const float* a
    , const float* b
    ) {

  auto op = [=] (
      tbb::blocked_range<size_t>& r,
      double in
      ) {

    // FIXME: a and r.end() - iter to one container?
    return std::inner_product(a + r.begin(), a + r.end()
                              , b + r.begin(), in
                              , std::plus<double>()
                              , std::multiplies<double>());
  };

  return tbb::parallel_reduce(
        tbb::blocked_range<size_t>(0, n)
        , double(0)  // best for templates
        , op
        , std::plus<double>());
}

TEST(StructConc, DotProducts) {
  tbb_sprod2(0, NULL, NULL);
}

// scan























