
package hello

/**
 * @author zaqwes
 */
object W3 extends App {
  // Peristent data structures
  abstract class IntSet {
    def incl(x: Int): IntSet
    def contains(x: Int): Boolean
  }

  class NonEmpty(elem: Int, left: IntSet, right: IntSet)
    extends IntSet {
    def contains(x: Int): Boolean =
      if (x < elem) left.contains(x)
      else if (x > elem) right contains x
      else true

    def incl(x: Int): IntSet =
      if (x < elem) new NonEmpty(elem, left.incl(x), right)
      else if (x > elem) new NonEmpty(elem, left, right incl x)
      else this

    override def toString = "{" + left + elem + right + "}"
  }

  class Empty extends IntSet {
    def contains(x: Int): Boolean = false
    def incl(x: Int): IntSet = new NonEmpty(x, new Empty, new Empty)

    override def toString = "."
  }

  val t1 = new NonEmpty(3, new Empty, new Empty)
  val t2 = t1 incl 4

  println(t2)

  // polym.
  trait List[T] {
    def isEmpty: Boolean
    def head: T
    def tail: List[T]
  }

  // val - make member
  class Cons[T](val head: T, val tail: List[T]) extends List[T] {
    def isEmpty = false
    //def head: T
    //def tail: List[T]
  }

  /*
  class Nil[T] extends List[T] {
    def isEmpty: Boolean = true
    def head: Nothing = throw NoSuchElementException("Nil.head")
    def tail: Nothing = throw NoSuchElementException("Nil.tail")
  }

  def nth[T](index: Int, l: List[T]): T = {
    val start = 0;
    def iter(x: Int, l: List[T]): T =
      if (l.isEmpty) throw IndexOutOfBoundsException("")
      else if (x == index) l.head
      else iter(x + 1, l.tail)

    iter(start, l)
  }

  // user in var
  def nth2[T](index: Int, l: List[T]): T = {
    if (l.isEmpty) throw IndexOutOfBoundsException("")
    else if (0 == index) l.head
    else nth2(index - 1, l.tail)
  }

  */

  // Lists
}



