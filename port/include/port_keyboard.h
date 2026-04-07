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
/**
 * @brief Configure the HW specifications of a given keyboard. 
 * 
 * @param keyboard_id Keyboard ID. This index is used to select the element of the keyboards_arr[] array 
 */
void port_keyboard_init (uint8_t keyboard_id);

/**
 * @brief Set the given row to high and lower the others.
 * 
 * @param keyboard_id ID of the keyboard to be scanned. 
 * @param row_idx Index of the row to be excited. 
 */
void port_keyboard_excite_row (uint8_t keyboard_id, uint8_t row_idx);

/**
 * @brief Start the scanning of a keyboard. 
 * 
 * @param keyboard_id Keyboard ID. This index is used to get the correct keyboard status struct. 
 */
void port_keyboard_start_scan (uint8_t keyboard_id);

/**
 * @brief Stop the scanning of a keyboard. 
 * 
 * @param keyboard_id Keyboard ID. This index is used to get the correct keyboard status struct. 
 */
void port_keyboard_stop_scan (uint8_t keyboard_id);

/**
 * @brief Update the row to be excited. 
 * 
 * @param keyboard_id Keyboard ID. This index is used to get the correct keyboard status struct. 
 */
void port_keyboard_excite_next_row (uint8_t keyboard_id);

/**
 * @brief Return the status of the keyboard (pressed or not). 
 * 
 * @param keyboard_id Keyboard ID. This index is used to get the correct keyboard status struct.
 * @return true If the keyboard has been pressed 
 * @return false If the keyboard has not been pressed 
 */
bool port_keyboard_get_key_pressed_status (uint8_t keyboard_id);

/**
 * @brief Set the status of the keyboard (pressed or not). 
 * 
 * @param keyboard_id Keyboard ID.
 * @param status the status to set on the key
 */
void port_keyboard_set_key_pressed_status (uint8_t keyboard_id, bool status);

/**
 * @brief Return the status of the column timeout flag. 
 * 
 * @param keyboard_id Keyboard ID. This index is used to get the correct keyboard status struct. 
 * @return true If the column timeout has occurred 
 * @return false If the column timeout has not occurred 
 */
bool port_keyboard_get_row_timeout_status (uint8_t keyboard_id);

/**
 * @brief Set the status of the row timeout flag. 
 * 
 * @param keyboard_id Keyboard ID. This index is used to get the correct keyboard status struct. 
 * @param status 	New status of the row timeout flag. 
 */
void port_keyboard_set_row_timeout_status (uint8_t keyboard_id, bool status);

/**
 * @brief Return the char representing the key pressed of a given keyboard based on its row that is being excited.
 * 
 * @param keyboard_id Keyboard ID. This index is used to get the correct keyboard status struct. 
 * @return char Key value of the key pressed. 
 */
char port_keyboard_get_key_value (uint8_t keyboard_id);

/**
 * @brief This function retrieves the null key value defined in the keyboard layout.
 * 
 * @param keyboard_id Keyboard ID.
 * @return char the null key value of a given keyboard. 
 */
char port_keyboard_get_invalid_key_value (uint8_t keyboard_id);

#endif /* PORT_KEYBOARD_H_ */