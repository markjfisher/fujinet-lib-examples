#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "fujinet-clock.h"
#include "main.h"

char *version = "v1.1.0";

char *alt_tz = "CET-1CEST,M3.5.0,M10.5.0/3";

uint8_t buffer[128];
char current_tz[128];
char original_tz[128];
uint8_t i = 0;

#ifdef __APPLE2__
extern uint8_t sp_network;
#endif

void debug() {
}

void clear_buffers() {
	memset(buffer, 0, 128);
	memset(current_tz, 0, 128);
	memset(original_tz, 0, 128);
}

int main(void)
{
	clrscr();
	clear_buffers();
	printf("FujiNet Clock %s\n\n", version);

	// store the system FN timezone
	printf("Setting FN tz to Western Australia\n");
	printf("use alt_tz: %s\n", alt_tz);

	// Change the system FN timezone to Western Australia for demostrating system timezone being different to UTC, or the alternate timezone
	clock_set_tz("UTC-8:45");

	// returns bytes
	prodos_time();
	simple_time();
	ape_time();

	// returns strings
	utc_time();
	iso_time();

	show_tz();

	printf("Resetting to original tz\n");
	clock_set_tz(original_tz);
	show_tz();

	cgetc();
	return 0;
}

void print_buffer(const char *msg, uint8_t sz) {
	uint8_t i;
	printf(msg);
	for (i = 0; i < sz; i++) {
		printf("%02x ", buffer[i]);
	}
	printf("\n");
}

void prodos_time() {
	clock_get_time(buffer, PRODOS_BINARY);
	print_buffer("prodos sys: ", 4);
	clock_get_time_tz(buffer, alt_tz, PRODOS_BINARY);
	print_buffer("prodos alt: ", 4);
}

void simple_time() {
	clock_get_time(buffer, SIMPLE_BINARY);
	print_buffer(" simple   : ", 7);
	clock_get_time_tz(buffer, alt_tz, SIMPLE_BINARY);
	print_buffer(" simple tz: ", 7);
}

void iso_time() {
	clock_get_time(buffer, TZ_ISO_STRING);
	printf("sys iso_tz: ");
	printf("%s\n", buffer);
	clock_get_time_tz(buffer, alt_tz, TZ_ISO_STRING);
	printf("alt iso_tz: ");
	printf("%s\n", buffer);
}

void utc_time() {
	clock_get_time(buffer, UTC_ISO_STRING);
	printf("   iso_utc: ");
	printf("%s\n", buffer);
}

void ape_time() {
	clock_get_time(buffer, APETIME_BINARY);
	print_buffer("sys ape   : ", 6);
	clock_get_time_tz(buffer, alt_tz, APETIME_BINARY);
	print_buffer("alt ape   : ", 6);
}

void show_tz() {
	memset(current_tz, 0, 128);

	// get the current system TZ
	clock_get_tz(current_tz);
	printf("Current tz: >%s<\n", current_tz);

}