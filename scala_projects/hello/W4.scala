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

}