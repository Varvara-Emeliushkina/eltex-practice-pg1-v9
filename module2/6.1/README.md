# Статическая библиотека для телефонной книги
Доработать решение задачи 4.1 (список контактов) так, чтобы структуры и функции по работе с двухсвязным упорядоченным списком находились в статической библиотеке.

### Компиляция объектных файлов
```c
gcc -c contact.c -o contact.o  
gcc -c phonebook.c -o phonebook.o  
gcc -c ui.c -o ui.o  
gcc -c main.c -o main.o  
```
### Создание статической библиотеки
```c
ar rc libphonebook.a contact.o phonebook.o
```
### Проверка содержимого библиотеки
```c
ar t libphonebook.a
```
### Компиляция с статической библиотекой
```c
gcc main.o ui.o -o phonebook.exe -L. -lphonebook
```
### Проверка размера (будет большим тк вся библиотека внутри)
```c
Get-ChildItem phonebook.exe | Format-List Name, Length
```
### Очистка (если надо)
```c
Remove-Item *.o
Remove-Item *.a
Remove-Item phonebook.exe
```


gcc -c contact.c phonebook.c
ar rc libphonebook.a contact.o phonebook.o
