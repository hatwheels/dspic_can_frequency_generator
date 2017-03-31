/*
	2014 - 2015

	\file can.c
	\brief Contains functions for the CAN module initialization
*/

#include "freq_gen.h"

/* Defines */
#define CAN_PDO_SID_RX_USER_CONFIG			(0x0200)
#define CAN_PDO_SID_TX_ERROR						(0x0180)

//*****************************************************************************
// Static functions
//*****************************************************************************
// Put can into configuration mode
static void CAN_set_in_config_mode(void)
{
	/* Request configuration mode */
	C1CTRLbits.REQOP  = 4;
  
	/* CAN master clock is FCY */
	C1CTRLbits.CANCKS = 1;
  
	/* Wait until configuration mode entered */
	while(C1CTRLbits.OPMODE != 4)
	{
	}
  
	return;
}

/*	This function sets the values for the acceptance filter masks for
	standard length identifiers.
	Parameters: 	uint8_t:	mask_no:	(Mask number 0-1)
					uint16_t:	mask:		(Mask value)

	C1RxMnSID = 0x0001 -> Only accept one message type as
										specified in EXIDE
*/
static void CAN_set_mask_rx(uint8_t mask_no, uint16_t mask)
{
	switch(mask_no)
	{
		case 0: 
			C1RXM0SID = 0x0001;
			C1RXM0SIDbits.SID = mask & 0x7FF;
			break;

		case 1: 
			C1RXM1SID = 0x0001;
			C1RXM1SIDbits.SID = mask & 0x7F;
			break;

		default:
			C1RXM0SID = 0x0001;
			C1RXM0SIDbits.SID = mask & 0x7F;
		break;
	}
	return;
}

/* 	This function sets the acceptance filter, SID for standard 
	length identifiers.
	Parameters: 	uint8_t :	filter_no:	(Filter number 0-5)
					uint16_t:	sid:		(SID value)

	C1RXF0SID = 0x0000 -> Enable filter for standard identifier
*/
static void CAN_set_filter_rx(uint8_t filter_no, uint16_t sid)
{
	switch(filter_no)
	{
		case 0: 
			C1RXF0SID = 0x0000;
			C1RXF0SIDbits.SID = (sid & 0x07FF);
			break;

		case 1: 
			C1RXF1SID = 0x0000;
			C1RXF1SIDbits.SID = (sid & 0x07FF);
			break;

		case 2: 
			C1RXF2SID = 0x0000;
			C1RXF2SIDbits.SID = (sid & 0x07FF);
			break;

		case 3: 
			C1RXF3SID = 0x0000;
			C1RXF3SIDbits.SID = (sid & 0x07FF);
			break;

		case 4: 
			C1RXF4SID = 0x0000;
			C1RXF4SIDbits.SID = (sid & 0x07FF);
			break;

		case 5: 
			C1RXF5SID = 0x0000;
			C1RXF5SIDbits.SID = (sid & 0x07FF);
			break;

		default: 
			C1RXF0SID = 0x0000;
			C1RXF0SIDbits.SID = (sid & 0x07FF);
			break;
	}

	return;
}

/* This function sets the message transmission priority bits.
   Parameters: 	uint8_t: buffer:   (Transmit buffer number 0-3)
						uint8_t: priority: (Transmit priority 0-3)
*/
static void CAN_set_priority(uint8_t buffer, uint8_t priority)
{
	if (priority > 3)
	{
		priority = 3;
	}

	switch(buffer)
	{
		case 0: C1TX0CONbits.TXPRI = priority;
					break;
		case 1: C1TX1CONbits.TXPRI = priority;
					break;
		case 2: C1TX2CONbits.TXPRI = priority;
					break;
		default:
					break;
	}

	return;
}

/* Sets baud rate to 250Kbps with 10Tq for a clock of 20MHz
	TODO LVDK: read DIP switches (PORTF) to set baud rate accordingly
*/
static void CAN_configure(void)
{
	/*  Nominal Bit Time NBT = (SJW + PRSEG + SEG1PH + SEG2PH) * Tq = 10Tq
		Nominal Bit Rate NBR = 1 / 10Tq = 1 / 4 usec = 250Kbps 
	*/
	C1CFG1bits.BRP      	= 3;    /* 20 MHz clock */
	C1CFG1bits.SJW      	= 0;    /* Synchronized jump width time = 1Tq */
	C1CFG2bits.PRSEG    	= 4;    /* Propagation time segment = 5Tq */
	C1CFG2bits.SEG1PH   	= 1;    /* Phase buffer Segment 1 = 2Tq */
	C1CFG2bits.SAM      	= 0;    /* Bus is sampled once */
	C1CFG2bits.SEG2PHTS		= 1;    /* PhaseSeg2 is freely programmable */
	C1CFG2bits.SEG2PH   	= 1;    /* Phase buffer Segment 2 = 2Tq */
	C1CTRLbits.REQOP    	= 0;    /* Can clock is Fcy = 20MHz. Request normal operation mode. */
	/*  NBT = (1 + 5 + 2 + 2) * Tq = 10Tq
		point where Sampling of bit takes place: (1 + 5 + 2) / 10 = 8 / 10 = 80%
	*/
	while(C1CTRLbits.OPMODE != 0) {}

	return;
}

