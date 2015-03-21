package hello

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
  
}