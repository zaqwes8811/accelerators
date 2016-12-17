
1. http://preshing.com/
2. CppCon 2014: Jeff Preshing "How Ubisoft Develops Games for Multicore - Before and After C++11" https://www.youtube.com/watch?v=X1T3IQ4N-3g

=============

**Concurrency archs**
"Better Code: Concurency"

Distributed patterns
https://msdn.microsoft.com/en-us/library/jj591569.aspx - saga

Away from threads and locks - Actors / Active Object / Tasks / STM

"Isolation and Immutability" - Fauler 
"Isolation zones"

Steps
https://www.youtube.com/watch?v=MUDOIAssBDs
- callbacks - callback hell
- futures/promises - callback hell to
- async/await - ! - But need lang support!

https://www.youtube.com/watch?v=MUDOIAssBDs
"Async good fit in event-driven progr" => async != event-driven

http://www.slideshare.net/drorbr/the-actor-model-towards-better-concurrency
http://tech.puredanger.com/presentations/actor-concurrency

http://state-machine.com/resources/articles.php#Active

Java
http://www.slideshare.net/chris.e.richardson/map-flatmap-and-reduce-are-your-new-best-friends-simpler-collections-concurrency-and-big-data-jax-jax2014

(!!) http://www.carlgibbs.co.uk/blog/?p=237

"A Task is an item of work, which will be run for you on another thread, has mechanisms to run other Tasks on completion or for letting the caller know the result either by polling for results, blocking until complete or setting a callback."

"State, You're Doing It Wrong: Exploring Alternative Concurrency Paradigms on the JVM"
!! Какую выбрать модель, зависит от задачи

Very deps from task!


Actors
data races http://james-iry.blogspot.co.uk/2009/04/erlang-style-actors-are-all-about.html
http://c2.com/cgi/wiki?TransactionalActorModel - ! транзакции
Cons:
- Смешанная архитектура слабо поддерживается

Actors + Event-driveng + FSM
Quantum programming

FSM - могут быть состояния без выхода - как попасть - ошибки, исключения
http://www.infoq.com/presentations/Death-by-Accidental-Complexity
- filtration - Filtered Event Loop,
Q: что делать если не попали в состояние? Обратно в очередь?

Асинхр.
Только futures мало, нужна поддержка языком, а иначе похоже будут колбеки.
А как быть с гонками данных? Ок, с консистентностью
"Easy Asynchrony with C#: No More Callbacks!"
- cancelation throut exception

Shared State! "With great responsiveness come great responsobility"

await in C++ 
https://github.com/jbandela/cppcomponents_libuv/wiki/Easy-Asynchronous-Programming-with-cppcomponents_libuv

"Yet another threading framework: асинхронная разработка на C++ под мобильные устройства"

- Отмена задачи
- Прогресс задачи

Solution:  Многопоточность в браузере. Модель акторов
Trouble: Проблемы с композицией 
Solution: then(), via()...
Trouble: как быть с согласованностью!?
Trouble: call-back hell
Trouble: Design solution https://software.intel.com/en-us/articles/the-folly-of-do-it-yourself-multithreading
Solution: https://www.dre.vanderbilt.edu/~schmidt/PDF/HS-HA.pdf - half-sync/half-async
Trouble: Как отменить? Точки остановки потоков - wait/sleep/get/...
Trouble: Как сообщать об ошибках?
Solution: Скрещение потоков и асинхронности от Саттера - link
Solution: Похоже порой от асинхронности большой профит
Solution: Task Based Parall. - подходить не для всего, но важна возможность использовать
Solution: pools of threads
Solution: I/O (actors?) - потоки отдельные,  все осталное - pool
Trouble: .then() и контекст выполнения

Thread Pools

http://rsdn.ru/forum/cpp.applied/5468499.flat.1

Schedulers
- preemptive
- cooperative time-trigged arch

Threads vs Event loops

"Qt event loop, networking and I/O API Thiago Macieira, Qt Core Maintainer San Francisco, November 2013"

