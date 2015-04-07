package org.glassfish.samples

import akka.actor.Actor
import akka.actor.Props

class Counter extends Actor {
  var count = 0
  override def receive = {
    case "incr" => count += 1
    case "get"  => this.sender ! count
  }
}

/**
 * @author ${user.name}
 */
object App {

  def foo(x: Array[String]) = x.foldLeft("")((a, b) => a + b)

  def main(args: Array[String]) {
    println("Hello World!")
    println("concat arguments = " + foo(args))
  }

}
