#define _POSIX_C_SOURCE 200809L // для  strdup() определяем POSIX стандарт
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // для работы с unix (fork, exec, access)
#include <sys/wait.h> // для одидания завершения процессов
#include <sys/types.h> // библиотека с определениями типов данных (pid_t)

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64

// функция для разделения строки на аргументы
int parse_input(char *input, char *args[]) {
    int argc = 0;
    char *token = strtok(input, " \t\n"); // ищем первое слово в строке (разделители: проеблы табуляция перенос строк)
    
    while (token != NULL && argc < MAX_ARGS - 1) {
        args[argc] = token;
        argc++;
        token = strtok(NULL, " \t\n");
    }
    args[argc] = NULL; // завершаем массив NULL
    
    return argc;
}

// функция для поиска программы в PATH
char* find_program_in_path(const char *program) {
    char *path = getenv("PATH"); // получаем значение переменно среды 
    if (path == NULL) return NULL;
    
    char *path_copy = strdup(path); // создаем копию PATH члобы потом изменять ее strtok (оригинал нам портить нельзя)
    char *dir = strtok(path_copy, ":"); // разбиваем на папки (dir указывает на первую)
    
    static char full_path[1024];
    
    while (dir != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, program); // соединяем папку и имя программы
        if (access(full_path, X_OK) == 0) { // проверяем есль ли такой файл и можно ли его выполнить
            free(path_copy);
            return full_path;
        }
        dir = strtok(NULL, ":"); // берем следующую папку из path и тд
    }
    
    free(path_copy); // в печальном случае мы ничего не нашли и чистим копию
    return NULL;
}

// функция запуска 
int execute_command(char *args[]) {
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    
    if (pid == 0) {
        // дочерний процесс
        execvp(args[0], args); // ищет программу в PATH  и запускает с аргс
        
        // execvp вернул управление, значит ошибка => будем пробовать искать по полному пути
        char *full_path = find_program_in_path(args[0]);
        if (full_path != NULL) {
            execv(full_path, args);
        }
        
        // если и это не сработало пробуем как относительный путь
        execv(args[0], args);
        
        perror("exec");
        exit(EXIT_FAILURE);
    } else {
        // родительский процесс
        int status;
        waitpid(pid, &status, 0);
        return status;
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];
    char *args[MAX_ARGS];
    
    printf("=== Командный интерпретатор ===\n");
    printf("Введите 'exit' для выхода\n\n");
    
    while (1) {
        
        printf("myshell> ");
        fflush(stdout);
        
        // чтение ввода
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nДо свидания!\n");
            break;
        }
        
        // удаление символа новой строки
        input[strcspn(input, "\n")] = 0;
        
        // проверка на выход
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            printf("До свидания!\n");
            break;
        }
        
        // пропуск пустых строк
        if (strlen(input) == 0) {
            continue;
        }
        
        // разбор ввода на аргументы
        int argc = parse_input(input, args);
        
        if (argc == 0) {
            continue;
        }
        
        // запуск команды
        execute_command(args);
    }
    
    return 0;
}