Why threads?

Good reasons
• Calling blocking functions
• CPU-intensive work
• Real-time work
• Scalability

Bad reasons
• To use sleep()
• Networking and I/O ‒ Except for scalability

!Thread bounded and IO bounded tasks

coroutines
https://events.yandex.ru/lib/talks/2308/

http://habrahabr.ru/post/210330/ генераторы

Модель акторов
http://habrahabr.ru/post/150788/


Rob Pike - 'Concurrency Is Not Parallelism'

http://learnyousomeerlang.com/the-hitchhikers-guide-to-concurrency#dont-panic !!

fut/prom - скорее для вычислений, а не для работы в event-loop. См у яндекса, там сделана сопрограммы с future/promise. Их можно закинуть в event-loop и подождать, но обратно проблема в том что как-то нужно ждать (poll or sleep)

Design:
- проблемы с состояниями и инвариантами
http://www.codeproject.com/Articles/581181/Implementing-a-finite-state-machine-using-async-aw

FSM:
http://www.personal.rdg.ac.uk/~stsgrimb/teaching/fsm.pdf
http://tech.pro/blog/1402/five-patterns-to-help-you-tame-asynchronous-javascript
http://stackoverflow.com/questions/10589154/is-an-fsm-the-right-solution-for-my-asynchronous-akka-actor-based-calculation-e
Akka http://blog.michaelhamrah.com/2014/01/programming-akkas-finite-state-machines-in-scala/

!! http://akisaarinen.fi/blog/2012/05/13/functional-finite-state-machines-using-akka/ - callbacks
- promises

reactor, proactor, sync/async event handling, event driven

Rx
"Rx = Observable +LINQ + Schedulers"

http://channel9.msdn.com/Series/Rx-Workshop/Rx-Workshop-Schedulers

Push-based and Pull-based arch - async/sync

http://habrahabr.ru/post/128772/ по русски
"Опрос происходит с помощью библиотеки «синхронного» ввода/вывода, который, при этом будет являться «неблокирующим» (в системную функцию ввода/вывода передаётся флаг O_NONBLOCK). "

"While the thread-based model covers both--I/O-based and CPU-based concurrency, the initial event-based architecture solely addresses I/O concurrency. For exploiting multiple CPUs or cores, event-driven servers must be further adapted."

!!http://berb.github.io/diploma-thesis/original/042_serverarch.html

https://www.dre.vanderbilt.edu/~schmidt/PDF/Proactor.pdf

epoll()

http://kovyrin.net/2006/04/13/epoll-asynchronous-network-programming/

=============

**C++**

Video "A C++ REST SDK: OSS web services on Windows and Linux"

C++
Отнесу к производительности, но вообще параллелизм на уровне задач , например, повышает инкапсуляцию (не помню где слышал это). Use logical parallelism!

C++ await
https://paoloseverini.wordpress.com/2014/04/22/async-await-in-c/
http://channel9.msdn.com/Events/GoingNative/2013/Bringing-await-to-Cpp
http://www.slideshare.net/cppfrug/async-await-in-c

До c++11 и после него http://tech.yandex.ru/events/yagosti/cpp-user-group/talks/1795/ - good ideas
http://channel9.msdn.com/Events/CPP/C-PP-Con-2014/Overview-of-Parallel-Programming-in-CPP
"Параллельное программирование в С++ это сложно!" лучше пользоваться алгоритмическими фреймворками.
- низкий уровень абстракции
- отсутствие знания и опыта
- Некоторые концепции действительно сложны"


http://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Herb-Sutter-Concurrency-and-Parallelism

High-level multitreading libs, frameworks and lang extension
Библиотеки для работы с многопоточностью
- нужна была переносимая От Windows on Intel до Linux on ARM
- достаточно высокоуровневая
- out-of-the-box
- Хорошо бы иметь возможно легко создавать потоки, что удобно для функционального тестирования.


