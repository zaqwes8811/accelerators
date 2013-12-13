//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ReadTh.h"
#include "Unit1.h" // заголовок формы
#include "UCom.h"
// !!!!  в целом если что то будет проходить не гладко, то из за того что
// мало провероквсе будет криво
// 

//---------------------------------------------------------------------------

#pragma package(smart_init)


//---------------------------------------------------------------------------
//конструктор потока ReadThread, по умолчанию пустой
__fastcall ReadThread::ReadThread(bool CreateSuspended) : TThread(CreateSuspended)
{}

//---------------------------------------------------------------------------

//главная функция потока, реализует приём байтов из COM-порта
void __fastcall ReadThread::Execute()
{
 COMSTAT comstat;		//структура текущего состояния порта, в данной программе используется для определения количества принятых в порт байтов
 DWORD btr, temp, mask, signal, signal2;	//переменная temp используется в качестве заглушки
 overlapped.hEvent = CreateEvent(NULL, true, true, NULL);	//создать сигнальный объект-событие для асинхронных операций
 ov.hEvent=CreateEvent(NULL, true, true, NULL);
 //ov.hEvent = CreateEvent(NULL, true, true, NULL);	//создать сигнальный объект-событие для асинхронных операций

 SetCommMask(Form1->ports.GetHandCom(), EV_RXCHAR);                   	        //установить маску на срабатывание по событию приёма байта в порт
 // !!!!могут быть лишние системные вызовы отсюда и скорость!!!
 // !! даже если приняты все байты, в режим ожидания приходит не сразу
 while(!Terminated){						//пока поток не будет прерван, выполняем цикл
  // WaitCommEvent(COMport, &mask, &overlapped);               	//ожидать события приёма байта (это и есть перекрываемая операция)
  WaitCommEvent(Form1->ports.GetHandCom(), &mask, &overlapped); //== 0){  // вроде ничего страшного не происходит
         // ResetEvent(overlapped.hEvent) ;
          //ERROR_IO_PENDING - вернет ошибку   ? недождалась
          //}
          //DWORD er=GetLastError();
   signal = WaitForSingleObject(overlapped.hEvent, INFINITE);	//приостановить поток до прихода байта
   // Inf нужно бы заменить на что то другое
   if(signal == WAIT_TIMEOUT){ // ? что будет дальше, когда ожиадние истеке
      ShowMessage("Отсутствуем связь с модулятором");
   }
   if(signal == WAIT_OBJECT_0) //если событие прихода байта произошло
   {     // что ниже только из-за того, что даже прочитав все цикл еще раз оборачивается
     if(GetOverlappedResult(Form1->ports.GetHandCom(), &overlapped, &temp, true)){ //проверяем, успешно ли завершилась перекрываемая операция WaitCommEvent
      if((mask & EV_RXCHAR)!=0)
      {					//если произошло именно событие прихода байта
        ClearCommError(Form1->ports.GetHandCom(), &temp, &comstat);		//нужно заполнить структуру COMSTAT
        btr = comstat.cbInQue;                          	//и получить из неё количество принятых байтов
        if(btr)  // даже если считали все байты, то все равно overlapped.hEvent=занято
        {                         			//если действительно есть байты для чтения
         ReadFile(Form1->ports.GetHandCom(), bufrd, 39, &temp, &ov);
              //????????? ????? ?? ????? ? ????? ?????????
              signal2=WaitForSingleObject(ov.hEvent, INFINITE);
              if(signal2 ==WAIT_OBJECT_0 ){
         if(GetOverlappedResult(Form1->ports.GetHandCom(), &ov, &temp, true)){
           tmplen=temp;     //??????????? ??????? ??????
           cycle++;
              Synchronize(Printing);
           }
           }
          //1
        }  //2
      } //3
    }
  }
 }
 CloseHandle(overlapped.hEvent);		//перед выходом из потока закрыть объект-событие
 CloseHandle(ov.hEvent);		//перед выходом из потока закрыть объект-событие

}

