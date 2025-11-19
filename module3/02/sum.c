#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s <число1> <число2> ... <числоN>\n", argv[0]);
        return 1;
    }
    
    double sum = 0;
    printf("Суммируем: ");
    
    for (int i = 1; i < argc; i++) {
        double num = atof(argv[i]);
        printf("%.2f ", num);
        sum += num;
    }
    
    printf("\nРезультат: %.2f\n", sum);
    return 0;
}