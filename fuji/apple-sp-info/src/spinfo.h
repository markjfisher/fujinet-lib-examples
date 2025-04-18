#ifndef TESTSP2_H
#define TESTSP2_H

#include <stdint.h>

void sp_info();

extern uint8_t sp_payload[];
extern uint8_t sp_dispatch_address[2];
extern uint16_t sp_count;
extern uint8_t sp_network;
extern uint8_t sp_is_init;

extern uint8_t read_memory(uint8_t offset, uint16_t address);
extern uint8_t sp_init(void);
extern uint8_t sp_get_network_id();

int8_t sp_status(uint8_t dest, uint8_t statcode);
uint8_t sp_init_c(void);

#endif