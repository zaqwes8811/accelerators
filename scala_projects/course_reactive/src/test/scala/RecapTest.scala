
import java.util.Random

import org.junit.Assert._
import org.junit._


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

}




