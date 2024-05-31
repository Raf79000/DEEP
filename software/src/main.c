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

// RTC libs
#include "stm32f1_rtc.h"
#include "time.h"

// HC-SR04 télemètre ultrasons
//#include "telemetre.h"
//#include "stm32f103xb.h"
#include "HC-SR04/HCSR04.h" // on évite les warnings dégeulasses

#define PERIOD_MEASURE			100
#define NB_MEASURES				10

// SG90 servo
#include "stm32f1_adc.h"
#include "servo.h"
#include "set_servo.h"

// Servo init vars
#define Left_US        	30
#define Middle_US    	50
#define Right_US    	70
#define MIN_DISTANCE	150
volatile int16_t t = 0;
volatile RTC_TimeTypeDef h;
static uint16_t pin_state;
static uint16_t Led_value;
typedef enum {
			INIT,
			FAIL,
			LAUNCH_MEASURE,
			RUN,
			WAIT_DURING_MEASURE,
			WAIT_BEFORE_NEXT_MEASURE
}HCSR04_state_e;
struct area_struct {
	uint8_t id;
	HCSR04_state_e state;
	GPIO_TypeDef * trigg_gpio;
	uint16_t trigg_pin;
	GPIO_TypeDef * echo_gpio;
	uint16_t echo_pin;
	uint16_t distance;
	bool_e object_flag;
	bool_e mesure_flag;
};
static struct area_struct area_a = {0, INIT, GPIOA, GPIO_PIN_10, GPIOB, GPIO_PIN_4, 0, 0, 0};
static struct area_struct area_b = {0, INIT, GPIOA, GPIO_PIN_11, GPIOB, GPIO_PIN_5, 0, 0, 0};
static struct area_struct area_c = {0, INIT, GPIOA, GPIO_PIN_12, GPIOB, GPIO_PIN_6, 0, 0, 0};

void HCSR04_state_machine(struct area_struct * area) {
		static uint32_t tlocal;

		switch(area->state){
		case INIT:
			if(HCSR04_add(&area->id, area->trigg_gpio, area->trigg_pin, area->echo_gpio, area->echo_pin) != HAL_OK)
			{
				printf("HCSR04 non ajout� - erreur g�nante\n");
				area->state = FAIL;
			}
			else
			{
				printf("HCSR04 ajout�\n");
				area->state = LAUNCH_MEASURE;
			}
			break;
		case LAUNCH_MEASURE:
			if(area->mesure_flag)
			{
				area->mesure_flag = FALSE;
				HCSR04_run_measure(area->id);
				tlocal = HAL_GetTick();
				area->state = WAIT_DURING_MEASURE;
			}
			break;
		case WAIT_DURING_MEASURE:
			switch(HCSR04_get_value(area->id, &area->distance))
			{
				case HAL_BUSY:
					//rien � faire... on attend...
					break;
				case HAL_OK:
//					printf("sensor %d - distance : %d\n", area->id, area->distance);
					area->object_flag = (area->distance < 100);
					area->state = WAIT_BEFORE_NEXT_MEASURE;
					break;
				case HAL_ERROR:
					printf("sensor %d - erreur ou mesure non lanc�e\n", area->id);
					area->state = WAIT_BEFORE_NEXT_MEASURE;
					break;

				case HAL_TIMEOUT:
//					printf("sensor %d - timeout\n", area->id);
					area->state = WAIT_BEFORE_NEXT_MEASURE;
					break;
			}
			break;
		case WAIT_BEFORE_NEXT_MEASURE:
			if(HAL_GetTick() > tlocal + PERIOD_MEASURE)
				area->state = LAUNCH_MEASURE;
			break;
		default:
			break;
		}


}

