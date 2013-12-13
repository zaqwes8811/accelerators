#include <iostream>
#include <memory>
#include <list>
using namespace std;
 
#include <windows.h>
 
namespace
{
  template <typename T>
  class Thread
  {
  public:
    Thread()
      : Thread_(NULL)
    {
    }
    ~Thread()
    {
      Stop();
    }
    void Run(T* functor)
    {
      if (Thread_)
        throw exception("Thread is run");
      DWORD ThreadId = 0;
      if (!(Thread_ = ::CreateThread(NULL, 0, &Thread::ThreadProc, functor, 0, &ThreadId)))
        throw exception("Can't create thread");
    }
    void Stop(bool wait = true)
    {
      if (!Thread_)
        return;
      if (wait)
        ::WaitForSingleObject(Thread_, INFINITE);
      else
        ::TerminateThread(Thread_, 0);
      ::CloseHandle(Thread_);
    }
  private:
    HANDLE Thread_; // 
    static DWORD WINAPI ThreadProc(LPVOID prm)
    {
      (*reinterpret_cast<T*>(prm))();
      return 0;
    }
    Thread(const Thread&);
    Thread& operator = (const Thread&);
  };
 
  struct Job
  {
    virtual ~Job()
    {
    }
    void Release()
    {
      delete this;
    }
    virtual void* Get() = 0;
  };
 
  template <typename T>
  class JobImpl : public Job
  {
  public:
    JobImpl(T* data)
      : Data(data)
    {
    }
    void* Get()
    {
      return Data.get();
    }
  private:
    auto_ptr<T> Data;
  };
 
  class CriticalSection
  {
  public:
    CriticalSection()
    {
      ::InitializeCriticalSection(&Section);
    }
    ~CriticalSection()
    {
      ::DeleteCriticalSection(&Section);
    }
    void Enter()
    {
      ::EnterCriticalSection(&Section);
    }
    void Leave()
    {
      ::LeaveCriticalSection(&Section);
    }
  private:
    CRITICAL_SECTION Section;
    CriticalSection(const CriticalSection&);
    CriticalSection& operator = (const CriticalSection&);
  };
 
  template <typename T>
  class Locker
  {
  public:
    Locker(T& synObj)
      : SynObj(synObj)
    {
      SynObj.Enter();
    }
    ~Locker()
    {
      SynObj.Leave();
    }
  private:
    Locker(const Locker&);
    Locker& operator = (const Locker&);
    T& SynObj;
  };
 
  class JobList
  {
  public:
    JobList()
    {
    }
    ~JobList()
    {
      Clear();
    }
    void Clear()
    {
      Locker<CriticalSection> Locl(Section);
      for (list<Job*>::iterator i  = Jobs.begin() ; i != Jobs.end() ; ++i)
        try
        {
          (*i)->Release();
        }
        catch (...)
        {
        }
      Jobs.clear();
    }
    void Add(Job* job)
    {
      Locker<CriticalSection> Locl(Section);
      Jobs.push_back(job);
    }
    Job* Get()
    {
      Locker<CriticalSection> Locl(Section);
      Job* Result = 0;
      if (!Jobs.empty())
      {
        Result = Jobs.front();
        Jobs.pop_front();
      }
      return Result;
    }
  private:
    CriticalSection Section;
    list<Job*> Jobs;
    JobList(const JobList&);
    JobList& operator = (const JobList&);
  };
 
  class Event
  {
  public:
    Event()
      : Event_(NULL)
    {
      if (!(Event_ = ::CreateEvent(NULL, FALSE, FALSE, NULL)))
        throw exception("Can't create event");
    }
    ~Event()
    {
      ::CloseHandle(Event_);
    }
    void Enter()
    {
      ::WaitForSingleObject(Event_, INFINITE);
    }
    void Leave()
    {
      ::SetEvent(Event_);
    }
  private:
    Event(const Event&);
    Event& operator = (const Event&);
    HANDLE Event_;
  };
 
  class Writer
  {
  public:
    Writer(JobList& list, Event& event, int count)
      : List(list)
      , Count(count)
      , Event_(event)
    {
    }
    void operator () ()
    {
      for (int i = Count ; i >= 0 ; --i)
      {
        List.Add(new JobImpl<int>(new int(i)));
        Event_.Leave();
        if (!(i % 5)) // Спим после каждых 5 заданий :)
          ::Sleep(100);
      }
    }
  private:
    JobList& List;
    Event& Event_;
    int Count;
  };
 
  class Reader
  {
  public:
    Reader(JobList& list, Event& event)
      : List(list)
      , Event_(event)
    {
    }
    void operator () ()
    {
      while (true)
      {
        Job* CurJob = List.Get();
        if (!CurJob)
        {
          Event_.Enter();
          continue;
        }
        int Data = *reinterpret_cast<int*>(CurJob->Get());
        CurJob->Release();
        cout << Data << endl;
        if (!Data)
          break;
      }
    }
  private:
    JobList& List;
    Event& Event_;
  };
 
  class Sample
  {
  public:
    Sample()
      : Writer_(List, Event_, 100)
      , Reader_(List, Event_)
    {
      WriterThread.Run(&Writer_);
      ReaderThread.Run(&Reader_);
    }
  private:
    JobList List;
    Event Event_;
    Writer Writer_;
    Thread<Writer> WriterThread;
    Reader Reader_;
    Thread<Reader> ReaderThread;
  };
}
 
void main()
  try
  {
    Sample();
  }
  catch (exception& err)
  {
    cerr << err.what() << endl;
  }