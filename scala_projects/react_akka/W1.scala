package react_akka

object W1 extends App {

  // Recup
  trait Seq[Elem] extends (Int => Elem) // from func

  //val f: String => String = { case "ping" => "pong" } // FIXME: where is match?
  val f: PartialFunction[String,String] = { case "ping" => "pong" } // FIXME: where is match?

  //f("pi")  // Runtime error
  
  // http://martinfowler.com/articles/collection-pipeline/flat-map.html
}