//---------------------------------------------------------------------------
//выводим принятые байты на экран и в файл (если включено)
void __fastcall ReadThread::Printing()
{ // обарботка полученного сообщения
  // обработка полученного массива идет здесь
 Form1->ListBox1->Items->Add("Всего принято " + IntToStr(tmplen) + " байт");	//выводим счётчик в строке состояния
 // сохранение
 if(cycle == 1){ // была дана команда получения состояни
   /* - проверяем контрольную сумму
      - если все хорошо проверяем анализируем
      - елси нет (ошибка связи с модучлятором)
   */
    Form1->UTestCSum.GetConSum(bufrd, tmplen-1);  // вычисляем контрольную сумму приянтого
    if(Form1->UTestCSum.cSum[0] != bufrd[tmplen-1]){ // сравниваем
      Form1->Shape2->Brush->Color=clRed; //  ошибка связи с модулятором(контр. сум. )
      Form1->Label4->Caption="Ошибка связи с модулятором";
      Form1->ListBox1->Items->Add("Несовпадение контрольных сумм");
     } else{
     Form1->Shape2->Brush->Color=clBackground; //  ошибка связи с модулятором(контр. сум. )
     Form1->Label4->Caption="Контрольные суммы совпали";
     Form1->AnalyseState(bufrd,tmplen-1); // анализируем результаты
    }
   }
   if(cycle == 2){ // отправлена команда на получение установовк
     Form1->UTestCSum.GetConSum(bufrd, tmplen-1);  // вычисляем контрольную сумму приянтого
     //if(Form1->UTestCSum.cSum[0] != bufrd[tmplen-1]){
     // Form1->Shape2->Brush->Color=clRed; //  ошибка связи с модулятором(контр. сум. )
     // Form1->Label4->Caption="Ошибка связи с модулятором";
     //// Form1->ListBox1->Items->Add("Несовпадение контрольных сумм");
     //}else{
       AnsiString Out_set;
       Form1->Shape2->Brush->Color=clBackground; //  ошибка связи с модулятором(контр. сум. )
       Form1->Label4->Caption="Контрольные суммы совпали";
       // перобразуем в анси код хекс
       Out_set="Установки: ";
       for(int i=0;i<11-1;i++){   // для первых 10 так
         int tmpt=static_cast<int>(bufrd[i]);
         int ibuf[2];
         ibuf[1]=tmpt%16; // L
         tmpt/=16;
         ibuf[0]=tmpt%16;
         if(ibuf[0]>9){
         switch(ibuf[0]){
          case 10: Out_set+="A"; break;
          case 11: Out_set+="B"; break;
          case 12: Out_set+="C"; break;
          case 13: Out_set+="D"; break;
          case 14: Out_set+="E"; break;
          case 15: Out_set+="F"; break;
          }
         }else{
         Out_set+=IntToStr(ibuf[0]);
         }
        if(ibuf[1]>9){
        switch(ibuf[1]){
        case 10: Out_set+="A"; break;
        case 11: Out_set+="B"; break;
        case 12: Out_set+="C"; break;
        case 13: Out_set+="D"; break;
        case 14: Out_set+="E"; break;
        case 15: Out_set+="F"; break;
       }
      }else{
       Out_set+=IntToStr(ibuf[1]);
      }
        Out_set+=" "; // через тире
       }
       Form1->ListBox1->Items->Add(Out_set);
       Out_set="A0-A3 : ";
       for(int i=10;i<14;i++){   // для блок из 4А
         int tmpt=static_cast<int>(bufrd[i]);
         int ibuf[2];
         ibuf[1]=tmpt%16; // L
         tmpt/=16;
         ibuf[0]=tmpt%16;
         if(ibuf[0]>9){
         switch(ibuf[0]){
          case 10: Out_set+="A"; break;
          case 11: Out_set+="B"; break;
          case 12: Out_set+="C"; break;
          case 13: Out_set+="D"; break;
          case 14: Out_set+="E"; break;
          case 15: Out_set+="F"; break;
          }
         }else{
         Out_set+=IntToStr(ibuf[0]);
         }
        if(ibuf[1]>9){
        switch(ibuf[1]){
        case 10: Out_set+="A"; break;
        case 11: Out_set+="B"; break;
        case 12: Out_set+="C"; break;
        case 13: Out_set+="D"; break;
        case 14: Out_set+="E"; break;
        case 15: Out_set+="F"; break;
       }
      }else{
       Out_set+=IntToStr(ibuf[1]);
      }
      if((i)%2 != 0)Out_set+=" "; // через тире
      } // end for
    Form1->ListBox1->Items->Add(Out_set);
    // перобразуем в анси код хекс
       Out_set="B0-B5 :   ";
       for(int i=14;i<20;i++){   // для блок из 4А
         int tmpt=static_cast<int>(bufrd[i]);
         int ibuf[2];
         ibuf[1]=tmpt%16; // L
         tmpt/=16;
         ibuf[0]=tmpt%16;
         if(ibuf[0]>9){
         switch(ibuf[0]){
          case 10: Out_set+="A"; break;
          case 11: Out_set+="B"; break;
          case 12: Out_set+="C"; break;
          case 13: Out_set+="D"; break;
          case 14: Out_set+="E"; break;
          case 15: Out_set+="F"; break;
          }
         }else{
         Out_set+=IntToStr(ibuf[0]);
         }
        if(ibuf[1]>9){
        switch(ibuf[1]){
        case 10: Out_set+="A"; break;
        case 11: Out_set+="B"; break;
        case 12: Out_set+="C"; break;
        case 13: Out_set+="D"; break;
        case 14: Out_set+="E"; break;
        case 15: Out_set+="F"; break;
       }
      }else{
       Out_set+=IntToStr(ibuf[1]);
      }
      if((i)%2 != 0)Out_set+=" "; //
      } // end for
    Form1->ListBox1->Items->Add(Out_set);
    Out_set="B6-B11 : ";
    for(int i=20;i<26;i++){   // для блок из 4А
         int tmpt=static_cast<int>(bufrd[i]);
         int ibuf[2];
         ibuf[1]=tmpt%16; // L
         tmpt/=16;
         ibuf[0]=tmpt%16;
         if(ibuf[0]>9){
         switch(ibuf[0]){
          case 10: Out_set+="A"; break;
          case 11: Out_set+="B"; break;
          case 12: Out_set+="C"; break;
          case 13: Out_set+="D"; break;
          case 14: Out_set+="E"; break;
          case 15: Out_set+="F"; break;
          }
         }else{
         Out_set+=IntToStr(ibuf[0]);
         }
        if(ibuf[1]>9){
        switch(ibuf[1]){
        case 10: Out_set+="A"; break;
        case 11: Out_set+="B"; break;
        case 12: Out_set+="C"; break;
        case 13: Out_set+="D"; break;
        case 14: Out_set+="E"; break;
        case 15: Out_set+="F"; break;
       }
      }else{
       Out_set+=IntToStr(ibuf[1]);
      }
      if((i)%2 != 0)Out_set+=" "; // через тире
      } // end for
    Form1->ListBox1->Items->Add(Out_set);
   }
   if(cycle == 3){  // должны получить эхо
      if(bufrd[0]== 3){ // все хорошо
         // отпраляем данные
         Form1->AssembleSettings();
      } else{
       Form1->ListBox1->Items->Add("Несовпадение контрольных сумм");
     }
   } //
   if(cycle == 4){  // читаем вернувшуюся контрльную сумму
      if(bufrd[0]==Form1->UCharArr.cSum[0]){
          Form1->Shape2->Brush->Color=clBackground; //  ошибка связи с модулятором(контр. сум. )
          Form1->Label4->Caption="Контрольные суммы совпали";
          Form1->AnalyseState(bufrd,tmplen-1); // анализируем результаты
        } else {
      Form1->Shape2->Brush->Color=clRed; //  ошибка связи с модулятором(контр. сум. )
      Form1->Label4->Caption="Ошибка связи с модулятором";
      Form1->ListBox1->Items->Add("Несовпадение контрольных сумм");
      }
   }
   if(cycle == 5){   // послана команда на перезагрузку
       if(bufrd[0]== 2){
           Form1->ListBox1->Items->Add("Модулятор перезагружен");
       } else{
         Form1->ListBox1->Items->Add("Эхо получено неверно");
         Form1->Shape2->Brush->Color=clBackground; //  ошибка связи с модулятором(контр. сум. )
         Form1->AnalyseState(bufrd,tmplen-1); // анализируем результаты
   
       }
   }

 //
 //if(Form1->CheckBox3->Checked == true)  //если включен режим вывода в файл
  //{
  write(Form1->handle, bufrd, tmplen); //записать в файл данные из приёмного буфера
  //}
 memset(bufrd, 0, 255);	        //очистить буфер (чтобы данные не накладывались друг на друга)
 tmplen=0;
}
//

