#ifndef NETWORK_H
#define NETWORK_H

#include "ip.h"

// структура для статистики
typedef struct {
    int total_packets;
    int same_network_count;
    int different_network_count;
    double same_network_percent;
    double different_network_percent;
    int forced_same_network;
} SimulationStats;

SimulationStats* simulate_network_traffic(IPv4Address gateway, IPv4Address subnet_mask, int packet_count);
void free_simulation_stats(SimulationStats* stats);

#endif