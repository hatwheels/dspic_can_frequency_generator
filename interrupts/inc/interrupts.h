/*  
    2014 - 2015

    \file  interrupts.h
    \brief Contains function declarations and variable declarations linked to interrupts. 
    
    The interrupts sub-module does not contain the implementation of the different interrupts, but
    they are implemented at the end of the module they belong to.
*/

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

/* Function declarations */
void disable_ISR(void);
void ISR_init(void);
void enable_ISR(void);

#endif
