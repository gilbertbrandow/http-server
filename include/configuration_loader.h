#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

void load_configuration_variables(const char *filename);

int get_address_family();
int get_protocol();

#endif