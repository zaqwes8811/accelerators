// http://stackoverflow.com/questions/3726570/using-boostlock-guard-for-simple-shared-data-locking
/// App:
//
// Передает пакеты и prodecer thread to consimer
// Так похоже правильнее было реализовать поток данных
//   из интерфейса в базу данных
//
//
// Deamonds:
//   Task parallelize
//   каждый пакет должн быть обработан
//   io-thread развязан от потока обработки
//   correct concurent read form database
//
// Info:
// http://web.stanford.edu/class/cs140/cgi-bin/lecture.php?topic=process
// http://programmers.stackexchange.com/questions/206962/should-multi-threading-be-used-for-tasks-which-does-not-involve-io-operation