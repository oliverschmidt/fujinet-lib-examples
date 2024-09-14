#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef __APPLE2__
#include <apple2.h>
#endif

#include "fujinet-network.h"

#include "mastodon.h"

uint8_t buffer[1024];
char url[] = "n1:https://oldbytes.space/api/v1/timelines/public?limit=1";
char display_name_query[] = "/0/account/display_name";
char created_at_query[] = "/0/created_at";
char content_query[] = "/0/content";
char version[] = "v1.1.0";

void main(void)
{
	uint8_t err;
	int16_t count;

	setup();
	clrscr();
	printf("mastodon %s\n", version);

	while (1) {
		printf("\n\n");

		network_open(url, OPEN_MODE_READ, OPEN_TRANS_NONE);
		err = network_json_parse(url);
		if (err != FN_ERR_OK) {
			  handle_err(err, "parse");
		}

		line('=');

		count = network_json_query(url, display_name_query, (char *) buffer);
		if (count < 0) {
			handle_err(-count, "query");
		}
		printf("%40s\n", buffer);

		count = network_json_query(url, created_at_query, (char *) buffer);
		if (count < 0) {
			handle_err(-count, "query");
		}
		printf("%40s\n", buffer);

		line('-');

		count = network_json_query(url, content_query, (char *) buffer);
		if (count < 0) {
			handle_err(-count, "query");
		}
		printf("%s\n", buffer);

		line('-');

		network_close(url);

		pause(32000);
	}
}

void setup(void)
{
	uint8_t init_r;

#ifdef __APPLE2__
#ifdef __APPLE2ENH__
	videomode(VIDEOMODE_80COL);
#else
	if (get_ostype() >= APPLE_IIIEM) {
		allow_lowercase(true);
	}
#endif
#endif

	init_r = network_init();
	if (init_r != FN_ERR_OK) {
		handle_err(init_r, "network_init failed");
	}
}

void handle_err(uint8_t err, char *reason)
{
	printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
	cgetc();
	exit(1);
}

void line(char type)
{
	unsigned char x, y;

	screensize(&x, &y);
	while (x--) {
		putchar(type);
	}
}

void pause(unsigned long time)
{
	unsigned long i;

	for (i = 0; i < time; i++)
		;
}
