@echo off
chcp 65001 >nul

contact.c, contact.h
phonebook.c, phonebook.h  
ui.c, ui.h
main.c

gcc -c -fPIC contact.c -o contact.o
gcc -c -fPIC phonebook.c -o phonebook.o
gcc -shared -o libphonebook.dll contact.o phonebook.o

gcc -c ui.c -o ui.o
gcc -c main.c -o main.o

gcc main.o ui.o -o phonebook.exe -L. -lphonebook

echo Сборка динамической библиотеки завершена!
pause
exit /b 0

:error
echo Ошибка на этапе сборки!
pause
exit /b 1