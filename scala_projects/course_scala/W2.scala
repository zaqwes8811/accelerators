package hello

import Math.abs;

object W2 extends App {
  def sumInts(a: Int, b: Int): Int =
    if (a > b) 0 else a + sumInts(a + 1, b)

  def cube(x: Int): Int = x * x * x

  def sumCubes(a: Int, b: Int): Int =
    if (a > b) 0 else cube(a) + sumCubes(a + 1, b)

  def sum(f: Int => Int, // ! 
          a: Int, b: Int): Int =
    if (a > b) 0
    else f(a) + sum(f, a + 1, b)

  // anon
  (x: Int) => x * x * x

  def factorialIter(n: Int) {
    def loop(acc: Int, n: Int): Int =
      if (n == 0) acc
      else loop(acc * n, n - 1) // pure call! not n * loop!!

    loop(1, n)
  }

  def sumBig(f: Int => Int, a: Int, b: Int): Int = {
    def loop(a: Int, acc: Int): Int = {
      if (a > b) acc
      else loop(a + 1, acc + f(a))
    }
    loop(a, 0)
  }

  // carring

  def sumCurring(f: Int => Int)(a: Int, b: Int): Int =
    if (a > b) 0
    else f(a) + sumCurring(f)(a + 1, b)

  def product(f: Int => Int)(a: Int, b: Int): Int =
    if (a > b) 1
    else f(a) * product(f)(a + 1, b)

  def factorial(a: Int): Int =
    product(x => x)(1, a)

  println(factorial(3))

  // finding Fixed Point
  val tolerance = 0.0001
  def isCloseEnough(x: Double, y: Double) =
    abs((x - y) / x) / x < tolerance

  def fixedPoint(f: Double => Double)(firstGuess: Double) = {
    def iterate(guess: Double): Double = {
      val next = f(guess)
      if (isCloseEnough(guess, next)) next
      else iterate(next)
    }
  }

  fixedPoint(x => 1 + x / 2)(1)

  // x in closure
  def sqrtBad(x: Double) = fixedPoint(y => x / y)(1.0)
  def sqrt(x: Double) = fixedPoint(y => (y + x / y) / 2)(1.0)

  // return function? carring?
  def averageDamp(f: Double => Double)(x: Double) = (x + f(x)) / 2

  def fixedPointAverage(f: Double => Double)(firstGuess: Double) = {
    def iterate(guess: Double): Double = {
      val next = averageDamp(f)(guess)
      if (isCloseEnough(guess, next)) next
      else iterate(next)
    }
  }

  // syntax

  // incaps. data
  class Rational(x: Int, y: Int) {
    // Run..
    require(y != 0, "denom")

    def this(x: Int) = this(x, 1)

    // Run..
    private val g = gcd(x, y)

    //def numer = x / g  // every call?
    val numer = x / g
    def denom = y / g

    def add(that: Rational) =
      new Rational(numer * that.denom + that.numer * denom, denom * that.denom)

    override def toString = numer + "/" + denom

    def neg = //() =
      new Rational(-numer, denom)

    def sub(that: Rational) = add(that.neg)

    // next
    private def gcd(a: Int, b: Int): Int = if (b == 0) a else gcd(b, a % b)

    def less(that: Rational) = numer * that.denom < that.numer * denom

    def max(that: Rational) = if (this.less(that)) that else this
  }

  val r = new Rational(1, 2)
  r.neg

  val x = new Rational(1, 3);
  val y = new Rational(5, 7);
  val z = new Rational(3, 2);

  print(x.sub(y).sub(z))

  // next 
  print(y.add(y)) // not simpl.

  val strange = new Rational(1, 0)
  //strange.add(strange)
}

























