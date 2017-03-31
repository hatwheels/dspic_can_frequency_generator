/*  
	2014 - 2015

	\file signal_calc.c
	\brief Contains functions relevant for the output signal calculation
*/

#include "freq_gen.h"

//*********************************************************************************************
// Static functions
//*********************************************************************************************
// Initialize Base signal and its 2nd Harmonic
static void GEN_output_init(T_signal_output *output)
{
	output->cosine = 16384; // 1 << 14
	output->sine = 0;
	output->cosine2 = 0;
	output->buffer = 0;

	return;
}
	
// Initialize step variables
static void GEN_step_init(T_signal_step *step)
{
	step->cosine = 16362; // 3°
	step->sine = 857; // 3°
	
	return;
}

// Channel Initialization
static void GEN_channel_init(T_signal_channel *channel)
{
	GEN_output_init(&(channel->output));
	// Initialize amplitude
	channel->amplitude = 0;
	GEN_step_init(&(channel->step));

	return;
}

//*********************************************************************************************
// Global functions
//*********************************************************************************************
// Initialize Output Signal
void GEN_init(T_signal *signal)
{
	/* Memset assumed ok at initialization time */
	memset((void*)signal,0,sizeof(T_signal));	

	// Initialize each channel
	GEN_channel_init(&(signal->channel_A));
	GEN_channel_init(&(signal->channel_B));
	// Read ID of Hex Switch position for channel step size -> frequency
	signal->channel_A.freq_id = !(PORTGbits.RG7) + ((!PORTGbits.RG6) << 1);
	signal->channel_B.freq_id = !(PORTGbits.RG9) + ((!PORTGbits.RG8) << 1);
	// set status of the channels OK
	signal->status = UPDATE_OK;
	
	return;
}

