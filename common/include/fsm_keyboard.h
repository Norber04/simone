/**
 * @file fsm_keyboard.h
 * @brief Header for fsm_keyboard.c file.
 * @author Norberto de los Rios Gutierrez
 * @author Alejandro Suarez Suarez
 * @date fecha
 */

#ifndef FSM_KEYBOARD_H_
#define FSM_KEYBOARD_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include "fsm.h"
/* Defines and enums ----------------------------------------------------------*/

enum  	FSM_KEYBOARD {
  KEYBOARD_RELEASED_WAIT_ROW = 0,
  KEYBOARD_RELEASED_WAIT,
  KEYBOARD_PRESSED,
  KEYBOARD_PRESSED_WAIT
};
/* Typedefs --------------------------------------------------------------------*/
typedef struct 
{
    fsm_t 	f;                      /*!< Keyboard FSM */
    uint32_t 	debounce_time_ms;     /*!< Keyboard debounce time in ms */
    uint32_t 	next_timeout;         /*!< Next timeout for the anti-debounce in ms */
    uint32_t 	tick_pressed;         /*!< Number of ticks when the keyboard was pressed */
    char      key_value;            /*!< Key pressed of the last keyboard press  */
    uint8_t 	keyboard_id;          /*!< Keyboard ID*/
    char      invalid_key;          /*!< Value to indicate an invalid key */
}fsm_keyboard_t;

/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief This function creates a new keyboard FSM with the given debounce time for all keys and keyboard ID.
 * 
 * @param debounce_time_ms Debounce time in milliseconds for all keys. 
 * @param keyboard_id Keyboard ID. Must be unique. 
 * @return fsm_keyboard_t* Pointer to the keyboard FSM. 
 */
fsm_keyboard_t * 	fsm_keyboard_new (uint32_t debounce_time_ms, uint8_t keyboard_id);

void 	fsm_keyboard_destroy (fsm_keyboard_t *p_fsm);

/**
 * @brief This function is used to fire the keyboard FSM. It is used to check the transitions and execute the actions of the keyboard FSM.
 * 
 * @param p_fsm p_fsm	Pointer to an fsm_keyboard_t struct. 
 */
void 	fsm_keyboard_fire (fsm_keyboard_t *p_fsm);

/**
 * @brief Start the keyboard scanning. 
 * 
 * @param p_fsm Pointer to an fsm_keyboard_t struct. 
 */
void 	fsm_keyboard_start_scan (fsm_keyboard_t *p_fsm);

/**
 * @brief Stop the keyboard scanning. 
 * 
 * @param p_fsm Pointer to an fsm_keyboard_t struct. 
 */
void 	fsm_keyboard_stop_scan (fsm_keyboard_t *p_fsm);

/**
 * @brief Return the key pressed of the last keyboard press. 
 * 
 * @param p_fsm p_fsm	Pointer to an fsm_keyboard_t struct. 
 * @return char Key pressed of the last keyboard press. 
 */
char 	fsm_keyboard_get_key_value (fsm_keyboard_t *p_fsm);

/**
 * @brief Check if the last key pressed is valid. 
 * 
 * @param p_fsm p_fsm	Pointer to an fsm_keyboard_t struct. 
 * @return true if the field in key value is different from invalid key
 * @return false 
 */
bool 	fsm_keyboard_get_is_valid_key (fsm_keyboard_t *p_fsm);

/**
 * @brief Reset the key pressed of the last keyboard press. 
 * 
 * @param p_fsm Pointer to an fsm_keyboard_t struct. 
 */
void 	fsm_keyboard_reset_key_value (fsm_keyboard_t *p_fsm);

/**
 * @brief Check if the keyboard FSM is active, or not. 
 * 
 * @param p_fsm Pointer to an fsm_keyboard_t struct. 
 * @return true 
 * @return false allways 
 */
bool 	fsm_keyboard_check_activity (fsm_keyboard_t *p_fsm);

#endif /* FSM_KEYBOARD_H_ */