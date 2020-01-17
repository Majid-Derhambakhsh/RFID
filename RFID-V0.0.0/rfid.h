/*
------------------------------------------------------------------------------
~ File   : rfid.h
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

#ifndef __RFID_H_
#define __RFID_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <stdint.h>

#include "rfid_conf.h"

#ifndef __I2C_MEMORY_H_
	#include "I2C_MEMORY/i2c_memory.h"
#endif /* __I2C_MEMORY_H_ */

#ifndef _INTERNAL_EEPROM_H
	#include "Internal EEPROM/internal_eeprom.h"
#endif /* _INTERNAL_EEPROM_H */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifdef __CODEVISIONAVR__

	#define _USE_AVR_MCU

#elif defined(__GNUC__)

	#define _USE_AVR_MCU

#elif defined(USE_HAL_DRIVER)

	#define _USE_ARM_MCU

#endif /* __CODEVISIONAVR__ */

/* ------------- If user does not set card ID length -------------- */

#ifndef _RFID_CARD_ID_LENGTH
	#define _RFID_CARD_ID_LENGTH 10
#endif

/* ------------- If user does not set memory time out ------------- */

#ifndef _RFID_MEMORY_TIME_OUT
	#define _RFID_MEMORY_TIME_OUT 100
#endif

/* ------------- If user does not select memory type -------------- */

#ifndef _RFID_USING_INTERNAL_MEMORY
	
	#ifndef _RFID_USING_EXTERNAL_MEMORY
	
		#ifdef _USE_AVR_MCU /* AVR Microcontroller */
		
			#define _RFID_USING_INTERNAL_MEMORY
			
			#warning [RFID WARN01] --> "Your memory is set to Internal EEPROM"
		
		#elif defined(_USE_ARM_MCU) /* ARM Microcontroller */
		
			#define _RFID_USING_EXTERNAL_MEMORY
			
			#warning [RFID WARN01] --> "Your memory is set to External EEPROM"
		
		#else
		
			#error Driver or library is not supported in RFID Library
		
		#endif /* __CODEVISIONAVR__ */
	
	#endif /* _RFID_USING_EXTERNAL_MEMORY */
	
#endif

/* ------ If the user does not select correctly memory type ------- */

#ifdef _USE_AVR_MCU /* AVR Microcontroller */
	
	#ifdef _RFID_USING_INTERNAL_MEMORY
	
		#ifdef _RFID_USING_EXTERNAL_MEMORY
		
			#undef  _RFID_USING_EXTERNAL_MEMORY
			
			#warning [RFID WARN02] --> "Your memory is set to default (Internal EEPROM) because your defined both Internal EEPROM & External EEPROM"
		
		#endif /* _RFID_USING_EXTERNAL_MEMORY */
	
	#endif /* _RFID_USING_INTERNAL_MEMORY */
	
#elif defined(_USE_ARM_MCU) /* ARM Microcontroller */
	
	#ifdef _RFID_USING_INTERNAL_MEMORY
	
		#undef  _RFID_USING_INTERNAL_MEMORY
		#define _RFID_USING_EXTERNAL_MEMORY
		
		#warning [RFID WARN02] --> "Your selected memory is change to External EEPROM because this microcontroller not support Internal EEPROM"
	
	#endif /* _RFID_USING_INTERNAL_MEMORY */
	
#endif /* __CODEVISIONAVR__ */

/* ----------------------- Exported Macros ------------------------ */

#ifdef _RFID_USING_INTERNAL_MEMORY

	#define __RFID_Get_MemorySize _EEPROM_SIZE

#elif defined(_RFID_USING_EXTERNAL_MEMORY)

	#define __RFID_Get_MemorySize _MEM_MAX_SIZE

#endif /* _RFID_USING_INTERNAL_MEMORY */

#define __RFID_Get_NumberOfMainCard     RFID_Control.RegisteredMainCard
#define __RFID_Get_NumberOfMasterCard   RFID_Control.RegisteredMasterCard
#define __RFID_Get_NumberOfUserCard     RFID_Control.RegisteredUserCard

#define __RFID_Get_MainCard_UsedSpace   ((uint32_t)RFID_Control.RegisteredMainCard * _RFID_CARD_ID_LENGTH)
#define __RFID_Get_MasterCard_UsedSpace ((uint32_t)RFID_Control.RegisteredMasterCard * _RFID_CARD_ID_LENGTH)
#define __RFID_Get_UserCard_UsedSpace   ((uint32_t)RFID_Control.RegisteredUserCard * _RFID_CARD_ID_LENGTH)
#define __RFID_Get_UsedSpace            ((__RFID_Get_MainCard_UsedSpace) + (__RFID_Get_MasterCard_UsedSpace) + (__RFID_Get_UserCard_UsedSpace) + _RFID_CONFIGS_LENGTH)

