package hello

object W4 extends App {
  // DS
  // Lists

  def insert(x: Int, xs: List[Int]): List[Int] = xs match {
    // xs - sorted
    case List() => List(x)
    case y :: ys => {
      if (x < y) x :: xs //y::ys
      else y :: insert(x, ys)
    }
  }

  def isort(xs: List[Int]): List[Int] = xs match {
    case List()  => List()
    case y :: ys => insert(y, isort(ys))
  }

  val l = List(1, 2, 5)
  println(insert(4, l))

  println(isort(List(3, 2, 5)))

  def init[T](xs: List[T]): List[T] = xs match {
    case List()  => throw new Error("")
    case List(x) => List()
    case y :: ys => y :: init(ys)
  }

  println(init(l))

  ///def removeAt[T](n: Int, xs: List[T]) = ???

  //removeAt(1, List('a', 'b', 'c', 'd')) // List(a, c, d)

  // http://stackoverflow.com/questions/13059590/scala-flatten-list
  //def flatten(xs: List[Any]): List[Any] = xs match {  case List() => List() }

  def flatten(xs: List[Any]): List[Any] = xs match {
    case List()           => List()
    case (y :: ys) :: yss => flatten(y :: ys) ::: flatten(yss)
    case y :: ys          => y :: flatten(ys)
  }

  //println(flatten(List(List(1, 1), 2, List(3, List(5, 8)))))
  println(flatten(List(List(1, 1), 2)))

  def merge(xs: List[Int], ys: List[Int]): List[Int] =
    xs match {
      case Nil => ys
      case x :: xs1 =>
        ys match {
          case Nil => xs
          case y :: ys1 =>
            if (x < y) x :: merge(xs1, ys)
            else y :: merge(xs, ys1)
        }
    }

  def mergeT(xs: List[Int], ys: List[Int]): List[Int] =
    (xs, ys) match {
      case (Nil, ys) => ys
      case (xs, Nil) => xs
      case (x :: xs1, y :: ys1) =>
        if (x < y) x :: merge(xs1, ys)
        else y :: merge(xs, ys1)
    }

  def squareList(xs: List[Int]): List[Int] = xs match {
    case Nil     => xs
    case y :: ys => y * y :: squareList(ys)
  }

  def pack[T](xs: List[T]): List[List[T]] = xs match {
    case Nil => Nil
    case x :: xs1 => {
      val (b, e) = (x :: xs1).span(y => y == x)
      b :: pack(e)
    }
  }

  println(pack(List("a", "a", "a", "b", "c", "c", "a")))

  def encode[T](xs: List[T]): List[(T, Int)] = pack(xs).map(ys => (ys.head, ys.length))
  
  println(encode(List("a", "a", "a", "b", "c", "c", "a")))

}