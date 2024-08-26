#ifndef LPSPI_CONFIG_H
#define LPSPI_CONFIG_H

#include "main.h"
#include <stdint.h>

void LPSPI1_Config(void);
void LPSPI1_Configure(void);
void LPSPI1_transmit(uint32_t data);
uint32_t LPSPI1_receive(void);

#endif /* LPSPI_CONFIG_H */
