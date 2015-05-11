/**
 * Created by zaqwes on 5/11/15.
 */

// http://scalareactive.org/core/Signal
// http://www.slideshare.net/remeniuk/frp-scalagwt
// https://www.typesafe.com/community/core-projects/scala

// Flat map
//   http://twitter.github.io/effectivescala/#Twitter's%20standard%20libraries-Futures

import org.junit.Test

import scala.concurrent._
import rx._

@Test
class RxTest {

  @Test
  def testObs() = {
    val a = Var(1); val b = Var(2)
    val c = Rx{ a() + b() }
    println(c()) // 3
    a() = 4
    println(c()) // 6
  }

  @Test
  def testBank() = {

    // FRP
    // http://pl.postech.ac.kr/~maidinh/blog/?p=199
    // http://infoscience.epfl.ch/record/176887/files/DeprecatingObservers2012.pdf
    // https://github.com/lihaoyi/scala.rx
    class BankAccount {
      //private
      //var
      val balance = Var(0)
      //balance = 0

      def deposit(amount: Int): Unit =
        if (amount > 0) {
          val b = balance()  // solved
          balance() =
            //balance()
            b
          + amount // wrong - cyclic
        }

      def withdraw(amount: Int): Unit =
        if (0 < amount && amount <= balance()) {
          balance() = balance() - amount
        } else throw new Error()
    }
  }

  @Test
  def testMayer() = {

  }

}
