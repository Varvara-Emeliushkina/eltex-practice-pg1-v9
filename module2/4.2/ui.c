#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void display_menu(void) {
    printf("\n=== ПРИОРИТЕТНАЯ ОЧЕРЕДЬ ===\n");
    printf("1. Добавить элемент\n");
    printf("2. Извлечь первый элемент\n");
    printf("3. Извлечь элемент с указанным приоритетом\n");
    printf("4. Извлечь элемент с приоритетом не ниже заданного\n");
    printf("5. Сгенерировать тестовые сообщения\n");
    printf("6. Показать очередь\n");
    printf("7. Очистить очередь\n");
    printf("0. Выход\n");
    printf("Выберите действие: ");
}

void handle_enqueue(PriorityQueue* queue) {
    int data;
    unsigned priority;
    
    printf("Введите данные (целое число): ");
    if (scanf("%d", &data) != 1) {
        printf("Ошибка ввода данных!\n");
        while (getchar() != '\n'); // очистка буфера
        return;
    }
    
    printf("Введите приоритет (0-255): ");
    if (scanf("%u", &priority) != 1 || priority > 255) {
        printf("Ошибка: приоритет должен быть от 0 до 255!\n");
        while (getchar() != '\n');
        return;
    }
    
    if (enqueue(queue, data, (unsigned int)priority)) {
        printf("Элемент %d с приоритетом %u успешно добавлен\n", data, priority);
    } else {
        printf("Ошибка при добавлении элемента!\n");
    }
}

void handle_dequeue_front(PriorityQueue* queue) {
    int data;
    unsigned int priority;
    
    if (dequeue_front(queue, &data, &priority)) {
        printf("Извлечен элемент: %d с приоритетом %d\n", data, priority);
    } else {
        printf("Очередь пуста!\n");
    }
}

void handle_dequeue_with_priority(PriorityQueue* queue) {
    unsigned target_priority;
    int data;
    
    printf("Введите приоритет для поиска (0-255): ");
    if (scanf("%u", &target_priority) != 1 || target_priority > 255) {
        printf("Ошибка: приоритет должен быть от 0 до 255!\n");
        while (getchar() != '\n');
        return;
    }
    
    if (dequeue_with_priority(queue, (unsigned int)target_priority, &data)) {
        printf("Извлечен элемент: %d с приоритетом %u\n", data, target_priority);
    } else {
        printf("Элемент с приоритетом %u не найден\n", target_priority);
    }
}

void handle_dequeue_min_priority(PriorityQueue* queue) {
    unsigned min_priority;
    int data;
    unsigned int actual_priority;
    
    printf("Введите минимальный приоритет (0-255): ");
    if (scanf("%u", &min_priority) != 1 || min_priority > 255) {
        printf("Ошибка: приоритет должен быть от 0 до 255!\n");
        while (getchar() != '\n');
        return;
    }
    
    if (dequeue_min_priority(queue, (unsigned int)min_priority, &data, &actual_priority)) {
        printf("Извлечен элемент: %d с приоритетом %d\n", data, actual_priority);
    } else {
        printf("Элемент с приоритетом не ниже %u не найден\n", min_priority);
    }
}

void simulate_message_generation(PriorityQueue* queue) {
    int num_messages;
    
    printf("Сколько сообщений сгенерировать? ");
    if (scanf("%d", &num_messages) != 1 || num_messages <= 0) {
        printf("Ошибка ввода!\n");
        return;
    }
    
    srand((unsigned int)time(NULL));
    
    printf("Генерация %d сообщений:\n", num_messages);
    for (int i = 0; i < num_messages; i++) {
        int data = 1000 + i; // произвольные данные
        unsigned int priority = (unsigned int)(rand() % 256); // случайный приоритет 0-255
        
        if (enqueue(queue, data, priority)) {
            printf("  Сообщение %d с приоритетом %d\n", data, priority);
        }
    }
    
    printf("Все сообщения добавлены в очередь\n");
}

void print_queue_status(const PriorityQueue* queue) {
    printf("\n--- Состояние очереди ---\n");
    print_queue(queue);
    printf("--------------------------\n");
}