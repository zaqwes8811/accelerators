//---------------------------------------------------------------------------

#ifndef WriteThH
#define WriteThH
#include <Classes.hpp>
//---------------------------------------------------------------------------
//поток для записи последовательности байтов из буфера в COM-порт
class WriteThread : public TThread
{
private:
        OVERLAPPED overlappedwr;       	//будем использовать для операций записи (см. поток WriteThread)
 	void __fastcall Printing();	//вывод состояния на экран
protected:
        void __fastcall Execute();      //основная функция потока
public:
        __fastcall WriteThread(bool CreateSuspended);	//конструктор потока

};
#endif
