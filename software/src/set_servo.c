/*
 * Set_Servo.c
 *
 *  Created on: Apr 26, 2024
 *      Author: matth
 */
#include "servo.h"
#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "config.h"
#include "stm32f1_timer.h"


void Test_Servo_Button(){
	uint16_t button_value;
	uint16_t value_servo;

	button_value = HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN);
	if(!button_value){
		value_servo = value_servo + 10;
		if (value_servo> 100){
			value_servo = 0;
		}
		PWM_set_duty(TIMER1_ID,TIM_CHANNEL_1,value_servo);
	}
}

void Servo_US(uint16_t side_US){
	PWM_set_duty(TIMER1_ID,TIM_CHANNEL_1,side_US);
}
