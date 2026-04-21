/**
 * @file stm32f4_keyboard.c
 * @brief Portable functions to interact with the keyboard FSM library. All portable functions must be implemented in this file.
 * @author Norberto de los Rios Gutierrez
 * @author Alejandro Suarez Suarez
 * @date date
 */

/* Standard C includes */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
/* HW dependent includes */
#include "port_keyboard.h"
#include "port_system.h"
#include "stm32f4_system.h"
#include "stm32f4_keyboard.h"
#include "keyboards.h"
/* Microcontroller dependent includes */

/* Typedefs --------------------------------------------------------------------*/

/* Global variables */
/* Static arrays for main keyboard (pointed by the double pointers in the struct) */

/**
 * @brief Array of GPIO ports for the rows of the main keyboard.
 *
 */
static GPIO_TypeDef *keyboard_main_row_ports[] = {
    STM32F4_KEYBOARD_MAIN_ROW_0_GPIO,
    STM32F4_KEYBOARD_MAIN_ROW_1_GPIO,
    STM32F4_KEYBOARD_MAIN_ROW_2_GPIO,
    STM32F4_KEYBOARD_MAIN_ROW_3_GPIO};

/**
 * @brief Array of GPIO pins for the rows of the main keyboard.
 *
 */
static uint8_t keyboard_main_row_pins[] = {
    STM32F4_KEYBOARD_MAIN_ROW_0_PIN,
    STM32F4_KEYBOARD_MAIN_ROW_1_PIN,
    STM32F4_KEYBOARD_MAIN_ROW_2_PIN,
    STM32F4_KEYBOARD_MAIN_ROW_3_PIN};

/**
 * @brief Array of GPIO ports for the columns of the main keyboard.
 *
 */
static GPIO_TypeDef *keyboard_main_col_ports[] = {
    STM32F4_KEYBOARD_MAIN_COL_0_GPIO,
    STM32F4_KEYBOARD_MAIN_COL_1_GPIO,
    STM32F4_KEYBOARD_MAIN_COL_2_GPIO,
    STM32F4_KEYBOARD_MAIN_COL_3_GPIO};

/**
 * @brief Array of GPIO pins for the columns of the main keyboard.
 *
 */
static uint8_t keyboard_main_col_pins[] = {
    STM32F4_KEYBOARD_MAIN_COL_0_PIN,
    STM32F4_KEYBOARD_MAIN_COL_1_PIN,
    STM32F4_KEYBOARD_MAIN_COL_2_PIN,
    STM32F4_KEYBOARD_MAIN_COL_3_PIN};

stm32f4_keyboard_hw_t 	keyboards_arr[] = 
{
    [PORT_KEYBOARD_MAIN_ID] = {
        .p_keyboard = &standard_keyboard,
        .p_row_ports = keyboard_main_row_ports, 
        .p_row_pins=keyboard_main_row_pins, 
        .p_col_ports = keyboard_main_col_ports, 
        .p_col_pins = keyboard_main_col_pins, 
        .flag_key_pressed = false, 
        .flag_row_timeout = false, 
        .col_idx_interrupt = -1, 
        .current_excited_row = -1
    },
};
/* Private functions ----------------------------------------------------------*/
/**
 * @brief Get the keyboard struct with the given ID. 
 * 
 * @param keyboard_id Keyboard ID.
 * @return stm32f4_keyboard_hw_t* Pointer to the keyboard struct. 
 * @return NULL If the keyboard ID is not valid. 
 */
stm32f4_keyboard_hw_t * _stm32f4_keyboard_get (uint8_t keyboard_id)
{
    if (keyboard_id < sizeof(keyboards_arr) / sizeof(keyboards_arr[0]))
    {
        return &keyboards_arr[keyboard_id];
    }
    else
    {
        return NULL;
    }
}

void _timer_scan_column_config()
{
    /*Enable the clock of the timer that controls the column scanning.*/
    RCC -> APB1ENR|=RCC_APB1ENR_TIM5EN;
    /*Disable the counter of the timer*/
    TIM5 -> CR1 &= ~TIM_CR1_CEN;
    /*Enable the autoreload preload*/
    TIM5 -> CR1 |= TIM_CR1_ARPE;
    /*Set the counter of the timer to 0*/
    TIM5 -> CNT = 0;

    double SystemClock = (double)SystemCoreClock;
    double t_inter = (double)PORT_KEYBOARDS_TIMEOUT_MS / 1000.0;

    double psc = round((SystemClock*t_inter)/(65535.0+1.0)-1.0);
    double arr = round((SystemClock*t_inter)/(psc + 1.0)-1.0);
    if(arr > 65535.0){
        psc += 1.0;
        arr = round((SystemClock*t_inter)/(psc + 1.0)-1.0);
    }
    TIM5 -> ARR = arr;
    TIM5 -> PSC = psc;

  
    /*trigger an update event to set the arr and psc values*/
    TIM5 -> EGR = TIM_EGR_UG;
    /*Clear the update interrupt flag*/
    TIM5 -> SR = ~TIM_SR_UIF;
    /*Enable the interrupts of the timer*/
    TIM5 -> DIER |= TIM_DIER_UIE;
    /*Set the priority of the timer interrupt */
    NVIC_SetPriority ( TIM5_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping (),2,0));
} 	

