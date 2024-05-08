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
 * | 12/09/2023 | Document creation		                         |
 *
 * @author Jessenia J. Rojas Garrido (jrojasgarrido.ingenieria.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <led.h>
#include <lcditse0803.h>
#include <hc_sr04.h>
#include <switch.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <gpio_mcu.h>
/*==================[macros and definitions]=================================*/
uint8_t Dist = 0;
uint64_t teclas;
bool Encendido=0, Hold=0;
uint8_t X;
uint8_t CONFIG_BLINK_PERIOD = 1000;
uint8_t CONFIG_BLINK_PERIOD_TECLAS = 100;
TaskHandle_t MostrarLed_task_handle=NULL;
TaskHandle_t MostrarDisplay_task_handle=NULL;
TaskHandle_t FuncionTeclas_task_handle=NULL;
#define ECHO GPIO_3
#define TRIGGER GPIO_2
/*==================[internal data definition]===============================*/
void prenderLeds(void);
void FuncionTeclas(void *pvParameter);
void Mostrar(void *pvParameter);
/*==================[internal functions declaration]=========================*/
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

void FuncionTeclas(void *pvParameter)
{
	    while(1)    {
    	teclas  = SwitchesRead();
    	switch(teclas){
    		case SWITCH_1:
    			Encendido=!Encendido;
				//printf("se apreto tecla1");
    		break;
    		case SWITCH_2:
				Hold=!Hold;
				//printf("se apreto tecla2");
    		break;
    	}
		vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
	}
}
void Mostrar(void *pvParameter)
{
	while (1)
	{
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
		vTaskDelay(CONFIG_BLINK_PERIOD_TECLAS / portTICK_PERIOD_MS);
	}
}
/*==================[external functions definition]==========================*/
void app_main(void){
LedsInit();
HcSr04Init(ECHO, TRIGGER);
SwitchesInit();
LcdItsE0803Init();
    xTaskCreate(&Mostrar, "TAREA LEDS", 512, NULL, 5, &MostrarLed_task_handle);
    xTaskCreate(&FuncionTeclas, "FUNCION TECLAS", 512, NULL, 5, &FuncionTeclas_task_handle);
}

/*==================[end of file]============================================*/