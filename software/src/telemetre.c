/*
 * foo.c - ce fichier contient des choses absolument inutiles... sauf � des fins purement p�dagogiques.
 *
 *  Created on: 26 ao�t 2015
 *      Author: S. Poiraud
 */
#include "stm32f1xx_hal.h"
#include "macro_types.h"
#include "stm32f103xb.h"
#include "HC-SR04/HCSR04.h"

#define PERIOD_MEASURE			100
#define NB_MEASURES				10


uint16_t HCSRO4_mesures(void)
{
	typedef enum
	{
		INIT,
		FAIL,
		LAUNCH_MEASURE,
		RUN,
		WAIT_DURING_MEASURE,
		WAIT_BEFORE_NEXT_MEASURE
	} state_e;

	static state_e state = INIT;
	static uint32_t tlocal;
	static uint8_t id_sensor;
	uint16_t distance;

	//ne pas oublier d'appeler en t�che de fond cette fonction.
	HCSR04_process_main();


	switch(state)
	{
		case INIT:
			if(HCSR04_add(&id_sensor, GPIOC, GPIO_PIN_7, GPIOB, GPIO_PIN_6) != HAL_OK)
			{
				printf("HCSR04 non ajout� - erreur g�nante\n");
				state = FAIL;
			}
			else
			{
				printf("HCSR04 ajout�\n");
				state = LAUNCH_MEASURE;
			}
			break;
		case LAUNCH_MEASURE:
			HCSR04_run_measure(id_sensor);
			tlocal = HAL_GetTick();
			state = WAIT_DURING_MEASURE;
			break;
		case WAIT_DURING_MEASURE:
			switch(HCSR04_get_value(id_sensor, &distance))
			{
				case HAL_BUSY:
					//rien � faire... on attend...
					break;
				case HAL_OK:
					printf("sensor %d - distance : %d\n", id_sensor, distance);
					state = WAIT_BEFORE_NEXT_MEASURE;
					break;
				case HAL_ERROR:
					printf("sensor %d - erreur ou mesure non lanc�e\n", id_sensor);
					state = WAIT_BEFORE_NEXT_MEASURE;
					break;

				case HAL_TIMEOUT:
					printf("sensor %d - timeout\n", id_sensor);
					state = WAIT_BEFORE_NEXT_MEASURE;
					break;
			}
			break;
		case WAIT_BEFORE_NEXT_MEASURE:
			if(HAL_GetTick() > tlocal + PERIOD_MEASURE)
				state = LAUNCH_MEASURE;
			break;
		default:
			break;
	}
}
