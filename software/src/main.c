/**
  ******************************************************************************
  * @file    main.c
  * @author  CahierDeVac
  * @date    18-march-2024
  * @brief   Default main function.
  ******************************************************************************
*/
// Imports de base
#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"

// Project libs
#include "telemetre.h"
#include "stm32f1_rtc.h"
#include "time.h"

// HC-SR04 télemètre ultrasons
#include "stm32f103xb.h"
#include "HC-SR04/HCSR04.h" // on évite les warnings dégeulasses

// SG90 servo
#include "stm32f1_adc.h"
#include "servo.h"
#include "set_servo.h"

// Servo init vars
#define Left_US        30
#define Middle_US    50
#define Right_US    70
uint16_t Led_value;


void UART_full_init(void){
	//Initialisation de l'UART2 � la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reli�es jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirig�es vers la sonde de d�bogage, la liaison UART �tant ensuite encapsul�e sur l'USB vers le PC de d�veloppement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le p�riph�rique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);
};

int main(void)
{

	// uint16_t distance;
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la premi�re �tape de la fonction main().
	HAL_Init();

	//Initialisation du de l'interfaçage UART.
	UART_full_init();

	// Servo init
	SERVO_init();
	Led_value = 0;
	ADC_init();
//	RTC_init(FALSE);

	// Servo init
	SERVO_init();
	Led_value = 0;
	ADC_init();
//	RTC_init(FALSE);

	// RTC Init
	RTC_init(FALSE);
	while (1)
	{
//		HCSR04_process_main();
		HCSRO4_mesures(GPIOC, GPIO_PIN_7, GPIOB, GPIO_PIN_6);
		HCSRO4_mesures(GPIOC, GPIO_PIN_7, GPIOB, GPIO_PIN_6);
		HCSRO4_mesures(GPIOC, GPIO_PIN_7, GPIOB, GPIO_PIN_6);
	}
	
}

static void state_machine(void){
    typedef enum
    {
        INIT,
        DAY,
        NIGHT,
        TIME
    }state_e;
    static state_e state = INIT;
    static state_e previous_state = INIT;
    previous_state = state;
    switch(state){
        case INIT:
            //todo en f° de la photodiode passer en etat nuit ou jour
            if (Led_value > 100){
                state = DAY;
            }else{
                state = NIGHT;
            }
            break;
        case DAY:
            //todo eteindre la led et mettre la stm en mode veille

            break;
        case NIGHT:
            //todo get position
            //todo put servo in position
            //todo
    }

}
