#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // для процессов
#include <sys/wait.h> // для ожидания процессов
#include <string.h>
#include <ctype.h> // для проверки типов символов

// проверка на число (целое или вещественное)
int is_number(const char *str) {
    char *endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

// функция для обработки аргументов
void process_args(int start, int end, char *argv[]) {
    for (int i = start; i < end; i++) {
        if (is_number(argv[i])) {
            double num = atof(argv[i]);
            printf("PID %d: \n %s \n %s * 2 = %.2f\n", getpid(), argv[i], argv[i], num * 2);
        } else {
            printf("PID %d: %s\n", getpid(), argv[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <arg1> <arg2> ... <argN>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int total_args = argc - 1;
    int mid = total_args / 2;

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // дочерний процесс обрабатывает первую половину аргументов
        printf("Child process (PID %d):\n", getpid());
        process_args(1, 1 + mid, argv);
        exit(EXIT_SUCCESS);
    } else {
        // родительский процесс вторую
        printf("Parent process (PID %d):\n", getpid());
        process_args(1 + mid, argc, argv);
        wait(NULL); // ждем завершения дочернего
    }

    return 0;
}