/**
 * @file port_keyboard.h
 * @brief Header for the portable functions to interact with the HW of the keyboards. The functions must be implemented in the platform-specific code.
 * @author Norberto de los Rios Gutierrez
 * @author Alejandro Suarez Suarez
 * @date fecha
 */
#ifndef PORT_KEYBOARD_H_
#define PORT_KEYBOARD_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
/* Defines and enums ----------------------------------------------------------*/
#define PORT_KEYBOARD_MAIN_ID 0
#define PORT_KEYBOARDS_TIMEOUT_MS 25
#define PORT_KEYBOARD_MAIN_DEBOUNCE_TIME_MS 150

enum  	PORT_KEYBOARD_COL_IDS {
  PORT_KEYBOARD_COL_0,
  PORT_KEYBOARD_COL_1,
  PORT_KEYBOARD_COL_2,
  PORT_KEYBOARD_COL_3
};

/* Function prototypes and explanation -------------------------------------------------*/
void port_keyboard_init (uint8_t keyboard_id);
void port_keyboard_excite_row (uint8_t keyboard_id, uint8_t row_idx);
void port_keyboard_start_scan (uint8_t keyboard_id);
void port_keyboard_stop_scan (uint8_t keyboard_id);
void port_keyboard_excite_next_row (uint8_t keyboard_id);
bool port_keyboard_get_key_pressed_status (uint8_t keyboard_id);
void port_keyboard_set_key_pressed_status (uint8_t keyboard_id, bool status);
bool port_keyboard_get_row_timeout_status (uint8_t keyboard_id);
void port_keyboard_set_row_timeout_status (uint8_t keyboard_id, bool status);
char port_keyboard_get_key_value (uint8_t keyboard_id);
char port_keyboard_get_invalid_key_value (uint8_t keyboard_id);

#endif /* PORT_KEYBOARD_H_ */