/*  
	2014 - 2015

	\file spi.c
	\brief Contains functions for the Serial Peripheral Interface initialization
*/

#include "freq_gen.h"


// Initialize the SPI1 registers
void SPI1_init(void)
{
	/* Clear SPI1 Interrupt Flag*/
	IFS0 = IFS0 & !(0x0100) ; // bit <8>      SPI1IF = 0
	/* Set SPI1 Interrupt priority, lower than Change Notification priority */
	IPC2bits.SPI1IP = 4; // 3rd in priority
	/* Configure SPI1 module */
	SPI1CON  = SPI1CON    | (0x0003) ;  // bit <0,1>  PPRE = 1:1
	SPI1CON  = SPI1CON    | (0x001C) ;  // bit <2:4>  SPRE = 1:1
	SPI1CON  = SPI1CON    | (0x0020) ;    // bit <5>    MSTEN = 1
	SPI1CON  = SPI1CON    | (0x0040) ;  // bit <6>    CKP = ?
	SPI1CON  = SPI1CON    | (0x0100) ;  // bit <8>    CKE = ?
	SPI1CON  = SPI1CON    | (0x0400) ;    // bit <10>   MODE16 = 1
	/* Clear receive overflow flag and enable the SPI */
	SPI1STAT = SPI1STAT & !(0x0040) ;     // bit <6>    SPIROV = 0
	SPI1STAT = SPI1STAT  | (0x8000) ;     // bit <15>   SPIEN = 1
	
	return;
}

/*! Special Peripheral Interface 1 (SPI1) module is useful for communicating
	with other peripheral or microcontroller devices.

	_SPI1Interrupt is the Special Peripheral Interface 1 interrupt that fires up
	when data has been successfully transferred to the SDO1 pin.
*/
void __attribute__((interrupt,auto_psv)) _SPI1Interrupt(void)
{
	// Clear SPI1 Interrupt Flag manually 
	IFS0bits.SPI1IF = 0;
}
