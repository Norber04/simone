/**
 * @file fsm_button.h
 * @brief Header for fsm_button.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

#ifndef FSM_BUTTON_H_
#define FSM_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum FSM_BUTTON {
    BUTTON_RELEASED = 0,
    BUTTON_RELEASED_WAIT,
    BUTTON_PRESSED,
    BUTTON_PRESSED_WAIT
};

/* Typedefs --------------------------------------------------------------------*/
typedef struct 
{
    fsm_t 	f;                      /*!< Button FSM */
    uint32_t 	debounce_time_ms;   /*!< Button debounce time in ms */
    uint32_t 	next_timeout;       /*!< Next timeout for the anti-debounce in ms */
    uint32_t 	tick_pressed;       /*!< Number of ticks when the button was pressed */
    uint32_t 	duration;           /*!< How much time the button has been pressed  */
    uint8_t 	button_id;          /*!< Button ID*/
}fsm_button_t;


/* Function prototypes and explanation -------------------------------------------------*/
fsm_button_t * 	fsm_button_new (uint32_t debounce_time_ms, uint8_t button_id);
void 	fsm_button_destroy (fsm_button_t *p_fsm);
void 	fsm_button_fire (fsm_button_t *p_fsm);
uint32_t 	fsm_button_get_duration (fsm_button_t *p_fsm);
void 	fsm_button_reset_duration (fsm_button_t *p_fsm);
uint32_t 	fsm_button_get_debounce_time_ms (fsm_button_t *p_fsm);
bool 	fsm_button_check_activity (fsm_button_t *p_fsm);

#endif