#define ONE_MEASURE_DURATION	130
#define PERIOD_US	3*ONE_MEASURE_DURATION
void process_ms(void)
{
        //lecture 3 US sensor
        //lecture photodiode
    Led_value = ADC_getValue(ADC_CHANNEL_1);
    if(t){
        t--;
    }

    static uint16_t us_t = 0;
    us_t = (us_t+1)%PERIOD_US;
    if(us_t%ONE_MEASURE_DURATION == 0)
    {
    	switch(us_t/ONE_MEASURE_DURATION)
    	{
    		case 0:
    			area_a.mesure_flag = TRUE;
    			break;
    		case 1:
    			area_b.mesure_flag = TRUE;
    			break;
    		case 2:
    			area_c.mesure_flag = TRUE;
    			break;
    		default:
    			break;
    	}
    }

}

void full_init(void) {
	// uint16_t distance;
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la premi�re �tape de la fonction main().
	HAL_Init();

	//Initialisation de l'UART2 � la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
	//Attention, les pins PA2 et PA3 ne sont pas reli�es jusqu'au connecteur de la Nucleo.
	//Ces broches sont redirig�es vers la sonde de d�bogage, la liaison UART �tant ensuite encapsul�e sur l'USB vers le PC de d�veloppement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le p�riph�rique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	// Servo init
	SERVO_init();
	ADC_init();
	SERVO_set_position(0);

	// RTC Init
	RTC_init(FALSE);

	// process_ms init
	Systick_add_callback_function(&process_ms);
}

static void servo_orientation(void){
	typedef enum {
		INIT,
		AREA_A,
		AREA_B,
		AREA_C,
		MULTI
	}servo_state_e;
	static servo_state_e SERVO_state = INIT;

	switch(SERVO_state){
	case INIT:
		if (
				(area_a.object_flag == area_b.object_flag && area_a.object_flag == 1)||
				(area_a.object_flag == area_c.object_flag && area_a.object_flag == 1)||
				(area_c.object_flag == area_b.object_flag && area_c.object_flag == 1)){
			printf("multi");
			SERVO_state = MULTI;
		} else if (area_a.object_flag == 1){
			printf("set A");
			SERVO_state = AREA_A;
		} else if (area_c.object_flag == 1) {
			printf("set C");
			SERVO_state = AREA_C;
		} else {
			printf("set B");
			SERVO_state = AREA_B;
		}
		break;
	case AREA_A:
		SERVO_set_position(5);
		SERVO_state = INIT;
		break;
	case AREA_B:
		SERVO_set_position(12);
		SERVO_state = INIT;
		break;
	case AREA_C:
		SERVO_set_position(25);
		SERVO_state = INIT;
		break;
	case MULTI:
		SERVO_set_position(12);
		SERVO_state = INIT;
		break;
	}
}

static void state(void){
    typedef enum
    {
        INIT,
        DAY,
        NIGHT
    }state_e;
    static state_e state = INIT;
//    static state_e previous_state = INIT;
//    previous_state = state;
    switch(state){
        case INIT:
            //todo en f° de la photodiode passer en etat nuit ou jour
            if (Led_value > 100 && h.Seconds < 20){ //
                state = DAY;
            }else{
                state = NIGHT;
            }
            break;
        case DAY:
            //todo eteindre la led et mettre la stm en mode veille
        	pin_state = FALSE;
        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, pin_state);
        	if (Led_value <= 100 || h.Seconds >= 20){
        		state = NIGHT;
        	}
            break;
        case NIGHT:
        	servo_orientation();
        	pin_state = TRUE;
        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, pin_state);
        	if (Led_value > 100 && h.Seconds < 20){
        		state = DAY;
        	}
        	break;
    }

}
int main(void)
{
	// initialisation
	full_init();
	Led_value = 0;
	pin_state = TRUE;
	BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_11, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);

	while (1)
	{
		if(!t)
		{
			t = 1000;
			h = DEMO_RTC_process_main(FALSE);
		}
		//		printf("%d\n", Led_value);
		//		printf("%2d\n", h.Seconds);

		// traitement capteurs ultrasons
		HCSR04_process_main();
		HCSR04_state_machine(&area_a);
		HCSR04_state_machine(&area_b);
		HCSR04_state_machine(&area_c);
		// traitement servos
		state();
		HAL_Delay(500);
	}
}
