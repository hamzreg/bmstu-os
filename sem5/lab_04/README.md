# Лабораторная работа 4

Работа с процессами:

1) Создание процессов-сирот;
2) При помощи wait() процесс-предок дожидается завершения процессов-потомков;
3) Выполнение процессами-потомками других программ при помощи execlp();
4) Обмен сообщениями между процессом-предком и процессами-потомками при помощи pipe();
5) 4 + обработчик сигналов Ctrl+Z и Ctrl+C.

## Вопросы

1. "Создаются карты трансляции адресов для процесса-потомка". 
Что это значит?

Создаются таблицы страниц, описывающие виртуальное адресное пространство процесса-потомка - все ячейки памяти, к которым может ссылаться или обращаться программа.

--На семинаре--
Создать адресное пространство - описать его с помощью таблиц страниц. 

2. "Постранично дублируются области данных и стека родителя и модифицируются карты адресации потомка."
Вписывается ли это в концепцию copy-on-write?

Постраничное дублирование области данных и стека родителя не вписывается в концепцию copy-on-write, так как в данной концепции происходит копирование только тех страниц памяти, которые необходимо изменить. Копирование при записи происходит следующим образом. У процесса-потомка собственные таблицы страниц, но они ссылаются на страницы адресного пространства процесса-родителя. Для страниц данных и стека процесса-родителя права доступа меняются на "только для чтения", и для них устанавливается флаг copy-on-write. При попытке изменить страницу возникнет исключение по правам доступа. Будет запущен обработчик исключения, который обнаружит, что установлен флаг copy-on-write для страницы, и создаст ее копию в адресном пространстве того процесса, который пытался ее изменить.

--На семинаре--
У потомка собственные таблицы страниц, но они ссылаются на страницы адресного пространства предка. При этом, страницы данных и стека предка получают измененные права доступа - только для чтения. Устанавливается флаг copy-on-write. Если какой-то из процессов попытается изменить страницу, то возникнет исключение по правам доступа. Будет запущен на выполнение обработчик исключения, в результате выполнения которого будет обнаружен флаг copy-on-write. В результате, система создаст копию нужной страницы в адресном пространстве того процесса, который пытался её изменить. 