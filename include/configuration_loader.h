#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

void load_configuration_variables(const char *filename);

int get_address_family();
int get_socket_type();
uint32_t get_domain();
int get_protocol();
int get_port();
int get_backlog();

#endif