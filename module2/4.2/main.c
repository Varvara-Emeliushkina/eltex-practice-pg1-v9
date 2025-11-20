#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "priority_queue.h"
#include "ui.h"

int main(void) {
    
    setlocale(LC_ALL, "ru_RU.UTF-8");

    PriorityQueue queue;
    int choice;
    
    // инициализация очереди
    init_queue(&queue);
    
    printf("Демонстрация приоритетной очереди\n");
    printf("Приоритет: 255 (низкий) - 0 (высокий)\n");
    
    do {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода!\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1:
                handle_enqueue(&queue);
                break;
            case 2:
                handle_dequeue_front(&queue);
                break;
            case 3:
                handle_dequeue_with_priority(&queue);
                break;
            case 4:
                handle_dequeue_min_priority(&queue);
                break;
            case 5:
                simulate_message_generation(&queue);
                break;
            case 6:
                print_queue_status(&queue);
                break;
            case 7:
                clear_queue(&queue);
                printf("Очередь очищена\n");
                break;
            case 0:
                printf("Выход...\n");
                break;
            default:
                printf("Неверный выбор!\n");
                break;
        }
        
    } while (choice != 0);
    
    clear_queue(&queue);
    
    return 0;
}