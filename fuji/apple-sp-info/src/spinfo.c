#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "spinfo.h"

#define BASE_ADDRESS 0xC000

char *version = "v1.1.0";

void debug() {}

int main(void) {
    clrscr();
    printf("spinfo %s\n", version);
    sp_info();
    printf("\n");

    printf("press a key to run sp_init_c");
    cgetc();
    sp_init_c();
    printf("\n");

    printf("press a key to run sp_init");
    // reset init flag
    sp_is_init = 0;
    cgetc();
    sp_init();

    return 0;
}

void sp_print_devices() {
  int8_t status_r = 0;
	uint8_t device_count = 0;
  int i = 0;
  uint8_t char_count = 0;

  status_r = sp_status(0, 0);
  printf("status: %d\n", status_r);
  if (status_r != 0) return;

  device_count = sp_payload[0];
  printf("device count: %d\n", device_count);
  if (device_count == 0) return;

  // DIB request on each device to get the name back
  for (i = 1; i <= device_count; i++) {
    status_r = sp_status(i, 3);
    printf("%d/%d/%u/", i, status_r, sp_count);
    if (status_r == 0 && sp_count > 0) {
      char_count = sp_payload[4];
      printf(">");
      fwrite(&sp_payload[5], sizeof(char), char_count, stdout);
      printf("<\n");
    } else {
      printf("\n");
    }
  }
  printf("press a key to continue.\n");
  cgetc();
}

void sp_info() {
  // for each SP card found, get its dispatch address, then call sp_print_devices for each
  uint8_t i = 0;
  uint8_t *p;
  unsigned int dispatch_address;

  for (i = 1; i < 8; i++) {
    p = (uint8_t *)(BASE_ADDRESS + (i * 0x100));
    // printf("Checking 0x%04x\n", (unsigned int) p);
    if (p[1] == 0x20 && p[3] == 0x00 && p[5] == 0x03 && p[7] == 0x00) {
      dispatch_address = (unsigned int) &p[p[0xff]] + 3;
      debug();
      sp_dispatch_address[0] = dispatch_address & 0xff;
      sp_dispatch_address[1] = (dispatch_address >> 8) & 0xff;
      printf("SP card at %d, dispatch: 0x%02x%02x\n", i, sp_dispatch_address[1], sp_dispatch_address[0]);
      sp_print_devices();
    }
  }
}

uint8_t sp_init_c(void) {
    const uint8_t sp_markers[] = {0x20, 0x00, 0x03, 0x00};
    uint16_t base;
    uint8_t i;
    bool match;
    uint8_t offset;
    uint16_t dispatch_address;

    // reset network id and is_init flag, we are going to rescan.
    sp_network = 0;
    sp_is_init = 0;

    for (base = 0xC701; base >= 0xC101; base -= 0x0100) {
        match = true;
        for (i = 0; i < 4; i++) {
            if (read_memory(i * 2, base) != sp_markers[i]) {
                match = false;
                break;
            }
        }

        if (match) {
            // If a match is found, calculate the dispatch function address
            offset = read_memory(0xFE, base);
            dispatch_address = base + offset + 2;
            sp_dispatch_address[0] = dispatch_address & 0xFF;
            sp_dispatch_address[1] = dispatch_address >> 8;

            // now find and return the network id. it's stored in sp_network after calling sp_get_network_id.
            // we need to set sp_is_init to 1 to stop sp_get_network_id from calling init again and recursing.
            sp_is_init = 1;
            sp_get_network_id();
            if (sp_network != 0) {
                return sp_network;
            }
            // it failed to find a network on this SP device, so reset sp_is_init and reloop/exit
            sp_is_init = 0;
        }
    }

    // no match is found, return 0 for network not found.
    return 0;
}