/* This function returns an available empty transmit buffer.
	Returns: Transmit buffer number 0-2.
*/
static uint8_t CAN_get_txbuffer(void)
{
	uint8_t buffer = 0;

	if(C1TX0CONbits.TXREQ == 0)
	{
		buffer = 0;
	}
	else if(C1TX1CONbits.TXREQ == 0)
	{
		buffer = 1;
	}
	else if(C1TX2CONbits.TXREQ == 0)
	{
		buffer = 2;
	}

	return buffer;
}

/* This function writes the message identifier(SID), writes the data to be
	transmitted into the Transmit buffer and sets the corresponding Transmit
	request bit.

	Parameters: Pointer to structure T_can_msg_t defined in can.h
*/
static void CAN_transmit_message(const T_can_msg_t *message)
{
	uint8_t   buffer;
	uint16_t  ix;

	/* Divide 11 bits of sid over 2 SID's in CiTXnSID */
	ix = ((message->sid & 0x07C0) << 5) | ((message->sid & 0x003F) << 2); // result: ix = xxxx x000 xxxx xx00 (x == 0 OR 1)
	// Get available empty transmit buffer
	buffer = CAN_get_txbuffer();

	switch(buffer)
	{
		case 0: C1TX0SID = ix;
					break;
		case 1: C1TX1SID = ix;
					break;
		case 2: C1TX2SID = ix;
					break;
		default:	C1TX0SID = ix;
					break;
	}
  
	// Reset ix variable and use it as index of the message (range 0 ... 7)
	for(ix = 0; ix < message->length; ++ix)
	{
		switch(buffer)
		{
			case 0: *((uint8_t *)&C1TX0B1+ix)= message->content[ix];
						break;
			case 1: *((uint8_t *)&C1TX1B1+ix)= message->content[ix];
						break;
			case 2: *((uint8_t *)&C1TX2B1+ix)= message->content[ix];
						break;
			default:	*((uint8_t *)&C1TX0B1+ix)= message->content[ix];
						break;
		}
	}

	switch(buffer)
	{
		case 0: C1TX0DLC = 0x0180;
					C1TX0DLCbits.DLC = message->length;
					C1TX0CONbits.TXREQ = 1;
					break;
		case 1: C1TX1DLC = 0x0180;
					C1TX1DLCbits.DLC = message->length;
					C1TX1CONbits.TXREQ = 1;
					break;
		case 2: C1TX2DLC = 0x0180;
					C1TX2DLCbits.DLC = message->length;
					C1TX2CONbits.TXREQ = 1;
					break;
		default:
				break;
	}
  
	return;
}

/* If a message has been received, read the data from the receive buffer into 
	the structure T_can_msg_t and clear the RXFUL bit. */
static void CAN_receive_message(E_can_rx_buffer_t buffer_id, uint8_t *msg_content)
{
	uint8_t ix;
	T_can_msg_t  *message;

	message = &(CAN_data.can_rx_msg_buffer[buffer_id]);

	switch(buffer_id)
	{
		case 0:
			message->sid    = (C1RX0SID >> 2) & 0x07ff;
			message->length = C1RX0DLCbits.DLC;
			break;

		case 1: 
			message->sid    = (C1RX1SID >> 2) & 0x07ff;
			message->length = C1RX1DLCbits.DLC;
			break;

		default:
			break;
	}

	for(ix = 0; ix < message->length; ++ix)
	{
		switch(buffer_id)
		{
			case 0: 
				msg_content[ix] = *((uint8_t *)&C1RX0B1 + ix);
				break;
  
			case 1: 
				msg_content[ix] = *((uint8_t *)&C1RX1B1 + ix);
				break;

			default:
				break;
		}
	}

	/* Process data */
	switch (message->sid - CAN_data.nodeID_DIP)
	{		
		case (CAN_PDO_SID_RX_USER_CONFIG): // node: 0x0200
			// Update Frequencies/Step with received values
			GEN_params_update(msg_content, corr_step, &(OutputSignal.status));
			break;
				
		default:
			break;
	}

	return;
}

