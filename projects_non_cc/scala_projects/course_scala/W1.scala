package hello

import Math.abs;
import scala.annotation.tailrec;

object W1 extends App {
  // w1
  def sqrt(x: Double) {
    // x - lock in closure

    def sqrtIter(guess: Double): Double =
      if (isGoodEnough(guess)) guess // "cycle out"
      else sqrtIter(improve(guess))

    def isGoodEnough(guess: Double) =
      // was (..) < 0.001
      abs(guess * guess - x) / x < 0.001

    def improve(guess: Double) =
      (guess + x / guess) / 2

    sqrtIter(1.0)
  }

  sqrt(2)
  sqrt(4)
  sqrt(1e-6)

  // ; - property
  //+ op - bad
  // op + - good

  // w1 - tail recursion - two calls
  // TR = iteration - GCD - reuse stack
  // Factorial - !TR
  // FIXME: how reduce result?
  // FIXME: dive then up?
  // http://stackoverflow.com/questions/3114142/what-is-the-scala-annotation-to-ensure-a-tail-recursive-function-is-optimized
  //@tailrec
  def gcd(a: Int, b: Int) {

  }

  def factorial(n: Int): Int =
    if (n == 0) 1
    else n * factorial(n - 1)

  // FIXME: to tail rec
  def factorialIter(n: Int) {
    def loop(acc: Int, n: Int): Int =
      if (n == 0) acc
      else loop(acc * n, n - 1) // pure call! not n * loop!!

    loop(1, n)
  }

  factorialIter(2)
 

  println("hello");
}