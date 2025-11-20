# Калькулятор с динамическими библиотеками
Доработать решение задачи 2.3 (калькулятор) так, чтобы функции загружались из динамических библиотек.
В одной библиотеке находится одна функция. При запуске программы считывается каталог с библиотеками и загружаются найденные функции.

### Создаем папки для библиотек
```c
mkdir libs
```
### Потом компиляция каждой функции в отдельную DLL
```c
gcc -shared -o libs\add.dll add.c
gcc -shared -o libs\subtract.dll subtract.c
gcc -shared -o libs\multiply.dll multiply.c
gcc -shared -o libs\divide.dll divide.c
```
### Компиляция основной программы
```c
gcc main.c -o calculator.exe
```
### Проверка структуры
```c
Get-ChildItem libs
```
### Очистка (если надо)
```c
Remove-Item calculator.exe
Remove-Item libs\* -Recurse
```