@echo off
chcp 65001 >nul


gcc -shared -o libs\add.dll add.c
gcc -shared -o libs\subtract.dll subtract.c
gcc -shared -o libs\multiply.dll multiply.c
gcc -shared -o libs\divide.dll divide.c

gcc main.c -o calculator.exe


echo Сборка динамической библиотеки завершена!
pause
exit /b 0

:error
echo Ошибка на этапе сборки!
pause
exit /b 1