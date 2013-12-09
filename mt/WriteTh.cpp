//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "WriteTh.h"
#include "Unit1.h" // заголовок формы
#include "UCom.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
__fastcall WriteThread::WriteThread(bool CreateSuspended) : TThread(CreateSuspended)
{}

//---------------------------------------------------------------------------

//главная функция потока, выполняет передачу байтов из буфера в COM-порт
void __fastcall WriteThread::Execute()
{
 DWORD temp, signal;	//temp - переменная-заглушка

 overlappedwr.hEvent = CreateEvent(NULL, true, true, NULL);   	  //создать событие

 while(!Terminated)     //пока поток не будет завершён, выполнять цикл
 {
 // хэндл нельзя копировать, верннее от то работает, то нет Form1->UCharArr.store

  char stor=static_cast<char>(0xFF);
  char stora[2];
  stora[0]=stor;
  int buf=static_cast<int>(Form1->UCharArr.store[0]);
  WriteFile(Form1->ports.GetHandCom(),stora ,1, &temp, &overlappedwr);  //записать байты в порт (перекрываемая операция!)
  Form1->lensend=0;  // сбразсываем число байт для записи
  // для режима ответ-выдать-получть нужно сделать ожидание еще одного сбытия
  signal = WaitForSingleObject(overlappedwr.hEvent, INFINITE);	  //приостановить поток, пока не завершится перекрываемая операция WriteFile
  if((signal == WAIT_OBJECT_0) && (GetOverlappedResult(Form1->ports.GetHandCom(), &overlappedwr, &temp, true))) {
  Form1->fl = true;

  } //если операция завершилась успешно, установить соответствующий флажок
  else Form1->fl = false;

  Synchronize(Printing);	//вывести состояние операции в строке состояния
  Suspend();		//приостановить поток записи в порт, пока он не потребуется снова
 }
 CloseHandle(overlappedwr.hEvent);		//перед выходом из потока закрыть объект-событие
}

//---------------------------------------------------------------------------

//вывод состояния передачи данных на экран
void __fastcall WriteThread::Printing()
{
 if(!Form1->fl)	//проверяем состояние флажка
  {
   Form1->ListBox1->Items->Add("Ошибка передачи");
   return;
  }
 Form1->ListBox1->Items->Add("Передача прошла успешно");

}
