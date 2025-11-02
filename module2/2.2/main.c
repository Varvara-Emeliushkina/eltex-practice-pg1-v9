#include <stdio.h>
#include <locale.h>
#include "calculator.h"
#include "ui.h"

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    run_calculator();
    return 0;
}