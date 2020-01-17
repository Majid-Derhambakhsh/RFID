/*
------------------------------------------------------------------------------
~ File   : internal_eeprom.c
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

/* -------------------- Library include -------------------- */

#include "internal_eeprom.h"

/* ----------------------- Functions ----------------------- */

uint8_t EEPROM_SingleWrite(uint16_t _address, uint8_t _udata, uint16_t _time_out) /* Function For Write To EEPROM */
{
	
	uint8_t operation_status = _STAT_ERROR;
	
	/* ----------------------------- */
	
	#if (_EEPROM_SIZE > _MAX_BYTE_SIZE)
	
	EEARH = (uint8_t)(_address >> _BYTE_SHIFT); /* Set up address register */
	EEARL = (uint8_t)_address; /* Set up address register */
	
	#else
	
	EEAR = (uint8_t)_address; /* Set up address register */
	
	#endif
	
	while( (EECR & (1 << EEWE)) && (_time_out > 0) ) /* Wait for completion of previous write */
	{
		_time_out--;
	}
	
	if (_time_out > 0)
	{
		
		#if (_EEPROM_SIZE > _MAX_BYTE_SIZE)
		
		EEARH = (uint8_t)(_address >> _BYTE_SHIFT); /* Set up address register */
		EEARL = (uint8_t)_address; /* Set up address register */
		
		#else
		
		EEAR = (uint8_t)_address; /* Set up address register */
		
		#endif
		
		EEDR = _udata; /* Set up data registers */
		EECR |= (1 << EEMWE); /* Write logical one to EEMWE */
		EECR |= (1 << EEWE); /* Start eeprom write by setting EEWE */
		
		operation_status = _STAT_OK;
		
		_DELAY_MS(_WAIT_TIME);
		
	}
	
	return operation_status;
	
}

/*-----------------------------------------------*/

uint8_t EEPROM_Erase(uint16_t _address, uint16_t _quantity, uint16_t _time_out) /* Function For Erase EEPROM */
{
	
	uint8_t operation_status = _STAT_ERROR;
	
	/* ----------------------------- */
	
	_quantity += _address;
	
	for(; _address < _quantity; _address++) /* Loop For Counts And Copy */
	{
		operation_status = EEPROM_SingleWrite(_address, _NULL, _time_out); /* Write Erase Byte To EEPROM */
	}
	
	return operation_status;
}

/*-----------------------------------------------*/

uint8_t EEPROM_BurstWrite(uint16_t _address, uint8_t *_udata, uint16_t _quantity, uint16_t _time_out) /* Function For Write String To EEPROM */
{
	
	uint8_t operation_status = _STAT_ERROR;
	
	/* ----------------------------- */
	
	_quantity += _address; /* Set Value For Counter */
	
	for(; _address < _quantity; _address++) /* Loop For Counts And Copy */
	{
		
		operation_status = EEPROM_SingleWrite(_address, *_udata, _time_out); /* Write String To EEPROM */
		_udata++; /* Select Next Byte */
		
	}
	
	return operation_status;
	
}

/* --------------------------------------------- */

uint8_t EEPROM_SingleRead(uint16_t _address, uint8_t *_udata, uint16_t _time_out) /* Function For Read EEPROM Data */
{
	
	uint8_t operation_status = _STAT_ERROR;
	
	/* ----------------------------- */
	
	#if (_EEPROM_SIZE > _MAX_BYTE_SIZE)
	
	EEARH = (uint8_t)(_address >> _BYTE_SHIFT); /* Set up address register */
	EEARL = (uint8_t)_address; /* Set up address register */
	
	#else
	
	EEAR = (uint8_t)_address; /* Set up address register */
	
	#endif
	
	while( (EECR & (1 << EEWE)) && (_time_out > 0) ) /* Wait for completion of previous write */
	{
		_time_out--;
	}
	
	if (_time_out > 0)
	{
		
		#if (_EEPROM_SIZE > _MAX_BYTE_SIZE)
		
		EEARH = (uint8_t)(_address >> _BYTE_SHIFT); /* Set up address register */
		EEARL = (uint8_t)_address; /* Set up address register */
		
		#else
		
		EEAR = (uint8_t)_address; /* Set up address register */
		
		#endif
		
		EECR |= (1 << EERE); /* Start eeprom read by writing EERE */
		
		*_udata = EEDR; /* Copy data from data register */
		
		operation_status = _STAT_OK;
		
		//_DELAY_MS(_WAIT_TIME);
		
	}
	
	return operation_status;
	
}

/* --------------------------------------------- */

uint8_t EEPROM_BurstRead(uint16_t _address, uint8_t *_udata, uint16_t quantity, uint16_t _time_out)
{
	
	uint8_t operation_status = _STAT_ERROR;
	
	/* ----------------------------- */
	
	quantity += _address; /* Set Value For Counter */
	
	for(; _address < quantity; _address++) /* Loop For Counts And Copy */
	{
		
		operation_status = EEPROM_SingleRead(_address, _udata, _time_out); /* Write String To EEPROM */
		_udata++; /* Select Next Byte */
		
	}
	
	return operation_status;
	
}

/* End Program */