#define __RFID_Get_AddressOfStartStoredData (RFID_Config.StartOfDataLocationAddress)
#define __RFID_Get_AddressOfEndStoredData   (_USER_CARDID_START_ADD + ((uint32_t)RFID_Config.NumberOfUserCard * _RFID_CARD_ID_LENGTH))

/* ---------------------------- Public ---------------------------- */

#ifndef _TRUE
	#define _TRUE 1
#endif

#ifndef _FALSE
	#define _FALSE 0
#endif

#ifndef _BYTE_SHIFT
	#define _BYTE_SHIFT 8
#endif

#define _RFID_MEM_NULL 255

#define _ASCII_ZERO '0' /* ASCII Code */
#define _ASCII_NINE '9' /* ASCII Code */

/* ------------------------- Data address ------------------------- */

#define _RFID_CONFIGS_LENGTH 10UL

#define _REGISTERED_MASTER_CARD_ADD (RFID_Config.StartOfDataLocationAddress + 2)
#define _REGISTERED_USER_CARD_ADD   (RFID_Config.StartOfDataLocationAddress + 4)

#define _MAIN_CARDID_START_ADD   (RFID_Config.StartOfDataLocationAddress + _RFID_CONFIGS_LENGTH)
#define _MASTER_CARDID_START_ADD (_MAIN_CARDID_START_ADD + ((uint32_t)RFID_Config.NumberOfMainCard * _RFID_CARD_ID_LENGTH))
#define _USER_CARDID_START_ADD   (_MASTER_CARDID_START_ADD + ((uint32_t)RFID_Config.NumberOfMasterCard * _RFID_CARD_ID_LENGTH))

/* ------------------- Define for data control -------------------- */

#ifdef _KartachKA11

	#ifdef _MangoRF01D
	
		#undef _MangoRF01D
		
		#warning [RFID WARN03] --> "Your RFID module is set to Kartach KA11 because your defined both Kartach KA11 & Mango RF01D module"
	
	#endif /* _MangoRF01D */
	
	#define RFID_DataControl(_input_data)  Kartach_KA11_DecimalBasic((_input_data))

#elif defined(_MangoRF01D)

	#define RFID_DataControl(_input_data)  Mango_RF01D_DecimalBasic((_input_data))

#else

	#define RFID_DataControl(_input_data)  RFID_DecimalBasic((_input_data))
	
	#warning [RFID WARN04] --> "Your RFID module data control is set to default (Decimal code control)"

#endif /* _KartachKA11 */

/* ------------------- Define for card control -------------------- */

#define _CARD_LAST_NMB (_RFID_CARD_ID_LENGTH - 1)

#define _NUMBER_IS_CONFLICT 0
#define _NUMBER_IS_CORRECT  1

#define _CARD_IS_CONFLICT   0
#define _CARD_IS_IDENTIFIED 1

/* ---------------------- Define by compiler ---------------------- */

#ifdef _RFID_USING_INTERNAL_MEMORY

	#define RFID_Mem_BurstWrite(_address,_udata,_quantity,_time_out)  EEPROM_BurstWrite((_address),(_udata),(_quantity),(_time_out))
	#define RFID_Mem_BurstRead(_address,_udata,_quantity,_time_out)   EEPROM_BurstRead((_address),(_udata),(_quantity),(_time_out))

	#define RFID_Mem_SingleWrite(_address,_udata,_time_out)           EEPROM_SingleWrite((_address),(_udata),(_time_out))
	#define RFID_Mem_SingleRead(_address,_udata,_time_out)            EEPROM_SingleRead((_address),(_udata),(_time_out))

	#define RFID_Mem_Erase(_address,_quantity,_time_out)              EEPROM_Erase((_address),(_quantity),(_time_out))

#elif defined (_RFID_USING_EXTERNAL_MEMORY)

	#define RFID_Mem_BurstWrite(_address,_udata,_quantity,_time_out)  I2C_Memory_BurstWrite((_address),(_udata),(_quantity),(_time_out))
	#define RFID_Mem_BurstRead(_address,_udata,_quantity,_time_out)   I2C_Memory_BurstRead((_address),(_udata),(_quantity),(_time_out))

	#define RFID_Mem_SingleWrite(_address,_udata,_time_out)           I2C_Memory_SingleWrite((_address),(_udata),(_time_out))
	#define RFID_Mem_SingleRead(_address,_udata,_time_out)            I2C_Memory_SingleRead((_address),(_udata),(_time_out))

	#define RFID_Mem_Erase(_address,_quantity,_time_out)              I2C_Memory_Erase((_address),(_quantity),(_time_out))

