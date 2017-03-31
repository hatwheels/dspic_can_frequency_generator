/*  
	2014 - 2015

	\file can.h
	\brief Contains function declarations related to the CAN module
*/

#ifndef CAN_H
#define CAN_H

#include "stypes.h"

/* Typedefs */
typedef struct
{
	uint16_t  sid;
	uint8_t   length;
	uint8_t   content[8]; // 8x8 bytes
}T_can_msg_t;

typedef enum
{
	CAN_RX_MSG_BUFFER_0 = 0,
	CAN_RX_MSG_BUFFER_1,
	CAN_RX_MSG_BUFFER_LAST
}E_can_rx_buffer_t;

typedef struct
{
	T_can_msg_t		can_tx_msg_buffer;
	T_can_msg_t		can_rx_msg_buffer[CAN_RX_MSG_BUFFER_LAST];
	uint8_t					nodeID_DIP; // Node ID as set by DIP switches 2,3,4,5
}T_can_data_t;

/* Function declarations */
void CAN_init(void);
void CAN_transmit_error(T_can_data_t *can_data, uint8_t *msg_content);

/* Global variable, extern to be visible */
extern T_can_data_t CAN_data;

#endif
