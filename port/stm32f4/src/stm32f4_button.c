/**
 * @file stm32f4_button.c
 * @brief Portable functions to interact with the button FSM library. All portable functions must be implemented in this file.
 * @author n.delosrios@alumnos.upm.es   
 * @author alejandro.suarez@alumnos.upm.es
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdio.h>
/* HW dependent includes */
#include "port_button.h" // Used to get general information about the buttons (ID, etc.)
#include "port_system.h" // Used to get the system tick
#include "stm32f4_button.h"
#include "stm32f4_system.h"


/* Microcontroller dependent includes */
// TO-DO alumnos: include the necessary files to interact with the GPIOs


/* Global variables ------------------------------------------------------------*/


stm32f4_button_hw_t buttons_arr[] = {
    [PORT_USER_BUTTON_ID] = {.p_port = STM32F4_USER_BUTTON_GPIO,
                             .pin = STM32F4_USER_BUTTON_PIN,
                             .pupd_mode = STM32F4_GPIO_PUPDR_NOPULL},
};

/* Private functions ----------------------------------------------------------*/
/**
 * @brief Get the button status struct with the given ID.
 *
 * @param button_id Button ID.
 *
 * @return Pointer to the button state struct.
 * @return NULL If the button ID is not valid.
 */
stm32f4_button_hw_t *_stm32f4_button_get(uint8_t button_id)
{
    // Return the pointer to the button with the given ID. If the ID is not valid, return NULL.
    if (button_id < sizeof(buttons_arr) / sizeof(buttons_arr[0]))
    {
        return &buttons_arr[button_id];
    }
    else
    {
        return NULL;
    }
}

/* Public functions -----------------------------------------------------------*/
void port_button_init(uint8_t button_id)
{
    // Retrieve the button struct using the private function and the button ID
    stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);


    /* TO-DO alumnos */
    // configures the button as input with no pullup neither pulldown
    stm32f4_system_gpio_config( p_button->p_port,
                                p_button->pin, 
                                STM32F4_GPIO_MODE_IN, 
                                STM32F4_GPIO_PUPDR_NOPULL);

    // allowis the interruption mode in both rising and falling edges, enables the interruption request
    stm32f4_system_gpio_config_exti(p_button->p_port, 
                                    p_button->pin, 
                                    STM32F4_TRIGGER_BOTH_EDGE | STM32F4_TRIGGER_ENABLE_INTERR_REQ);
    //set the priority to 1 and subpriority to 0
    stm32f4_system_gpio_exti_enable(p_button->pin,1,0);
}

bool port_button_get_pressed(uint8_t button_id)
{
    // Retrieve the button struct using the private function and the button ID
    stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);

    return p_button->flag_pressed;
}
