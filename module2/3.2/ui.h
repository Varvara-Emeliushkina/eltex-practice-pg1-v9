#ifndef UI_H
#define UI_H

#include "network.h"

void display_simulation_results(SimulationStats* stats, IPv4Address gateway, IPv4Address subnet_mask);
void display_menu(void);

void process_simulation_input(void);
char* read_input(void);

#endif