//*****************************************************************************
// Global functions
//*****************************************************************************
// Can_init() is used to configure CAN module
void CAN_init(void)
{
	/* Obtain node ID: restart system if nodeID changed as it requires new filters
		for CAN message reception (CAN system needs to be in configuration mode) */
	CAN_data.nodeID_DIP = ((~PORTB/* which port is to be found */) & 0x000F);

	/* Go into configuration mode */
	CAN_set_in_config_mode();

	/* Receive buffer 0 status and control register
		- No receive buffer overflow from RB0 to RB1 (for now) */
	C1RX0CONbits.DBEN = 0;
  
	/* Transmit buffer n Standard identifier
		- Enable filter for standard identifier */
	C1TX0SIDbits.TXIDE = 0;
	C1TX1SIDbits.TXIDE = 0;
	C1TX2SIDbits.TXIDE = 0;

	/* Reset receive buffer */
	C1RX0B1 = 0;
	C1RX0B2 = 0;
	C1RX0B3 = 0;
	C1RX0B4 = 0;

	/* Set masks and filters */
	CAN_set_mask_rx(0,0x000F);
	CAN_set_mask_rx(1,0x000F);

	CAN_set_filter_rx(0,CAN_PDO_SID_RX_USER_CONFIG
								+ (uint16_t)(CAN_data.nodeID_DIP));
	CAN_set_filter_rx(1,CAN_PDO_SID_RX_USER_CONFIG
								+ (uint16_t)(CAN_data.nodeID_DIP));

	/* Transmission priority Bits */
	CAN_set_priority(0,2);
	CAN_set_priority(1,2);
	CAN_set_priority(2,2);

	CAN_configure();

	/*	CAN interrupt enable register: enable all CAN interrupt sources
		IVRIE (invalid message received), WAKIE (Bus Wake Up), ERRIE (Error),
		TX2IE (Transmit Buffer 2), TX1IE (Transmit Buffer 1), TX0IE (Transmit Buffer 0),
		RX1IE (Receive Buffer 1), RX0IE (Receive Buffer 0)
	*/
	C1INTE = 0x00FF;	

	// Clear interrupt flag status bit associated with CAN (bit 11)
	IFS1bits.C1IF = 0;

	// Set interrupt priority for CAN (bit 14-12)
	IPC6bits.C1IP = 6; // 1st in priority

	return;
}

// Transmit Invalid message @ EEPROM error
void CAN_transmit_error(T_can_data_t *can_data, uint8_t *msg_content)
{
	T_can_msg_t    *can_msg;
	uint8_t           	nodeID;

	can_msg  = &(can_data->can_tx_msg_buffer);
	nodeID   = can_data->nodeID_DIP;

	can_msg->sid    = CAN_PDO_SID_TX_ERROR + (uint16_t)nodeID;
	can_msg->length = 8;

	/*	Fill content error:
		msg: [0x18n    0xFF 0xFF 0xFF 0xFF]
	*/
	uint8_t i;
	for (i = 0; i < can_msg->length; ++i)
		msg_content[i] = 0xFF;
	
	CAN_transmit_message(can_msg);

	return;
}

//*****************************************************************************
// CAN1 module ISR 
//*****************************************************************************
// _C1Interrupt() is the CAN1 receive interrupt.
void __attribute__((interrupt, auto_psv)) _C1Interrupt(void)
{
	/* For now, only messages are transmitted and only receive interrupt is
		cleared. They are not processed */
	if (C1INTFbits.TX0IF) C1INTFbits.TX0IF = 0;
	if (C1INTFbits.TX1IF) C1INTFbits.TX1IF = 0;
	if (C1INTFbits.TX2IF) C1INTFbits.TX2IF = 0;
	if (C1INTFbits.RX0IF)
	{
		CAN_receive_message(CAN_RX_MSG_BUFFER_0,
			CAN_data.can_rx_msg_buffer[CAN_RX_MSG_BUFFER_0].content);
		C1RX0CONbits.RXFUL 	= 0;
		C1INTFbits.RX0IF   		= 0;
	}
	if (C1INTFbits.RX1IF)
	{
		CAN_receive_message(CAN_RX_MSG_BUFFER_1,
			CAN_data.can_rx_msg_buffer[CAN_RX_MSG_BUFFER_1].content);
		C1RX1CONbits.RXFUL 	= 0;
		C1INTFbits.RX1IF 			= 0;
	}
	if (C1INTFbits.WAKIF) C1INTFbits.WAKIF = 0; // Add wake-up handler code
	if (C1INTFbits.ERRIF) C1INTFbits.ERRIF = 0; // Add error handler code
	if (C1INTFbits.IVRIF) C1INTFbits.IVRIF = 0; // Add invalid message received handler code
	if ( (C1INTF & C1INTE) == 0 ) IFS1bits.C1IF = 0;
}
