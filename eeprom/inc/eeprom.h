/* 
	2014 - 2015

	\file eeprom.h
	\brief Contains function declarations related to the EEPROM
*/

#ifndef __HAL_EEPROM_H
#define __HAL_EEPROM_H

#include "stypes.h"

/* Defines */
// Storage beginning 
#define EEPROM_ADDRESS_START							(0x7FFD00)
// Storage for Frequency parameters @index = ID_FREQ1 
#define EEPROM_ADDRESS_FREQ1_HZ					(0x7FFD02)
#define EEPROM_ADDRESS_FREQ1_COS				(0x7FFD04)
#define EEPROM_ADDRESS_FREQ1_SIN					(0x7FFD06)
#define EEPROM_ADDRESS_FREQ1_COS_PLUS		(0x7FFD08)
#define EEPROM_ADDRESS_FREQ1_SIN_PLUS		(0x7FFD0A)
#define EEPROM_ADDRESS_FREQ1_COS_MIN		(0x7FFD0C)
#define EEPROM_ADDRESS_FREQ1_SIN_MIN			(0x7FFD0E)
// Storage for Frequency parameters @index = ID_FREQ2
#define EEPROM_ADDRESS_FREQ2_HZ					(0x7FFD10)
#define EEPROM_ADDRESS_FREQ2_COS				(0x7FFD12)
#define EEPROM_ADDRESS_FREQ2_SIN					(0x7FFD14)
#define EEPROM_ADDRESS_FREQ2_COS_PLUS		(0x7FFD16)
#define EEPROM_ADDRESS_FREQ2_SIN_PLUS		(0x7FFD18)
#define EEPROM_ADDRESS_FREQ2_COS_MIN		(0x7FFD1A)
#define EEPROM_ADDRESS_FREQ2_SIN_MIN			(0x7FFD1C)
// Storage for Frequency parameters @index = ID_FREQ3
#define EEPROM_ADDRESS_FREQ3_HZ					(0x7FFD1E)
#define EEPROM_ADDRESS_FREQ3_COS				(0x7FFD20)
#define EEPROM_ADDRESS_FREQ3_SIN					(0x7FFD22)
#define EEPROM_ADDRESS_FREQ3_COS_PLUS		(0x7FFD24)
#define EEPROM_ADDRESS_FREQ3_SIN_PLUS		(0x7FFD26)
#define EEPROM_ADDRESS_FREQ3_COS_MIN		(0x7FFD28)
#define EEPROM_ADDRESS_FREQ3_SIN_MIN			(0x7FFD2A)
// Storage for Frequency parameters @index = ID_FREQ4
#define EEPROM_ADDRESS_FREQ4_HZ					(0x7FFD2C)
#define EEPROM_ADDRESS_FREQ4_COS				(0x7FFD2E)
#define EEPROM_ADDRESS_FREQ4_SIN					(0x7FFD30)
#define EEPROM_ADDRESS_FREQ4_COS_PLUS		(0x7FFD32)
#define EEPROM_ADDRESS_FREQ4_SIN_PLUS		(0x7FFD34)
#define EEPROM_ADDRESS_FREQ4_COS_MIN		(0x7FFD36)
#define EEPROM_ADDRESS_FREQ4_SIN_MIN			(0x7FFD38)
/* 	Storage defining which freq. index is in user mode (stored) 
	or in default mode (not stored)
*/  
#define EEPROM_ADDRESS_DATA_WRITTEN			(0x7FFD3A)
// Storage end
#define EEPROM_ADDRESS_END							(0x7FFD3C)
// Threshold for the erase-write procedure 
#define  VERIFY_WRITE_COUNT                 				(3)
// Invalid EEPROM data
#define EEPROM_INVALID_DATA								(0xFFFF)

/* Function declarations */
uint16_t eeprom_read_word(uint32_t read_address);
bool_t eeprom_write_word(uint32_t write_address, uint16_t word_to_write);
bool_t eeprom_test_read_write(void);

#endif