Варианты:
- Intel TBB - http://stackoverflow.com/questions/6427977/intel-tbb-will-work-on-amd-processors
- Qt - была выбрана
- pthread - не высокоуровневая
- OpenMP
- Look-free library http://libcds.sourceforge.net/
- NVIDIA CUDA


Q: Как быть с исключениями?
A: Scope locks

TBB enables you to specify logical parallelism instead of threads. Most threading packages require you to specify threads. Programming directly in terms of threads can be tedious and lead to inefficient programs, because threads are low-level, heavy constructs that are close to the hardware. Direct programming with threads forces you to efficiently map logical tasks onto threads. In contrast, the Intel® Threading Building Blocks run-time library automatically maps logical parallelism onto threads in a way that makes efficient use of processor resources."

=============

**Effective CPU Caching / IO and CPU bounded tasks**

"What Every Programmer Should Know About Memory" Ulrich Drepper
MIT 6.851 Advanced Data Structures - about

Guidlines:
Async Tip #2: It's crucial to distinguish CPU-bound work (should be done on threadpool) from IO-bound work (which needn't).

Вообще похоже дело в I/O - они медленные, поэтому лучше асинхронность. Но результат то куда потом отправлять - если работу продолжаем, то что-то может изменится. API сети часто асинхронный

Memory

http://www.akkadia.org/drepper/cpumemory.pdf

I/O (no new thread, better in UI thread) and CPU - bounded code

Arch:
"So far I've talked about a user. This naturally raises the question of what happens when there is no a human being driving the software. This could be something new and fashionable like a Web service or something mundane and useful like a batch process. In the latter case the user is the client program. At this point it becomes apparent that there is a lot of similarity between the presentation and data source layers in that they both are about connection to the outside world. This is the logic behind Alistair Cockburn's Hexagonal Architecture pattern [wiki], which visualizes any system as a core surrounded by interfaces to external systems. In Hexagonal Architecture everything external is fundamentally an outside interface, and thus it's a symmetrical view rather than my asymmetric layering scheme."

Presentaion(IO) / business (CPU / GPU) / DAL(IO) - но схема асиметричная (предст - пердст сервис) - (dal - использует сервисы)

Tmp:
http://en.wikipedia.org/wiki/Locality_of_reference
http://en.wikipedia.org/wiki/Loop_blocking
http://en.wikipedia.org/wiki/Category:Compiler_optimizations
оптимизация для gpu - похоже что-то годится и для cpu
Что-то про промахи кеша
http://www.tantalon.com/pete/cppopt/general.htm#BadOptimizationStrategies

Q: А как быть с java - там на пути еще и виртуальная машина?

Похоже проблема производительности теперь больше в памяти. С GPU та же проблема, но она более очевидна.

NVIDIA CUDA - там много идей для оптимизации

V = time_in_calc/time_mem_access

- Vectorization

Cache friendly code
http://stackoverflow.com/questions/16699247/what-is-cache-friendly-code
Как понял код должен стать как можно пердсказуемей

Sutter false sharding - даже если разделили на куски, то все равно могут быть проблемы - там в соседние элементы массива пишут разные потоки

Q: сокращая область видемости переменной и часто ее использую, мы увеличиваем производительность? Т.е. если переменную объявили, долго не использовали, а потом начали использовать, то она уже остыла"? http://en.wikipedia.org/wiki/Locality_of_reference temporal location

C++:
http://en.wikibooks.org/wiki/Optimizing_C%2B%2B/Writing_efficient_code !! high-level
http://www.geeksforgeeks.org/structure-member-alignment-padding-and-data-packing/

