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
 * @author Albano Peñalva (albano.penalva@uner.edu.ar)
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
typedef struct
{
	gpio_t pin;			/*!< GPIO pin number */
	io_t dir;			/*!< GPIO direction '0' IN;  '1' OUT*/
}  gpioConf_t;
/*==================[internal data definition]===============================*/
gpioConf_t arreglo [3] = {{GPIO_20,GPIO_OUTPUT},{GPIO_21,GPIO_OUTPUT},{GPIO_22,GPIO_OUTPUT}};// (agua, PHA,PHB)
uint8_t entrada=0,pH=0,banderaPHA=0,banderaPHB;
#define CONFIG_BLINK_PERIOD_MEDICION 3000000;
/*==================[internal functions declaration]=========================*/
void EncenderBomba(gpioConf_t* Bomba,uint8_t Bandera)
{
	while(1){
		if(Bandera==1) 
			GPIOOn(Bomba);
		else GPIOOff (Bomba);
	vTaskDelay (CONFIG_BLINK_PERIOD/portTICK_PERIOD_MS);
	}
}

void bombaPH (void *param)
{
	while(1)
	{
		pH=entrada*(3/14);
		if(pH<14 && pH>6.7){
			EncenderBomba(arreglo[1],banderaPHA);}
		else if(pH<6 && pH>0){
			EncenderBomba(arreglo[2],banderaPHB);}
	}
}
/*==================[external functions definition]==========================*/
void app_main(void){
	//Bombas[0]: Agua, Bombas[1]: PHA; Bombas[2]: PHB
	gpioConf_t Bombas[3] = {{GPIO_20,GPIO_OUTPUT},{GPIO_21,GPIO_OUTPUT},{GPIO_22,GPIO_OUTPUT}};
    GPIOInit(Bombas[0].pin,Bombas[0].dir);
	GPIOInit(Bombas[1].pin,Bombas[1].dir);
	GPIOInit(Bombas[2].pin,Bombas[2].dir);


}
/*==================[end of file]============================================*/