package hello

object W7 extends App {
  // Streams

  def streamRange(lo: Int, hi: Int): Stream[Int] =
    if (lo >= hi) Stream.empty
    else Stream.cons(lo, streamRange(lo + 1, hi))

  // http://locrianmode.blogspot.ru/2011/07/scala-by-name-parameter.html
  // http://www.tutorialspoint.com/scala/functions_call_by_name.htm
}

object Test {
  def main(args: Array[String]) {
    delayed(time());
  }

  def time() = {
    println("Getting time in nano seconds")
    System.nanoTime
  }

  // !! => between name and type
  def delayed(t: => Long) = {
    println("In delayed method")
    println("Param: " + t)
    t
  }

  //
  class Pouring(capacity: Vector[Int]) {
    // States
    type State = Vector[Int]
    val initialState = capacity map (x => 0)

    //Moves
    trait Move {
      def change(state: State): State
    }
    case class Empty(glass: Int) extends Move {
      def change(state: State) = state updated (glass, 0)
    }
    case class Fill(glass: Int) extends Move {
      def change(state: State) = state updated (glass, capacity(glass))
    }
    case class Pour(from: Int, to: Int) extends Move {
      def change(state: State) = {
        val amount = state(from) min (capacity(to) - state(to))
        state updated (from, state(from) - amount) updated (to, state(to) + amount)
      }
    }

    val glasses = 0 until capacity.length

    val moves =
      (for (g <- glasses) yield Empty(g)) ++
        (for (g <- glasses) yield Fill(g)) ++
        (for (from <- glasses; to <- glasses if from != to) yield Pour(from, to))

    //Paths
    class Path(history: List[Move], val endState: State) {
      //def endState: State = (history foldRight initialState)(_ change _)
      def extend(move: Move) = new Path(move :: history, move change endState)
      override def toString = (history.reverse mkString " ") + "-->" + endState
    }

    val initialPath = new Path(Nil, initialState)

    def from(paths: Set[Path], explored: Set[State]): Stream[Set[Path]] =
      if (paths.isEmpty) Stream.empty
      else {
        val more = for {
          path <- paths
          next <- moves map path.extend
          if !(explored contains next.endState)
        } yield next
        paths #:: from(more, explored ++ (more map (_.endState)))
      }

    val pathSets = from(Set(initialPath), Set(initialState))

    def solution(target: Int): Stream[Path] =
      for {
        pathSet <- pathSets
        path <- pathSet
        if path.endState contains target
      } yield path
  }

  // Test
  val problem = new Pouring(Vector(4, 7, 19))
  //println(problem.moves)
  //println(problem.pathSets.take(3).toList)

  println(problem.solution(6))
}