// Initialize parameters
void GEN_params_init(T_params_step *corr)
{
	/* Memset assumed ok at initialization time */
	memset((void*)corr,0,sizeof(T_params_step));

	/* EEPROM erase-write procedure failed */
	if (eeprom_test_read_write() == FALSE)
	// Use default parameters
	{
		corr[ID_FREQ1].freq_hz = HZ_FREQ1;
		corr[ID_FREQ1].cosine = COSTICK_1;
		corr[ID_FREQ1].sine = SINTICK_1;
		corr[ID_FREQ1].cosine_plus = COSTICKPLUS_1;
		corr[ID_FREQ1].sine_plus = SINTICKPLUS_1;
		corr[ID_FREQ1].cosine_min = COSTICKMIN_1;
		corr[ID_FREQ1].sine_min = SINTICKMIN_1;
		corr[ID_FREQ1].mode = MODE_DEFAULT;
		
		corr[ID_FREQ2].freq_hz = HZ_FREQ2;
		corr[ID_FREQ2].cosine = COSTICK_2;
		corr[ID_FREQ2].sine = SINTICK_2;
		corr[ID_FREQ2].cosine_plus = COSTICKPLUS_2;
		corr[ID_FREQ2].sine_plus = SINTICKPLUS_2;
		corr[ID_FREQ2].cosine_min = COSTICKMIN_2;
		corr[ID_FREQ2].sine_min = SINTICKMIN_2;
		corr[ID_FREQ2].mode = MODE_DEFAULT;
		
		corr[ID_FREQ3].freq_hz = HZ_FREQ3;
		corr[ID_FREQ3].cosine = COSTICK_3;
		corr[ID_FREQ3].sine = SINTICK_3;
		corr[ID_FREQ3].cosine_plus = COSTICKPLUS_3;
		corr[ID_FREQ3].sine_plus = SINTICKPLUS_3;
		corr[ID_FREQ3].cosine_min = COSTICKMIN_3;
		corr[ID_FREQ3].sine_min = SINTICKMIN_3;
		corr[ID_FREQ3].mode = MODE_DEFAULT;
		
		corr[ID_FREQ4].freq_hz = HZ_FREQ4;
		corr[ID_FREQ4].cosine = COSTICK_4;
		corr[ID_FREQ4].sine = SINTICK_4;
		corr[ID_FREQ4].cosine_plus = COSTICKPLUS_4;
		corr[ID_FREQ4].sine_plus = SINTICKPLUS_4;
		corr[ID_FREQ4].cosine_min = COSTICKMIN_4;
		corr[ID_FREQ4].sine_min = SINTICKMIN_4;
		corr[ID_FREQ4].mode = MODE_DEFAULT;
		
		//////MAKE IT KNOWN TO USER, PROLLY THROUGH CAN//////
		//////////SO USER WILL KNOW IF HE CAN CONFIGURE///////////
		////////////THE FREQ GEN TO USE HIS FREQUENCIES////////////
		CAN_transmit_error(&CAN_data, CAN_data.can_tx_msg_buffer.content);
	}
	
	uint16_t overall_mode;
	overall_mode = eeprom_read_word(EEPROM_ADDRESS_DATA_WRITTEN);

	/* Default mode */
	if (overall_mode == EEPROM_INVALID_DATA)
	// No parameters stored in EEPROM -> All default frequencies, default mode
	{
		corr[ID_FREQ1].freq_hz = HZ_FREQ1;
		corr[ID_FREQ1].cosine = COSTICK_1;
		corr[ID_FREQ1].sine = SINTICK_1;
		corr[ID_FREQ1].cosine_plus = COSTICKPLUS_1;
		corr[ID_FREQ1].sine_plus = SINTICKPLUS_1;
		corr[ID_FREQ1].cosine_min = COSTICKMIN_1;
		corr[ID_FREQ1].sine_min = SINTICKMIN_1;
		corr[ID_FREQ1].mode = MODE_DEFAULT;
		
		corr[ID_FREQ2].freq_hz = HZ_FREQ2;
		corr[ID_FREQ2].cosine = COSTICK_2;
		corr[ID_FREQ2].sine = SINTICK_2;
		corr[ID_FREQ2].cosine_plus = COSTICKPLUS_2;
		corr[ID_FREQ2].sine_plus = SINTICKPLUS_2;
		corr[ID_FREQ2].cosine_min = COSTICKMIN_2;
		corr[ID_FREQ2].sine_min = SINTICKMIN_2;
		corr[ID_FREQ2].mode = MODE_DEFAULT;
		
		corr[ID_FREQ3].freq_hz = HZ_FREQ3;
		corr[ID_FREQ3].cosine = COSTICK_3;
		corr[ID_FREQ3].sine = SINTICK_3;
		corr[ID_FREQ3].cosine_plus = COSTICKPLUS_3;
		corr[ID_FREQ3].sine_plus = SINTICKPLUS_3;
		corr[ID_FREQ3].cosine_min = COSTICKMIN_3;
		corr[ID_FREQ3].sine_min = SINTICKMIN_3;
		corr[ID_FREQ3].mode = MODE_DEFAULT;
		
		corr[ID_FREQ4].freq_hz = HZ_FREQ4;
		corr[ID_FREQ4].cosine = COSTICK_4;
		corr[ID_FREQ4].sine = SINTICK_4;
		corr[ID_FREQ4].cosine_plus = COSTICKPLUS_4;
		corr[ID_FREQ4].sine_plus = SINTICKPLUS_4;
		corr[ID_FREQ4].cosine_min = COSTICKMIN_4;
		corr[ID_FREQ4].sine_min = SINTICKMIN_4;
		corr[ID_FREQ4].mode = MODE_DEFAULT;
	}
	/* User modes */
	else
	// There are parameters are stored in EEPROM
	{
		// Get mode (default/user) for each frequency
		corr[ID_FREQ1].mode = overall_mode & 0x1;
		corr[ID_FREQ2].mode = (overall_mode & 0x2) >> 1;
		corr[ID_FREQ3].mode = (overall_mode & 0x4) >> 2;
		corr[ID_FREQ4].mode = (overall_mode & 0x8) >> 3;
		
		switch (corr[ID_FREQ1].mode)
		{
			case MODE_USER: // @1st freq. index, user frequency is to be used
				corr[ID_FREQ1].freq_hz = eeprom_read_word(EEPROM_ADDRESS_FREQ1_HZ);
				corr[ID_FREQ1].cosine = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ1_COS);
				corr[ID_FREQ1].sine = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ1_SIN);
				corr[ID_FREQ1].cosine_plus = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ1_COS_PLUS);
				corr[ID_FREQ1].sine_plus = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ1_SIN_PLUS);
				corr[ID_FREQ1].cosine_min = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ1_COS_MIN);
				corr[ID_FREQ1].sine_min = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ1_SIN_MIN);
				break;
			
			case MODE_DEFAULT: // Default frequency
				corr[ID_FREQ1].freq_hz = HZ_FREQ1;
				corr[ID_FREQ1].cosine = COSTICK_1;
				corr[ID_FREQ1].sine = SINTICK_1;
				corr[ID_FREQ1].cosine_plus = COSTICKPLUS_1;
				corr[ID_FREQ1].sine_plus = SINTICKPLUS_1;
				corr[ID_FREQ1].cosine_min = COSTICKMIN_1;
				corr[ID_FREQ1].sine_min = SINTICKMIN_1;
				break;
			
			default:
				break;
		}
		switch (corr[ID_FREQ2].mode)
		{
			case MODE_USER: // @2nd freq. index, user frequency is to be used
				corr[ID_FREQ2].freq_hz = eeprom_read_word(EEPROM_ADDRESS_FREQ2_HZ);
				corr[ID_FREQ2].cosine = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ2_COS);
				corr[ID_FREQ2].sine = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ2_SIN);
				corr[ID_FREQ2].cosine_plus = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ2_COS_PLUS);
				corr[ID_FREQ2].sine_plus = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ2_SIN_PLUS);
				corr[ID_FREQ2].cosine_min = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ2_COS_MIN);
				corr[ID_FREQ2].sine_min = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ2_SIN_MIN);
				break;
			
			case MODE_DEFAULT: // Default frequency
				corr[ID_FREQ2].freq_hz = HZ_FREQ2;
				corr[ID_FREQ2].cosine = COSTICK_2;
				corr[ID_FREQ2].sine = SINTICK_2;
				corr[ID_FREQ2].cosine_plus = COSTICKPLUS_2;
				corr[ID_FREQ2].sine_plus = SINTICKPLUS_2;
				corr[ID_FREQ2].cosine_min = COSTICKMIN_2;
				corr[ID_FREQ2].sine_min = SINTICKMIN_2;
				break;
			
			default:
				break;
		}
		switch (corr[ID_FREQ3].mode)
		{
			case MODE_USER: // @3rd freq. index, user frequency is to be used
				corr[ID_FREQ3].freq_hz = eeprom_read_word(EEPROM_ADDRESS_FREQ3_HZ);
				corr[ID_FREQ3].cosine = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ3_COS);
				corr[ID_FREQ3].sine = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ3_SIN);
				corr[ID_FREQ3].cosine_plus = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ3_COS_PLUS);
				corr[ID_FREQ3].sine_plus = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ3_SIN_PLUS);
				corr[ID_FREQ3].cosine_min = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ3_COS_MIN);
				corr[ID_FREQ3].sine_min = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ3_SIN_MIN);
				break;
			
			case MODE_DEFAULT: // Default frequency
				corr[ID_FREQ3].freq_hz = HZ_FREQ3;
				corr[ID_FREQ3].cosine = COSTICK_3;
				corr[ID_FREQ3].sine = SINTICK_3;
				corr[ID_FREQ3].cosine_plus = COSTICKPLUS_3;
				corr[ID_FREQ3].sine_plus = SINTICKPLUS_3;
				corr[ID_FREQ3].cosine_min = COSTICKMIN_3;
				corr[ID_FREQ3].sine_min = SINTICKMIN_3;
				break;
				
			default:
				break;
		}
		switch (corr[ID_FREQ4].mode)
		{
			case MODE_USER: // @4th freq. index, user frequency is to be used
				corr[ID_FREQ4].freq_hz = eeprom_read_word(EEPROM_ADDRESS_FREQ4_HZ);
				corr[ID_FREQ4].cosine = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ4_COS);
				corr[ID_FREQ4].sine = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ4_SIN);
				corr[ID_FREQ4].cosine_plus = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ4_COS_PLUS);
				corr[ID_FREQ4].sine_plus = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ4_SIN_PLUS);
				corr[ID_FREQ4].cosine_min = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ4_COS_MIN);
				corr[ID_FREQ4].sine_min = (int16_t)eeprom_read_word(EEPROM_ADDRESS_FREQ4_SIN_MIN);
				break;
			
			case MODE_DEFAULT: // Default frequency
				corr[ID_FREQ4].freq_hz = HZ_FREQ4;
				corr[ID_FREQ4].cosine = COSTICK_4;
				corr[ID_FREQ4].sine = SINTICK_4;
				corr[ID_FREQ4].cosine_plus = COSTICKPLUS_4;
				corr[ID_FREQ4].sine_plus = SINTICKPLUS_4;
				corr[ID_FREQ4].cosine_min = COSTICKMIN_4;
				corr[ID_FREQ4].sine_min = SINTICKMIN_4;
				break;
				
			default:
				break;
		}
	}
	
	return;
}

