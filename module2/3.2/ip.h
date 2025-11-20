#ifndef IP_H
#define IP_H

#include <stdint.h>

// структура для IPv4 адреса
typedef struct {
    uint8_t octet1;
    uint8_t octet2;
    uint8_t octet3;
    uint8_t octet4;
} IPv4Address;

// для сети
typedef struct {
    IPv4Address gateway;
    IPv4Address subnet_mask;
    IPv4Address network_address;
    IPv4Address broadcast_address;
} NetworkInfo;

// функции преобразования
IPv4Address string_to_ip(const char* ip_str);
char* ip_to_string(IPv4Address ip);
uint32_t ip_to_uint32(IPv4Address ip);
IPv4Address uint32_to_ip(uint32_t ip_int);

// функции работы с сетями
IPv4Address calculate_network_address(IPv4Address ip, IPv4Address mask);
IPv4Address calculate_broadcast_address(IPv4Address ip, IPv4Address mask);
int is_same_network(IPv4Address ip1, IPv4Address ip2, IPv4Address mask);
int is_valid_host_address(IPv4Address ip, IPv4Address network, IPv4Address broadcast);

// функции для генерации случайных адресов
IPv4Address generate_random_ip(void);
IPv4Address generate_network_ip(IPv4Address network, IPv4Address broadcast);
IPv4Address generate_mixed_ip(IPv4Address gateway, IPv4Address subnet_mask, int* force_same_network);

#endif