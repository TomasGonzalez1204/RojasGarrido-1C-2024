/*! @mainpage Blinking
 *
 * \section genDesc General Description
 *
 * This example makes LED_1 and LED_2 blink at different rates, using FreeRTOS tasks and timer interrupts.
 * 
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 12/09/2023 | Document creation		                         |
 *
 * @author Albano Peñalva (albano.penalva@uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "timer_mcu.h"
#include "led.h"
#include <lcditse0803.h>
#include <hc_sr04.h>
#include <switch.h>
/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD_LED_1_US 1000000
#define CONFIG_BLINK_PERIOD_LED_2_US 1300000
/*==================[internal data definition]===============================*/
TaskHandle_t led1_task_handle = NULL;
TaskHandle_t led2_task_handle = NULL;
/*==================[internal functions declaration]=========================*/
uint8_t Dist = 0;
uint64_t teclas;
bool Encendido=1, Hold=1;
uint8_t X;
uint8_t CONFIG_BLINK_PERIOD_MEDICION = 1000;
uint8_t CONFIG_BLINK_PERIOD_TECLAS = 100;
TaskHandle_t medicion_task_handle=NULL;
TaskHandle_t MostrarLed_task_handle=NULL;
#define ECHO GPIO_3
#define TRIGGER GPIO_2
#define TIME_PERIOD 1000000

void prenderLeds(void)
{
	Dist=HcSr04ReadDistanceInCentimeters();
	if (Dist < 10){
		LedsOffAll();
	} else if (Dist > 10 && Dist < 20){
		LedOn(LED_1);
	} else if (Dist > 20 && Dist < 30){
		LedOn(LED_2);
		LedOn(LED_1);
	} else if(Dist > 30){
		LedOn(LED_3);
		LedOn(LED_2);
		LedOn(LED_1);
	}
	
}
void FuncionTecla1(void)
{
	Encendido=!Encendido;
	LedToggle (LED_1);

}

void FuncionTecla2(void)
{
	Hold=!Hold;
	}
void Mostrar(void *pvParameter)
{
	while (1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if(Encendido==1)
		{
				prenderLeds();
				if (Hold==1)
				{
					LcdItsE0803Write(Dist);
				}
		}
		else 
			{
			LedsOffAll();
			LcdItsE0803Off();
			}
		//vTaskDelay(CONFIG_BLINK_PERIOD_TECLAS / portTICK_PERIOD_MS);
	}
}
/**
 * @brief Función invocada en la interrupción del timer A
 */
void FuncTimerA(void* param){
    xTaskNotifyGive(led1_task_handle);    /* Envía una notificación a la tarea asociada al LED_1 */
}

/**
 * @brief Función invocada en la interrupción del timer B
 */
void FuncTimerB(void* param){
    xTaskNotifyGive(led2_task_handle);    /* Envía una notificación a la tarea asociada al LED_2 */
}



/**
 * @brief Tarea encargada de blinkear el LED_2
 */
static void Led2Task(void *pvParameter){
    while(true){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);    /* La tarea espera en este punto hasta recibir una notificación */
        printf("LED_2 Toggle\n");
    }
}

/*==================[external functions definition]==========================*/
void app_main(void){

    LedsInit();
	HcSr04Init(ECHO,TRIGGER);
	SwitchesInit();
	LcdItsE0803Init();
    /* Inicialización de timers */
    timer_config_t timer_led_1 = {
        .timer = TIMER_A,
        .period = CONFIG_BLINK_PERIOD_LED_1_US,
        .func_p = FuncTimerA,
        .param_p = NULL
    };
    TimerInit(&timer_led_1);
    timer_config_t timer_led_2 = {
        .timer = TIMER_B,
        .period = CONFIG_BLINK_PERIOD_LED_2_US,
        .func_p = FuncTimerB,
        .param_p = NULL
    };
    TimerInit(&timer_led_2);
    SwitchActivInt(SWITCH_1, FuncionTecla1, NULL);
	SwitchActivInt(SWITCH_2, FuncionTecla2, NULL);
    /* Creación de tareas */
    xTaskCreate(&Mostrar, "LED_1", 512, NULL, 5, &led1_task_handle);
    //xTaskCreate(&Led2Task, "LED_2", 512, NULL, 5, &led2_task_handle);
    /* Inicialización del conteo de timers */
    TimerStart(timer_led_1.timer);
    //TimerStart(timer_led_2.timer);
}
