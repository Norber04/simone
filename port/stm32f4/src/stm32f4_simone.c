/**
 * @file stm32f4_simone.c
 * @brief Portable functions to interact with the Simone FSM library. All portable functions must be implemented in this file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Standard C includes */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h> // Used to compute the timer ARR and PSC values

/* HW dependent includes */
#include "stm32f4xx.h"

/* Microcontroller dependent includes */
#include "stm32f4_simone.h"
#include "stm32f4_system.h"

/* Global variables */
stm32f4_simone_hw_t simone_hw;

/* Private functions ----------------------------------------------------------*/
static void _timer_simone_setup()
{
    /*Enable the clock of the timer that controls the column scanning.*/
    RCC -> APB1ENR|=RCC_APB1ENR_TIM3EN;
    /*Disable the counter of the timer*/
    TIM3 -> CR1 &= ~TIM_CR1_CEN;
    /*Enable the autoreload preload*/
    TIM3 -> CR1 |= TIM_CR1_ARPE;

    /*Clear the update interrupt flag*/
    TIM3 -> SR = ~TIM_SR_UIF;
    /*Enable the interrupts of the timer*/
    TIM3 -> DIER |= TIM_DIER_UIE;
    /*Set the priority of the timer interrupt */
    NVIC_SetPriority ( TIM3_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping (),3,0));
    //TODO REVISAR SI ESTO ESTA BIEN
}

void port_simone_set_timer_timeout(uint32_t duration_ms)
{
    /*Disable the counter of the timer*/
    TIM3 -> CR1 &= ~TIM_CR1_CEN;
    /*Set the counter of the timer to 0*/
    TIM3 -> CNT = 0;

    double SystemClock = (double)SystemCoreClock;
    double t_inter = (double)duration_ms / 1000.0;

    double psc = round((SystemClock*t_inter)/(65535.0+1.0)-1.0);
    double arr = round((SystemClock*t_inter)/(psc + 1.0)-1.0);
    if(arr > 65535.0){
        psc += 1.0;
        arr = round((SystemClock*t_inter)/(psc + 1.0)-1.0);
    }
    TIM3 -> ARR = arr;
    TIM3 -> PSC = psc;

    /*trigger an update event to set the arr and psc values*/
    TIM3 -> EGR = TIM_EGR_UG;
    /*Clear the update interrupt flag*/
    TIM3 -> SR = ~TIM_SR_UIF;
    /*Enable the interrupts of the timer*/
    TIM3 -> DIER |= TIM_DIER_UIE;

    simone_hw.flag_timer_timeout = false;

    /*Enable the rimer*/
    TIM3->CR1 |= TIM_CR1_CEN;
}

void port_simone_stop_timer(void)
{
    TIM3->CR1 &= ~TIM_CR1_CEN;
    TIM3->DIER &= ~TIM_DIER_UIE;
}

bool port_simone_get_timeout_status(void)
{
    return simone_hw.flag_timer_timeout;
}

void port_simone_set_timeout_status(bool status)
{
    simone_hw.flag_timer_timeout = status;
}

void port_simone_init(void)
{
    /* TODO students: */
    // 1. Reset the flag of the timer timeout
    // 2. Configure the timer that controls the duration of the different aspects of the game
}