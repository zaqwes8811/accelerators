
import java.util.Random

import org.junit.Assert._
import org.junit._


// http://scalareactive.org/core/Signal
// http://www.slideshare.net/remeniuk/frp-scalagwt
// https://www.typesafe.com/community/core-projects/scala
import scala.concurrent._
import rx._

// https://github.com/lihaoyi/utest
// http://stackoverflow.com/questions/28898445/cannot-get-utest-to-see-my-tests

// Underscore in Scala
// http://stackoverflow.com/questions/8000903/what-are-all-the-uses-of-an-underscore-in-scala

@Test
class RecapTest {
  @Test
  def testOK() = {
    assertTrue(true)

    // partial matching
    // http://sandrasi-sw.blogspot.ru/2012/03/understanding-scalas-partially-applied.html
    val f: PartialFunction[String, String] = {
      case "ping" => "pong"
    }

    // NOTE: first match only
    val i = f.isDefinedAt("ping")
    assertFalse(f.isDefinedAt("abc"))
  }

  @Test
  def testCollectionRecap() = {
    // http://stackoverflow.com/questions/6559996/scala-list-concatenation-vs
    // :: make list
    // ++ concat list
    //
    // flatMap
    //  http://martinfowler.com/articles/collection-pipeline/flat-map.html
    // "map and the single-level flatten"
  }

  @Test
  def testFuncRandomGen() = {


    trait Generator[+T] {
      self =>
      // an alias from "this"

      def generate: T

      // add
      //

      def map[S](f: T => S): Generator[S] = new Generator[S] {
        def generate = f(self.generate) // need self! else recursion
        //Generator.this.generate  // equal
      }

      def flatMap[S](f: T => Generator[S]): Generator[S] = new Generator[S] {
        def generate = f(self.generate).generate
      }
    }

    // Too much boilerplate
    val integers = new Generator[Int] {
      val rand = new Random()

      def generate = rand.nextInt()
    }

    // for boolean start again
    // want...
    // FIXME: but need create one
    val booleans = for (x <- integers) yield x > 0

    def pairs[T, U](t: Generator[T], u: Generator[U]) = t flatMap {
      x => u map { y => (x, y) }
    }

    // Random list/tree
    // Random structure and random values
    val r = (1 until 7) map (i => (1 until i) map (j => (i, j)))
    print(r)
  }

  @Test
  def testObs() = {
    val a = Var(1); val b = Var(2)
    val c = Rx{ a() + b() }
    println(c()) // 3
    a() = 4
    println(c()) // 6
  }
}


// FIXME: was error
// http://stackoverflow.com/questions/4651730/program-works-when-run-with-scala-get-compile-errors-when-try-to-compile-it-wit
object fu {

  def show(json: JSON): String = json match {
    case JNull() => "null"
    case JSeq(elems) => "elems"
  }

  abstract class JSON

  case class JSeq(elems: List[JSON]) extends JSON

  // JNull is deprecated

  case class JNull() extends JSON

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




