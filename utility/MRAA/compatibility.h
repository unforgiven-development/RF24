/* 
 * RF24 - utility/MRAA/compatibility.h
 *
 * (c) 2016 Gerad Munsch <gmunsch@unforgivendevelopment.com>
 * (c) TMRh20 and any other contributors
 * (c) 2012 purinda
 *     ---- File created 24 June 2012, 3:08 PM by purinda
 *
 * Re-implementation of some timing functions to facilitate compatibility with
 * MRAA
 */

#ifndef COMPATIBLITY_H
#define	COMPATIBLITY_H

#ifdef	__cplusplus
extern "C" {
#endif
	
#include <stddef.h>
#include <time.h>
#include <sys/time.h>

void __msleep(int milisec);
void __usleep(int microsec);
void __start_timer();
long __millis();

#ifdef	__cplusplus
}
#endif

#endif	/* COMPATIBLITY_H */