// Re-initialization of channel variables if frequency has changed (-> new step)
void GEN_reinit(T_signal *signal)
{	
	GEN_channel_init(&(signal->channel_A));
	GEN_channel_init(&(signal->channel_B));
	
	// update done
	signal->status = UPDATE_OK;
	
	return;
}

// Signal calculation per channel
void GEN_compute(T_signal_output *output, T_signal_step *step, const uint16_t ch_id_offset)
{
	/* Base signal calculation */
	//sin(a+d) = sin(a)cos(d)+cos(a)sin(d) 
	//cos(a+d) = cos(a)cos(d)-sin(a)sin(d)
	output->buffer = (int16_t) (((int32_t)output->sine * step->cosine +
						(int32_t)output->cosine * step->sine) >> 14);
	output->cosine = (int16_t) (((int32_t)output->cosine * step->cosine -
						(int32_t)output->sine * step->sine) >> 14);
	output->sine = output->buffer;	

	LATGbits.LATG15 = 1;
	
	/* 2nd Harmonic signal calculation */
	//cos(2a) = 2cos(a)² - 1
	//cos(2a)/2 = cos(a)² - 0.5
	//output.cosine2 = cos(2a)/2
	output->cosine2 = ((int16_t) (((int32_t)output->cosine * output->cosine) >> 14)) - 8192;

	LATGbits.LATG15 = 0;

	/* Output Signal channel a/b*/
	//Sum Signal = constant + (Base Sine + 2nd Harm Cosine * 0.5) / 16
	//mySine = OFFSET + (uint16_t)(Sum Signal >> 4). OFFSET = 2000
	//SPI1BUF = channel SPI ID + mySine = 0x57D0/0x97D0 + (uint16_t)((output.sine+output.cosine2)>>4);
	//0x5xxx: Enable DAQ and select channel a; 22480d = 0x57D0; between about 500 and 3500 mV
	// 0x9xxx: Enable DAQ and select channel b; 38864d = 0x97D0; between about 500 and 3500 mV
	SPI1BUF = ch_id_offset + (uint16_t)((output->sine + 3 * output->cosine2 / 10) >> 4);

	return;
}

