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
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "analog_io_mcu.h"
#include "uart_mcu.h"
#include "timer_mcu.h"

/*==================[macros and definitions]=================================*/
#define BUFFER_SIZE 231
#define CONFIG_BLINK_PERIOD_TIMER 2000
#define CONFIG_BLINK_PERIOD_TIMER_ECG 4000
/*==================[internal data definition]===============================*/
TaskHandle_t main_task_handle = NULL;
TaskHandle_t AdquirirDato_task_handle=NULL;
TaskHandle_t ConvertirSenial_task_handle=NULL;
const char ecg[BUFFER_SIZE] = {
    76, 77, 78, 77, 79, 86, 81, 76, 84, 93, 85, 80,
    89, 95, 89, 85, 93, 98, 94, 88, 98, 105, 96, 91,
    99, 105, 101, 96, 102, 106, 101, 96, 100, 107, 101,
    94, 100, 104, 100, 91, 99, 103, 98, 91, 96, 105, 95,
    88, 95, 100, 94, 85, 93, 99, 92, 84, 91, 96, 87, 80,
    83, 92, 86, 78, 84, 89, 79, 73, 81, 83, 78, 70, 80, 82,
    79, 69, 80, 82, 81, 70, 75, 81, 77, 74, 79, 83, 82, 72,
    80, 87, 79, 76, 85, 95, 87, 81, 88, 93, 88, 84, 87, 94,
    86, 82, 85, 94, 85, 82, 85, 95, 86, 83, 92, 99, 91, 88,
    94, 98, 95, 90, 97, 105, 104, 94, 98, 114, 117, 124, 144,
    180, 210, 236, 253, 227, 171, 99, 49, 34, 29, 43, 69, 89,
    89, 90, 98, 107, 104, 98, 104, 110, 102, 98, 103, 111, 101,
    94, 103, 108, 102, 95, 97, 106, 100, 92, 101, 103, 100, 94, 98,
    103, 96, 90, 98, 103, 97, 90, 99, 104, 95, 90, 99, 104, 100, 93,
    100, 106, 101, 93, 101, 105, 103, 96, 105, 112, 105, 99, 103, 108,
    99, 96, 102, 106, 99, 90, 92, 100, 87, 80, 82, 88, 77, 69, 75, 79,
    74, 67, 71, 78, 72, 67, 73, 81, 77, 71, 75, 84, 79, 77, 77, 76, 76,
};

/*==================[internal functions declaration]=========================*/
void AdquirirDato(void *param)
{
	while (1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		uint16_t valor;
		AnalogInputReadSingle(CH1, &valor);		
		UartSendString(UART_PC, (char*) UartItoa(valor, 10));
		UartSendString(UART_PC, "\r");
	}
}

void ConvertirSenial(void *param)
{
	uint8_t i=0;
	while (1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		i++;
		AnalogOutputWrite(ecg[i]);
		if(i==BUFFER_SIZE)
		{
			i=0;
		}
	}
}

void FuncTimer(void* param)
{
    xTaskNotifyGive(AdquirirDato_task_handle);   
}

void FuncTimerECG(void* param)
{
    xTaskNotifyGive(ConvertirSenial_task_handle);   
}



/*==================[external functions definition]==========================*/
void app_main(void)
{
	timer_config_t timer_medicion = {
        .timer = TIMER_A,
        .period = CONFIG_BLINK_PERIOD_TIMER,
        .func_p = FuncTimer,
        .param_p = NULL
    };

	timer_config_t timer_ecg = {
        .timer = TIMER_B,
        .period = CONFIG_BLINK_PERIOD_TIMER_ECG,
        .func_p = FuncTimerECG,
        .param_p = NULL
    };

	serial_config_t serial_salida = {
        .port = UART_PC,
        .baud_rate = 115200,
        .func_p = NULL,
        .param_p = NULL
    };
    
	analog_input_config_t SenialEntrada ={
	.input = CH1,
	.mode = ADC_SINGLE,
	.func_p = NULL,
	.param_p = NULL,
	.sample_frec = NULL
};

TimerInit(&timer_medicion);
TimerInit(&timer_ecg);
UartInit(&serial_salida);
AnalogInputInit(&SenialEntrada);
AnalogOutputInit();

xTaskCreate(&AdquirirDato, "Dato", 512, NULL, 5, &AdquirirDato_task_handle);
xTaskCreate(&ConvertirSenial, "ECG", 512, NULL, 5, &ConvertirSenial_task_handle);
TimerStart(timer_medicion.timer);
TimerStart(timer_ecg.timer);

}
/*==================[end of file]============================================*/