/**
 * @file port_button.h
 * @brief Header for the portable functions to interact with the HW of the buttons. The functions must be implemented in the platform-specific code.
 * @author Norberto de los Rios Gutierrez
 * @author Alejandro Suarez Suarez
 * @date fecha
 */

#ifndef PORT_BUTTON_H_
#define PORT_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
// Define here all the button identifiers that are used in the system
#define PORT_USER_BUTTON_ID 0
#define PORT_USER_BUTTON_DEBOUNCE_TIME_MS

enum FSM_BUTTON
{
    BUTTON_RELEASED = 0, /*!< */
    BUTTON_RELEASED_WAIT, /*!< */
    BUTTON_PRESSED, /*!< */
    BUTTON_PRESSED_WAIT /*!< */
};


/* Function prototypes and explanation -------------------------------------------------*/

void check_button_pressed();
void do_store_tick_pressed();

void check_timeout();

void check_button_released();
void do_set_duration();
#endif