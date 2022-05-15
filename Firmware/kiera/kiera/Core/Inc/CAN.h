/*
 * CAN.h
 *
 *  Created on: Apr 26, 2022
 *      Author: konrad
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_



//#endif /* INC_CAN_H_ */

// od molika
//#ifndef INC_CAN_INIT_DRIVER_H_
//#define INC_CAN_INIT_DRIVER_H_

#include "stm32l4xx_hal.h"

void can_init(void);
void can_filter_init(void);
void can_tx_header_init(void);

#endif /* INC_CAN_INIT_DRIVER_H_ */
