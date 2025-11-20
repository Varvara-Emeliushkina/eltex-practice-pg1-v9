#include "network.h"
#include <stdlib.h>
#include <stdio.h>

SimulationStats* simulate_network_traffic(IPv4Address gateway, IPv4Address subnet_mask, int packet_count) {
    SimulationStats* stats = malloc(sizeof(SimulationStats));
    if (!stats) return NULL;
    
    // инициализация статистики
    stats->total_packets = packet_count;
    stats->same_network_count = 0;
    stats->different_network_count = 0;
    stats->forced_same_network = 0;
    
    // симуляция пакетов
    for (int i = 0; i < packet_count; i++) {
        int force_same_network = 0;
        IPv4Address random_ip = generate_mixed_ip(gateway, subnet_mask, &force_same_network);
        
        if (is_same_network(gateway, random_ip, subnet_mask)) {
            stats->same_network_count++;
            if (force_same_network) {
                stats->forced_same_network++;
            }
        } else {
            stats->different_network_count++;
        }
    }
    
    // расчет процентов
    if (packet_count > 0) {
        stats->same_network_percent = (double)stats->same_network_count / packet_count * 100.0;
        stats->different_network_percent = (double)stats->different_network_count / packet_count * 100.0;
    } else {
        stats->same_network_percent = 0.0;
        stats->different_network_percent = 0.0;
    }
    
    return stats;
}

void free_simulation_stats(SimulationStats* stats) {
    free(stats);
}