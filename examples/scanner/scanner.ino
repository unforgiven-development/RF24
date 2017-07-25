/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Channel scanner
 *
 * Example to detect interference on the various channels available.
 * This is a good diagnostic tool to check whether you're picking a
 * good channel for your application.
 *
 * Inspired by cpixip.
 * See http://arduino.cc/forum/index.php/topic,54795.0.html
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

/* ---( HARDWARE CONFIGURATION )--- */

/* Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);


/* ---( CONSTANTS & PROJECT SETTINGS )--- */

const uint8_t num_channels = 126;		/*!< Defines the amount of channels available for use on nRF24L01(+) hardware */
uint8_t values[num_channels];			/*!< An array to hold the reading values, one for each channel */
const int num_reps = 100;				/*!< The amount of times to scan each channel */


void setup(void) {
	int h = 0;

	/* Print our "preamble" */
	Serial.begin(115200);
	printf_begin();
	Serial.println(F("\n\rRF24/examples/scanner/"));

	/* Start radio operation, and set runtime configuration options */
	radio.begin();
	radio.setAutoAck(false);

	// Get into standby mode
	radio.startListening();
	radio.stopListening();

	radio.printDetails();

	/* Print out header, high then low digit */
	h = 0;
	while (h < num_channels) {
		printf("%x", h >> 4);
		++h;
	}
	Serial.println();

	h = 0;
	while (h < num_channels) {
		printf("%x", h & 0x0F);
		++h;
	}
	Serial.println();
}


void loop(void) {
	// Clear measurement values
	memset(values, 0, sizeof(values));

	// Scan each channels "num_reps" times
	int rep_counter = num_reps;
	while (rep_counter--) {
		int n = num_channels;
		while (n--) {
			// Select this channel
			radio.setChannel(n);

			// Listen for a little
			radio.startListening();
			delayMicroseconds(128);
			radio.stopListening();

			// Did we get a carrier?
			if (radio.testCarrier()) {
				++values[n];
			}
		}
	}

	// Print out channel measurements, clamped to a single hex digit
	int i = 0;
	while (i < num_channels) {
		printf("%x", min(0x0F, values[i]));
		++i;
	}
	Serial.println();
}
