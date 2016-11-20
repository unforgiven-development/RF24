/*
<<<<<<< HEAD
 * TMRh20 2015
 * ATTiny Configuration File
*/
=======
 * RF24 -- utility/ATTiny/RF24_arch_config.h
 *
 * (c) 2016 Gerad Munsch <gmunsch@unforgivendevelopment.com>
 * (c) 2015 TMRh20
 *
 * RF24 Configuration file for ATTiny devices, particularly the ATTiny 85/84
 */
>>>>>>> github/master

#ifndef __RF24_ARCH_CONFIG_H__
#define __RF24_ARCH_CONFIG_H__

<<<<<<< HEAD
  /********** USER DEFINES: **********/  
  //#define FAILURE_HANDLING
  //#define MINIMAL
  /***********************************/
=======
  /************************************/
  /********** USER DEFINES:  **********/
  /************************************/
  //#define FAILURE_HANDLING
  //#define MINIMAL
  /************************************/
>>>>>>> github/master

  #define rf24_max(a,b) (a>b?a:b)
  #define rf24_min(a,b) (a<b?a:b)

<<<<<<< HEAD

=======
>>>>>>> github/master
  #if ARDUINO < 100
  	#include <WProgram.h>
  #else
  	#include <Arduino.h>
  #endif

  #include <stddef.h>  
<<<<<<< HEAD
=======
  
  /*
   * Include the header file for SPI functions
   * (Main SPI code is contained in RF24.cpp for simplicity)
   */
  #include "spi.h"
>>>>>>> github/master

  // Include the header file for SPI functions (Main SPI code is contained in RF24.cpp for simplicity)
  #include "spi.h"
  #define _SPI SPI

  #ifdef SERIAL_DEBUG
    #define IF_SERIAL_DEBUG(x) ({x;})
  #else
	  #define IF_SERIAL_DEBUG(x)
	  #if defined(RF24_TINY)
	    #define printf_P(...)
    #endif
  #endif

  #include <avr/pgmspace.h>
  #define PRIPSTR "%S"
<<<<<<< HEAD

#endif // __RF24_ARCH_CONFIG_H__
=======
>>>>>>> github/master

#endif // __RF24_ARCH_CONFIG_H__