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
 * | 3/04/2024 | Document creation		                         |
 *
 * @author Jessenia J. Rojas Garrido (Jrojasgarrido@ingenieria.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "gpio_mcu.h"
/*==================[macros and definitions]=================================*/


typedef struct
{
	gpio_t pin;			/*!< GPIO pin number */
	io_t dir;			/*!< GPIO direction '0' IN;  '1' OUT*/
}  gpioConf_t;

/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/
void BCDtoGPIO(gpioConf_t array,uint8_t digito){
	uint8_t mascara=1;
	for (uint8_t i = 0; i < 4; i++){
		GPIOInit(array[i].pin, array[i].dir);
	}
	for (uint8_t j = 0; j < 4; j++)
	
	{
		if (mascara & digito ==0)
		{
			GPIOOff(array[i].pin);
		} else {GPIOOn(array[i].pin)};
	}
	mascara = mascara << 1;	
	}
}
/*==================[external functions definition]==========================*/
void app_main(void){
	BCDtoGPIO ;
}
/*==================[end of file]============================================*/