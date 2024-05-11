/*! @mainpage Template
 *
 * @section genDesc General Description
 *
 * This section describes how the program works.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 10/05/2024 | Document creation		                         |
 *
 * @author Jessenia J. Rojas Garrido (jrojasgarrido@ingenieria.uner.edu.ar)
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
#define CONFIG_BLINK_PERIOD_TIMER 1000000
#define ECHO GPIO_3
#define TRIGGER GPIO_2
/*==================[internal data definition]===============================*/
uint8_t Dist = 0;
uint64_t teclas;
bool Encendido=1, Hold=1;
uint8_t X;
uint8_t CONFIG_BLINK_PERIOD_MEDICION = 1000;
uint8_t CONFIG_BLINK_PERIOD_TECLAS = 100;
TaskHandle_t medicion_task_handle=NULL;
TaskHandle_t MostrarLed_task_handle=NULL;
/*==================[internal functions declaration]=========================*/

//funcion para prender Leds segun la distancia
void prenderLeds(void)
{
	Dist=HcSr04ReadDistanceInCentimeters();
	if (Dist < 10)
	{
		LedsOffAll();
	} else if (Dist > 10 && Dist < 20)
	{
		LedOn(LED_1);
	} else if (Dist > 20 && Dist < 30)
	{
		LedOn(LED_2);
		LedOn(LED_1);
	} else if(Dist > 30)
	{
		LedOn(LED_3);
		LedOn(LED_2);
		LedOn(LED_1);
	}	
}

//cambia el estado de la bandera de encendido
void FuncionTecla1(void)
{
	Encendido=!Encendido;
}

//cambia el estado de la bandera de Hold
void FuncionTecla2(void)
{
	Hold=!Hold;
}

//tarea para mostrar por pantalla segun Encendido y segun tecla hold, con timer e interrupciones
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
	}
}

// Tarea Timer
void FuncTimer(void* param)
{
    xTaskNotifyGive(MostrarLed_task_handle);   
}

/*==================[external functions definition]==========================*/
void app_main(void){

    LedsInit();
	HcSr04Init(ECHO,TRIGGER);
	SwitchesInit();
	LcdItsE0803Init();
	timer_config_t timer_mediciones = 
	{
        .timer = TIMER_A,
        .period = CONFIG_BLINK_PERIOD_TIMER,
        .func_p = FuncTimer,
        .param_p = NULL
    };
    TimerInit(&timer_mediciones);

    SwitchActivInt(SWITCH_1, FuncionTecla1, NULL);
	SwitchActivInt(SWITCH_2, FuncionTecla2, NULL);

    xTaskCreate(&Mostrar, "LED_1", 512, NULL, 5, &MostrarLed_task_handle);

    TimerStart(timer_mediciones.timer);
}