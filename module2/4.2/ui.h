#ifndef UI_H
#define UI_H

#include "priority_queue.h"

void display_menu(void);
void handle_enqueue(PriorityQueue* queue);
void handle_dequeue_front(PriorityQueue* queue);
void handle_dequeue_with_priority(PriorityQueue* queue);
void handle_dequeue_min_priority(PriorityQueue* queue);
void simulate_message_generation(PriorityQueue* queue);
void print_queue_status(const PriorityQueue* queue);

#endif