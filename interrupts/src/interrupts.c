/*
    2014 - 2015

    \file  interrupts.c
    \brief Contains functions that are needed to configure the interrupts. 
    The interrupts sub-module does not contain the implementation of the different interrupts, but
    they are implemented at the end of the module they belong to.
*/

#include "freq_gen.h"

// Disable all Interrupts @ device start
void disable_ISR(void)
{
    /* CAN (reset all CAN interrupts) */
    C1INTF = 0;

    return;
}

 // Initialize General Interrupt Service Routine Registers
void ISR_init(void)
{
    /* Interrupt control register 1: Do not allow nested interrupts (bit 15)*/
    INTCON1bits.NSTDIS = 1;

    /* Interrupt control register 2: Use standard vector table */
    INTCON2bits.ALTIVT = 0;

    /* Status register (In CPU): CPU interrupt level is 3 (bit 7-5), enable CPU priority levels 4-7. 
       If this value is 0 it is disabled, linked with CORCONbits.IPL3 */
    SRbits.IPL         = 1;

    /* Core control register: CPU interrupt priority level <7 (bit 3), linked with SRbits.IPL */
    CORCONbits.IPL3    = 0;

  return;
}

void enable_ISR(void)
{
    // Enable CAN interrupt (bit 11)
    IEC1bits.C1IE  = 1;

    /* Enable CN interrupts */
    IEC0bits.CNIE = 1;

    /* Enable the SPI1 Interrupt (bit 8) */
    IEC0bits.SPI1IE = 1;

    return;
}
