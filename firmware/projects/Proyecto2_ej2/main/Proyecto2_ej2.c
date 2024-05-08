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
 * @author Jessenia J. Rojas Garrido (jrojasgarrido@ingenieria.uner.edu.ar)
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
#include <freertos/queue.h>
#include <freertos/timers.h>
//#include <gpio_mcu.h>

/*==================[macros and definitions]=================================*/
#define ECHO GPIO_3
#define TRIGGER GPIO_2

#define SWITCH_1_BIT 0
#define SWITCH_2_BIT 1

#define SWITCH_1_MASK (1 << SWITCH_1_BIT)
#define SWITCH_2_MASK (1 << SWITCH_2_BIT)

#define TIMER_PERIOD pdMS_TO_TICKS(1000)

/*==================[internal data definition]===============================*/
uint8_t Dist = 0;
bool Encendido = false, Hold = false;
uint8_t X;

TimerHandle_t timer;
QueueHandle_t ColaSwitches;
TaskHandle_t MostrarLed_task_handle=NULL;
TaskHandle_t MostrarDisplay_task_handle=NULL;
TaskHandle_t FuncionTeclas_task_handle=NULL;
TaskHandle_t llamadaTimer_task_handle=NULL;
TaskHandle_t Interruptor_task_handle=NULL;
/*==================[internal functions declaration]=========================*/
void MostrarConLed(void);
void MostrarConDisplay(void);
void FuncionTeclas(void);
void llamadaTimer(TimerHandle_t pxTimer);
void controladorInterrupciones(void);
/*==================[internal functions definition]=========================*/
void MostrarConLed(void) { //igual a ejercicio 1
    Dist = HcSr04ReadDistanceInCentimeters();

    if (Dist < 10) {
        LedsOffAll();
    } else if (Dist >= 10 && Dist < 20) {
        LedOn(1);
    } else if (Dist >= 20 && Dist < 30) {
        LedOn(2);
        LedOn(1);
    } else if (Dist >= 30 && Dist < 40) {
        LedOn(3);
        LedOn(2);
        LedOn(1);
    }
}

void MostrarConDisplay(void) {//igual a ejercicio 1
    LcdItsE0803Write(Dist);
}

void FuncionTeclas(void) {
    BaseType_t altaPrioridad = pdFALSE;
    uint32_t switches;

    while (1) {
        if (xQueueReceiveFromISR(ColaSwitches, &switches, &altaPrioridad) == pdPASS) {
            if (switches & SWITCH_1_MASK) {
                Encendido = !Encendido;
            }
            if (switches & SWITCH_2_MASK) {
                Hold = !Hold;
            }
        }

        portYIELD_FROM_ISR(altaPrioridad);
    }
}

void llamadaTimer(TimerHandle_t pxTimer) {
    if (Encendido) {
        HcSr04ReadDistanceInCentimeters();
    } else {
        LcdItsE0803Off();
        LedsOffAll();
    }

    if (Hold) {
        MostrarConDisplay();
    } else {
        LcdItsE0803Write(X);
        LedsOffAll();
    }
}

void Interruptor(void) {
    BaseType_t altaPrioridad = pdFALSE;
    uint32_t switches = SwitchesRead();

    xQueueSendFromISR(ColaSwitches, &switches, &altaPrioridad);

    portYIELD_FROM_ISR(altaPrioridad);
}

/*==================[external functions definition]==========================*/
void app_main(void) {
    LedsInit();
    HcSr04Init(ECHO, TRIGGER);
    SwitchesInit();
    LcdItsE0803Init();
        xTaskCreate(&MostrarConLed, "TAREA LEDS", 512, NULL, 5, &MostrarLed_task_handle);
        xTaskCreate(&MostrarConDisplay, "TAREA DISPLAY", 512, NULL, 5, &MostrarDisplay_task_handle);
        xTaskCreate(&FuncionTeclas, "FUNCION TECLAS", 512, NULL, 5, &FuncionTeclas_task_handle);
        xTaskCreate(&llamadaTimer, "llamada Timer", 512, NULL, 5, &llamadaTimer_task_handle);
        xTaskCreate(&Interruptor, "llamada Timer", 512, NULL, 5, &Interruptor_task_handle);
    }