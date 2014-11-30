# coding: utf-8
""" Нужно получить все
    
    thinks : walk and regex
"""


import sys
import sys
from subprocess import call
import time
import os
slash = '\\'

def _check_extension(string, listOfExtension, listOfIgnoreExtention):
    """ может быть ошибка, хотя маловероятна. Точка вероятность повышает """
    for k in listOfExtension:
        if '.'+k == string[-len(k)-1:].lower():
            return True
    return False

def find_files_down_tree_PC(head, listOfExtension, ignoreDictOfLists):
    """ Получить список файлов заданных типов с полными путями
        
        thinks : сделать бы фильтрацию
        
        ignoreList
            1. пути - папки
            2. расширения, которые похожи на разрешенные
            3. целые файлы (с путем(1 шт) и без(может быть много))
            4. регулярные выражения - подстроки
            
        troubles testing :
            разные типы данных - возвр. знач. и сообщение - но нужно 
                принимать из функции два значения
                
        Simple local host 
    """
    
    def onErrorWalkPC(err):
        """ Error handler """
        print err
        
        # TODO: сделать свой класс обработки ошибок
        raise OSError
    
    resultList = list('')
    msg = ''
    # получаем объект для обхода
    # Если корня нет исключение генерируется при доступе
    try:
        gettedList = os.walk(head, onerror = onErrorWalkPC)
        for root, dirs, files in gettedList:
            for name in files:
                if _check_extension(name, listOfExtension, ignoreDictOfLists[ 'Extentions' ]):
                    bResult = True
                    if ignoreDictOfLists['Dirs']:
                        for it in ignoreDictOfLists['Dirs']:
                            if it in root:
                                bResult = False
                        
                    if bResult:
                        resultList.append(root+slash+name)
    except OSError:
        msg = 'OSError on dir walk.'
        return None, msg

    # возвращаем что насобирали
    return resultList, msg    # может в питоне и нече, но вообеще-то...?
        # вобщем нужно подумать над обработкой ошибок

""" Mapping """
find_files_down_tree_ = find_files_down_tree_PC    # поиск по обычному компьютеру

def init_test():
    roots = ['uno']
    listOfExtension = ['py']
    
    ignoreLists = {}
    ignoreLists[ 'Extentions' ] = [ '' ]
    ignoreLists[ 'Dirs' ] = ['.\\bin', '.\\.git', '.\\sbox', 'Analysers']
    return roots, listOfExtension, ignoreLists, ignoreLists, ignoreLists


def Run(initializer):
    listToOpen = []
    listToOpen.append("notepad++")
    listToOpen.extend(get_list_to_open(initializer))
    for at in listToOpen:
        print at
    call(listToOpen)
    
def get_list_to_open(initializer):
    # get settings for find
    roots, listOfExtension, ignoreLists, ignoreLists, ignoreLists = initializer()
    
    result_argv = ''
    for pathes in roots:
        for at in listOfExtension:
            listSlice = list()
            listSlice.append(at)
         
            # поиск
            resultList, msg = find_files_down_tree_(pathes, listSlice, ignoreLists)
            
            # список получен, можно его обработать
            # в принципе можно передать указатель на функцию обработки
            if resultList:
                resultList.append('#')
                result_argv += '#'.join(resultList)
        
 
    # open editor
    result_argv = result_argv.split('#')
    return result_argv
    
    
def get_list_files(initializer):
    """ """
    # get settings for find
    roots, listOfExtension, ignoreLists, ignoreLists, ignoreLists = initializer()
    
    result_argv = ''
    for pathes in roots:
        for at in listOfExtension:
            listSlice = list()
            listSlice.append(at)
         
            # поиск
            resultList, msg = find_files_down_tree_(pathes, listSlice, ignoreLists)
            
            # список получен, можно его обработать
            # в принципе можно передать указатель на функцию обработки
            resultList.append('#')
            result_argv += '#'.join(resultList)
        
 
    # open editor
    result_argv = result_argv.split('#')
    return result_argv

def init():
  roots = ['third_party', 'cuda']
  listOfExtension = ['c', 'h', 'cu', 'cc', 'cpp', 'hpp']
  
  ignoreLists = {}
  ignoreLists[ 'Extentions' ] = [ '' ]
  ignoreLists[ 'Dirs' ] = ['.\\bin', '.\\.git', 'third_party/gmock-1.6.0']
  return roots, listOfExtension, ignoreLists, ignoreLists, ignoreLists
  
if __name__ == "__main__":
  Run(init)
