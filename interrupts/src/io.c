/*  
	2014 - 2015

	\file io.h
	\brief Contains functions for the I/O interface initialization
	(digital).
*/

#include "freq_gen.h"

// Initialize Change Notification Interrupt ( Pins CN<8:11> ->Digital Input pins Port G <6:9>)
// see excel file HEX_switch_pins.xlsx
void IO_init(void)
{
	/* Enable selected CN pins*/
	CNEN1 = 0xF00;
	
	/* Clear Change Notification Interrupt flag*/
	IFS0bits.CNIF = 0;

	/* Set Change Notification Interrupt priority, lower than CAN*/
	IPC3bits.CNIP = 5; // 2nd in priority
	
	return;
}

/*! Change Notification (CN) is a feature that, if enabled, provides us with an
	interrupt when the logic level on the pin changes.
	_CNInterrupt() is the Change Notification interrupt.
*/
void __attribute__((interrupt, auto_psv)) _CNInterrupt(void)
{
	// Change step size of channels according to new PORT G input values (new Hex Switch position) 
	GEN_update(&(OutputSignal.channel_A.freq_id), 
						&(OutputSignal.channel_B.freq_id),
						&(OutputSignal.status));

	// It is necessary to clear manually the interrupt flag for CN 
	IFS0bits.CNIF = 0;

	return;
}
