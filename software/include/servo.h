/*
 * servo.h
 *
 *  Created on: Mar 18, 2024
 *      Author: matth
 */
#include "stm32f1_sys.h"


#ifndef SERVO_H_
#define SERVO_H_

void SERVO_init(void);
void SERVO_set_position(uint16_t position);
void SERVO_process_test(void);



#endif /* SERVO_H_ */
