#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* read_input(void) {
    char buffer[1024];
    if (scanf("%1023s", buffer) != 1) {
        return NULL;
    }
    
    char* result = malloc(strlen(buffer) + 1);
    if (result) {
        strcpy(result, buffer);
    }
    return result;
}

void display_simulation_results(SimulationStats* stats, IPv4Address gateway, IPv4Address subnet_mask) {
    if (!stats) {
        printf("Ошибка: статистика не доступна\n");
        return;
    }
    
    char* gateway_str = ip_to_string(gateway);
    char* mask_str = ip_to_string(subnet_mask);
    IPv4Address network = calculate_network_address(gateway, subnet_mask);
    IPv4Address broadcast = calculate_broadcast_address(gateway, subnet_mask);
    char* network_str = ip_to_string(network);
    char* broadcast_str = ip_to_string(broadcast);
    
    printf("\n=== Результаты симуляции сети ===\n");
    printf("Шлюз: %s\n", gateway_str);
    printf("Маска подсети: %s\n", mask_str);
    printf("Адрес сети: %s\n", network_str);
    printf("Широковещательный адрес: %s\n", broadcast_str);
    printf("Количество пакетов: %d\n", stats->total_packets);
    printf("\n--- Статистика ---\n");
    printf("Пакеты своей подсети: %d шт. (%.2f%%)\n", 
           stats->same_network_count, stats->same_network_percent);
    printf("  - из них сгенерировано принудительно: %d шт.\n", stats->forced_same_network);
    printf("Пакеты других сетей: %d шт. (%.2f%%)\n", 
           stats->different_network_count, stats->different_network_percent);
    printf("==============================\n\n");
    
    free(gateway_str);
    free(mask_str);
    free(network_str);
    free(broadcast_str);
}

void display_menu(void) {
    printf("\n=== Симулятор сетевого трафика ===\n");
    printf("1. Запустить симуляцию\n");
    printf("2. Выйти\n");
    printf("Выберите опцию: ");
}

void process_simulation_input(void) {
    printf("Введите IP адрес шлюза (например, 192.168.1.1): ");
    char* gateway_input = read_input();
    if (!gateway_input) {
        printf("Ошибка чтения IP шлюза\n");
        return;
    }
    
    printf("Введите маску подсети (например, 255.255.255.0): ");
    char* mask_input = read_input();
    if (!mask_input) {
        printf("Ошибка чтения маски подсети\n");
        free(gateway_input);
        return;
    }
    
    printf("Введите количество пакетов для симуляции: ");
    char* packets_input = read_input();
    if (!packets_input) {
        printf("Ошибка чтения количества пакетов\n");
        free(gateway_input);
        free(mask_input);
        return;
    }
    
    // преобразование введенных данных
    IPv4Address gateway = string_to_ip(gateway_input);
    IPv4Address subnet_mask = string_to_ip(mask_input);
    int packet_count = atoi(packets_input);
    
    if (packet_count <= 0) {
        printf("Ошибка: количество пакетов должно быть положительным числом\n");
        free(gateway_input);
        free(mask_input);
        free(packets_input);
        return;
    }
    
    printf("\nЗапуск симуляции для %d пакетов...\n", packet_count);
    
    // инициализация генератора случайных чисел
    srand((unsigned int)time(NULL));
    
    SimulationStats* stats = simulate_network_traffic(gateway, subnet_mask, packet_count);
    if (stats) {
        display_simulation_results(stats, gateway, subnet_mask);
        free_simulation_stats(stats);
    } else {
        printf("Ошибка при выполнении симуляции\n");
    }
    
    free(gateway_input);
    free(mask_input);
    free(packets_input);
}