Статья "Pitfalls of Object Oriented Programming" - одна из идей в том, что скорость процессора растет быстрее скорости памяти, и порой производительность упирается в память, а не процессор. Но оптимизация на примере для С++. С чем связана методология - Data Oriented Desing (http://gamesfromwithin.com/data-oriented-design)

http://stackoverflow.com/questions/1478280/what-can-i-do-in-java-code-to-optimize-for-cpu-caching - DOD для Java похоже не прокатит.

Q: когерентность кешей, а ведь кеш это например некая копия, как быть распространением изменений в системе? Если указатели расшаривающие, то выходит, что логически мы удалили, но все равно в системе данные где-то живут. ACID - про согласованность данных в системе

Value and Ref semantic - при работе с первой может возникнуть проблема согласованности в системе. исходной значение может быть удалено, а его копия еще может бродить по системе. Или тут дело в том, что переходим из одной семантики в другую. Та же проблема с shared_ptrs.

=====================================

** Concurrency base **
Correctness of concurrent programs http://www.doc.ic.ac.uk/~jnm/concurrency/online/monitors/tsld017.htm
https://docs.oracle.com/javase/tutorial/essential/concurrency/liveness.html

Data base transactions:
https://technet.microsoft.com/en-us/library/ms190805(v=sql.105).aspx

- Safety Hazards 
- Liveness Hazards 
- Performance Hazards 
"It is critically important to pay attention to thread safety issues when developing concurrent code: safety cannot be compromised." eff conc. java

event-driven / async / concurency / parallism / fsm
high performance computing / scalability

https://www.youtube.com/watch?v=MUDOIAssBDs
Долго ждать IO и долго ждать системыне вызовы

Summary: http://berb.github.io/diploma-thesis/original/index.html

http://www.javaworld.com/article/2824163/application-performance/stability-patterns-applied-in-a-restful-architecture.html

High performance computing
http://cs.swan.ac.uk/~csdavec/HPC/

Places:
http://highscalability.com
http://highscalability.com/all-time-favorites/

http://www.infoq.com/presentations/Big-Data-H2O

akka

ttp://habrahabr.ru/post/127696/ - про ноду

http://horicky.blogspot.ru/ - "человек часто пишет про основы нагруженных систем, поисковых систем и т.п."

http://www.hpcs.cs.tsukuba.ac.jp/~tatebe/lecture/h23/dsys/dsd-tutorial.html - base from Google
http://stackoverflow.com/questions/2258332/distributed-computing-in-c
http://www.drdobbs.com/web-development/building-scalable-web-architecture-and-d/240142422


Scalability:
Work and step complexity - см. курсы по CUDA from NVIDIA
В последователный алгоритмая сложность O(f(n,m...)) - тоже связана с масштабируемостью. Step complexity, work complexity, memory complexity. разные уровни

Places:
http://highscalability.com/

Clouds
https://cloud.google.com/appengine/docs/java/?csw=1#threads


Look-free - Very Dark Magic
http://habrahabr.ru/company/ifree/blog/195770/

Похоже для реализации многопоточности важна поддержка железом некоторых операций - значит не переносимы, возможно.

"Если look-free-котнейнер работает хорошо, то варианта Вы что-то сделали неправильно, Вы что-то сделали неправильно, У вас появилась идея для дистертации." (http://tech.yandex.ru/events/yagosti/cpp-user-group/talks/1798/)

http://www.drdobbs.com/lock-free-data-structures/184401865

Trouble: Sutter "The Free Lunch Is Over" (http://www.gotw.ca/publications/concurrency-ddj.htm). Про повышение частоты http://habrahabr.ru/company/intel/blog/194836/
Trouble: http://habrahabr.ru/post/196110/ про хаскель и параллельное программирование
Trouble: Далеко не все программы хорошо параллелизуются (закон Амдала и Густвсона) ".. и почему прогресс увяз на одном месте" (http://habrahabr.ru/company/intel/blog/174719/) http://preshing.com/
Trouble: Sharing Is the Root of All Contention - Sutter, а логи это следствие


Key words:
Hazard Pointers, look-free,  caсhe line, сопрограммы, false sharing, barrier, lock (have bad scalability), scan - very important in parallel world, mapreduce, async, future, генераторы, epoll, kqueue 

Thread-safity:
http://beta.boost.org/doc/libs/1_31_0/libs/thread/doc/definitions.html#Thread-safe
- Race Condition
- Deadlock
- Starvation
- Priority Failure
- Memory Visibility
- Балансировка нагрузки
- http://blogs.msdn.com/b/ruericlippert/archive/2009/10/19/9929760.aspx что-то похожее как Java Concurecy In Practice.

http://stackoverflow.com/questions/10553761/tcp-server-w-boostasio-scalability-of-thread-pool-vs-stackless-coroutines
http://www.kegel.com/c10k.html

"Параллелизм - разделение обязанностей и производительность. Не применять если затраты меньше выигрыша." - C++ Con. in practice
http://intel-software-academic-program.com/pages/courses#manycore

Q: Как писать многопоточные/параллельные приложения?
A: Лучше профилировать приложение и искать нагруженные места, а потом постараться свести к одной из типичных схем - gather, scatter, sort (http://tech.yandex.ru/events/yagosti/cpp-user-group/talks/1795/). http://www.drdobbs.com/tools/introducing-multithreading-to-mature-des/231000894
A: Использовать располагающие к этому языки - Erlang and Haskel (http://habrahabr.ru/post/196110/)

Писать обычный ОО код, тестировать, а потом аккуратно обернуть в многопоточный фреймворк. А производительность? Лучше выделять паттерны. Специальные структуры данных - Google Guave Cache.

Q: В "Многопоточное программировании" от Intel рекомендуют сперва написать последовательную программу. Но всегда ли это возможно?

Писать последовательный код, а затем постепенно снимая константность и добавлять точки пересечения.

Q: Но некоторые алгоритмы, если учитывать параллельность, могут не иметь последовательной версии?

Q: Параллелизм на уровне задач? "Работу с I/O выносить в потоки отдельные от обработки." Многоядерное программирование

Параллелизм != многопоточность

http://iproc.ru/programming/openmp-visual-studio/ - Как поставить на VSEE

Можно использовать в тестах и в качестве модели

Q: . Передаются ссылки на объекты для использования из разных потоков, что будет при вызове разрушающегося объекта?

A: Для контроля ресурсов и доступа к ним использовать weak_prt+shared_ptr.

Виртуальные функции про конструировании http://stackoverflow.com/questions/36832/virtual-functions-in-constructors-why-do-languages-differ

Java
http://tutorials.jenkov.com/java-concurrency/read-write-locks.html
http://www.nurkiewicz.com/2013/02/javautilconcurrentfuture-basics.html - async and pools

Parallel primitives

http://www.chromium.org/developers/lock-and-condition-variable

Потоки не лучшие понятия которыми думают о параллелизме, как и мьютексы (Sean Parent Ch9). Лучше задачи, а вместо мьютексов очереди (см. Саттера на Channel9).

http://channel9.msdn.com/Events/CPP/C-PP-Con-2014/022-Decomposing-a-Problem-for-Parallel-Execution  !!!
mutex and atomit -> contention

Trouble: "Потоки — это Goto параллельного программирования" - см на хабре в статьях от intel. Почему потоки плохая идея   - http://www.cs.sfu.ca/~vaughan/teaching/431/papers/ousterhout-threads-usenix96.pdf  

blocking(mutex) -> non-blocking(message queue)

Locks:
http://demin.ws/blog/russian/2012/05/05/atomic-spinlock-mutex/ 
http://home.roadrunner.com/~hinnant/mutexes/locking.html

=================================

**Patterns**
Concurrent patterns:
Google I/O 2012 - Go Concurrency Patterns
Patterns Parallel progrmming:
- NVIDIA - Intro to Parallel programming (scan, gather, scatter, sort, stentiel, ..)
- Patters of Parallel Pro C# от мелкомягких
- https://software.intel.com/ru-ru/node/506112  Design Patterns
- Designing and Building Parallel Programs - online book

Про оверхед при применении - последовательные да, может лучше просто ООП, нет не всегда!,  или свободные функции, но в параллельном коде - тут все строже, и лучше найти подходящие паттерн и воспользоваться существующим решением, в С++ это на это особо стоит обратить внимание - искать нужно логический параллелизм.

Patterns  Concurrency and Network:
- Proactor