/* Public functions -----------------------------------------------------------*/
void port_keyboard_init(uint8_t keyboard_id)
{
    /* Get the keyboard sensor */
    stm32f4_keyboard_hw_t *p_keyboard = _stm32f4_keyboard_get(keyboard_id);

    /* TO-DO alumnos: */

    /* Rows configuration */
    for(uint8_t i = 0; i<p_keyboard->p_keyboard->num_rows; i++){
        stm32f4_system_gpio_config(
            p_keyboard->p_row_ports[i],
            p_keyboard->p_row_pins[i],
            STM32F4_GPIO_MODE_OUT,
            STM32F4_GPIO_PUPDR_NOPULL);
    }
    /* Columns configuration */
    for(uint8_t i = 0; i<p_keyboard->p_keyboard->num_cols; i++){
        stm32f4_system_gpio_config(
            p_keyboard->p_col_ports[i],
            p_keyboard->p_col_pins[i],
            STM32F4_GPIO_MODE_IN,
            STM32F4_GPIO_PUPDR_PULLDOWN);

        stm32f4_system_gpio_config_exti(
            p_keyboard->p_col_ports[i],
            p_keyboard->p_col_pins[i],
            STM32F4_TRIGGER_BOTH_EDGE | STM32F4_TRIGGER_ENABLE_INTERR_REQ);

        stm32f4_system_gpio_exti_enable(
            p_keyboard->p_col_pins[i],
            1,
            1);
    }
    
    /* Clean/set all configurations */
    p_keyboard->col_idx_interrupt = -1; 
    /* Configure timer */
    _timer_scan_column_config();  
}

void port_keyboard_excite_row (uint8_t keyboard_id, uint8_t row_idx)
{
    /* Get the keyboard sensor */
    stm32f4_keyboard_hw_t *p_keyboard = _stm32f4_keyboard_get(keyboard_id);
    for(uint8_t i = 0; i<p_keyboard->p_keyboard->num_rows; i++)
    {
        if(i == row_idx)
        {
            stm32f4_system_gpio_write(p_keyboard->p_row_ports[i],p_keyboard->p_row_pins[i],true);
        }
        else stm32f4_system_gpio_write(p_keyboard->p_row_ports[i],p_keyboard->p_row_pins[i],false);
    }
}	

void port_keyboard_excite_next_row 	(uint8_t keyboard_id) 
{
    stm32f4_keyboard_hw_t *p_keyboard = _stm32f4_keyboard_get(keyboard_id);
    p_keyboard->current_excited_row = (p_keyboard->current_excited_row + 1) %p_keyboard->p_keyboard->num_rows;
    port_keyboard_excite_row(keyboard_id,p_keyboard->current_excited_row);
}

bool port_keyboard_get_key_pressed_status (uint8_t keyboard_id)
{
    stm32f4_keyboard_hw_t *p_keyboard = _stm32f4_keyboard_get(keyboard_id);
    return p_keyboard->flag_key_pressed;
}

void port_keyboard_set_key_pressed_status (uint8_t keyboard_id, bool status)
{
    stm32f4_keyboard_hw_t *p_keyboard = _stm32f4_keyboard_get(keyboard_id);
    p_keyboard->flag_key_pressed = status;
}

bool port_keyboard_get_row_timeout_status (uint8_t keyboard_id)
{
    stm32f4_keyboard_hw_t *p_keyboard = _stm32f4_keyboard_get(keyboard_id);
    return p_keyboard->flag_row_timeout;
}

void port_keyboard_set_row_timeout_status (uint8_t keyboard_id, bool status)
{
    stm32f4_keyboard_hw_t *p_keyboard = _stm32f4_keyboard_get(keyboard_id);
    p_keyboard->flag_row_timeout = status;
}

char port_keyboard_get_key_value (uint8_t keyboard_id)
{
    stm32f4_keyboard_hw_t *p_keyboard = _stm32f4_keyboard_get(keyboard_id);
    return p_keyboard->p_keyboard->keys[(p_keyboard->current_excited_row*p_keyboard->p_keyboard->num_cols)+p_keyboard->col_idx_interrupt];
}

char port_keyboard_get_invalid_key_value (uint8_t keyboard_id)
{
    stm32f4_keyboard_hw_t *p_keyboard = _stm32f4_keyboard_get(keyboard_id);
    return p_keyboard->p_keyboard->null_key;
}

void port_keyboard_start_scan(uint8_t keyboard_id)
{
    port_keyboard_set_row_timeout_status(keyboard_id,true);
    //reset the CNT
    TIM5 -> CNT = 0;
    //excite the first row
    port_keyboard_excite_row(keyboard_id,0);
    //enable the interupt
    NVIC_EnableIRQ(TIM5_IRQn);
    //Enable the counter
    TIM5->CR1 |= TIM_CR1_CEN;
}

void port_keyboard_stop_scan(uint8_t keyboard_id)
{
    stm32f4_keyboard_hw_t *p_keyboard = _stm32f4_keyboard_get(keyboard_id);
    /*Disable the counter of the timer*/
    TIM5 -> CR1 &= ~TIM_CR1_CEN;
    /*Disable the timer interrupt*/
    NVIC_DisableIRQ(TIM5_IRQn);
    for(uint8_t i = 0; i<p_keyboard->p_keyboard->num_rows; i++)
    {
        stm32f4_system_gpio_write(p_keyboard->p_row_ports[i],p_keyboard->p_row_pins[i],false);
    }
}
