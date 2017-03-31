/* 
	2014 - 2015

	\file eeprom.c
	\brief Contains function implementations related to the EEPROM. This file
		only contains wrapping functions for the compiler specific library 
		functions
*/

#include "freq_gen.h"

/* Local variables */
static uint16_t Eeprom_read_data;

/* Read data stored in address and return it */
uint16_t eeprom_read_word(uint32_t    read_address)
{  
	_memcpy_p2d16(&Eeprom_read_data, read_address, _EE_WORD);

  return (Eeprom_read_data);
}

/* Write value to address and check if writing succeeded */
bool_t eeprom_write_word(uint32_t write_address, uint16_t word_to_write)
{
	// Counter for the erase-write loop
	uint16_t cnt = 0;

	/*
		The loop checks if the word to be written was successfully stored
		by reading back the content of the destination address.
	*/ 
	while (eeprom_read_word(write_address) != word_to_write)
	/*
		- 1st iteration: Check FIRST (to avoid writing the same word) if the stored 
		word is identical to the word to be stored. If not, THEN initiate
		erase-write procedure.
		- subsequent iterations: if the stored word is not identical to the word to 
		be stored, then rewrite the word to be stored to the destination address.
	*/
	{
		/* If the counter surpasses a threshold defined by VERIFY_WRITE_COUNT,
			then the writing procedure has failed too many times and the right
			word was not read. Return that the word to write could not be written
			to the destination address.
		*/
		if (++cnt > VERIFY_WRITE_COUNT)
		return FALSE;

		/* First, the content of the EEDATA address must be erased */
		_erase_eedata(write_address, _EE_WORD);

		/* Wait until erase operation is completed */
		_wait_eedata();

		/* Write Word to blank address */
		_write_eedata_word(write_address, word_to_write);

		/* Wait until write operation is completed */
		_wait_eedata();  
	}
  
	// Word to write was successfully read. Return that writing procedure succeeded
	return TRUE;
}

/* Test read/write procedure from/to EEPROM */
bool_t eeprom_test_read_write(void)
{
	uint16_t wait_read_counter = 0;
	uint16_t write_enable = 1;
	uint16_t read_enable = 0;
	uint32_t write_address = EEPROM_ADDRESS_START;
	uint32_t read_address = EEPROM_ADDRESS_START;
	uint16_t	Eeprom_test_data_read = 0;
	uint16_t	Eeprom_test_data_write = 0x5555;
	
	uint16_t cnt = 0;
	while(cnt < 6)
	{		
		if (write_enable)
		{
			eeprom_write_word(write_address, Eeprom_test_data_write);
			write_enable  = 0;
			wait_read_counter = 0;
			read_enable   = 1;
			read_address = write_address;
		}
		if (read_enable)
		{
			if (wait_read_counter > 5)
			{
				Eeprom_test_data_read = eeprom_read_word(read_address);
				read_enable = 0;
				write_enable = 1;
				Eeprom_test_data_write = 0x6666;
				write_address =   EEPROM_ADDRESS_END;
				++cnt;
			}
			else
				++wait_read_counter;
		}
		if (Eeprom_test_data_read == 0x6666)
			break;
	}
	// read/write procedure succeeded
	if (cnt < 6)
		return TRUE;
	// read/write procedure failed
	return FALSE;
}