// Bang-Bang Control per channel
void GEN_BangBang (T_signal_output *output, T_signal_step *step,
								int16_t *amplitude,   T_params_step *corr_ch)
{
	/* Calculate square amplitude */
	// amplitude² = cos(a)² + sin(a)²
	*amplitude = (int16_t)(((int32_t)output->cosine * output->cosine + 
					(int32_t)output->sine * output->sine) >> 16);
		
	/* Reference square Amplitude 16384² >> 16 = 4096 -> 0x1000
		square Amplitude error = ref. sqrt ampl. - myampl */
	// Correction of about 0.1% if amplitude error > 1/256: >> 8
	if (*amplitude > AMPLITUDE_MAX)
	// AMPLITUDE_MAX = ref. sqrt. ampl. + (ref. sqrt ampl. >> 8) (=4096+16=4112=0x1010)
	{
		step->cosine = corr_ch->cosine_min;
		step->sine		= corr_ch->sine_min;
	}
	else if (*amplitude < AMPLITUDE_MIN)
	// AMPLITUDE_MIN = ref. sqrt. ampl. - (ref. sqrt ampl. >> 8) (=4096-16=4080=0x0FF0)
	{
		step->cosine = corr_ch->cosine_plus;
		step->sine		= corr_ch->sine_plus;
	}
	else
	{
		step->cosine	= corr_ch->cosine;
		step->sine		= corr_ch->sine;
	}

	return;
}

