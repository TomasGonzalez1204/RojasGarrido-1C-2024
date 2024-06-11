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
#include "gpio_mcu.h"
/*==================[macros and definitions]=================================*/

uint8_t CONT;
uint8_t vect[3];
uint8_t i;
#define CONFIG_BLINK_PERIOD 1000
#define N_DIGITOS 3
typedef struct
{
	gpio_t pin;			/*!< GPIO pin number */
	io_t dir;			/*!< GPIO direction '0' IN;  '1' OUT*/
}  gpioConf_t;
/*==================[internal data definition]===============================*/
gpioConf_t arreglo [4] = {{GPIO_20,GPIO_OUTPUT},{GPIO_21,GPIO_OUTPUT},{GPIO_22,GPIO_OUTPUT},{GPIO_23,GPIO_OUTPUT}};
/*==================[internal functions declaration]=========================*/
int8_t  convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number)
{
	for(CONT=0; CONT<digits; CONT++){
		bcd_number [CONT] = data%10;
		data = data/10;
	}
	return 1;
}

void BCDtoGPIO(gpioConf_t* array,uint8_t digito)
{
	uint8_t mascara=1;
	for (uint8_t i = 0; i < 4; i++)
	{
		GPIOInit(arreglo[i].pin, arreglo[i].dir);
	}
	for (uint8_t j = 0; j < 4; j++)
	{
		if ((mascara&digito) == 0)
			{
				GPIOOff(array[j].pin);
				//printf("SE PONE EN BAJO PIN ",array[j].pin);
			} else 
			{
				GPIOOn(array[j].pin);
				//printf("SE PONE EN ALTO PIN ",array[j].pin);
			}
		mascara = mascara << 1;	
	}
}

void displayLeds(uint32_t data, uint digitos, gpioConf_t *vectorGPIO, gpioConf_t *vectorGPIO_map)
{
	uint8_t arreglo[digitos];
	convertToBcdArray (data, digitos, arreglo);

	for (int i=0; i<N_DIGITOS; i++){ 

		BCDtoGPIO(arreglo[i], digitos);

		GPIOOn(vectorGPIO_map[i].pin);
		GPIOOff(vectorGPIO_map[i].pin);
		
	}
}
	
/*==================[external functions definition]==========================*/
void app_main(void)
{
	/*BCDtoGPIO(arreglo,2);
	convertToBcdArray (138,3,vect);
	for (i=0;i<3;i++){
		printf("nº: %d en posición %d\n", vect[i], i); ejercicio 3 y 4
	}*/
	

	uint32_t numero = 125;
	uint digitos = 3;
	
	gpioConf_t vectorGPIOs[4] = {{GPIO_20,GPIO_OUTPUT},{GPIO_21,GPIO_OUTPUT},{GPIO_22,GPIO_OUTPUT},{GPIO_23,GPIO_OUTPUT}};
	
	GPIOInit(vectorGPIOs[0].pin, vectorGPIOs[0].dir);
	GPIOInit(vectorGPIOs[1].pin, vectorGPIOs[1].dir);
	GPIOInit(vectorGPIOs[2].pin, vectorGPIOs[2].dir);
	GPIOInit(vectorGPIOs[3].pin, vectorGPIOs[3].dir);

	gpioConf_t vectorGPIO_mapeo[3] = {{GPIO_19,GPIO_OUTPUT},{GPIO_18,GPIO_OUTPUT},{GPIO_9,GPIO_OUTPUT}};
	GPIOInit(vectorGPIO_mapeo[0].pin, vectorGPIO_mapeo[0].dir);
	GPIOInit(vectorGPIO_mapeo[1].pin, vectorGPIO_mapeo[1].dir);
	GPIOInit(vectorGPIO_mapeo[2].pin, vectorGPIO_mapeo[2].dir);
	
	displayLeds(numero, digitos, vectorGPIOs, vectorGPIO_mapeo);
	
}
/*==================[end of file]============================================*/

