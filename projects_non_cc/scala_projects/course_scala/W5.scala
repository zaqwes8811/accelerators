package hello

import scala.collection.immutable._;

object W5 extends App {
  // reduceLeft
  // ((()op)op) - I elem
  // foldLeft() - A elem insted I - Return U for List[T]

  // x1 op ( x ( x ())

  def mapFun[T, U](xs: List[T], f: T => U): List[U] =
    (xs foldRight List[U]())(f(_) :: _)

  def lengthFun[T](xs: List[T]): Int =
    (xs foldRight 0)((x, y) => y + 1)

  val test = List(1, 2, 3)
  println(mapFun(test, (x: Int) => x + 1))
  println(lengthFun(test))

  // Seq
  val s = "Hello World"
  s flatMap (c => List('.', c))

  //(1 to M) flatMap(x => (1..N//))(

  def scalarProduct(xs: Vector[Double], ys: Vector[Double]): Double =
    (xs zip ys).map { case (x, y) => x * y }.sum

  // equal - x => x match {case p1 => ...}

  def isPrime(n: Int): Boolean = (2 until n) forall (n % _ != 0)

  val n = 7

  // triangle
  ((1 until n) map (i => (1 until i) map (j => (i, j)))).flatten

  (1 until n) flatMap (i =>
    (1 until i) map (j => (i, j))) filter (pair => isPrime(pair._1 + pair._2))

  // simple

  //for (generator, filter) yield e

  def scalarProduct_for(xs: Vector[Double], ys: Vector[Double]): Double = {
    val t = for {
      (x, y) <- xs zip ys
    } yield x * y

    println(t)

    t.sum
  }

  val v1 = Vector(1.0, 2, 3);
  val v2 = Vector(1.0, 2, 1);
  println(scalarProduct_for(v1, v2))

  //
  def queens(n: Int): Set[List[Int]] = {
    def isSafe(col: Int, queens: List[Int]): Boolean = {
      val row = queens.length

      val tmp = (row - 1 to 0 by -1) zip queens

      tmp forall {
        case (r, c) => col != c && math.abs(col - c) != row - r
      }
    }

    def placeQueens(k: Int): Set[List[Int]] =
      if (k == 0) Set(List())
      else
        for {
          queens <- placeQueens(k - 1)
          col <- 0 until n
          if isSafe(col, queens)

        } yield col :: queens // push_front
    placeQueens(n)
  }

  case class Book(val title: String, val authors: List[String])

  val books = List[Book]()

  for (b <- books; a <- b.authors if a startsWith "Bird")
    yield b.title

  books.flatMap(b => for (a <- b.authors.withFilter { a => a startsWith "Bird" }) yield b.title)

  //b.flatMap(x=> for (a <- x.authors if a.startWith("Bird")))
  // Wrong! Type
  books.flatMap(b => b.authors.withFilter { a => a startsWith "Bird" }.map { x => b.title })
  //books.flatMap(b => b.authors.withFilter(a => a startsWith "Bird") map (b => b.title))  // b.title

  // Maps
  //Option
  val capitalOfCountry = Map("US" -> "Wash", "Swit" -> "Bern")
  for ((exp, coeff) <- capitalOfCountry) yield coeff + "x^" + exp

  def showCapital(country: String) = capitalOfCountry.get(country) match {
    case Some(capital) => capital
    case None          => "miss"
  }

  class Poly(terms0: Map[Int, Double]) {
    val terms = terms0.withDefaultValue(0.0)

    def this(bindings: (Int, Double)*) = this(bindings.toMap)

    def +(other: Poly) = {
      new Poly(terms ++ (other.terms map adjust))
    }

    def adjust(term: (Int, Double)): (Int, Double) = {
      val (exp, coeff) = term;
      exp -> (coeff + terms(exp))
    }

    def addTerm(terms: Map[Int, Double], term: (Int, Double)) = ???

    override def toString = {
      (for ((exp, k) <- terms.toList.sorted.reverse) yield k + "x^" + exp) mkString " + "
    }
  }

  // list. groupBy - Multimap from Guava
  val p1 = new Poly(1 -> 2.0, 3 -> 4.0, 5 -> 6.2)
  val p2 = new Poly(Map(0 -> 3.0, 3 -> 7.0))
  println(p1 + p2)

}

object PolyM {
  class Poly(terms0: Map[Int, Double]) {
    val terms = terms0.withDefaultValue(0.0)

    def this(bindings: (Int, Double)*) = this(bindings.toMap)

    def + (other: Poly) = new Poly((other.terms foldLeft terms)(addTerm))

    def addTerm(terms: Map[Int, Double], term: (Int, Double)) = {
      val (exp, coeff) = term
      terms + (exp -> (coeff + terms(exp)))
    }

    override def toString = {
      (for ((exp, k) <- terms.toList.sorted.reverse) yield k + "x^" + exp) mkString " + "
    }
  }

  // list. groupBy - Multimap from Guava
  //val p1 = new Poly(1 -> 2.0, 3 -> 4.0, 5 -> 6.2)
  val p2 = new Poly(Map(0 -> 3.0, 3 -> 7.0))
  //println(p1 + p2)
}























