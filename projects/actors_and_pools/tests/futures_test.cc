// asio
//   http://stackoverflow.com/questions/18202398/is-there-any-way-to-asynchronously-wait-for-a-future-in-boost-asio
//   http://stackoverflow.com/questions/17282434/using-futures-with-boostasio



/**
// Looks like good real use case
void CallerMethod() {
  // …

  // launch work asynchronously (in any
  // fashion; for yuks let's use a thread pool)
  // note that the types of "result" and
  // "outTheOther" are now futures.
  result = pool.run( ()=>{
    DoSomething( this, that, outTheOther ) } );

  // These could also take a long time
  // but now run concurrently with DoSomething
  OtherWork();
  MoreOtherWork();

  // … now use result.wait() (might block) and outOther…
}


*/

