# Динамическая библиотека для телефонной книги
Доработать решение задачи 6.1 (список контактов) так, чтобы структуры и функции по работе с двухсвязным упорядоченным списком находились в динамической библиотеке.

### Берем те же исходные файлы что в 6.1:
```c
contact.c, contact.h
phonebook.c, phonebook.h  
ui.c, ui.h
main.c
```
### Компилируем как динамическую библиотеку
```c
gcc -c -fPIC contact.c -o contact.o
gcc -c -fPIC phonebook.c -o phonebook.o
gcc -shared -o libphonebook.dll contact.o phonebook.o
```
### Компилируем остальные
```c
gcc -c ui.c -o ui.o
gcc -c main.c -o main.o
```
### Линкуем с динамической библиотекой
```c
gcc main.o ui.o -o phonebook.exe -L. -lphonebook
```
### Очистка (если надо)
```c
Remove-Item *.o
Remove-Item *.a
Remove-Item phonebook.exe
```