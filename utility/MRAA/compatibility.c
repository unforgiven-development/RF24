/*
 * RF24 - utility/MRAA/compatibility.c
 *
 * (c) 2016 Gerad Munsch <gmunsch@unforgivendevelopment.com>
 * (c) TMRh20 and any other contributors
 *
 * Re-implementation of some timing functions to facilitate compatibility with
 * MRAA
 */

#include "compatibility.h"


static struct timeval start, end;
//static long mtime, seconds, useconds;

/******************************************************************************/

/**
 * __msleep() -- This function is implemented in order to simulate the Arduino
 * "delay()" function
 * 
 * @param milisec
 */
void __msleep(int milisec)
{
	struct timespec req = {0};
	req.tv_sec = 0;
	req.tv_nsec = milisec * 1000000L;
	nanosleep(&req, (struct timespec *)NULL);	
	//usleep(milisec*1000);
}

/**
 * __usleep -- This function is implemented in order to simulate the Arduino
 * "delayMicroseconds()" function
 * 
 * @param microsec
 */
void __usleep(int microsec) {
	struct timespec req = {0};
	req.tv_sec = 0;
	req.tv_nsec = microsec * 1000L;
	nanosleep(&req, (struct timespec *)NULL);	
	//usleep(microsec);
}

/**
 * __start_timer() -- This function performs functions related to the project's
 * re-implementation of the Arduino "millis()" function
 */
void __start_timer()
{
	gettimeofday(&start, NULL);
}

/**
 * __millis() -- This function is essentially a re-implementation of the Arduino
 * "millis()" function
 */
long __millis()
{
    static long mtime, seconds, useconds;
	
	gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;	
	return mtime;
}
