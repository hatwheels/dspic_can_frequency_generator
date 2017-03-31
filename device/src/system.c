/*  
	2014 - 2015

	@file system.c
	@brief Contains functions that are needed to configure the whole system/device.

	Macros for Configuration Fuse Registers (refer to compiler target header file for details: 
	p30f6014.h). These parameters need to be configured as global variable outside system_init()
 */

#include "freq_gen.h"
 
 /****************** Configure main registers of the device ************************/
/* Set up external crystal with 8x PLL, no switching to internal oscillator */
_FOSC(CSW_FSCM_OFF & XT_PLL8); 

/* Turn off the Watch-Dog Timer. */
_FWDT(WDT_OFF);                 

/* Enable MCLR reset pin and turn off the power-up timers. */
_FBORPOR(MCLR_EN & PWRT_OFF);

/* Disable Code Protection */
_FGS(CODE_PROT_OFF);
/**************************************************************************************/

// Initialize the system
void system_init(void)
{
	// Stop all systems for configuration
	disable_ISR();

	// Set the 16th Pin of port G as Output for SPI1 module
	TRISGbits.TRISG15 = 0;
	
	// Initialize general settings of the Interrupt Service Routine
	ISR_init();
	
	// Initialize the Special Peripheral Interface 1 module
	SPI1_init();
	
	// Initialize Change Notification Interrupt 
	IO_init();
	
	// Initialize CAN1 module
	CAN_init();
	
	// Enable all Interrupts
	enable_ISR();
	
	return;
}
