/**
 * @file fsm_button.c
 * @brief Button FSM main file.
 * @author Norberto de los Rios Gutierrez
 * @author Alejandro Suarez Suarez
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
/* HW dependent includes */
#include "port_button.h"
#include "port_system.h"
#include "fsm.h"
/* Project includes */
#include "fsm_button.h"

/* State machine input or transition functions */

/**
 * @brief Check if the button has been pressed
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_button_t
 * @return true: the button is pressed
 * @return false: the button is not pressed 
 */
static bool check_button_pressed (fsm_t *p_this)
{
    fsm_button_t *p_button =(fsm_button_t *)p_this;
    return port_button_get_pressed(p_button->button_id);
}

/**
 * @brief Check if the button has been released
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_button_t
 * @return true: the button has been released 
 * @return false: the button has not been realeased 
 */
static bool check_button_released (fsm_t *p_this)
{
    fsm_button_t *p_button =(fsm_button_t *)p_this;
    return !port_button_get_pressed(p_button->button_id);
}
/**
 * @brief Check if the debounce-time has passed. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_button_t
 * @return true 
 * @return false 
 */
static bool check_timeout(fsm_t *p_this)
{
    fsm_button_t *p_button =(fsm_button_t *)p_this;
    return port_system_get_millis() > p_button->next_timeout;
}


/* State machine output or action functions */
/**
 * @brief Store the system tick when the button was pressed. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_button_t. 
 */
static void do_store_tick_pressed(fsm_t * p_this)
{
    fsm_button_t *p_button =(fsm_button_t *)p_this;
    p_button->tick_pressed = port_system_get_millis();
    p_button->next_timeout = port_system_get_millis() + p_button->debounce_time_ms;

}

/**
 * @brief Store the duration of the button press. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_button_t. 
 */
static void do_set_duration(fsm_t *p_this)
{
    fsm_button_t *p_button =(fsm_button_t *)p_this;
    p_button->duration = port_system_get_millis() - p_button->tick_pressed;
    p_button->next_timeout = port_system_get_millis() + p_button->debounce_time_ms;
}


static fsm_trans_t fsm_trans_button[] = {
    {BUTTON_RELEASED,   check_button_pressed,  BUTTON_PRESSED_WAIT,    do_store_tick_pressed},
    {BUTTON_PRESSED_WAIT,   check_timeout,   BUTTON_PRESSED, NULL},
    {BUTTON_PRESSED,    check_button_released,  BUTTON_RELEASED_WAIT,    do_set_duration},
    {BUTTON_RELEASED_WAIT,  check_timeout, BUTTON_RELEASED, NULL},
    {-1,    NULL,   -1,     NULL}
};

/* Other auxiliary functions */
void fsm_button_init(fsm_button_t *p_fsm_button, uint32_t debounce_time, uint8_t button_id)
{
    fsm_init(&p_fsm_button->f, fsm_trans_button);

    /* TODO alumnos: */
    p_fsm_button->debounce_time_ms=debounce_time;
    p_fsm_button->button_id = button_id;

    p_fsm_button->tick_pressed = 0;
    fsm_button_reset_duration(p_fsm_button);

    port_button_init(p_fsm_button->button_id);
}

/* Public functions -----------------------------------------------------------*/
fsm_button_t *fsm_button_new(uint32_t debounce_time, uint8_t button_id)
{
    fsm_button_t *p_fsm_button = malloc(sizeof(fsm_button_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_button_init(p_fsm_button, debounce_time, button_id);   /* Initialize the FSM */
    return p_fsm_button;                                       /* Composite pattern: return the fsm_t pointer as a fsm_button_t pointer */
}

/* FSM-interface functions. These functions are used to interact with the FSM */
void fsm_button_fire(fsm_button_t *p_fsm)
{
    fsm_fire(&p_fsm->f); // Is it also possible to it in this way: fsm_fire((fsm_t *)p_fsm);
}

void fsm_button_destroy(fsm_button_t *p_fsm)
{
    free(&p_fsm->f);
}

uint32_t fsm_button_get_duration (fsm_button_t *p_fsm)
{
        return p_fsm->duration;
}


void fsm_button_reset_duration (fsm_button_t *p_fsm)
{
    p_fsm->duration = 0;
}

uint32_t fsm_button_get_debounce_time_ms (fsm_button_t *p_fsm)
{
    return p_fsm->debounce_time_ms;
}

bool fsm_button_check_activity(fsm_button_t * p_fsm)
{
    return !(p_fsm->f.current_state == BUTTON_RELEASED);
}
