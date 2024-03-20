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
 * | 12/09/2023 | guia1ej3.c									 |
 *
 * @author Jessenia Rojas Garrido(jrojasgarrido@ingenieria.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <led.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/*==================[macros and definitions]=================================*/
#define DELAY 100
enum {ON,OFF,TOGGLE};
struct leds
{
    uint8_t mode;       //ON, OFF, TOGGLE
	uint8_t n_led;       //indica el número de led a controlar
	uint8_t n_ciclos;  // indica la cantidad de ciclos de encendido/apagado
	uint16_t periodo;   // indica el tiempo de cada ciclo
} my_leds;             //Use como guía para la implementación el diagrama de flujo que se observa en la Figura 1;
uint8_t j = 0, ciclos = 0;

/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/
void control_led (struct leds *LEDS){
	switch (LEDS->mode)
{
case ON:
	LedOn (LEDS->n_led);
	break;
case OFF:
	LedOff(LEDS->n_led);
	break;
case TOGGLE:
	for (ciclos = 0; ciclos < LEDS->n_ciclos; ciclos++)
	{
		LedToggle(LEDS->n_led);
	for (j = 0; j < (LEDS->periodo/DELAY); j++)
	{
		vTaskDelay(DELAY / portTICK_PERIOD_MS);
	}
	}
	
	break;
}
}
/*==================[external functions definition]==========================*/
void app_main(void){
	LedsInit ();
	my_leds.mode = TOGGLE;
	my_leds.n_ciclos = 10;
	my_leds.n_led = LED_3;
	my_leds.periodo = 500;
	control_led (&my_leds);
	while (1)
	{
		vTaskDelay(DELAY / portTICK_PERIOD_MS);
	}
	
}
/*==================[end of file]============================================*/