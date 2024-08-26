#ifndef MAX7219_CONFIG_H
#define MAX7219_CONFIG_H

#include "main.h"
#include <stdint.h>

unsigned int SPI_send_char(char char_sign, unsigned int char_position);
void SPI_send_date_time(char* data);

#endif /* MAX7219_CONFIG_H */