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
 * | 22/05/2024 | Proyecto Integrador                        |
 *
 * @author Jessenia Jazmin Rojas Garrido(jrojasgarrido@ingenieria.uner.edu.ar)
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
#include "neopixel_stripe.h"
#include "ble_mcu.h"
#include "switch.h"
/*==================[macros and definitions]=================================*/
#define BUFFER_SIZE 64
#define CONFIG_BLINK_PERIOD_TIMER 2000
#define CONFIG_BLINK_PERIOD_TIMER_SENIAL 125
#define LED_BT LED_1
/*==================[internal data definition]===============================*/
TaskHandle_t main_task_handle = NULL;
TaskHandle_t AdquirirDato_task_handle = NULL;
TaskHandle_t MandarSenial_task_handle = NULL;
uint8_t sine_wave[BUFFER_SIZE] = {
    128, 140, 153, 165, 177, 188, 199, 209,
    218, 226, 234, 240, 246, 251, 254, 255,
    255, 253, 250, 245, 240, 233, 225, 215,
    205, 194, 182, 170, 157, 144, 131, 119,
    106, 94, 83, 72, 62, 53, 45, 38,
    32, 27, 23, 20, 18, 17, 17, 18,
    20, 23, 27, 32, 38, 45, 53, 62,
    72, 83, 94, 106, 119, 131, 144, 157};

uint8_t periodo, volumen;
uint8_t audiometria[7][2];
timer_config_t timer_Senial;
/*==================[internal functions declaration]=========================*/

void ModificarPeriodo(void)
{
    periodo = periodo / 2;

    volumen = 0.1; // reinicia el volumen en cada frecuencia

    TimerStop(timer_Senial.timer);
    timer_Senial.period = periodo;
    TimerInit(&timer_Senial);
    TimerStart(timer_Senial.timer);
} // frecuencia=(1/periodo)*1.000.000*64(muestras)

void ModificarVolumen(void)
{
    if (volumen < 1)
        volumen = 0.1 + volumen;
}

void MandarSenial(void *param)
{
    uint8_t i = 0;
    while (1)
    {

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        i++;
        AnalogOutputWrite(volumen * sine_wave[i]);
        if (i == BUFFER_SIZE)
        {
            i = 0;
        }
    }
}

void FuncTimer(void *param)
{
    xTaskNotifyGive(AdquirirDato_task_handle);
}

void FuncTimerSenial(void *param)
{
    xTaskNotifyGive(MandarSenial_task_handle);
}

/*==================[external functions definition]==========================*/
void app_main(void)
{
    ble_config_t ble_configuration = {
        "ESP_EDU_1",
        NULL
    };

    BleInit(&ble_configuration);

    timer_config_t timer_medicion = {
        .timer = TIMER_A,
        .period = CONFIG_BLINK_PERIOD_TIMER,
        .func_p = FuncTimer,
        .param_p = NULL};

    periodo = CONFIG_BLINK_PERIOD_TIMER_SENIAL;

    timer_Senial.timer = TIMER_B,
    timer_Senial.period = periodo,
    timer_Senial.func_p = FuncTimerSenial,
    timer_Senial.param_p = NULL;

    analog_input_config_t SenialSalida = {
        .input = CH1,
        .mode = ADC_SINGLE,
        .func_p = NULL,
        .param_p = NULL,
        .sample_frec = NULL};

    TimerInit(&timer_medicion);
    TimerInit(&timer_Senial);
    AnalogInputInit(&SenialSalida);
    AnalogOutputInit();

    SwitchActivInt(SWITCH_1, ModificarPeriodo, NULL);
    SwitchActivInt(SWITCH_2, ModificarVolumen, NULL);

    xTaskCreate(&MandarSenial, "senial", 512, NULL, 5, &MandarSenial_task_handle);
    TimerStart(timer_medicion.timer);
    TimerStart(timer_Senial.timer);
}
/*==================[end of file]============================================*/