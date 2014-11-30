//---------------------------------------------------------------------------

#ifndef ReadThH
#define ReadThH
#include <Classes.hpp>
//---------------------------------------------------------------------------
//поток для чтения последовательности байтов из COM-порта в буфер
class ReadThread : public TThread
{
 private:
        int tmplen;
        void __fastcall Printing();	//вывод принятых байтов на экран и в файл
        OVERLAPPED overlapped;		//будем использовать для операций чтения (см. поток ReadThread)
        OVERLAPPED ov;       	//будем использовать для операций записи (см. поток WriteThread)
 protected:
        void __fastcall Execute();	//основная функция потока
 public:
        __fastcall ReadThread(bool CreateSuspended);	//конструктор потока
        char bufrd[255]; //приёмный
//
        int cycle; //число циклов(адресс) чтения
};
#endif
