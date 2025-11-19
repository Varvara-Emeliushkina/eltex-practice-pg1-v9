#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s <строка1> <строка2> ... <строкаN>\n", argv[0]);
        return 1;
    }
    
    int max_length = 0;
    char *longest_string = NULL;
    
    for (int i = 1; i < argc; i++) {
        int len = strlen(argv[i]);
        printf("'%s' - длина %d\n", argv[i], len);
        
        if (len > max_length) {
            max_length = len;
            longest_string = argv[i];
        }
    }
    
    if (longest_string != NULL) {
        printf("Самая длинная строка: '%s' (длина: %d)\n", longest_string, max_length);
    }
    
    return 0;
}