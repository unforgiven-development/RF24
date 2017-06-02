/**
 * \file rpi-hub.cpp
 *
 * This program makes the Raspberry Pi act as a hub, listening to all six pipes from the remote sensor nodes (which are
 * usually Arduino-based devices) and will return the packet back to the sensor node on \b pipe0 so that the sender can
 * calculate the round trip delays when the payload matches.
 *
 * I encounter that at times, it also receive from \b pipe7 (or <B>pipe0</B>) with content of \c 0xFFFFFFFFF that I will
 * not sent back to the sender
 *
 * Refer to <B>./examples/rpi_hub_arduino/</B> for the corresponding Arduino sketches to work with this code.
 *
 *
 * CE is not used and CSN is GPIO25 (not pinout)
 *
 * Refer to the Raspberry Pi docs for GPIO numbering
 *
 *  Author : Stanley Seow
 *  e-mail : stanleyseow@gmail.com
 *  date   : 6th Mar 2013
 *
 * 03/17/2013 : Charles-Henri Hallard (http://hallard.me)
 *              Modified to use with Arduipi board http://hallard.me/arduipi
 *						  Changed to use modified bcm2835 and RF24 library
 *
 *
 */

#include <cstdlib>
#include <iostream>
#include <RF24/RF24.h>

using namespace std;

/* -----[ DEFINITIONS ]----- */
#define RF24_CHANNEL_NUMBER 76			/*!< Defines the channel number to use for this project */

/**
 * Defines the radio pipe addresses for the 2 nodes to communicate.
 * \note The first pipe is for writing; 2nd, 3rd, 4th, 5th & 6th are for reading...
 */
const uint64_t pipes[6] = {
	0xF0F0F0F0D2LL,
	0xF0F0F0F0E1LL,
	0xF0F0F0F0E2LL,
	0xF0F0F0F0E3LL,
	0xF0F0F0F0F1LL,
	0xF0F0F0F0F2LL
};

// CE Pin, CSN Pin, SPI Speed

// Setup for GPIO 22 CE and GPIO 25 CSN with SPI Speed @ 1Mhz
//RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_18, BCM2835_SPI_SPEED_1MHZ);

// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 4Mhz
//RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ);

// Setup for GPIO 22 CE and CE1 CSN with SPI Speed @ 8Mhz
RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);


int main(int argc, char** argv) {
	uint8_t len;

	/*!< \note Refer to RF24.h or nRF24L01 DS for settings */
	radio.begin();
	radio.enableDynamicPayloads();
	radio.setAutoAck(1);
	radio.setRetries(15, 15);
	radio.setDataRate(RF24_1MBPS);
	radio.setPALevel(RF24_PA_MAX);
	radio.setChannel(RF24_CHANNEL_NUMBER);
	radio.setCRCLength(RF24_CRC_16);

	/* Open 6 pipes for readings (5 plus pipe0, also can be used for reading) */
	radio.openWritingPipe(pipes[0]);
	radio.openReadingPipe(1, pipes[1]);
	radio.openReadingPipe(2, pipes[2]);
	radio.openReadingPipe(3, pipes[3]);
	radio.openReadingPipe(4, pipes[4]);
	radio.openReadingPipe(5, pipes[5]);

	/* ---( START LISTENING )--- */
	radio.startListening();

	/* Dump the configuration of the radio for debugging */
	radio.printDetails();

	printf("Output below : \n");
	delay(1);

	while (1) {
		char receivePayload[32];
		uint8_t pipe = 1;

		/* ---( START LISTENING )--- */
		radio.startListening();

		while (radio.available(&pipe)) {
			len = radio.getDynamicPayloadSize();
			radio.read(receivePayload, len);

			/* Display it on screen */
			printf("Recv: size=%i payload=%s pipe=%i", len, receivePayload, pipe);

			/* Send back payload to sender... */
			radio.stopListening();

			/* ...if pipe is 7, do not send it back... */
			if (pipe != 7) {
				radio.write(receivePayload, len);
				receivePayload[len] = 0;
				printf("\t Send: size=%i payload=%s pipe:%i\n", len, receivePayload, pipe);
			} else {
				printf("\n");
			}

			/* ...work on the next pipe... */
			pipe++;

			/* ...and reset pipe to 0 once we're through all 6 pipes */
			if (pipe > 6) {
				pipe = 0;
			}

		}

		delayMicroseconds(20);
	}

	return 0;
}

