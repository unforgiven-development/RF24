/*
	TMRh20 2014 - Optimized RF24 Library Fork
*/

/**
 * Example using Dynamic Payloads
 *
 * This is an example of how to use payloads of a varying (dynamic) size.
 *
 * \date		2014-2017
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/types.h>
#include <getopt.h>
#include <RF24/RF24.h>


using namespace std;


const char[] PROGNAME = "pingpair_dyn_int";

//
// Hardware configuration
// Configure the appropriate pins for your connections

/****************** Raspberry Pi ***********************/

RF24 radio(22,0); // CE GPIO, CSN SPI-BUS

int interruptPin = 23; // GPIO pin for interrupts

/**************************************************************/

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = {
	0xF0F0F0F0E1LL,
	0xF0F0F0F0D2LL
};



const int min_payload_size				= 4;
const int max_payload_size				= 32;
const int payload_size_increments_by	= 1;
int next_payload_size					= min_payload_size;

char receive_payload[max_payload_size + 1]; // +1 to allow room for a terminating NULL char
bool role_ping_out	= 1;
bool role_pong_back	= 0;
bool role			= 0;

static int print_usage(int error_code) {
	printf("\nUSAGE: %s [-v] [-V] [-h] [-F channel] [-s datarate] [-P PA level] \\\n", PROGNAME);
	printf("\t[-I IRQ pin] [-C CSn pin] [-E CE pin] [-D retry delay] [-R retry count] \\\n");
	printf("\t[-S SPI bus #] [-A autoACK status] [-c message CRC] [-f config file]");
	printf("  -v                - Enables verbose output.");
	printf("  -V                - Displays version info, and quits.");
	printf("  -h                - Displays this usage info, and quits.");
	printf("  -F channel        - ");
	printf("  -s datarate       - ");
	printf("  -P PA level       - ");
	printf("  -I IRQ pin        - ");
	printf("  -C CSn pin        - ");
	printf("  -E CE pin         - ");
	printf("  -D retry delay    - ");
	printf("  -R retry count    - ");
	printf("  -S SPI bus #      - ");
	printf("  -A autoACK status - ");
	printf("  -c message CRC    - ");
	printf("  -f config file    - (not yet implemented)");
	return error_code;
}

void intHandler() {
	//
	// Pong back role.  Receive each packet, dump it out, and send it back
	//

	if (role == role_pong_back) {
		// if there is data ready
		if (radio.available()) {
			// Dump the payloads until we've gotten everything
			uint8_t len = 0;

			while (radio.available()) {
				// Fetch the payload, and see if this was the last one.
				len = radio.getDynamicPayloadSize();
				radio.read(receive_payload, len);

				// Put a zero at the end for easy printing
				receive_payload[len] = 0;

				// Spew it
				printf("Got payload size=%i value=%s\n\r", len, receive_payload);
			}

			// First, stop listening so we can talk
			radio.stopListening();

			// Send the final one back.
			radio.write(receive_payload, len);
			printf("Sent response.\n\r");

			// Now, resume listening so we catch the next packets.
			radio.startListening();
		}
	}
}


int main(int argc, char** argv) {
	int rf24_channel_number = 45;
	while ((opt = getopt(argc, argv, "vV?hF:s:P:I:C:E:D:R:S:A:c:f:")) != EOF) {
		switch (opt) {
			case 'F':
				rf24_channel_number = atoi(optarg);
				if ((rf24_channel_number < 0) || (rf24_channel_number > 125)) {
					exit(-126);
				}
				break;
		}
	}

  // Print preamble:
  cout << "RF24/examples/pingpair_dyn/\n";

  // Setup and configure rf radio
  radio.begin();
  radio.enableDynamicPayloads();
  radio.setRetries(5,15);
  radio.printDetails();


/********* Role chooser ***********/

  printf("\n ************ Role Setup ***********\n");
  string input = "";
  char myChar = {0};
  cout << "Choose a role: Enter 0 for receiver, 1 for transmitter (CTRL+C to exit) \n>";
  getline(cin,input);

  if(input.length() == 1) {
	myChar = input[0];
	if(myChar == '0'){
		cout << "Role: Pong Back, awaiting transmission " << endl << endl;
	}else{  cout << "Role: Ping Out, starting transmission " << endl << endl;
		role = role_ping_out;
	}
  }
/***********************************/

    if ( role == role_ping_out )    {
      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1,pipes[1]);
    } else {
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);
      radio.startListening();
    }
    attachInterrupt(interruptPin, INT_EDGE_FALLING, intHandler); //Attach interrupt to bcm pin 23

// forever loop
	while (1)
	{

if (role == role_ping_out)
  {
    // The payload will always be the same, what will change is how much of it we send.
    static char send_payload[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ789012";

    // First, stop listening so we can talk.
    radio.stopListening();

    // Take the time, and send it.  This will block until complete
    printf("Now sending length %i...",next_payload_size);
    radio.write( send_payload, next_payload_size );

    // Now, continue listening
    radio.startListening();

    // Wait here until we get a response, or timeout
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 500 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n\r");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      uint8_t len = radio.getDynamicPayloadSize();
      radio.read( receive_payload, len );

      // Put a zero at the end for easy printing
      receive_payload[len] = 0;

      // Spew it
      printf("Got response size=%i value=%s\n\r",len,receive_payload);
    }

    // Update size for next time.
    next_payload_size += payload_size_increments_by;
    if ( next_payload_size > max_payload_size )
      next_payload_size = min_payload_size;

    // Try again 1s later
    delay(100);
  }


}
}


