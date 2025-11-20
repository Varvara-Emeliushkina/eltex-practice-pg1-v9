#include "ip.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

IPv4Address string_to_ip(const char* ip_str) {
    IPv4Address ip = {0};
    sscanf(ip_str, "%hhu.%hhu.%hhu.%hhu", 
           &ip.octet1, &ip.octet2, &ip.octet3, &ip.octet4);
    return ip;
}

char* ip_to_string(IPv4Address ip) {
    char* result = malloc(16);
    if (result) {
        snprintf(result, 16, "%d.%d.%d.%d", 
                 ip.octet1, ip.octet2, ip.octet3, ip.octet4);
    }
    return result;
}

uint32_t ip_to_uint32(IPv4Address ip) {
    return (ip.octet1 << 24) | (ip.octet2 << 16) | (ip.octet3 << 8) | ip.octet4;
}

IPv4Address uint32_to_ip(uint32_t ip_int) {
    IPv4Address ip;
    ip.octet1 = (ip_int >> 24) & 0xFF;
    ip.octet2 = (ip_int >> 16) & 0xFF;
    ip.octet3 = (ip_int >> 8) & 0xFF;
    ip.octet4 = ip_int & 0xFF;
    return ip;
}

IPv4Address calculate_network_address(IPv4Address ip, IPv4Address mask) {
    uint32_t ip_int = ip_to_uint32(ip);
    uint32_t mask_int = ip_to_uint32(mask);
    uint32_t network_int = ip_int & mask_int;
    return uint32_to_ip(network_int);
}

IPv4Address calculate_broadcast_address(IPv4Address ip, IPv4Address mask) {
    uint32_t ip_int = ip_to_uint32(ip);
    uint32_t mask_int = ip_to_uint32(mask);
    uint32_t broadcast_int = ip_int | (~mask_int);
    return uint32_to_ip(broadcast_int);
}

int is_same_network(IPv4Address ip1, IPv4Address ip2, IPv4Address mask) {
    IPv4Address net1 = calculate_network_address(ip1, mask);
    IPv4Address net2 = calculate_network_address(ip2, mask);
    
    uint32_t net1_int = ip_to_uint32(net1);
    uint32_t net2_int = ip_to_uint32(net2);
    
    return (net1_int == net2_int);
}

int is_valid_host_address(IPv4Address ip, IPv4Address network, IPv4Address broadcast) {
    uint32_t ip_int = ip_to_uint32(ip);
    uint32_t network_int = ip_to_uint32(network);
    uint32_t broadcast_int = ip_to_uint32(broadcast);
    
    // !адрес не должен быть сетевым адресом или широковещательным
    return (ip_int > network_int) && (ip_int < broadcast_int);
}


IPv4Address generate_mixed_ip(IPv4Address gateway, IPv4Address subnet_mask, int* force_same_network) {
        IPv4Address ip;
        
        // генерируем случайный IP
        ip.octet1 = rand() % 256;
        ip.octet2 = rand() % 256;
        ip.octet3 = rand() % 256;
        ip.octet4 = rand() % 256;
        
        
        return ip;
    
}