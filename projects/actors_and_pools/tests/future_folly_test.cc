
#include "actors.h"

#include <folly/futures/Future.h>
#include <gtest/gtest.h>
//#include <boost/shared_ptr.hpp>

#include <iostream>

using namespace folly;
using namespace std;

void foo(int x) {
  // do something with x
  cout << "foo(" << x << ")" << endl;
}

TEST(FollyTest, My) {
  e_cc11::Active a;

  cout << "making Promise" << endl;
  shared_ptr<Promise<int> > p = make_shared<Promise<int> >();
  Future<int> f = p->getFuture();

  // continue
  f.then(
    [](Try<int>&& t) {
      foo(t.value());
    });
  cout << "Future chain made" << endl;

  // launch
  a.Send([p] {
    cout << "fulfilling Promise" << endl;
    p->setValue(42);
    cout << "Promise fulfilled" << endl;
  });
}
