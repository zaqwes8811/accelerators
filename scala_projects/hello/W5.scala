package hello

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
  s flatMap(c => List('.', c))
}