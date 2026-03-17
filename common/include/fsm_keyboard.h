/**
 * @file fsm_keyboard.h
 * @brief Header for fsm_keyboard.c file.
 * @author alumno1
 * @author alumno2
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
    uint32_t 	debounce_time_ms;   /*!< Keyboard debounce time in ms */
    uint32_t 	next_timeout;       /*!< Next timeout for the anti-debounce in ms */
    uint32_t 	tick_pressed;       /*!< Number of ticks when the keyboard was pressed */
    char        key_value;          /*!< Char of the key */
    uint32_t 	keyboard_id;        /*!< Keyboard ID*/
    char        invalid_key
}fsm_keyboard_t;

/* Function prototypes and explanation -------------------------------------------------*/
fsm_keyboard_t * 	fsm_keyboard_new (uint32_t debounce_time_ms, uint8_t keyboard_id);
void 	fsm_keyboard_destroy (fsm_keyboard_t *p_fsm);
void 	fsm_keyboard_fire (fsm_keyboard_t *p_fsm);
void 	fsm_keyboard_start_scan (fsm_keyboard_t *p_fsm);
void 	fsm_keyboard_stop_scan (fsm_keyboard_t *p_fsm);
char 	fsm_keyboard_get_key_value (fsm_keyboard_t *p_fsm);
bool 	fsm_keyboard_get_is_valid_key (fsm_keyboard_t *p_fsm);
void 	fsm_keyboard_reset_key_value (fsm_keyboard_t *p_fsm);
bool 	fsm_keyboard_check_activity (fsm_keyboard_t *p_fsm);

#endif /* FSM_KEYBOARD_H_ */