#endif /* _RFID_USING_INTERNAL_MEMORY */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef enum /* RFID control codes */
{
	
	_START_CODE      = 0x02, /* Start code */
	_STOP_CODE       = 0x03, /* Stop code */
	_RFID_ENTER_CODE = 0x0D, /* Enter 1 code */
	_RFID_END_CODE   = 0x0A, /* Enter 2 code */
	_ESCAPE_CODE     = 0x10, /* Escape code */
	_CONTROL3_CODE   = 0x13  /* Control code */
	
}InputCodes;

typedef enum /* RFID card types */
{
	
	_NOT_FOUND_CARD = 0, /* Not found card */
	_MAIN_CARD      = 1, /* Main card */
	_MASTER_CARD    = 2, /* Master card */
	_USER_CARD      = 3  /* User card */
	
}RFID_CardType_TypeDef;

typedef enum /* Status messages */
{
	_RFID_STAT_ERROR                    = _ERROR_VAL,
	_RFID_STAT_OK                       = _OK_VAL,
	_RFID_CARD_REGISTERED               = 3,
	_RFID_CARD_NOT_REGISTERED           = 4,
	_RFID_MEM_FULL                      = 5,
	_RFID_CARD_TYPE_ERROR               = 6,
	_RFID_CARD_READED                   = 7,
	_RFID_NMB_OF_REGISTERED_CARD_ERROR  = 8,
	_RFID_CARD_SAVED                    = 9,
	_RFID_CARD_REMOVED                  = 10
	
}RFID_MessageTypeDef;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef struct /* RFID option config structure */
{
	
	uint32_t StartOfDataLocationAddress;
	
	uint16_t NumberOfMainCard; /* Maximum number of card */
	uint16_t NumberOfMasterCard; /* Maximum number of card */
	uint16_t NumberOfUserCard; /* Maximum number of card */
	
}RFID_ConfigTypeDef;

typedef struct /* RFID status structure */
{
	
	volatile uint8_t CardSaved         : 1; /* This flag set when card is saved in memory */
	volatile uint8_t CardRemoved       : 1; /* This flag set when card is removed from memory */
	volatile uint8_t CardDetected      : 1; /* This flag set when card is detected */
	volatile uint8_t CardNotRegistered : 1; /* This flag set when card is not registered */
	
}RFID_StatusTypeDef;

typedef struct /* RFID Data control structure */
{
	
	volatile uint16_t RegisteredMainCard; /* Number of registered card */
	volatile uint16_t RegisteredMasterCard; /* Number of registered card */
	volatile uint16_t RegisteredUserCard; /* Number of registered card */
	
	volatile uint8_t CopyToSRAM; /* Data Counter for copy card ID */
	
}RFID_ControlTypeDef;

extern RFID_StatusTypeDef  RFID_Status;
extern RFID_ConfigTypeDef  RFID_Config;
extern RFID_ControlTypeDef RFID_Control;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef void (*RFID_InitTypeDef)(void);

typedef void (*RFID_ModuleName_Type)(uint8_t _data);

void RFID_DecimalBasic(uint8_t _usart_data);

void Mango_RF01D_DecimalBasic(uint8_t _usart_data);

void Kartach_KA11_DecimalBasic(uint8_t _usart_data);

void RFID_ConfigData_Load(void);
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

/* ---------------------------------- */

void RFID_Init(RFID_ConfigTypeDef *_settings);

void RFID_DefaultInit(void);

void RFID_FullInit(RFID_ConfigTypeDef *_settings);

RFID_MessageTypeDef RFID_GetCardID(uint8_t *_card_id);
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

RFID_MessageTypeDef RFID_VerifyCard(RFID_CardType_TypeDef *_card_type, uint16_t *_card_number);
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

RFID_MessageTypeDef RFID_VerifyCard2(uint8_t *_card_id, RFID_CardType_TypeDef *_card_type, uint16_t *_card_number);
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

RFID_MessageTypeDef RFID_SaveCard(RFID_CardType_TypeDef _card_type);
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

RFID_MessageTypeDef RFID_SaveCard2(uint8_t *_card_id, RFID_CardType_TypeDef _card_type);
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

RFID_MessageTypeDef RFID_ReadCard(uint8_t *_card_id, RFID_CardType_TypeDef _card_type, uint16_t _card_number);
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

RFID_MessageTypeDef RFID_RemoveCard(RFID_CardType_TypeDef _card_type, uint16_t _card_number);
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

RFID_MessageTypeDef RFID_RemoveCard2(uint8_t *_card_id);
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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif
