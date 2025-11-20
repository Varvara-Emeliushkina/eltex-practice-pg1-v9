#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stddef.h>

// структура элемента очереди
typedef struct QueueNode {
    int data;
    unsigned int priority;  // приоритет от 0 до 255
    struct QueueNode* next;
} QueueNode;

// структура очереди
typedef struct {
    QueueNode* front;
    size_t size;
} PriorityQueue;

// инициализация очереди
void init_queue(PriorityQueue* queue);

// добавление элемента в очередь
int enqueue(PriorityQueue* queue, int data, unsigned int priority);

// извлечение первого элемента
int dequeue_front(PriorityQueue* queue, int* data, unsigned int* priority);

// извлечение элемента с указанным приоритетом
int dequeue_with_priority(PriorityQueue* queue, unsigned int target_priority, int* data);

// извлечение элемента с приоритетом не ниже заданного
int dequeue_min_priority(PriorityQueue* queue, unsigned int min_priority, int* data, unsigned int* actual_priority);


size_t queue_size(const PriorityQueue* queue);
int is_queue_empty(const PriorityQueue* queue);
void clear_queue(PriorityQueue* queue);
void print_queue(const PriorityQueue* queue);

#endif