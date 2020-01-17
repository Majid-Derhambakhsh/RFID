/*
------------------------------------------------------------------------------
~ File   : rfid_conf.h
~ Author : Majid Derhambakhsh
~ Version: V0.0.0
~ Created: 12/11/2019 03:15:00 AM
~ Brief  :
~ Support:
           E-Mail : Majid.Derhambakhsh@gmail.com (subject : Embedded Library Support)
           
           Github : https://github.com/Majid-Derhambakhsh
------------------------------------------------------------------------------
~ Description:    RFID library for manage Card IDs in AVR/STM32 ARM microcontroller.

~ Attention  :    

~ Changes    :    
------------------------------------------------------------------------------
*/

#ifndef __RFID_CONF_H_
#define __RFID_CONF_H_

/* ~~~~~~~~~~~~~~~~~~ Options ~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~ Verify control ~~~~~~ */

#define _RFID_VERIFY_IN_SAVE
#define _RFID_VERIFY_IN_REMOVE

/* ~~~~~~~ Card setting ~~~~~~~ */

#define _RFID_CARD_ID_LENGTH 10

/* ~~~~~~ Memory setting ~~~~~~ */

//#define _RFID_USING_INTERNAL_MEMORY
#define _RFID_USING_EXTERNAL_MEMORY

#define _RFID_MEMORY_TIME_OUT 100

/* ~~~~ RFID module setting ~~~ */

#define _KartachKA11
//#define _MangoRF01D

/* ~~~~~~~~~~~~~~~ Include files ~~~~~~~~~~~~~~~ */

#include "I2C_MEMORY/i2c_memory.h"
#include "Internal EEPROM/internal_eeprom.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* __RFID_CONF_H_ */
