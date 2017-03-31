/*  
	2014 - 2015

	\file signal_calc.h
	\brief All parameters relevant to the Output Signal calculation
*/

#ifndef SIGNAL_CALC_H
#define SIGNAL_CALC_H

#include "stypes.h"

/* Defines */

// Number of generated frequencies
#define NBR_FREQ				(4)

/* Frequencies, used in default (MODE_DEFAULT)
	as parameters (T_params_step)
*/
#if (NBR_FREQ == 4)
	#define HZ_FREQ1			(2790)
	#define HZ_FREQ2			(3209)
	#define HZ_FREQ3			(3627)
	#define HZ_FREQ4			(4046)
#else
	#error "Wrong number of Frequencies! Check NBR_FREQ macro, it should be 4."
#endif

// Index for the parameters (T_params_step)
#define ID_FREQ1				(0)
#define ID_FREQ2				(1)
#define ID_FREQ3				(2)
#define ID_FREQ4				(3)

/* Frequencies 
	Corrected Step Cosine/Sine values +/- 0.1%
	COS = 2^14 * cos(step angle)
	SIN = 2^14 * sin(step_angle)	
*/
// 1st Frequency 2790 Hz
#define COSTICKMIN_1		15974	// 12,85°
#define SINTICKMIN_1		3644	// 12,85°
#define COSTICKPLUS_1		15990	// 12,85° +0.1%
#define SINTICKPLUS_1		3648	// 12,85° +0.1%
#define COSTICKMIN_1		15958	// 12,85° -0.1%
#define SINTICKMIN_1		3640	// 12,85° -0.1%
// 2nd Frequency 3209 Hz
#define COSTICKMIN_2		15842	// 14.78°
#define SINTICKMIN_2		4180	// 14.78°
#define COSTICKPLUS_2		15858	// 14.78° +0.1%
#define SINTICKPLUS_2		4184	// 14.78° +0.1%
#define COSTICKMIN_2		15826	// 14.78° -0.1%
#define SINTICKMIN_2		4176	// 14.78° -0.1%
// 3rd Frequency 3627 Hz
#define COSTICKMIN_3		15692	// 16,71°
#define SINTICKMIN_3		4711	// 16,71°
#define COSTICKPLUS_3		15708	// 16,71° +0.1%
#define SINTICKPLUS_3		4716	// 16,71° +0.1%
#define COSTICKMIN_3		15676	// 16,71° -0.1%
#define SINTICKMIN_3		4706	// 16,71° -0.1%
// 4th Frequency 4046 Hz
#define COSTICKMIN_4 		15525	// 18,63°
#define SINTICK_4			5237	// 18,63°
#define COSTICKPLUS_4		15540	// 18,63° +0.1%
#define SINTICKPLUS_4		5242	// 18,63° +0.1%
#define COSTICKMIN_4		15509	// 18,63° -0.1%
#define SINTICKMIN_4		5231	// 18,63° -0.1%

/* min/max square amplitude values +/- 0.1% */
#define AMPLITUDE_MAX	0x1010 	// 4112
#define AMPLITUDE_MIN	0x0FF0 	// 4080

/* Channel parameters */
#define OFFSET	2000	// 0x07D0
#define CHANNEL_A	20480 // 0x5000
#define CHANNEL_B	36864 // 0x9000
#define CHANNEL_A_OFFSET (CHANNEL_A + OFFSET)
#define CHANNEL_B_OFFSET (CHANNEL_B + OFFSET)

/* Typedefs */

/* Output Signal variables */
// Frequency modification status of the channels
typedef enum
{
	UPDATE_OK = 0,
	UPDATE_TODO,
}E_status;

// Output variables
typedef struct
{
	int16_t cosine;
	int16_t sine;
	int16_t cosine2; // = cos(2a)/2 half 2nd Harmonic cosine
	int16_t buffer; // temporary new sine value
}T_signal_output;

// Step variables
typedef struct
{
	int16_t cosine;
	int16_t sine;
}T_signal_step;

// Output of a single channel
typedef struct
{
	T_signal_output output;
	T_signal_step 	step;
	int16_t				amplitude; // amplitude^2
	uint16_t 			freq_id; // index of the currently used 
	// parameters (freq./corrected step)
}T_signal_channel;

// Output Signal 
typedef struct
{
	T_signal_channel	channel_A;
	T_signal_channel	channel_B;
	E_status 				status;
}T_signal;

/* Parameter variables */
// Frequency mode: default freq. or user freq.
typedef enum
{
	MODE_DEFAULT = 0,
	MODE_USER
}E_mode;

// Corrected step of a single frequency
typedef struct
{
	uint16_t 	freq_hz; // frequency value (in Hz)
	int16_t 		cosine;
	int16_t 		sine;
	int16_t 		cosine_plus;
	int16_t 		sine_plus;
	int16_t 		cosine_min;
	int16_t 		sine_min;
	E_mode 	mode; // default -> default freq. or user freq. through CAN ISR
}T_params_step;

/* Function declarations */
// Global functions
void GEN_init(T_signal *signal);
void GEN_params_init(T_params_step *corr);
void GEN_reinit(T_signal *signal);
void GEN_compute(T_signal_output *output, T_signal_step *step, const uint16_t ch_spi_id);
void GEN_BangBang(T_signal_output *output, T_signal_step *step, int16_t *amplitude, T_params_step *corr_ch);
// Function for the CN ISR (hex switch)
void GEN_update(uint16_t *id_A, uint16_t *id_B, E_status *status);
// Function for the CAN ISR (default/user parameters)
void GEN_params_update(uint8_t *content, T_params_step *corr, E_status *status);

// Global variables, extern to be visible
extern T_signal OutputSignal;
extern T_params_step corr_step[NBR_FREQ];

#endif
