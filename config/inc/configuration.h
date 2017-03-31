/*  
	2014 - 2015

	\file configuration.h
	\brief Configure program (application dependent, processor independent)
*/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/* Define to use correct stypes-set */
#define DSPIC30F6014                1

/* Macros to enable/disable various debugging features */
#define DEBUG                       0
#if DEBUG
	#define TIME_DBG_CALC           0 // Measure time of Signal Calculation
	#define TIME_DBG_BANG           0 // Measure time of Bang-Bang Control
	#define TIME_DBG_WHOLE          0 // Measure time of the whole algorithm = Sig. Calc + B-B Control
#endif

#endif

