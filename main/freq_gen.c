// 2014 - 2015
/*
	\file freq_gen.c
	\brief Contains the main function for Frequency Generator

	The goal of the Frequency Generator is to generate an output signal
	containing the base signal from a set frequency and a fraction of its
	2nd Harmonic.
	The output signal is transferred to 2 channels, Channel A and B.
	The frequency is set with a hex switch. Depending on the position 
	of the hex switch the frequencies in the channels are modified
	accordingly.
*/

#include "freq_gen.h"

//*****************************************************************************
// Debugging Time Variables
//*****************************************************************************
#if TIME_DBG_CALC
clock_t t_calc;
clock_t t_calc_sum;
uint16_t i = 0;
#endif
#if TIME_DBG_BANG
clock_t t_bang;
clock_t t_bang_sum;
uint16_t i = 0;
#endif
#if TIME_DBG_WHOLE
clock_t t_whole;
uint16_t i = 0;
#endif

//*****************************************************************************
// Global Variables
//*****************************************************************************
T_signal OutputSignal;
T_params_step corr_step[NBR_FREQ];
T_can_data_t CAN_data;

//*****************************************************************************
// MAIN function
//*****************************************************************************
int main()
{
	/***************** Initialize timer of the Algorithm ****************/
	/****/	#if TIME_DBG_WHOLE
	/****/	t_whole = clock();
	/****/	#endif
	/**/
	/****/	#if TIME_DBG_CALC
	/****/	t_calc_sum = 0;
	/****/	#endif
	/**/
	/****/	#if TIME_DBG_BANG
	/****/	t_bang_sum = 0;
	/****/	#endif
	/*********************************************************************/

	//************************************************************************************************************************************
	// Initialize hardware configuration for this device
	//************************************************************************************************************************************
	system_init();
	
	//************************************************************************************************************************************
	// Initialize Parameters
	//************************************************************************************************************************************
	GEN_params_init(corr_step);

	//************************************************************************************************************************************
	// Initialize Output Signal
	//************************************************************************************************************************************
	GEN_init(&OutputSignal);
	
	/* Start endless output signal calculation and transmission loop */
	do
	{
		/********* Initialize Output Signal calculation timer **********/
		/****/	#if TIME_DBG_CALC
		/****/	t_calc = clock();
		/****/	#endif
		/******************************************************************/

		
		//*********************************************************************************************************************************
		// Check channel status for  before each output signal computation
		//*********************************************************************************************************************************
		if (OutputSignal.status == UPDATE_TODO)
			// Change Notification ISR has been triggered
			GEN_reinit(&OutputSignal);
			
		//*********************************************************************************************************************************
		// Base & 2nd Harmonic signal calculation
		//*********************************************************************************************************************************
		// Channel A
		//*********************************************************************************************************************************
		GEN_compute(&(OutputSignal.channel_A.output), // pointer to output variables struct
								 &(OutputSignal.channel_A.step), // pointer to step variables struct
											 CHANNEL_A_OFFSET); // channel SPI1 ID + amplitude offset
		
		//*********************************************************************************************************************************
		/* Channel B */
		//*********************************************************************************************************************************
		GEN_compute(&(OutputSignal.channel_B.output), // pointer to output variables struct
								 &(OutputSignal.channel_B.step), // pointer to step variables struct
											 CHANNEL_B_OFFSET); // channel SPI1 ID + amplitude offset
		
		
		/*** End Output Signal timer, add time to summed timer **/
		/****/	#if TIME_DBG_CALC
		/****/	t_calc = clock() - t_calc;
		/****/	t_calc_sum += t_calc;
		/****/	#endif
		/************* Initialize Bang-Bang control timer *************/
		/****/	#if TIME_DBG_BANG
		/****/	t_bang = clock();
		/****/	#endif
		/******************************************************************/
		
		
		//*********************************************************************************************************************************
		// Compensate amplitude variation with bang-bang control
		//*********************************************************************************************************************************
		/* Channel A */
		//*********************************************************************************************************************************
		GEN_BangBang(&(OutputSignal.channel_A.output), // pointer to output variables struct
								&(OutputSignal.channel_A.step), // pointer to step variables struct
								&(OutputSignal.channel_A.amplitude), // pointer to amplitude
								&corr_step[OutputSignal.channel_A.freq_id]); // pointer to selected freq.'s corrected step values
		//*********************************************************************************************************************************
		/* Channel B */
		//*********************************************************************************************************************************
		GEN_BangBang(&(OutputSignal.channel_B.output), // pointer to output variables struct
								&(OutputSignal.channel_B.step), // pointer to step variables struct
								&(OutputSignal.channel_B.amplitude), // pointer to amplitude
								&corr_step[OutputSignal.channel_B.freq_id]); // pointer to selected freq.'s corrected step values


		/* End Bang-Bang control timer, add time to summed timer */
		/****/	#if TIME_DBG_BANG
		/****/	t_bang = clock() - t_bang;
		/****/	t_bang_sum += t_bang;
		/****/	#endif
		/*************** End loop @ uint16_t i == 2^16-1 ***************/
		/****/	#if TIME_DBG_WHOLE
		/****/	++i;
		/****/	if (i == 65535)
		/****/		break;
		/****/	#endif
		/*************** End loop @ uint16_t i == 2^16-1 ***************/
		/****/	#if TIME_DBG_CALC
		/****/	++i;
		/****/	if (i == 65535)
		/****/		break;
		/****/	#endif
		/*************** End loop @ uint16_t i == 2^16-1 ***************/
		/****/	#if TIME_DBG_BANG
		/****/	++i;
		/****/ if (i == 65535)
		/****/	break;
		/****/	#endif
		/*********************************************************************/
	} while (1);
		
	/*********************** End timer of the Algorithm *****************/
	/****/	#if TIME_DBG_WHOLE
	/****/	t_whole = clock() - t_whole;
	/****/	#endif
	/************************************************************************/

	return 0;
}

