#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>

void init_queue(PriorityQueue* queue) {
    queue->front = NULL;
    queue->size = 0;
}

int enqueue(PriorityQueue* queue, int data, unsigned int priority) {
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if (!new_node) {
        return 0;
    }
    
    new_node->data = data;
    new_node->priority = priority;
    new_node->next = NULL;
    
    // вставка в соответствии с приоритетом (чем выше число, тем выше приоритет)
    if (queue->front == NULL || priority > queue->front->priority) {
        // вставка в начало
        new_node->next = queue->front;
        queue->front = new_node;
    } else {
        // поиск позиции для вставки
        QueueNode* current = queue->front;
        while (current->next != NULL && current->next->priority >= priority) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
    
    queue->size++;
    return 1;
}

int dequeue_front(PriorityQueue* queue, int* data, unsigned int* priority) {
    if (is_queue_empty(queue)) {
        return 0;
    }
    
    QueueNode* temp = queue->front;
    *data = temp->data;
    if (priority != NULL) {
        *priority = temp->priority;
    }
    
    queue->front = queue->front->next;
    free(temp);
    queue->size--;
    
    return 1;
}

int dequeue_with_priority(PriorityQueue* queue, unsigned int target_priority, int* data) {
    if (is_queue_empty(queue)) {
        return 0;
    }
    
    // если первый элемент имеет нужный приоритет
    if (queue->front->priority == target_priority) {
        return dequeue_front(queue, data, NULL);
    }
    
    // поиск элемента с нужным приоритетом
    QueueNode* current = queue->front;
    while (current->next != NULL && current->next->priority != target_priority) {
        current = current->next;
    }
    
    if (current->next == NULL) {
        return 0; // элемент не найден
    }
    
    QueueNode* temp = current->next;
    *data = temp->data;
    current->next = temp->next;
    free(temp);
    queue->size--;
    
    return 1;
}

int dequeue_min_priority(PriorityQueue* queue, unsigned int min_priority, int* data, unsigned int* actual_priority) {
    if (is_queue_empty(queue)) {
        return 0;
    }
    
    QueueNode* current = queue->front;
    QueueNode* prev = NULL;
    QueueNode* candidate = NULL;
    QueueNode* prev_candidate = NULL;

    while (current != NULL) {
        if (current->priority <= min_priority) {
            candidate = current;
            prev_candidate = prev;
            break;
        }
        prev = current;
        current = current->next;
    }
    

    if (candidate == NULL && prev != NULL) {
        candidate = prev;
        prev_candidate = NULL; 
        current = queue->front;
        prev = NULL;
        while (current != NULL && current->next != NULL) {
            prev = current;
            current = current->next;
        }
        candidate = current;
        prev_candidate = prev;
    }
    
    if (candidate == NULL) {
        return 0; 
    }
    
    *data = candidate->data;
    *actual_priority = candidate->priority;
    
    if (prev_candidate == NULL) {
        queue->front = candidate->next;
    } else {
        prev_candidate->next = candidate->next;
    }
    
    free(candidate);
    queue->size--;
    return 1;
}

size_t queue_size(const PriorityQueue* queue) {
    return queue->size;
}

int is_queue_empty(const PriorityQueue* queue) {
    return queue->front == NULL;
}

void clear_queue(PriorityQueue* queue) {
    while (!is_queue_empty(queue)) {
        int temp_data;
        dequeue_front(queue, &temp_data, NULL);
    }
}

void print_queue(const PriorityQueue* queue) {
    if (is_queue_empty(queue)) {
        printf("Очередь пуста\n");
        return;
    }
    
    printf("Очередь (размер: %zu): ", queue->size);
    QueueNode* current = queue->front;
    while (current != NULL) {
        printf("[%d(p:%d)] ", current->data, current->priority);
        current = current->next;
    }
    printf("\n");
}