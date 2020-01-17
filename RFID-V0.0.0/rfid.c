/*
------------------------------------------------------------------------------
~ File   : rfid.c
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

#include "rfid.h"

/* ............. Global variables ............ */

uint8_t detected_card_id[_RFID_CARD_ID_LENGTH];

uint32_t g_mem_add = 0;

RFID_ConfigTypeDef  RFID_Config;
RFID_StatusTypeDef  RFID_Status;
RFID_ControlTypeDef RFID_Control;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~ */

struct /* Struct for check card informations */
{
	
	uint8_t CardStatus   : 1;
	uint8_t NumberStatus : 1;
	
}Card_DataCheck;

/* ................ Functions ................ */

void RFID_ConfigData_Load(void) /* This function is for load data from memory */
{
	uint8_t high_byte_data = 0;
	uint8_t low_byte_data  = 0;
	
	/* ~~~~~~ Main card ~~~~~~ */
	RFID_Mem_SingleRead(RFID_Config.StartOfDataLocationAddress, &high_byte_data, _RFID_MEMORY_TIME_OUT);
	RFID_Mem_SingleRead( (RFID_Config.StartOfDataLocationAddress + 1) , &low_byte_data, _RFID_MEMORY_TIME_OUT);
	
	if ( (high_byte_data == _RFID_MEM_NULL) && (low_byte_data == _RFID_MEM_NULL) )
	{
		RFID_Mem_SingleWrite(RFID_Config.StartOfDataLocationAddress , (uint8_t)(RFID_Control.RegisteredMainCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
		RFID_Mem_SingleWrite((RFID_Config.StartOfDataLocationAddress + 1) , (uint8_t)RFID_Control.RegisteredMainCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
	}
	else
	{
		RFID_Control.RegisteredMainCard = ((uint16_t)high_byte_data << _BYTE_SHIFT) | (uint16_t)low_byte_data;
	}
	
	/* ~~~~~~ Master card ~~~~~~ */
	RFID_Mem_SingleRead(_REGISTERED_MASTER_CARD_ADD, &high_byte_data, _RFID_MEMORY_TIME_OUT);
	RFID_Mem_SingleRead( (_REGISTERED_MASTER_CARD_ADD + 1) , &low_byte_data, _RFID_MEMORY_TIME_OUT);
	
	if ( (high_byte_data == _RFID_MEM_NULL) && (low_byte_data == _RFID_MEM_NULL) )
	{
		RFID_Mem_SingleWrite(_REGISTERED_MASTER_CARD_ADD , (uint8_t)(RFID_Control.RegisteredMasterCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
		RFID_Mem_SingleWrite( (_REGISTERED_MASTER_CARD_ADD + 1) , (uint8_t)RFID_Control.RegisteredMasterCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
	}
	else
	{
		RFID_Control.RegisteredMasterCard = ((uint16_t)high_byte_data << _BYTE_SHIFT) | (uint16_t)low_byte_data;
	}
	
	/* ~~~~~~ User card ~~~~~~ */
	RFID_Mem_SingleRead(_REGISTERED_USER_CARD_ADD, &high_byte_data, _RFID_MEMORY_TIME_OUT);
	RFID_Mem_SingleRead( (_REGISTERED_USER_CARD_ADD + 1) , &low_byte_data, _RFID_MEMORY_TIME_OUT);
	
	if ( (high_byte_data == _RFID_MEM_NULL) && (low_byte_data == _RFID_MEM_NULL) )
	{
		RFID_Mem_SingleWrite(_REGISTERED_USER_CARD_ADD , (uint8_t)(RFID_Control.RegisteredUserCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
		RFID_Mem_SingleWrite( (_REGISTERED_USER_CARD_ADD + 1) , (uint8_t)RFID_Control.RegisteredUserCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
	}
	else
	{
		RFID_Control.RegisteredUserCard = ((uint16_t)high_byte_data << _BYTE_SHIFT) | (uint16_t)low_byte_data;
	}
	
}
/*
	Guide   :
			Function description	This function is used to reload data from memory.
			
			Parameters
									-
			Return Values
									-
			
	Example :
			
			RFID_ConfigData_Load();
			
*/

void RFID_ResetFlags(void)
{
	
	/* ~~~~~ Reset all flags ~~~~~ */
	RFID_Status.CardDetected      = _FALSE;
	RFID_Status.CardNotRegistered = _FALSE;
	RFID_Status.CardRemoved       = _FALSE;
	RFID_Status.CardSaved         = _FALSE;
	
}

void RFID_Init(RFID_ConfigTypeDef *_settings)
{
	/* ~~~~~ Setting initialize ~~~~~ */
	RFID_Config.StartOfDataLocationAddress = _settings->StartOfDataLocationAddress;
	
	RFID_Config.NumberOfMainCard   = _settings->NumberOfMainCard;
	RFID_Config.NumberOfMasterCard = _settings->NumberOfMasterCard;
	RFID_Config.NumberOfUserCard   = _settings->NumberOfUserCard;
	
	/* ~~~~~ Load data ~~~~~ */
	RFID_ConfigData_Load();
	
}

void RFID_DefaultInit(void)
{
	/* ~~~~~ Setting initialize ~~~~~ */
	RFID_Config.StartOfDataLocationAddress = 0;
	
	RFID_Config.NumberOfMainCard   = 5;
	RFID_Config.NumberOfMasterCard = 5;
	RFID_Config.NumberOfUserCard   = 5;
	
	/* ~~~~~ Load data ~~~~~ */
	RFID_ConfigData_Load();
	
}

void RFID_FullInit(RFID_ConfigTypeDef *_settings)
{
	/* ~~~~~ Setting initialize ~~~~~ */
	RFID_Config.StartOfDataLocationAddress = _settings->StartOfDataLocationAddress;
	
	RFID_Config.NumberOfMainCard   = _settings->NumberOfMainCard;
	RFID_Config.NumberOfMasterCard = _settings->NumberOfMasterCard;
	RFID_Config.NumberOfUserCard   = _settings->NumberOfUserCard;
	
	#ifdef _USE_AVR_MCU /* AVR compilers */
	
	/* ~~~~~ I2C/Memory initialize ~~~~~ */
	I2C_Init();
	I2C_Memory_Init();
	
	/* ~~~~~ Load data ~~~~~ */
	RFID_ConfigData_Load();
	
	#endif
	
}

/* ,,,,,,,,,,,,, Data Control ,,,,,,,,,,,,, */

void RFID_DecimalBasic(uint8_t _usart_data)
{
	
	/* ~~ Flag reset ~~ */
	
	RFID_ResetFlags();
	
	/* ~~~~~~~~~~~~~~~~ */
	
	if (RFID_Control.CopyToSRAM == _RFID_CARD_ID_LENGTH)
	{
		
		RFID_Status.CardDetected = _TRUE; /* Set flag */
		RFID_Control.CopyToSRAM = 0; /* Reset value */
		
	}
	else if ( (_usart_data >= _ASCII_ZERO) && (_usart_data <= _ASCII_NINE) && (RFID_Control.CopyToSRAM < _RFID_CARD_ID_LENGTH) )
	{
		
		detected_card_id[RFID_Control.CopyToSRAM] = _usart_data; /* Copy the card number in the array */
		RFID_Control.CopyToSRAM++; /* Select next byte */
		
	}
	else{}
	
}

void Mango_RF01D_DecimalBasic(uint8_t _usart_data)
{
	
	/* ~~ Flag reset ~~ */
	
	RFID_ResetFlags();
	
	/* ~~~~~~~~~~~~~~~~ */
	
	if ( ((_usart_data == _CONTROL3_CODE) || (_usart_data == _ESCAPE_CODE)) && (RFID_Control.CopyToSRAM == _RFID_CARD_ID_LENGTH) )
	{
		
		RFID_Status.CardDetected = _TRUE; /* Set flag */
		RFID_Control.CopyToSRAM = 0; /* Reset value */
		
	}
	else if ( (_usart_data >= _ASCII_ZERO) && (_usart_data <= _ASCII_NINE) && (RFID_Control.CopyToSRAM < _RFID_CARD_ID_LENGTH) )
	{
		
		detected_card_id[RFID_Control.CopyToSRAM] = _usart_data; /* Copy the card number in the array */
		RFID_Control.CopyToSRAM++; /* Select next byte */
		
	}
	else{}
	
}

void Kartach_KA11_DecimalBasic(uint8_t _usart_data)
{
	/* ~~ Flag reset ~~ */
	
	RFID_ResetFlags();
	
	/* ~~~~~~~~~~~~~~~~ */
	
	if ( (_usart_data == _RFID_END_CODE) && (RFID_Control.CopyToSRAM == _RFID_CARD_ID_LENGTH) )
	{
		
		RFID_Status.CardDetected = _TRUE; /* Set flag */
		RFID_Control.CopyToSRAM = 0; /* Reset value */
		
	}
	else if ( (_usart_data >= _ASCII_ZERO) && (_usart_data <= _ASCII_NINE) && (RFID_Control.CopyToSRAM < _RFID_CARD_ID_LENGTH) )
	{
		
		detected_card_id[RFID_Control.CopyToSRAM] = _usart_data; /* Copy the card number in the array */
		RFID_Control.CopyToSRAM++; /* Select next byte */
		
	}
	else{}
	
}

/* ,,,,,,,,,, Card ID management ,,,,,,,,,, */

RFID_MessageTypeDef RFID_GetCardID(uint8_t *_card_id)
{
	
	uint8_t nmb_of_id_counter = 0; /* Numbers of ids counter */
	
	for (; nmb_of_id_counter < _RFID_CARD_ID_LENGTH; nmb_of_id_counter++)
	{
		
		*_card_id = detected_card_id[nmb_of_id_counter];
		_card_id++;
		
	}
	
	return _RFID_STAT_OK;
}
/*
	Guide   :
			Function description	This function is used to get detected card ID.
			
			Parameters
									* _card_id : Pointer to card id string.
			Return Values
									* _STAT_OK
									* _STAT_ERROR
			
	Example :
			uint8_t             user_card[10];
			RFID_MessageTypeDef com_status;
			
			com_status = RFID_GetCardID(user_card);
			
*/

RFID_MessageTypeDef RFID_VerifyCard(RFID_CardType_TypeDef *_card_type, uint16_t *_card_number)
{
	/* ---------- Local variable ---------- */
	
	uint8_t mem_data = 0; /* Memory data */
	uint8_t nmb_of_id_counter = 0; /* Numbers of ids counter */
	
	RFID_MessageTypeDef status_check = _RFID_CARD_NOT_REGISTERED;
	
	/* ----------- Load variable ---------- */
	*_card_type   = _NOT_FOUND_CARD;
	*_card_number = 0;
	
	/* ------------ Reset flag ------------ */
	
	RFID_ResetFlags();
	
	/* ----------- Check memory ----------- */
	#ifdef _RFID_USING_EXTERNAL_MEMORY
	if (I2C_MemoryIsReady(_RFID_MEMORY_TIME_OUT) == _RFID_STAT_OK) /* Connection check */
	{
	#endif /* _RFID_USING_EXTERNAL_MEMORY */
	/* ------------------------------------ */
		
		/* ~~~~~ Reset data ~~~~~ */
		
		g_mem_add = 0;
		Card_DataCheck.CardStatus   = _CARD_IS_CONFLICT;
		Card_DataCheck.NumberStatus = _NUMBER_IS_CORRECT;
		
		/* -------------- Reload -------------- */
		
		RFID_ConfigData_Load();
		
		/* ------------ Check cards ----------- */
		
		/* ~~~~~~~~~~~ Main card ~~~~~~~~~~~ */
		for (; ((RFID_Control.RegisteredMainCard > 0) && (Card_DataCheck.CardStatus == _CARD_IS_CONFLICT)); RFID_Control.RegisteredMainCard--) /* Card counter */
		{
			
			for (nmb_of_id_counter = 0; (nmb_of_id_counter < _RFID_CARD_ID_LENGTH) && (Card_DataCheck.NumberStatus == _NUMBER_IS_CORRECT); nmb_of_id_counter++) /* Numbers of card counter */
			{
				
				g_mem_add = (((uint32_t)RFID_Control.RegisteredMainCard - 1) * _RFID_CARD_ID_LENGTH) + (uint32_t)nmb_of_id_counter + _MAIN_CARDID_START_ADD; /* Memory address loading */
				
				RFID_Mem_SingleRead(g_mem_add, &mem_data, _RFID_MEMORY_TIME_OUT);
				
				if( detected_card_id[nmb_of_id_counter] == mem_data ) /* Check & compare number value */
				{
					
					if( nmb_of_id_counter == _CARD_LAST_NMB ) /* Check index of number */
					{
						
						Card_DataCheck.CardStatus = _CARD_IS_IDENTIFIED;
						
						*_card_type   = _MAIN_CARD;
						*_card_number = RFID_Control.RegisteredMainCard;
						
						status_check = _RFID_CARD_REGISTERED;
						
					}
					
				}
				else
				{
					Card_DataCheck.NumberStatus = _NUMBER_IS_CONFLICT;
				}
				
			}
			
			Card_DataCheck.NumberStatus = _NUMBER_IS_CORRECT;
			
		}
		
		/* ~~~~~~~~~~ Master card ~~~~~~~~~~ */
		if (Card_DataCheck.CardStatus == _CARD_IS_CONFLICT)
		{
			
			for (; ((RFID_Control.RegisteredMasterCard > 0) && (Card_DataCheck.CardStatus == _CARD_IS_CONFLICT)); RFID_Control.RegisteredMasterCard--) /* Card counter */
			{
				
				for (nmb_of_id_counter = 0; (nmb_of_id_counter < _RFID_CARD_ID_LENGTH) && (Card_DataCheck.NumberStatus == _NUMBER_IS_CORRECT); nmb_of_id_counter++) /* Numbers of card counter */
				{
					
					g_mem_add = (((uint32_t)RFID_Control.RegisteredMasterCard - 1) * _RFID_CARD_ID_LENGTH) + (uint32_t)nmb_of_id_counter + _MASTER_CARDID_START_ADD; /* Memory address loading */
					
					RFID_Mem_SingleRead(g_mem_add, &mem_data, _RFID_MEMORY_TIME_OUT);
					
					if( detected_card_id[nmb_of_id_counter] == mem_data ) /* Check & compare number value */
					{
						
						if( nmb_of_id_counter == _CARD_LAST_NMB ) /* Check index of number */
						{
							
							Card_DataCheck.CardStatus = _CARD_IS_IDENTIFIED;
							
							*_card_type   = _MASTER_CARD;
							*_card_number = RFID_Control.RegisteredMasterCard;
							
							status_check = _RFID_CARD_REGISTERED;
							
						}
						
					}
					else
					{
						Card_DataCheck.NumberStatus = _NUMBER_IS_CONFLICT;
					}
					
				}
				
				Card_DataCheck.NumberStatus = _NUMBER_IS_CORRECT;
				
			}
			
			/* ~~~~~~~~~~~ User card ~~~~~~~~~~~ */
			if (Card_DataCheck.CardStatus == _CARD_IS_CONFLICT)
			{
				
				for (; ((RFID_Control.RegisteredUserCard > 0) && (Card_DataCheck.CardStatus == _CARD_IS_CONFLICT)); RFID_Control.RegisteredUserCard--) /* Card counter */
				{
					
					for (nmb_of_id_counter = 0; (nmb_of_id_counter < _RFID_CARD_ID_LENGTH) && (Card_DataCheck.NumberStatus == _NUMBER_IS_CORRECT); nmb_of_id_counter++) /* Numbers of card counter */
					{
						
						g_mem_add = (((uint32_t)RFID_Control.RegisteredUserCard - 1) * _RFID_CARD_ID_LENGTH) + (uint32_t)nmb_of_id_counter + _USER_CARDID_START_ADD; /* Memory address loading */
						
						RFID_Mem_SingleRead(g_mem_add, &mem_data, _RFID_MEMORY_TIME_OUT);
						
						if( detected_card_id[nmb_of_id_counter] == mem_data ) /* Check & compare number value */
						{
							
							if( nmb_of_id_counter == _CARD_LAST_NMB ) /* Check index of number */
							{
								
								Card_DataCheck.CardStatus = _CARD_IS_IDENTIFIED;
								
								*_card_type   = _USER_CARD;
								*_card_number = RFID_Control.RegisteredUserCard;
								
								status_check = _RFID_CARD_REGISTERED;
								
							}
							
						}
						else
						{
							Card_DataCheck.NumberStatus = _NUMBER_IS_CONFLICT;
						}
						
					}
					
					Card_DataCheck.NumberStatus = _NUMBER_IS_CORRECT;
					
				}
				/* Check in master */
				
			}
			
		}
		
		/* ------------------------------------ */
		#ifdef _RFID_USING_EXTERNAL_MEMORY
	}
	else
	{
		status_check = _RFID_STAT_ERROR;
	}
	#endif /* _RFID_USING_EXTERNAL_MEMORY */
	/* ------------------------------------ */
	
	return status_check;
	
}
/*
	Guide   :
			Function description	This function is used to compare detected card with available cards in memory.
			
			Parameters
									* _card_type   : Type of registered card.
									* _card_number : Number of registered card.
			Return Values
									* _RFID_CARD_REGISTERED
									* _RFID_CARD_NOT_REGISTERED
									
									* _STAT_ERROR (in AVR)
									* HAL_ERROR (in STM32 ARM)
			
	Example :
			uint16_t              registered_card_number;
			RFID_CardType_TypeDef registered_card_type;
			RFID_MessageTypeDef   com_status;
			
			if(RFID_Status.CardDetected == _TRUE)
			{
				com_status = RFID_VerifyCard(&registered_card_type, &registered_card_number);
				
				if(com_status == _RFID_CARD_REGISTERED)
				{
					Lcd_PutString("This card is available");
					
					if(registered_card_type == _MAIN_CARD)
					{
						Lcd_PutString("This is main card");
					}
					else if(registered_card_type == _MASTER_CARD)
					...
					...
					...
				}
				else if(com_status == _RFID_CARD_NOT_REGISTERED)
				...
				...
				...
			}
			
*/

RFID_MessageTypeDef RFID_VerifyCard2(uint8_t *_card_id, RFID_CardType_TypeDef *_card_type, uint16_t *_card_number)
{
	/* ---------- Local variable ---------- */
	
	uint8_t mem_data = 0; /* Memory data */
	uint8_t *card_id_start_add = _card_id; /* Address of Card ID */
	uint8_t nmb_of_id_counter = 0; /* Numbers of ids counter */
	
	RFID_MessageTypeDef status_check = _RFID_CARD_NOT_REGISTERED;
	
	/* ----------- Load variable ---------- */
	*_card_type   = _NOT_FOUND_CARD;
	*_card_number = 0;
	
	/* ------------ Reset flag ------------ */
	
	RFID_ResetFlags();
	
	/* ----------- Check memory ----------- */
	#ifdef _RFID_USING_EXTERNAL_MEMORY
	if (I2C_MemoryIsReady(_RFID_MEMORY_TIME_OUT) == _RFID_STAT_OK) /* Connection check */
	{
	#endif /* _RFID_USING_EXTERNAL_MEMORY */
	/* ------------------------------------ */
		
		/* ~~~~~ Reset data ~~~~~ */
		
		g_mem_add = 0;
		Card_DataCheck.CardStatus   = _CARD_IS_CONFLICT;
		Card_DataCheck.NumberStatus = _NUMBER_IS_CORRECT;
		
		/* -------------- Reload -------------- */
		
		RFID_ConfigData_Load();
		
		/* ------------ Check cards ----------- */
		
		/* ~~~~~~~~~~~ Main card ~~~~~~~~~~~ */
		for (; ((RFID_Control.RegisteredMainCard > 0) && (Card_DataCheck.CardStatus == _CARD_IS_CONFLICT)); RFID_Control.RegisteredMainCard--) /* Card counter */
		{
			
			for (nmb_of_id_counter = 0; (nmb_of_id_counter < _RFID_CARD_ID_LENGTH) && (Card_DataCheck.NumberStatus == _NUMBER_IS_CORRECT); nmb_of_id_counter++) /* Numbers of card counter */
			{
				
				g_mem_add = (((uint32_t)RFID_Control.RegisteredMainCard - 1) * _RFID_CARD_ID_LENGTH) + (uint32_t)nmb_of_id_counter + _MAIN_CARDID_START_ADD; /* Memory address loading */
				
				RFID_Mem_SingleRead(g_mem_add, &mem_data, _RFID_MEMORY_TIME_OUT);
				
				if( *_card_id == mem_data ) /* Check & compare number value */
				{
					
					if( nmb_of_id_counter == _CARD_LAST_NMB ) /* Check index of number */
					{
						
						Card_DataCheck.CardStatus = _CARD_IS_IDENTIFIED;
						
						*_card_type   = _MAIN_CARD;
						*_card_number = RFID_Control.RegisteredMainCard;
						
						status_check = _RFID_CARD_REGISTERED;
						
					}
					
				}
				else
				{
					Card_DataCheck.NumberStatus = _NUMBER_IS_CONFLICT;
				}
				
				_card_id++;
				
			}
			
			_card_id = card_id_start_add;
			Card_DataCheck.NumberStatus = _NUMBER_IS_CORRECT;
			
		}
		
		/* ~~~~~~~~~~ Master card ~~~~~~~~~~ */
		if (Card_DataCheck.CardStatus == _CARD_IS_CONFLICT)
		{
			
			for (; ((RFID_Control.RegisteredMasterCard > 0) && (Card_DataCheck.CardStatus == _CARD_IS_CONFLICT)); RFID_Control.RegisteredMasterCard--) /* Card counter */
			{
				
				for (nmb_of_id_counter = 0; (nmb_of_id_counter < _RFID_CARD_ID_LENGTH) && (Card_DataCheck.NumberStatus == _NUMBER_IS_CORRECT); nmb_of_id_counter++) /* Numbers of card counter */
				{
					
					g_mem_add = (((uint32_t)RFID_Control.RegisteredMasterCard - 1) * _RFID_CARD_ID_LENGTH) + (uint32_t)nmb_of_id_counter + _MASTER_CARDID_START_ADD; /* Memory address loading */
					
					RFID_Mem_SingleRead(g_mem_add, &mem_data, _RFID_MEMORY_TIME_OUT);
					
					if( *_card_id == mem_data ) /* Check & compare number value */
					{
						
						if( nmb_of_id_counter == _CARD_LAST_NMB ) /* Check index of number */
						{
							
							Card_DataCheck.CardStatus = _CARD_IS_IDENTIFIED;
							
							*_card_type   = _MASTER_CARD;
							*_card_number = RFID_Control.RegisteredMasterCard;
							
							status_check = _RFID_CARD_REGISTERED;
							
						}
						
					}
					else
					{
						Card_DataCheck.NumberStatus = _NUMBER_IS_CONFLICT;
					}
					
					_card_id++;
					
				}
				
				_card_id = card_id_start_add;
				Card_DataCheck.NumberStatus = _NUMBER_IS_CORRECT;
				
			}
		
		/* ~~~~~~~~~~~ User card ~~~~~~~~~~~ */
			if (Card_DataCheck.CardStatus == _CARD_IS_CONFLICT)
			{
				
				for (; ((RFID_Control.RegisteredUserCard > 0) && (Card_DataCheck.CardStatus == _CARD_IS_CONFLICT)); RFID_Control.RegisteredUserCard--) /* Card counter */
				{
					
					for (nmb_of_id_counter = 0; (nmb_of_id_counter < _RFID_CARD_ID_LENGTH) && (Card_DataCheck.NumberStatus == _NUMBER_IS_CORRECT); nmb_of_id_counter++) /* Numbers of card counter */
					{
						
						g_mem_add = (((uint32_t)RFID_Control.RegisteredUserCard - 1) * _RFID_CARD_ID_LENGTH) + (uint32_t)nmb_of_id_counter + _USER_CARDID_START_ADD; /* Memory address loading */
						
						RFID_Mem_SingleRead(g_mem_add, &mem_data, _RFID_MEMORY_TIME_OUT);
						
						if( *_card_id == mem_data ) /* Check & compare number value */
						{
							
							if( nmb_of_id_counter == _CARD_LAST_NMB ) /* Check index of number */
							{
								
								Card_DataCheck.CardStatus = _CARD_IS_IDENTIFIED;
								
								*_card_type   = _USER_CARD;
								*_card_number = RFID_Control.RegisteredUserCard;
								
								status_check = _RFID_CARD_REGISTERED;
								
							}
							
						}
						else
						{
							Card_DataCheck.NumberStatus = _NUMBER_IS_CONFLICT;
						}
						
						_card_id++;
						
					}
					
					_card_id = card_id_start_add;
					Card_DataCheck.NumberStatus = _NUMBER_IS_CORRECT;
					
				}
				/* Check in master */
				
			}
			
		}
		
	/* ------------------------------------ */
	#ifdef _RFID_USING_EXTERNAL_MEMORY
	}
	else
	{
		status_check = _RFID_STAT_ERROR;
	}
	#endif /* _RFID_USING_EXTERNAL_MEMORY */
	/* ------------------------------------ */
	
	return status_check;
	
}
/*
	Guide   :
			Function description	This function is used to compare your card id string with available cards in memory.
			
			Parameters
									* _card_id     : ID for compare with available cards in memory.
									* _card_type   : Type of registered card.
									* _card_number : Number of registered card.
			Return Values
									* _RFID_CARD_REGISTERED
									* _RFID_CARD_NOT_REGISTERED
									
									* _STAT_ERROR (in AVR)
									* HAL_ERROR (in STM32 ARM)
			
	Example :
			uint8_t               my_detected_card_id[10];
			uint16_t              registered_card_number;
			RFID_CardType_TypeDef registered_card_type;
			RFID_MessageTypeDef   com_status;
			
			(First case)
			if(RFID_Status.CardDetected == _TRUE)
			{
				com_status = RFID_GetCardID(my_detected_card_id);
				com_status = RFID_VerifyCard(my_detected_card_id, &registered_card_type, &registered_card_number);
			}
			
			(Second case)
			com_status = RFID_VerifyCard2("0010035314", &registered_card_type, &registered_card_number);
				
			if(com_status == _RFID_CARD_REGISTERED)
			{
				Lcd_PutString("This card is available");
					
				if(registered_card_type == _MAIN_CARD)
				{
					Lcd_PutString("This is main card");
				}
				else if(registered_card_type == _MASTER_CARD)
				...
				...
				...
			}
			else if(com_status == _RFID_CARD_NOT_REGISTERED)
			...
			...
			...
			
*/

RFID_MessageTypeDef RFID_SaveCard(RFID_CardType_TypeDef _card_type)
{

	/*---------------------- Create Variable -----------------------*/
	
	RFID_MessageTypeDef   status_check;
	RFID_CardType_TypeDef verified_card_type;
	
	uint16_t verified_card_number;
	
	/*------------------- Compare For Exist Card -------------------*/
	
	status_check = RFID_VerifyCard(&verified_card_type, &verified_card_number);
	
	if (status_check == _RFID_CARD_NOT_REGISTERED)
	{
		
		/* ,,,,,, Reload ,,,,,, */
		
		RFID_ResetFlags();
		RFID_ConfigData_Load();
		
		/* ,,,,,, Check card type / Save information  ,,,,,, */
		
		switch (_card_type)
		{
			/* ~~~~~~~~~~~ Main card ~~~~~~~~~~~ */
			case _MAIN_CARD:
			{
				
				if (RFID_Control.RegisteredMainCard < RFID_Config.NumberOfMainCard)
				{
					
					g_mem_add = ((uint32_t)RFID_Control.RegisteredMainCard * _RFID_CARD_ID_LENGTH) + _MAIN_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_BurstWrite(g_mem_add, detected_card_id, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Save number of registered card ~~~ */
					RFID_Control.RegisteredMainCard++;
					
					RFID_Mem_SingleWrite(RFID_Config.StartOfDataLocationAddress , (uint8_t)(RFID_Control.RegisteredMainCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
					RFID_Mem_SingleWrite( (RFID_Config.StartOfDataLocationAddress + 1) , (uint8_t)RFID_Control.RegisteredMainCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Verify data ~~~ */
					#ifdef _RFID_VERIFY_IN_SAVE
					
					status_check = RFID_VerifyCard(&verified_card_type, &verified_card_number);
					
					if ((status_check == _RFID_CARD_REGISTERED) && (verified_card_type == _MAIN_CARD))
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_CARD_SAVED;
						
						RFID_Status.CardSaved = _TRUE;
					}
					else
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_STAT_ERROR;
						
						RFID_Status.CardSaved = _FALSE;
					}
					
					#else
					
					/* ~~~ Set status ~~~ */
					status_check = _RFID_CARD_SAVED;
					
					RFID_Status.CardSaved = _TRUE;
					
					#endif /* _VERIFY_IN_SAVE */
					
				}
				else
				{
					status_check = _RFID_MEM_FULL;
				}
				
			}break;
			
			/* ~~~~~~~~~~ Master card ~~~~~~~~~~ */
			case _MASTER_CARD:
			{
				
				if (RFID_Control.RegisteredMasterCard < RFID_Config.NumberOfMasterCard)
				{
					
					g_mem_add = ((uint32_t)RFID_Control.RegisteredMasterCard * _RFID_CARD_ID_LENGTH) + _MASTER_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_BurstWrite(g_mem_add, detected_card_id, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Save number of registered card ~~~ */
					RFID_Control.RegisteredMasterCard++;
					
					RFID_Mem_SingleWrite(_REGISTERED_MASTER_CARD_ADD , (uint8_t)(RFID_Control.RegisteredMasterCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
					RFID_Mem_SingleWrite( (_REGISTERED_MASTER_CARD_ADD + 1) , (uint8_t)RFID_Control.RegisteredMasterCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Verify data ~~~ */
					#ifdef _RFID_VERIFY_IN_SAVE
					
					status_check = RFID_VerifyCard(&verified_card_type, &verified_card_number);
					
					if ((status_check == _RFID_CARD_REGISTERED) && (verified_card_type == _MASTER_CARD))
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_CARD_SAVED;
						
						RFID_Status.CardSaved = _TRUE;
					}
					else
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_STAT_ERROR;
						
						RFID_Status.CardSaved = _FALSE;
					}
					
					#else
					
					/* ~~~ Set status ~~~ */
					status_check = _RFID_CARD_SAVED;
					
					RFID_Status.CardSaved = _TRUE;
					
					#endif /* _VERIFY_IN_SAVE */
					
				}
				else
				{
					status_check = _RFID_MEM_FULL;
				}
				
			}break;
			
			/* ~~~~~~~~~~~ User card ~~~~~~~~~~~ */
			case _USER_CARD:
			{
				
				if (RFID_Control.RegisteredUserCard < RFID_Config.NumberOfUserCard)
				{
					
					g_mem_add = ((uint32_t)RFID_Control.RegisteredUserCard * _RFID_CARD_ID_LENGTH) + _USER_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_BurstWrite(g_mem_add, detected_card_id, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Save number of registered card ~~~ */
					RFID_Control.RegisteredUserCard++;
					
					RFID_Mem_SingleWrite(_REGISTERED_USER_CARD_ADD , (uint8_t)(RFID_Control.RegisteredUserCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
					RFID_Mem_SingleWrite( (_REGISTERED_USER_CARD_ADD + 1) , (uint8_t)RFID_Control.RegisteredUserCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Verify data ~~~ */
					#ifdef _RFID_VERIFY_IN_SAVE
					
					status_check = RFID_VerifyCard(&verified_card_type, &verified_card_number);
					
					if ((status_check == _RFID_CARD_REGISTERED) && (verified_card_type == _USER_CARD))
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_CARD_SAVED;
						
						RFID_Status.CardSaved = _TRUE;
					}
					else
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_STAT_ERROR;
						
						RFID_Status.CardSaved = _FALSE;
					}
					
					#else
					
					/* ~~~ Set status ~~~ */
					status_check = _RFID_CARD_SAVED;
					
					RFID_Status.CardSaved = _TRUE;
					
					#endif /* _VERIFY_IN_SAVE */
					
				}
				else
				{
					status_check = _RFID_MEM_FULL;
				}
				
			}break;
			
			default:
			{
				status_check = _RFID_CARD_TYPE_ERROR;
			}
			break;
		}
		
	}
	
	return status_check;
	
}
/*
	Guide   :
			Function description	This function is used to save detected card in memory.
			
			Parameters
									* _card_type : Type of card for save.
			Return Values
									* _RFID_CARD_SAVED
									* _RFID_CARD_TYPE_ERROR
									* _RFID_CARD_REGISTERED
									* _RFID_MEM_FULL
									
									* _STAT_ERROR (in AVR)
									* HAL_ERROR (in STM32 ARM)
			
	Example :
			RFID_MessageTypeDef com_status;
			
			if(RFID_Status.CardDetected == _TRUE)
			{
				com_status = RFID_SaveCard(_MASTER_CARD);
				
				if(com_status == _RFID_CARD_SAVED)
				{
					Lcd_PutString("Card saved!");
				}
				else if(com_status == _RFID_CARD_REGISTERED)
				...
				...
				...
			}
			
*/

RFID_MessageTypeDef RFID_SaveCard2(uint8_t *_card_id, RFID_CardType_TypeDef _card_type)
{

	/*---------------------- Create Variable -----------------------*/
	
	RFID_MessageTypeDef   status_check;
	RFID_CardType_TypeDef verified_card_type;
	
	uint16_t verified_card_number;
	
	/*------------------- Compare For Exist Card -------------------*/
	
	status_check = RFID_VerifyCard2(_card_id, &verified_card_type, &verified_card_number);
	
	if (status_check == _RFID_CARD_NOT_REGISTERED)
	{
		
		/* ,,,,,, Reload ,,,,,, */
		
		RFID_ResetFlags();
		RFID_ConfigData_Load();
		
		/* ,,,,,, Check card type / Save information  ,,,,,, */
		
		switch (_card_type)
		{
			/* ~~~~~~~~~~~ Main card ~~~~~~~~~~~ */
			case _MAIN_CARD:
			{
				
				if (RFID_Control.RegisteredMainCard < RFID_Config.NumberOfMainCard)
				{
					
					g_mem_add = ((uint32_t)RFID_Control.RegisteredMainCard * _RFID_CARD_ID_LENGTH) + _MAIN_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_BurstWrite(g_mem_add, _card_id, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Save number of registered card ~~~ */
					RFID_Control.RegisteredMainCard++;
					
					RFID_Mem_SingleWrite(RFID_Config.StartOfDataLocationAddress , (uint8_t)(RFID_Control.RegisteredMainCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
					RFID_Mem_SingleWrite( (RFID_Config.StartOfDataLocationAddress + 1) , (uint8_t)RFID_Control.RegisteredMainCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Verify data ~~~ */
					#ifdef _RFID_VERIFY_IN_SAVE
					
					status_check = RFID_VerifyCard2(_card_id, &verified_card_type, &verified_card_number);
					
					if ((status_check == _RFID_CARD_REGISTERED) && (verified_card_type == _MAIN_CARD))
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_CARD_SAVED;
						
						RFID_Status.CardSaved = _TRUE;
					}
					else
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_STAT_ERROR;
						
						RFID_Status.CardSaved = _FALSE;
					}
					
					#else
					
					/* ~~~ Set status ~~~ */
					status_check = _RFID_CARD_SAVED;
					
					RFID_Status.CardSaved = _TRUE;
					
					#endif /* _VERIFY_IN_SAVE */
					
				}
				else
				{
					status_check = _RFID_MEM_FULL;
				}
				
			}break;
			
			/* ~~~~~~~~~~ Master card ~~~~~~~~~~ */
			case _MASTER_CARD:
			{
				
				if (RFID_Control.RegisteredMasterCard < RFID_Config.NumberOfMasterCard)
				{
					
					g_mem_add = ((uint32_t)RFID_Control.RegisteredMasterCard * _RFID_CARD_ID_LENGTH) + _MASTER_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_BurstWrite(g_mem_add, _card_id, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Save number of registered card ~~~ */
					RFID_Control.RegisteredMasterCard++;
					
					RFID_Mem_SingleWrite(_REGISTERED_MASTER_CARD_ADD , (uint8_t)(RFID_Control.RegisteredMasterCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
					RFID_Mem_SingleWrite( (_REGISTERED_MASTER_CARD_ADD + 1) , (uint8_t)RFID_Control.RegisteredMasterCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Verify data ~~~ */
					#ifdef _RFID_VERIFY_IN_SAVE
					
					status_check = RFID_VerifyCard2(_card_id, &verified_card_type, &verified_card_number);
					
					if ((status_check == _RFID_CARD_REGISTERED) && (verified_card_type == _MASTER_CARD))
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_CARD_SAVED;
						
						RFID_Status.CardSaved = _TRUE;
					}
					else
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_STAT_ERROR;
						
						RFID_Status.CardSaved = _FALSE;
					}
					
					#else
					
					/* ~~~ Set status ~~~ */
					status_check = _RFID_CARD_SAVED;
					
					RFID_Status.CardSaved = _TRUE;
					
					#endif /* _VERIFY_IN_SAVE */
					
				}
				else
				{
					status_check = _RFID_MEM_FULL;
				}
				
			}break;
			
			/* ~~~~~~~~~~~ User card ~~~~~~~~~~~ */
			case _USER_CARD:
			{
				
				if (RFID_Control.RegisteredUserCard < RFID_Config.NumberOfUserCard)
				{
					
					g_mem_add = ((uint32_t)RFID_Control.RegisteredUserCard * _RFID_CARD_ID_LENGTH) + _USER_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_BurstWrite(g_mem_add, _card_id, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Save number of registered card ~~~ */
					RFID_Control.RegisteredUserCard++;
					
					RFID_Mem_SingleWrite(_REGISTERED_USER_CARD_ADD , (uint8_t)(RFID_Control.RegisteredUserCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
					RFID_Mem_SingleWrite( (_REGISTERED_USER_CARD_ADD + 1) , (uint8_t)RFID_Control.RegisteredUserCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Verify data ~~~ */
					#ifdef _RFID_VERIFY_IN_SAVE
					
					status_check = RFID_VerifyCard2(_card_id, &verified_card_type, &verified_card_number);
					
					if ((status_check == _RFID_CARD_REGISTERED) && (verified_card_type == _USER_CARD))
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_CARD_SAVED;
						
						RFID_Status.CardSaved = _TRUE;
					}
					else
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_STAT_ERROR;
						
						RFID_Status.CardSaved = _FALSE;
					}
					
					#else
					
					/* ~~~ Set status ~~~ */
					status_check = _RFID_CARD_SAVED;
					
					RFID_Status.CardSaved = _TRUE;
					
					#endif /* _VERIFY_IN_SAVE */
					
				}
				else
				{
					status_check = _RFID_MEM_FULL;
				}
				
			}break;
			
			default:
			{
				status_check = _RFID_CARD_TYPE_ERROR;
			}
			break;
		}
		
	}
	
	return status_check;
	
}
/*
	Guide   :
			Function description	This function is used to save card id string in memory.
			
			Parameters
									* _card_id   : ID for save in memory.
									* _card_type : Type of card for save.
			Return Values
									* _RFID_CARD_SAVED
									* _RFID_CARD_TYPE_ERROR
									* _RFID_CARD_REGISTERED
									* _RFID_MEM_FULL
									
									* _STAT_ERROR (in AVR)
									* HAL_ERROR (in STM32 ARM)
			
	Example :
			uint8_t             my_card_id[10]
			RFID_MessageTypeDef com_status;
			
			(First case)
			com_status = RFID_SaveCard2(my_card_id, _MASTER_CARD);
			
			(Second case)
			com_status = RFID_SaveCard2("0010035314", _USER_CARD);
			
			if(com_status == _RFID_CARD_SAVED)
			{
				Lcd_PutString("Card saved!");
			}
			else if(com_status == _RFID_CARD_REGISTERED)
			...
			...
			...
			
*/

RFID_MessageTypeDef RFID_ReadCard(uint8_t *_card_id, RFID_CardType_TypeDef _card_type, uint16_t _card_number)
{
	/* ---------- Local variable ---------- */
	
	RFID_MessageTypeDef status_check = _RFID_STAT_ERROR;
	
	/* ~~~~~ Reset data ~~~~~ */
	
	g_mem_add = 0;
	
	/* -------------- Reload -------------- */
	
	RFID_ResetFlags();
	RFID_ConfigData_Load();
	
	/* ------------ Check cards ----------- */
	
	switch (_card_type)
	{
		/* ~~~~~~~~~~~ Main card ~~~~~~~~~~~ */
		case _MAIN_CARD:
		{
			
			if ((_card_number > 0) && (_card_number <= RFID_Control.RegisteredMainCard))
			{
				
				g_mem_add = (((uint32_t)_card_number - 1) * _RFID_CARD_ID_LENGTH) + _MAIN_CARDID_START_ADD; /* Preparing memory address */
				
				status_check = (RFID_MessageTypeDef)RFID_Mem_BurstRead(g_mem_add, _card_id, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT);
				
				if (status_check == _RFID_STAT_OK)
				{
					status_check = _RFID_CARD_READED;
				}
				
			}
			else
			{
				status_check = _RFID_CARD_NOT_REGISTERED;
			}
			
		}break;
		
		/* ~~~~~~~~~~ Master card ~~~~~~~~~~ */
		case _MASTER_CARD:
		{
			
			if ((_card_number > 0) && (_card_number <= RFID_Control.RegisteredMasterCard))
			{
				
				g_mem_add = (((uint32_t)_card_number - 1) * _RFID_CARD_ID_LENGTH) + _MASTER_CARDID_START_ADD; /* Preparing memory address */
				
				status_check = (RFID_MessageTypeDef)RFID_Mem_BurstRead(g_mem_add, _card_id, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT);
				
				if (status_check == _RFID_STAT_OK)
				{
					status_check = _RFID_CARD_READED;
				}
				
			}
			else
			{
				status_check = _RFID_CARD_NOT_REGISTERED;
			}
			
		}break;
		
		/* ~~~~~~~~~~~ User card ~~~~~~~~~~~ */
		case _USER_CARD:
		{
			
			if ((_card_number > 0) && (_card_number <= RFID_Control.RegisteredUserCard))
			{
				
				g_mem_add = (((uint32_t)_card_number - 1) * _RFID_CARD_ID_LENGTH) + _USER_CARDID_START_ADD; /* Preparing memory address */
				
				status_check = (RFID_MessageTypeDef)RFID_Mem_BurstRead(g_mem_add, _card_id, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT);
				
				if (status_check == _RFID_STAT_OK)
				{
					status_check = _RFID_CARD_READED;
				}
				
			}
			else
			{
				status_check = _RFID_CARD_NOT_REGISTERED;
			}
			
		}break;
		
		default:
		{
			status_check = _RFID_CARD_TYPE_ERROR;
		}
		break;
	}
	
	return status_check;
}
/*
	Guide   :
			Function description	This function is used to read registered card id from memory.
			
			Parameters
									* _card_id     : Pointer to string for read and save registered card id from memory.
									* _card_type   : Type of card for read.
									* _card_number : Number of card for read.
			Return Values
									* _RFID_CARD_READED
									* _RFID_CARD_TYPE_ERROR
									* _RFID_CARD_NOT_REGISTERED
									
									* _STAT_ERROR (in AVR)
									* HAL_ERROR (in STM32 ARM)
			
	Example :
			uint8_t             saved_card_id[10]
			RFID_MessageTypeDef com_status;
			
			com_status = RFID_ReadCard(saved_card_id, _MAIN_CARD, 2);
			
			if(com_status == _RFID_CARD_READED)
			{
				Lcd_PutString(saved_card_id);
			}
			else if(com_status == _RFID_CARD_NOT_REGISTERED)
			...
			...
			...
			
*/

RFID_MessageTypeDef RFID_RemoveCard(RFID_CardType_TypeDef _card_type, uint16_t _card_number)
{
	
	/* ---------- Local variable ---------- */
	
	uint8_t card_id_for_copy[_RFID_CARD_ID_LENGTH];
	
	RFID_MessageTypeDef status_check = _RFID_STAT_ERROR;
	
	#ifdef _RFID_VERIFY_IN_REMOVE
	
	uint8_t card_id_for_verify[_RFID_CARD_ID_LENGTH];
	
	uint16_t verified_card_number = 0;
	
	RFID_CardType_TypeDef verified_card_type;
	
	#endif /* _RFID_VERIFY_IN_REMOVE */
	
	/* ----------- Check memory ----------- */
	#ifdef _RFID_USING_EXTERNAL_MEMORY
	if (I2C_MemoryIsReady(_RFID_MEMORY_TIME_OUT) == _RFID_STAT_OK)
	{
	#endif /* _RFID_USING_EXTERNAL_MEMORY */
	/* ------------------------------------ */
		
		/* ~~~~~ Reset data ~~~~~ */
		
		g_mem_add = 0;
		
		/* -------------- Reload -------------- */
		
		RFID_ResetFlags();
		RFID_ConfigData_Load();
		
		/* ------------ Check cards ----------- */
		
		switch (_card_type)
		{
			/* ~~~~~~~~~~~ Main card ~~~~~~~~~~~ */
			case _MAIN_CARD:
			{
				
				if ((_card_number > 0) && (_card_number <= RFID_Control.RegisteredMainCard)) /* Check number */
				{
					
					#ifdef _RFID_VERIFY_IN_REMOVE
					
					g_mem_add = ((uint32_t)(_card_number - 1) * _RFID_CARD_ID_LENGTH) + _MAIN_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_BurstRead(g_mem_add, card_id_for_verify, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Read selected card id */
					
					#endif /* _RFID_VERIFY_IN_REMOVE */
					
					/* ,,,,, Remove card ,,,,, */
					for (; _card_number < RFID_Control.RegisteredMainCard; _card_number++)
					{
						
						g_mem_add = ((uint32_t)_card_number * _RFID_CARD_ID_LENGTH) + _MAIN_CARDID_START_ADD; /* Preparing memory address */
						
						/* ,,,,, Move card id's ,,,,, */
						RFID_Mem_BurstRead(g_mem_add, card_id_for_copy, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Read second card id */
						RFID_Mem_BurstWrite((g_mem_add - _RFID_CARD_ID_LENGTH), card_id_for_copy, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Rewrite card id */
						
					}
					
					g_mem_add = (((uint32_t)RFID_Control.RegisteredMainCard - 1) * _RFID_CARD_ID_LENGTH) + _MAIN_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_Erase(g_mem_add, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Erase memory */
					
					/* ~~~ Save number of registered card ~~~ */
					RFID_Control.RegisteredMainCard--;
					
					RFID_Mem_SingleWrite(RFID_Config.StartOfDataLocationAddress , (uint8_t)(RFID_Control.RegisteredMainCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
					RFID_Mem_SingleWrite( (RFID_Config.StartOfDataLocationAddress + 1) , (uint8_t)RFID_Control.RegisteredMainCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Verify data ~~~ */
					#ifdef _RFID_VERIFY_IN_REMOVE
					
					status_check = RFID_VerifyCard2(card_id_for_verify, &verified_card_type, &verified_card_number);
					
					if (status_check == _RFID_CARD_NOT_REGISTERED)
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_CARD_REMOVED;
						
						RFID_Status.CardRemoved = _TRUE;
					}
					else
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_STAT_ERROR;
						
						RFID_Status.CardRemoved = _FALSE;
					}
					
					#else
					
					/* ~~~ Set status ~~~ */
					status_check = _RFID_CARD_REMOVED;
					
					RFID_Status.CardRemoved = _TRUE;
					
					#endif /* _VERIFY_IN_SAVE */
					
				}
				else
				{
					status_check = _RFID_CARD_NOT_REGISTERED;
				}
				
			}break;
			
			/* ~~~~~~~~~~ Master card ~~~~~~~~~~ */
			case _MASTER_CARD:
			{
				
				if ((_card_number > 0) && (_card_number <= RFID_Control.RegisteredMasterCard))
				{
					
					#ifdef _RFID_VERIFY_IN_REMOVE
					
					g_mem_add = ((uint32_t)(_card_number - 1) * _RFID_CARD_ID_LENGTH) + _MASTER_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_BurstRead(g_mem_add, card_id_for_verify, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Read selected card id */
					
					#endif /* _RFID_VERIFY_IN_REMOVE */
					
					/* ,,,,, Remove card ,,,,, */
					for (; _card_number < RFID_Control.RegisteredMasterCard; _card_number++)
					{
						
						g_mem_add = ((uint32_t)_card_number * _RFID_CARD_ID_LENGTH) + _MASTER_CARDID_START_ADD; /* Preparing memory address */
						
						/* ,,,,, Move card id's ,,,,, */
						RFID_Mem_BurstRead(g_mem_add, card_id_for_copy, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Read second card id */
						RFID_Mem_BurstWrite((g_mem_add - _RFID_CARD_ID_LENGTH), card_id_for_copy, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Rewrite card id */
						
					}
					
					g_mem_add = (((uint32_t)RFID_Control.RegisteredMasterCard - 1) * _RFID_CARD_ID_LENGTH) + _MASTER_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_Erase(g_mem_add, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Erase memory */
					
					/* ~~~ Save number of registered card ~~~ */
					RFID_Control.RegisteredMasterCard--;
					
					RFID_Mem_SingleWrite(_REGISTERED_MASTER_CARD_ADD , (uint8_t)(RFID_Control.RegisteredMasterCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
					RFID_Mem_SingleWrite( (_REGISTERED_MASTER_CARD_ADD + 1) , (uint8_t)RFID_Control.RegisteredMasterCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Verify data ~~~ */
					#ifdef _RFID_VERIFY_IN_REMOVE
					
					status_check = RFID_VerifyCard2(card_id_for_verify, &verified_card_type, &verified_card_number);
					
					if (status_check == _RFID_CARD_NOT_REGISTERED)
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_CARD_REMOVED;
						
						RFID_Status.CardRemoved = _TRUE;
					}
					else
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_STAT_ERROR;
						
						RFID_Status.CardRemoved = _FALSE;
					}
					
					#else
					
					/* ~~~ Set status ~~~ */
					status_check = _RFID_CARD_REMOVED;
					
					RFID_Status.CardRemoved = _TRUE;
					
					#endif /* _VERIFY_IN_SAVE */
					
				}
				else
				{
					status_check = _RFID_CARD_NOT_REGISTERED;
				}
				
			}break;
			
			/* ~~~~~~~~~~~ User card ~~~~~~~~~~~ */
			case _USER_CARD:
			{
				
				if ((_card_number > 0) && (_card_number <= RFID_Control.RegisteredUserCard))
				{
					
					#ifdef _RFID_VERIFY_IN_REMOVE
					
					g_mem_add = ((uint32_t)(_card_number - 1) * _RFID_CARD_ID_LENGTH) + _USER_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_BurstRead(g_mem_add, card_id_for_verify, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Read selected card id */
					
					#endif /* _RFID_VERIFY_IN_REMOVE */
					
					/* ,,,,, Remove card ,,,,, */
					for (; _card_number < RFID_Control.RegisteredUserCard; _card_number++)
					{
						
						g_mem_add = ((uint32_t)_card_number * _RFID_CARD_ID_LENGTH) + _USER_CARDID_START_ADD; /* Preparing memory address */
						
						/* ,,,,, Move card id's ,,,,, */
						RFID_Mem_BurstRead(g_mem_add, card_id_for_copy, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Read second card id */
						RFID_Mem_BurstWrite((g_mem_add - _RFID_CARD_ID_LENGTH), card_id_for_copy, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Rewrite card id */
						
					}
					
					g_mem_add = (((uint32_t)RFID_Control.RegisteredUserCard - 1) * _RFID_CARD_ID_LENGTH) + _USER_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_Erase(g_mem_add, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Erase memory */
					
					/* ~~~ Save number of registered card ~~~ */
					RFID_Control.RegisteredUserCard--;
					
					RFID_Mem_SingleWrite(_REGISTERED_USER_CARD_ADD , (uint8_t)(RFID_Control.RegisteredUserCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
					RFID_Mem_SingleWrite( (_REGISTERED_USER_CARD_ADD + 1) , (uint8_t)RFID_Control.RegisteredUserCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Verify data ~~~ */
					#ifdef _RFID_VERIFY_IN_REMOVE
					
					status_check = RFID_VerifyCard2(card_id_for_verify, &verified_card_type, &verified_card_number);
					
					if (status_check == _RFID_CARD_NOT_REGISTERED)
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_CARD_REMOVED;
						
						RFID_Status.CardRemoved = _TRUE;
					}
					else
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_STAT_ERROR;
						
						RFID_Status.CardRemoved = _FALSE;
					}
					
					#else
					
					/* ~~~ Set status ~~~ */
					status_check = _RFID_CARD_REMOVED;
					
					RFID_Status.CardRemoved = _TRUE;
					
					#endif /* _VERIFY_IN_SAVE */
					
				}
				else
				{
					status_check = _RFID_CARD_NOT_REGISTERED;
				}
				
			}break;
			
			default:
			{
				status_check = _RFID_CARD_TYPE_ERROR;
			}
			break;
		}
		
	/* ------------------------------------ */
	#ifdef _RFID_USING_EXTERNAL_MEMORY
	}
	else
	{
		status_check = _RFID_STAT_ERROR;
	}
	#endif /* _RFID_USING_EXTERNAL_MEMORY */
	/* ------------------------------------ */
	
	return status_check;
	
}
/*
	Guide   :
			Function description	This function is used to remove detected card from available cards in memory.
			
			Parameters
									* _card_type   : Type of registered card.
									* _card_number : Number of registered card.
			Return Values
									* _RFID_CARD_REMOVED
									* _RFID_CARD_TYPE_ERROR
									* _RFID_CARD_NOT_REGISTERED
									
									* _STAT_ERROR (in AVR)
									* HAL_ERROR (in STM32 ARM)
			
	Example :
			RFID_MessageTypeDef   com_status;
			
			if(RFID_Status.CardDetected == _TRUE)
			{
				com_status = RFID_RemoveCard(_MAIN_CARD, 1);
				
				if(com_status == _RFID_CARD_REMOVED)
				{
					Lcd_PutString("Your card removed!");
				}
				else if(com_status == _RFID_CARD_NOT_REGISTERED)
				{
					Lcd_PutString("This card not registered");
				}
				else if(com_status == _RFID_CARD_TYPE_ERROR)
				...
				...
				...
			}
			
*/

RFID_MessageTypeDef RFID_RemoveCard2(uint8_t *_card_id)
{
	
	/* ---------- Local variable ---------- */
	
	uint16_t _card_number = 0;
	
	uint8_t card_id_for_copy[_RFID_CARD_ID_LENGTH];
	
	RFID_CardType_TypeDef _card_type;
	RFID_MessageTypeDef   status_check = _RFID_STAT_ERROR;
	
	#ifdef _RFID_VERIFY_IN_REMOVE
	
	uint8_t card_id_for_verify[_RFID_CARD_ID_LENGTH];
	
	uint16_t verified_card_number = 0;
	
	RFID_CardType_TypeDef verified_card_type;
	
	#endif /* _RFID_VERIFY_IN_REMOVE */
	
	/* ----------- Check memory ----------- */
	#ifdef _RFID_USING_EXTERNAL_MEMORY
	if (I2C_MemoryIsReady(_RFID_MEMORY_TIME_OUT) == _RFID_STAT_OK)
	{
		#endif /* _RFID_USING_EXTERNAL_MEMORY */
		/* ------------------------------------ */
		
		/* ~~~~~ Reset data ~~~~~ */
		
		g_mem_add = 0;
		
		/* -------------- Reload -------------- */
		
		RFID_ResetFlags();
		RFID_ConfigData_Load();
		
		/* ------------ Check cards ----------- */
		
		RFID_VerifyCard2(_card_id,&_card_type,&_card_number);
		
		switch (_card_type)
		{
			/* ~~~~~~~~~~~ Main card ~~~~~~~~~~~ */
			case _MAIN_CARD:
			{
				
				if ((_card_number > 0) && (_card_number <= RFID_Control.RegisteredMainCard)) /* Check number */
				{
					
					#ifdef _RFID_VERIFY_IN_REMOVE
					
					g_mem_add = ((uint32_t)(_card_number - 1) * _RFID_CARD_ID_LENGTH) + _MAIN_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_BurstRead(g_mem_add, card_id_for_verify, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Read selected card id */
					
					#endif /* _RFID_VERIFY_IN_REMOVE */
					
					/* ,,,,, Remove card ,,,,, */
					for (; _card_number < RFID_Control.RegisteredMainCard; _card_number++)
					{
						
						g_mem_add = ((uint32_t)_card_number * _RFID_CARD_ID_LENGTH) + _MAIN_CARDID_START_ADD; /* Preparing memory address */
						
						/* ,,,,, Move card id's ,,,,, */
						RFID_Mem_BurstRead(g_mem_add, card_id_for_copy, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Read second card id */
						RFID_Mem_BurstWrite((g_mem_add - _RFID_CARD_ID_LENGTH), card_id_for_copy, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Rewrite card id */
						
					}
					
					g_mem_add = (((uint32_t)RFID_Control.RegisteredMainCard - 1) * _RFID_CARD_ID_LENGTH) + _MAIN_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_Erase(g_mem_add, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Erase memory */
					
					/* ~~~ Save number of registered card ~~~ */
					RFID_Control.RegisteredMainCard--;
					
					RFID_Mem_SingleWrite(RFID_Config.StartOfDataLocationAddress , (uint8_t)(RFID_Control.RegisteredMainCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
					RFID_Mem_SingleWrite( (RFID_Config.StartOfDataLocationAddress + 1) , (uint8_t)RFID_Control.RegisteredMainCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Verify data ~~~ */
					#ifdef _RFID_VERIFY_IN_REMOVE
					
					status_check = RFID_VerifyCard2(card_id_for_verify, &verified_card_type, &verified_card_number);
					
					if (status_check == _RFID_CARD_NOT_REGISTERED)
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_CARD_REMOVED;
						
						RFID_Status.CardRemoved = _TRUE;
					}
					else
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_STAT_ERROR;
						
						RFID_Status.CardRemoved = _FALSE;
					}
					
					#else
					
					/* ~~~ Set status ~~~ */
					status_check = _RFID_CARD_REMOVED;
					
					RFID_Status.CardRemoved = _TRUE;
					
					#endif /* _VERIFY_IN_SAVE */
					
				}
				else
				{
					status_check = _RFID_CARD_NOT_REGISTERED;
				}
				
			}break;
			
			/* ~~~~~~~~~~ Master card ~~~~~~~~~~ */
			case _MASTER_CARD:
			{
				
				if ((_card_number > 0) && (_card_number <= RFID_Control.RegisteredMasterCard))
				{
					
					#ifdef _RFID_VERIFY_IN_REMOVE
					
					g_mem_add = ((uint32_t)(_card_number - 1) * _RFID_CARD_ID_LENGTH) + _MASTER_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_BurstRead(g_mem_add, card_id_for_verify, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Read selected card id */
					
					#endif /* _RFID_VERIFY_IN_REMOVE */
					
					/* ,,,,, Remove card ,,,,, */
					for (; _card_number < RFID_Control.RegisteredMasterCard; _card_number++)
					{
						
						g_mem_add = ((uint32_t)_card_number * _RFID_CARD_ID_LENGTH) + _MASTER_CARDID_START_ADD; /* Preparing memory address */
						
						/* ,,,,, Move card id's ,,,,, */
						RFID_Mem_BurstRead(g_mem_add, card_id_for_copy, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Read second card id */
						RFID_Mem_BurstWrite((g_mem_add - _RFID_CARD_ID_LENGTH), card_id_for_copy, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Rewrite card id */
						
					}
					
					g_mem_add = (((uint32_t)RFID_Control.RegisteredMasterCard - 1) * _RFID_CARD_ID_LENGTH) + _MASTER_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_Erase(g_mem_add, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Erase memory */
					
					/* ~~~ Save number of registered card ~~~ */
					RFID_Control.RegisteredMasterCard--;
					
					RFID_Mem_SingleWrite(_REGISTERED_MASTER_CARD_ADD , (uint8_t)(RFID_Control.RegisteredMasterCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
					RFID_Mem_SingleWrite( (_REGISTERED_MASTER_CARD_ADD + 1) , (uint8_t)RFID_Control.RegisteredMasterCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Verify data ~~~ */
					#ifdef _RFID_VERIFY_IN_REMOVE
					
					status_check = RFID_VerifyCard2(card_id_for_verify, &verified_card_type, &verified_card_number);
					
					if (status_check == _RFID_CARD_NOT_REGISTERED)
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_CARD_REMOVED;
						
						RFID_Status.CardRemoved = _TRUE;
					}
					else
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_STAT_ERROR;
						
						RFID_Status.CardRemoved = _FALSE;
					}
					
					#else
					
					/* ~~~ Set status ~~~ */
					status_check = _RFID_CARD_REMOVED;
					
					RFID_Status.CardRemoved = _TRUE;
					
					#endif /* _VERIFY_IN_SAVE */
					
				}
				else
				{
					status_check = _RFID_CARD_NOT_REGISTERED;
				}
				
			}break;
			
			/* ~~~~~~~~~~~ User card ~~~~~~~~~~~ */
			case _USER_CARD:
			{
				
				if ((_card_number > 0) && (_card_number <= RFID_Control.RegisteredUserCard))
				{
					
					#ifdef _RFID_VERIFY_IN_REMOVE
					
					g_mem_add = ((uint32_t)(_card_number - 1) * _RFID_CARD_ID_LENGTH) + _USER_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_BurstRead(g_mem_add, card_id_for_verify, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Read selected card id */
					
					#endif /* _RFID_VERIFY_IN_REMOVE */
					
					/* ,,,,, Remove card ,,,,, */
					for (; _card_number < RFID_Control.RegisteredUserCard; _card_number++)
					{
						
						g_mem_add = ((uint32_t)_card_number * _RFID_CARD_ID_LENGTH) + _USER_CARDID_START_ADD; /* Preparing memory address */
						
						/* ,,,,, Move card id's ,,,,, */
						RFID_Mem_BurstRead(g_mem_add, card_id_for_copy, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Read second card id */
						RFID_Mem_BurstWrite((g_mem_add - _RFID_CARD_ID_LENGTH), card_id_for_copy, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Rewrite card id */
						
					}
					
					g_mem_add = (((uint32_t)RFID_Control.RegisteredUserCard - 1) * _RFID_CARD_ID_LENGTH) + _USER_CARDID_START_ADD; /* Preparing memory address */
					
					RFID_Mem_Erase(g_mem_add, _RFID_CARD_ID_LENGTH, _RFID_MEMORY_TIME_OUT); /* Erase memory */
					
					/* ~~~ Save number of registered card ~~~ */
					RFID_Control.RegisteredUserCard--;
					
					RFID_Mem_SingleWrite(_REGISTERED_USER_CARD_ADD , (uint8_t)(RFID_Control.RegisteredUserCard >> _BYTE_SHIFT), _RFID_MEMORY_TIME_OUT); /* Save to memory */
					RFID_Mem_SingleWrite( (_REGISTERED_USER_CARD_ADD + 1) , (uint8_t)RFID_Control.RegisteredUserCard, _RFID_MEMORY_TIME_OUT); /* Save to memory */
					
					/* ~~~ Verify data ~~~ */
					#ifdef _RFID_VERIFY_IN_REMOVE
					
					status_check = RFID_VerifyCard2(card_id_for_verify, &verified_card_type, &verified_card_number);
					
					if (status_check == _RFID_CARD_NOT_REGISTERED)
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_CARD_REMOVED;
						
						RFID_Status.CardRemoved = _TRUE;
					}
					else
					{
						/* ~~~ Set status ~~~ */
						status_check = _RFID_STAT_ERROR;
						
						RFID_Status.CardRemoved = _FALSE;
					}
					
					#else
					
					/* ~~~ Set status ~~~ */
					status_check = _RFID_CARD_REMOVED;
					
					RFID_Status.CardRemoved = _TRUE;
					
					#endif /* _VERIFY_IN_SAVE */
					
				}
				else
				{
					status_check = _RFID_CARD_NOT_REGISTERED;
				}
				
			}break;
			
			default:
			{
				status_check = _RFID_CARD_TYPE_ERROR;
			}
			break;
		}
		
		/* ------------------------------------ */
		#ifdef _RFID_USING_EXTERNAL_MEMORY
	}
	else
	{
		status_check = _RFID_STAT_ERROR;
	}
	#endif /* _RFID_USING_EXTERNAL_MEMORY */
	/* ------------------------------------ */
	
	return status_check;
	
}
/*
	Guide   :
			Function description	This function is used to remove card from available cards in memory.
			
			Parameters
									* _card_id : ID for remove.
			Return Values
									* _RFID_CARD_REMOVED
									* _RFID_CARD_TYPE_ERROR
									* _RFID_CARD_NOT_REGISTERED
									
									* _STAT_ERROR (in AVR)
									* HAL_ERROR (in STM32 ARM)
			
	Example :
			RFID_MessageTypeDef   com_status;
			
			com_status = RFID_RemoveCard2("0010035314");
			
			if(com_status == _RFID_CARD_REMOVED)
			{
				Lcd_PutString("Your card removed!");
			}
			else if(com_status == _RFID_CARD_NOT_REGISTERED)
			{
				Lcd_PutString("This card not registered");
			}
			else if(com_status == _RFID_CARD_TYPE_ERROR)
			...
			...
			...
			
*/

