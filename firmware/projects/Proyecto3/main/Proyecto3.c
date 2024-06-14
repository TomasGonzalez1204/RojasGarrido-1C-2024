/*! @mainpage Proyecto Integrador
 *
 * @section Audiometro
 *
 * El objetivo del siguiente programa es tomar distintas mediciones de frecuencias a diferentes amplitudes
 * para poder realizar la grafica audiometrica correspondiente
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
/**
 * @def BUFFER_SIZE
 * @brief Tamaño de datos que corresponden a la señal senoidal
 */
#define BUFFER_SIZE 64
/**
 * @def CONFIG_BLINK_PERIOD_TIMER
 * @brief Periodo de timer del programa
 */
#define CONFIG_BLINK_PERIOD_TIMER 2000
/**
 * @def CONFIG_BLINK_PERIOD_SENIAL
 * @brief Periodo de timer de la senial enviada 
 */
#define CONFIG_BLINK_PERIOD_TIMER_SENIAL 125

/*==================[internal data definition]===============================*/
/** llamada a tarea Modificar señal*/
TaskHandle_t Modificar_task_handle = NULL;
/** llamada a tarea Mandar señal */
TaskHandle_t MandarSenial_task_handle = NULL;
/** arreglo que almacena una señal senoidal */
uint8_t sine_wave[BUFFER_SIZE] = {
    128, 140, 153, 165, 177, 188, 199, 209,
    218, 226, 234, 240, 246, 251, 254, 255,
    255, 254, 251, 246, 240, 234, 226, 218,
    209, 199, 188, 177, 165, 153, 140, 128,
    116, 103, 91, 79, 67, 56, 45, 35,
    26, 18, 12, 7, 3, 1, 0, 0,
    1, 3, 7, 12, 18, 26, 35, 45,
    56, 67, 79, 91, 103, 116, 128};
/** arreglos necesarios para las tareas */
uint16_t frecuencia[7] = {
    125, 250, 500, 1000,
    2000, 4000, 8000};
uint8_t volumen_audiometria[7];
/** variables enteras para el uso de los programas*/
uint8_t periodo = CONFIG_BLINK_PERIOD_TIMER_SENIAL, volumen = 1, punto = 0;
/** timer para la senial */
timer_config_t timer_Senial;
/*==================[internal functions declaration]=========================*/
/**
 * @fn void GuardarMedicion(void)
 * @brief Funcion encargada de guardar la medicion
 * @param [in]
 * @return
 */
void GuardarMedicion(void)
{
    volumen_audiometria[punto] = volumen;
    punto++;
}

/**
 * @fn void ModificarPeriodo(void)
 * @brief Funcion encargada de modificar el periodo
 * @param [in]
 * @return
 */
void ModificarPeriodo(void)
{
    GuardarMedicion();
    periodo = periodo / 2;
    printf("%d\r", periodo);
    // volumen = 0.1; // reinicia el volumen en cada frecuencia

    TimerStop(timer_Senial.timer);
    timer_Senial.period = periodo;
    TimerInit(&timer_Senial);
    TimerStart(timer_Senial.timer);

} // frecuencia=(1/periodo)*1.000.000*64(muestras)

/**
 * @fn void ModificarVolumen(void)
 * @brief Funcion encargada de modificar la amplitud (volumen)
 * @param [in]
 * @return
 */
void ModificarVolumen(void)
{
    if (volumen < 1)
        volumen = 1 + volumen;
    // printf ("%d\r",volumen);
}

/** @brief Tarea encargada de mandar la señal */
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
        // printf ("%d\r\n", sine_wave[i]);
    }
}

/*void FuncTimer(void *param)
{
    xTaskNotifyGive(AdquirirDato_task_handle);
}*/

/** @brief Tarea encargada de la funcion de las teclas */
void Teclas(void *param)
{
    while (1)
    {
        uint8_t teclas;
        teclas = SwitchesRead();
        switch (teclas)
        {
        case SWITCH_1:
            ModificarPeriodo();
            break;
        case SWITCH_2:
            ModificarVolumen();
            break;
        }
        vTaskDelay(CONFIG_BLINK_PERIOD_TIMER_SENIAL / portTICK_PERIOD_MS);
    }
}
/** @brief Tarea encargada del timer de la señal */
void FuncTimerSenial(void *param)
{
    xTaskNotifyGive(MandarSenial_task_handle);
}

/*==================[external functions definition]==========================*/
void app_main(void)
{
    ble_config_t ble_configuration = {
        "ESP_EDU_1",
        NULL};

    /*BleInit(&ble_configuration);*/

    /*timer_config_t timer_medicion = {
        .timer = TIMER_A,
        .period = CONFIG_BLINK_PERIOD_TIMER,
        .func_p = FuncTimer,
        .param_p = NULL};*/

    periodo = CONFIG_BLINK_PERIOD_TIMER_SENIAL;

    timer_Senial.timer = TIMER_A,
    timer_Senial.period = periodo,
    timer_Senial.func_p = FuncTimerSenial,
    timer_Senial.param_p = NULL;

    /*TimerInit(&timer_medicion);*/
    TimerInit(&timer_Senial);

    AnalogOutputInit();
    SwitchesInit();

    // SwitchActivInt(SWITCH_1, ModificarPeriodo, NULL);
    // SwitchActivInt(SWITCH_2, ModificarVolumen, NULL);

    xTaskCreate(&MandarSenial, "señal", 4096, NULL, 5, &MandarSenial_task_handle);
    xTaskCreate(&Teclas, "Modificar Señal", 4096, NULL, 5, &Modificar_task_handle);
    /*TimerStart(timer_medicion.timer);*/
    TimerStart(timer_Senial.timer);
    printf("inicio \r\n");
}
/*==================[end of file]============================================*/