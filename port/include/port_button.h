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

/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Return the status of the button
 * 
 * @param button_id button to read the status
 * @return true if the button has been pressed
 * @return false if the button has not been pressed
 */
bool port_button_get_pressed(uint8_t button_id);

/**
 * @brief Configures the button given as input, interruprions in rysing and falling edges.
 * 
 * @param button_id button given to configure
 */
void port_button_init(uint8_t button_id);
#endif