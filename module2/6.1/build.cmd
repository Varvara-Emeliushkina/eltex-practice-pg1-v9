@echo off
chcp 65001 >nul

gcc -c contact.c -o contact.o  
gcc -c phonebook.c -o phonebook.o  
gcc -c ui.c -o ui.o  
gcc -c main.c -o main.o  


ar rc libphonebook.a contact.o phonebook.o



ar t libphonebook.a


gcc main.o ui.o -o phonebook.exe -L. -lphonebook

echo Сборка динамической библиотеки завершена!
pause
exit /b 0

:error
echo Ошибка на этапе сборки!
pause
exit /b 1p