/*
------------------------------------------------------------------------------
~ File   : internal_eeprom.h
~ Author : Majid Derhambakhsh
~ Version: V1.1.1
~ Created: 11/25/2019 02:36:00 AM
~ Brief  :
~ Support: 
           E-Mail : Majid.Derhambakhsh@gmail.com (subject : Embedded Library Support)
		   
           Github : https://github.com/Majid-Derhambakhsh
------------------------------------------------------------------------------
~ Description:    Set up and use the Internal AVR EEPROM.

~ Attention  :    This library is for CVAVR/GNUC Compilers.

~ Changes    :    
                  Improve : code structure.
------------------------------------------------------------------------------
*/

#ifndef _INTERNAL_EEPROM_H
#define _INTERNAL_EEPROM_H

/* ------------------------------------------------ Includes ------------------------------------------------- */

#include <stdint.h> /* Import standard integer lib */

#include "internal_eeprom_conf.h"

/* ------------------------------------------------------------------ */

#ifdef __CODEVISIONAVR__  /* Check compiler */

#pragma warn_unref_func- /* Disable 'unused function' warning */

#include <io.h>          /* Import io library */
#include <delay.h>       /* Import delay library */

/* ------------------------------------------------------------------ */

#elif defined(__GNUC__)  /* Check compiler */

#pragma GCC diagnostic ignored "-Wunused-function" /* Disable 'unused function' warning */

#include <avr/io.h>      /* Import io library */
#include <util/delay.h>  /* Import delay library */

/* ------------------------------------------------------------------ */

#elif defined(USE_HAL_DRIVER)  /* Check compiler */

/* ------------------------------------------------------------------ */

#else                     /* Compiler not found */

#error Chip or Compiler not supported  /* Send error */

#endif /* __CODEVISIONAVR__ */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------- Defines ------------------------------------------------- */

#define _WAIT_TIME 9

#ifndef _STAT_ERROR
	#define _STAT_ERROR 0
#endif

#ifndef _STAT_OK
	#define _STAT_OK 1
#endif

#ifndef _BYTE_SHIFT
	#define _BYTE_SHIFT 8
#endif

#ifndef _MAX_BYTE_SIZE
	#define _MAX_BYTE_SIZE 256
#endif

#ifndef _NULL
	#define _NULL 0xFF
#endif

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ By compiler ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifdef __CODEVISIONAVR__  /* Check compiler */

#ifndef _DELAY_MS
	#define _DELAY_MS(t)                     delay_ms((t)) /* Change function */
#endif /* _DELAY_MS */

/* ------------------------------------------------------------------ */

#elif defined(__GNUC__)  /* Check compiler */

#ifndef _DELAY_MS
	#define _DELAY_MS(t)                     _delay_ms((t)) /* Change function */
#endif /* _DELAY_MS */

/* ------------------------------------------------------------------ */

#else
#endif /* __CODEVISIONAVR__ */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* ------------------------------------------------ Prototype ------------------------------------------------ */

uint8_t EEPROM_SingleWrite(uint16_t _address, uint8_t _udata, uint16_t _time_out);

uint8_t EEPROM_Erase(uint16_t _address, uint16_t _quantity, uint16_t _time_out);

uint8_t EEPROM_BurstWrite(uint16_t _address, uint8_t *_udata, uint16_t _quantity, uint16_t _time_out);

uint8_t EEPROM_SingleRead(uint16_t _address, uint8_t *_udata, uint16_t _time_out);

uint8_t EEPROM_BurstRead(uint16_t _address, uint8_t *_udata, uint16_t quantity, uint16_t _time_out);

#endif 
