/**
 * @file fsm_keyboard.c
 * @brief Keyboard sensor FSM main file.
 * @author Norberto de los Rios Gutierrez
 * @author Alejandro Suarez Suarez
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdio.h>
/* HW dependent includes */
#include "port_system.h"
#include "port_keyboard.h"
/* Project includes */
#include "fsm.h"
#include "fsm_keyboard.h"
#include "keyboards.h"
/* Typedefs --------------------------------------------------------------------*/

/* Private functions -----------------------------------------------------------*/

/* State machine input or transition functions */

/**
 * @brief Check if the timeout to activate a new row and scan columns has passed. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_keyboard_t.
 * @return true 
 * @return false 
 */
static bool check_row_timeout (fsm_t *p_this)
{
    fsm_keyboard_t *p_keyboard =(fsm_keyboard_t *)p_this;
    return port_keyboard_get_row_timeout_status(p_keyboard->keyboard_id);
}

/**
 * @brief Check if the keyboard has been pressed. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_keyboard_t. 
 * @return true the keyboard is pressed
 * @return false 
 */
static bool check_keyboard_pressed(fsm_t *p_this)
{
    fsm_keyboard_t *p_keyboard =(fsm_keyboard_t *)p_this;
    return port_keyboard_get_key_pressed_status(p_keyboard->keyboard_id);
}
/**
 * @brief Check if the keyboard has been released. 
 * 
 * @param p_this 	Pointer to an fsm_t struct than contains an fsm_keyboard_t. 
 * @return true the keyboard has been released
 * @return false 
 */
static bool check_keyboard_released(fsm_t *p_this)
{
    fsm_keyboard_t *p_keyboard =(fsm_keyboard_t *)p_this;
    return !port_keyboard_get_key_pressed_status(p_keyboard->keyboard_id);
}
/**
 * @brief Check if the debounce-time has passed. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_keyboard_t. 
 * @return true if the current system tick is higher than the field next timeout
 * @return false 
 */
static bool check_timeout(fsm_t *p_this)
{
    fsm_keyboard_t *p_keyboard =(fsm_keyboard_t *)p_this;
    return (port_system_get_millis()>p_keyboard->next_timeout);
}

/* State machine output or action functions */

/**
 * @brief Clean the row timeout flag and update the row to be excited. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_keyboard_t
 */
void do_excite_next_row(fsm_t *p_this)
{
    fsm_keyboard_t *p_keyboard =(fsm_keyboard_t *)p_this;
    port_keyboard_set_row_timeout_status(p_keyboard->keyboard_id,false);
    port_keyboard_excite_next_row(p_keyboard->keyboard_id);
}

/**
 * @brief Store the system tick when the keyboard was pressed. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_keyboard_t. 
 */
static void do_store_tick_pressed(fsm_t *p_this)
{
    fsm_keyboard_t *p_keyboard =(fsm_keyboard_t *)p_this;
    p_keyboard->tick_pressed = port_system_get_millis();
    p_keyboard->next_timeout = port_system_get_millis() + p_keyboard->debounce_time_ms;
}
/**
 * @brief Store the key value of the keyboard press. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_keyboard_t. 
 */
static void do_set_key_value(fsm_t *p_this)
{
    fsm_keyboard_t *p_keyboard =(fsm_keyboard_t *)p_this;
    p_keyboard->next_timeout = port_system_get_millis() + p_keyboard->debounce_time_ms;
    p_keyboard->key_value = port_keyboard_get_key_value(p_keyboard->keyboard_id);
}


fsm_trans_t fsm_trans_keyboard[] = {
    {KEYBOARD_RELEASED_WAIT_ROW,    check_row_timeout,      KEYBOARD_RELEASED_WAIT_ROW, do_excite_next_row},
    {KEYBOARD_RELEASED_WAIT_ROW,    check_keyboard_pressed, KEYBOARD_PRESSED_WAIT,      do_store_tick_pressed},
    {KEYBOARD_PRESSED_WAIT,         check_timeout,          KEYBOARD_PRESSED,           NULL},
    {KEYBOARD_PRESSED,              check_keyboard_released,KEYBOARD_RELEASED_WAIT,     do_set_key_value},
    {KEYBOARD_RELEASED_WAIT,        check_timeout,          KEYBOARD_RELEASED_WAIT_ROW, NULL},
    {-1,                            NULL,                   -1,                         NULL}
};

/* Other auxiliary functions */
/**
 * @brief This function initializes the default values of the FSM struct and calls to the port to initialize the associated HW given the ID.
 * 
 * @param p_fsm_keyboard Pointer to the keyboard FSM. 
 * @param debounce_time Anti-debounce time in milliseconds 
 * @param keyboard_id Unique keyboard identifier number 
 */
void fsm_keyboard_init(fsm_keyboard_t *p_fsm_keyboard, uint32_t debounce_time,uint8_t keyboard_id)
{
    // Initialize the FSM
    fsm_init(&p_fsm_keyboard->f, fsm_trans_keyboard);

    // Initialize the fields of the FSM structure
    p_fsm_keyboard->debounce_time_ms = debounce_time;
    p_fsm_keyboard->keyboard_id = keyboard_id;
    p_fsm_keyboard->tick_pressed = 0;

    p_fsm_keyboard->invalid_key = port_keyboard_get_invalid_key_value(p_fsm_keyboard->keyboard_id);
    p_fsm_keyboard->key_value = port_keyboard_get_invalid_key_value(p_fsm_keyboard->keyboard_id);

    port_keyboard_init(p_fsm_keyboard->keyboard_id);
}

/* Public functions -----------------------------------------------------------*/
fsm_keyboard_t *fsm_keyboard_new(uint32_t debounce_time_ms, uint8_t keyboard_id)
{
    fsm_keyboard_t *p_fsm_keyboard = malloc(sizeof(fsm_keyboard_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_keyboard_init(p_fsm_keyboard,debounce_time_ms, keyboard_id);                  /* Initialize the FSM */
    return p_fsm_keyboard;
}

void fsm_keyboard_start_scan(fsm_keyboard_t *p_fsm)
{
    port_keyboard_start_scan(p_fsm->keyboard_id);
}

void fsm_keyboard_stop_scan(fsm_keyboard_t *p_fsm)
{
    port_keyboard_stop_scan(p_fsm->keyboard_id);
}

char fsm_keyboard_get_key_value(fsm_keyboard_t *p_fsm) 
{
    return p_fsm->key_value;
}

bool fsm_keyboard_get_is_valid_key(fsm_keyboard_t *p_fsm)
{
    return p_fsm->key_value != p_fsm->invalid_key;
}

void fsm_keyboard_reset_key_value (fsm_keyboard_t *p_fsm) 	
{
    p_fsm->key_value = p_fsm->invalid_key;
}

void fsm_keyboard_fire(fsm_keyboard_t *p_fsm)
{
    fsm_fire(&p_fsm->f);
}

bool fsm_keyboard_check_activity (fsm_keyboard_t *p_fsm)
{
    return false;
}

void fsm_keyboard_destroy (fsm_keyboard_t *p_fsm)
{
    fsm_destroy(&p_fsm->f);
}