//*********************************************************************************************
// Function for the CN ISR (hex switch)
//*********************************************************************************************
// Change frequency/step size of the channels 
void GEN_update(uint16_t *id_A, uint16_t *id_B, E_status *status)
{
	// New frequency indices values for the step parameter array
	*id_A = !(PORTGbits.RG7) + ((!PORTGbits.RG6) << 1);
	*id_B = !(PORTGbits.RG9) + ((!PORTGbits.RG8) << 1);
	
	// change of frequency/step size -> signal variables to be re-initialized
	*status = UPDATE_TODO;

	return;
}

//*********************************************************************************************
// Function for the CAN ISR (default/user parameters)
//*********************************************************************************************
// Update parameters with new frequencies/step sizes -> user configuration 
void GEN_params_update(uint8_t *content, T_params_step *corr, E_status *status)
{
	/* EEPROM erase-write procedure failed */
	if (eeprom_test_read_write() == FALSE)
	{
		/////SEND ERROR CAN MSG/////
		CAN_transmit_error(&CAN_data, CAN_data.can_tx_msg_buffer.content);
	}

	/*	Read new step parameters or reset to default ones for each frequency index.
		Write the new parameters or delete the stored to/from EEPROM accordingly.  
	*/
	// ID FREQ 1
	if ((content[0] & content[1]) == 0xFF)
	// Reset frequency value to default
	{
		// check if parameters of Freq. 1 index are in user mode
		// if in default mode, no need to reset
		if (corr[ID_FREQ1].mode != MODE_DEFAULT)
		{
			// Reset to default (Freq, mode)
			corr[ID_FREQ1].freq_hz = HZ_FREQ1;
			corr[ID_FREQ1].mode = MODE_DEFAULT;
			// Delete content of EEPROM address -> write invalid data
			eeprom_write_word(EEPROM_ADDRESS_FREQ1_HZ, EEPROM_INVALID_DATA);
		}
	}
	// (content[0] & content[1]) == 0 -> keep frequency value and mode
	else if ((content[0] & content[1]) != 0)
	// New frequency value
	{
		// Set new freq, user mode
		corr[ID_FREQ1].freq_hz = (uint16_t)content[0] + (((uint16_t)content[1]) << 8);
		corr[ID_FREQ1].mode = MODE_USER;
		// Store new value to EEPROM
		eeprom_write_word(EEPROM_ADDRESS_FREQ1_HZ, corr[ID_FREQ1].freq_hz);
	}
	// ID FREQ 2
	if ((content[2] & content[3]) == 0xFF)
	// Reset frequency value to default
	{
		// check if parameters of Freq. 2 index are in user mode
		// if in default mode, no need to reset
		if (corr[ID_FREQ2].mode != MODE_DEFAULT)
		{
			// Reset to default (Freq, mode)
			corr[ID_FREQ2].freq_hz = HZ_FREQ2;
			corr[ID_FREQ2].mode = MODE_DEFAULT;
			// Delete content of EEPROM address -> write invalid data
			eeprom_write_word(EEPROM_ADDRESS_FREQ2_HZ, EEPROM_INVALID_DATA);
		}
	}
	// (content[2] & content[3]) == 0 -> keep frequency value and mode
	else if ((content[2] & content[3]) != 0)
	// New frequency value
	{
		// Set new freq, user mode
		corr[ID_FREQ2].freq_hz = (uint16_t)content[2] + (((uint16_t)content[3]) << 8);
		corr[ID_FREQ2].mode = MODE_USER;
		// Store new value to EEPROM
		eeprom_write_word(EEPROM_ADDRESS_FREQ2_HZ, corr[ID_FREQ2].freq_hz);
	}
	// ID FREQ 3
	if ((content[4] & content[5]) == 0xFF)
	// Reset frequency value to default
	{
		// check if parameters of Freq. 3 index are in user mode
		// if in default mode, no need to reset
		if (corr[ID_FREQ3].mode != MODE_DEFAULT)
		{
			// Reset to default (Freq, mode)
			corr[ID_FREQ3].freq_hz = HZ_FREQ3;
			corr[ID_FREQ3].mode = MODE_DEFAULT;
			// Delete content of EEPROM address -> write invalid data
			eeprom_write_word(EEPROM_ADDRESS_FREQ3_HZ, EEPROM_INVALID_DATA);
		}
	}
	// (content[4] & content[5]) == 0 -> keep frequency value and mode
	else if ((content[4] & content[5]) != 0)
	// New frequency value
	{
		// Set new freq, user mode
		corr[ID_FREQ3].freq_hz = (uint16_t)content[4] + (((uint16_t)content[5]) << 8);
		corr[ID_FREQ3].mode = MODE_USER;
		// Store new value to EEPROM
		eeprom_write_word(EEPROM_ADDRESS_FREQ3_HZ, corr[ID_FREQ3].freq_hz);
	}
	// ID FREQ 4
	if ((content[6] & content[7]) == 0xFF)
	// Reset frequency value to default
	{
		// check if parameters of Freq. 4 index are in user mode
		// if in default mode, no need to reset
		if (corr[ID_FREQ4].mode != MODE_DEFAULT)
		{
			// Reset to default (Freq, mode)
			corr[ID_FREQ4].freq_hz = HZ_FREQ4;
			corr[ID_FREQ4].mode = MODE_DEFAULT;
			// Delete content of EEPROM address -> write invalid data
			eeprom_write_word(EEPROM_ADDRESS_FREQ4_HZ, EEPROM_INVALID_DATA);
		}
	}
	// (content[6] & content[7]) == 0 -> keep frequency value and mode
	else if ((content[6] & content[7]) != 0)
	// New frequency value
	{
		// Set new freq, user mode
		corr[ID_FREQ4].freq_hz = (uint16_t)content[6] + (((uint16_t)content[7]) << 8);
		corr[ID_FREQ4].mode = MODE_USER;
		// Store new value to EEPROM
		eeprom_write_word(EEPROM_ADDRESS_FREQ4_HZ, corr[ID_FREQ4].freq_hz);
	}
	
	
	/* Store the modes of the frequency indices to EEPROM */
	if ((corr[ID_FREQ1].mode &
		corr[ID_FREQ2].mode &
		corr[ID_FREQ3].mode &
		corr[ID_FREQ4].mode) == MODE_DEFAULT)
	// All frequencies are default frequencies
	{
		// Store Invalid data, since all are in default mode
		eeprom_write_word(EEPROM_ADDRESS_DATA_WRITTEN, EEPROM_INVALID_DATA);
	}
	else
	// Some/All user-set frequencies
	{
		uint16_t overall_mode;
		overall_mode = (uint16_t)corr[ID_FREQ1].mode +
					(((uint16_t)corr[ID_FREQ2].mode) << 1) +
					(((uint16_t)corr[ID_FREQ3].mode) << 2) +
					(((uint16_t)corr[ID_FREQ4].mode) << 3);
		// Store overall mode to EEPROM
		eeprom_write_word(EEPROM_ADDRESS_DATA_WRITTEN, overall_mode);
	}
	
	
	/* Update status */
	// Frequencies were altered -> re-initialize signal variables
	*status = UPDATE_TODO;

	return;
}
