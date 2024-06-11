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
#include "uart_mcu.h"
/*==================[macros and definitions]=================================*/
typedef struct
{
	gpio_t pin; /*!< GPIO pin number */
	io_t dir;	/*!< GPIO direction '0' IN;  '1' OUT*/
} gpioConf_t;

/*==================[internal data definition]===============================*/
gpioConf_t arreglo[3] = {{GPIO_20, GPIO_OUTPUT}, {GPIO_21, GPIO_OUTPUT}, {GPIO_22, GPIO_OUTPUT}}; // (agua, PHA,PHB)
uint8_t pH = 0, banderaAgua, banderaPHA = 0, banderaPHB;
TaskHandle_t bombaPH_task_handle, bombaAgua_task_handle, pantalla_task_handle;
#define CONFIG_BLINK_PERIOD_MEDICION 3000000
#define CONFIG_BLINK_PERIOD_LECTURA 5000000

/*==================[internal functions declaration]=========================*/
void EncenderBomba(gpioConf_t Bomba, uint8_t Bandera)
{

	if (Bandera == 1)
	{
		GPIOOn(Bomba.pin);
	}
	else
	{
		GPIOOff(Bomba.pin);
	}
}

void bombaAgua(uint8_t *BanderaAgua)
{
	while (1)
	{
		EncenderBomba(arreglo[0], BanderaAgua);
		vTaskDelay(CONFIG_BLINK_PERIOD_MEDICION / portTICK_PERIOD_MS);
	}
}

void bombaPH(uint8_t *entrada)
{
	while (1)
	{
		pH = entrada * (3 / 14);
		if (pH < 14 && pH > 6.7)
		{
			EncenderBomba(arreglo[1].pin, banderaPHA);
		}
		else if (pH < 6 && pH > 0)
		{
			EncenderBomba(arreglo[2].pin, banderaPHB);
		}
		vTaskDelay(CONFIG_BLINK_PERIOD_MEDICION / portTICK_PERIOD_MS);
	}
}

void mandarAPantalla(void *vparameter)
{
	while (1)
	{
		if (banderaAgua == 0)
		{
			UartSendString(UART_PC, "pH: 6.5, humedad correcta");
		}
		else
		{
			UartSendString(UART_PC, "humedad incorrecta");
		}
		if (banderaPHA == 1)
		{
			UartSendString(UART_PC, "Bomba de pHA encendida");
		}
		else if (banderaPHB == 1)
		{
			UartSendString(UART_PC, "Bomba de pHB encendida");
		}
		else
		{
			UartSendString(UART_PC, "pH correcto");
		}
		vTaskDelay(CONFIG_BLINK_PERIOD_LECTURA / portTICK_PERIOD_MS);
	}
}

/*==================[external functions definition]==========================*/
void app_main(void)
{
	// Bombas[0]: Agua, Bombas[1]: PHA; Bombas[2]: PHB
	gpioConf_t Bombas[3] = {{GPIO_20, GPIO_OUTPUT}, {GPIO_21, GPIO_OUTPUT}, {GPIO_22, GPIO_OUTPUT}};

	serial_config_t serial_salida = {
		.port = UART_PC,
		.baud_rate = 115200,
		.func_p = NULL,
		.param_p = NULL};

	GPIOInit(Bombas[0].pin, Bombas[0].dir);
	GPIOInit(Bombas[1].pin, Bombas[1].dir);
	GPIOInit(Bombas[2].pin, Bombas[2].dir);

	UartInit(&serial_salida);
	xTaskCreate(&bombaPH, "bombaPH", 512, NULL, 5, &bombaPH_task_handle);
	xTaskCreate(&bombaAgua, "bombaAgua", 512, NULL, 5, &bombaAgua_task_handle);
	xTaskCreate(&mandarAPantalla, "Mostrar", 512, NULL, 5, &pantalla_task_handle);
}
/*==================[end of file]============================================*/