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
 * | 20/03/2024 | ejercicio 4 guia 4                             |
 *
 * @author Jessenia Jazmin Rojas Garrido (jrojasgarrido@ingenieria.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
/*==================[macros and definitions]=================================*/
uint8_t CONT;
uint8_t vect[3];
uint8_t i;
#define CONFIG_BLINK_PERIOD 1000
/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/
int8_t  convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number)
{
	for(CONT=0; CONT<digits; CONT++){
		bcd_number [CONT] = data%10;
		data = data/10;
	}
	return 1;
}

/*==================[external functions definition]==========================*/
void app_main(void){
	
	convertToBcdArray (138,3,vect);
	for (i=0;i<3;i++)
	{
		printf("nº: %d en posición %d\n", vect[i], i);
	}

	 while(true){
        printf("LED ON\n");
       // LedOn(LED_1);
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        printf("LED OFF\n");
       // LedOff(LED_1);
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
	
}
/*==================[end of file]============================================*/