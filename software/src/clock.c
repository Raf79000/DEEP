///*
// * foo.c - ce fichier contient des choses absolument inutiles... sauf � des fins purement p�dagogiques.
// *
// *  Created on: 26 ao�t 2015
// *      Author: S. Poiraud
// */
//#include "stm32f1xx_hal.h"
//#include "macro_types.h"
//#include "stm32f103xb.h"
//#include "stm32f1_rtc.h"
//
//running_e DEMO_RTC_process_main(bool_e ask_for_finish)
//{
//	typedef enum
//	{
//		INIT = 0,
//		RUN
//	}state_e;
//	static state_e state = INIT;
//	running_e ret;
//	ret = IN_PROGRESS;
//	static bool_e flag_alarm;
//	const char * weekday_str[7] = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};
//
//	switch(state)
//	{
//		case INIT: {
//			state = RUN;
//			//RTC_init(FALSE);	//Oscillateur INTERNE, impr�cis, mais testable sans ajout d'un oscillateur sur PC14 et PC15 !
//			RTC_init(TRUE);		//Oscillateur EXTERNE, pr�cis, mais n�cessite l'ajout d'un oscillateur sur PC14 et PC15 !
//
//			//	1/04/2015	12h34'56"
//			//RTC_TimeTypeDef time = {12, 34, 56};
//
//			RTC_set_alarm(&(RTC_AlarmTypeDef){00, 00, 10}, TRUE, &flag_alarm);
//
//			printf("This demo will print the time every second.\n");
//			printf("Commands :\n");
//			printf("h : help\n");
//			printf("r : reset time & date to default\n");
//
//			break;}
//		case RUN:{
//			static uint8_t previous_printed_time = 0;
//			RTC_TimeTypeDef time;
//			RTC_DateTypeDef date;
//			RTC_get_time_and_date(&time, &date);
//			if(time.Seconds != previous_printed_time)
//			{
//				printf("%s %2d/%2d/%2d - %2d:%2d:%2d\n", weekday_str[date.WeekDay], date.Date, date.Month, date.Year, time.Hours, time.Minutes, time.Seconds);
//				previous_printed_time = time.Seconds;
//			}
//
//			uint8_t c;
//			if(UART_data_ready(UART2_ID))
//			{
//				c = UART_getc(UART2_ID);
//				switch(c)
//				{
//					case 'r':
//						printf("reset time & date\n");
//						RTC_TimeTypeDef time = {00, 00, 00};
//						RTC_DateTypeDef date = {RTC_WEEKDAY_MONDAY, RTC_MONTH_APRIL, 1, 20};
//						RTC_set_time_and_date(&time, &date);
//						break;
//					case 'h':
//						printf("This demo will print the time every second.\n");
//						printf("Commands :\n");
//						printf("h : help\n");
//						printf("r : reset time & date to default\n");
//						break;
//				}
//			}
//			if(flag_alarm)
//			{
//				printf("alarm occured\n");
//				flag_alarm = FALSE;
//			}
//			if(ask_for_finish)
//			{
//				state = INIT;
//				ret = END_OK;
//			}
//			break;}
//		default:
//			break;
//	}
//	return ret;
//}
