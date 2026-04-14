/**
 * @file fsm_rgb_light.c
 * @brief RGB light system FSM main file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdio.h>
/* HW dependent includes */
#include "port_rgb_light.h"
#include "port_system.h"
/* Project includes */
#include "fsm.h"
#include "fsm_rgb_light.h"
#include "rgb_colors.h"
/* Typedefs --------------------------------------------------------------------*/

/* Private functions -----------------------------------------------------------*/

void _correct_rgb_light_levels	(rgb_color_t * p_color, uint8_t intensity_perc )
{
    uint32_t arr = TIM4->ARR;
    /*==========RED=============*/
    uint32_t duty_r = (color.r * arr) / COLOR_RGB_MAX_VALUE;
    TIM4->CCR1 = duty_r;
    TIM4->CCER |= TIM_CCER_CC1E;
}	

void port_rgb_light_set_rgb	(uint8_t rgb_light_id, rgb_color_t color)
{

}		
/* State machine input or transition functions */

/* State machine output or action functions */

/* Other auxiliary functions */

/* Public functions -----------------------------------------------------------*/
fsm_rgb_light_t *fsm_rgb_light_new(uint8_t rgb_light_id)
{
    fsm_rgb_light_t *p_fsm_rgb_light = malloc(sizeof(fsm_rgb_light_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_rgb_light_init(p_fsm_rgb_light, rgb_light_id); /* Initialize the FSM */
    return p_fsm_rgb_light;
}

void fsm_rgb_light_destroy (fsm_rgb_light_t *p_fsm)
{
    if(p_fsm != NULL){
        free(p_fsm);
    }
}

void fsm_rgb_light_set_color_intensity (fsm_rgb_light_t *p_fsm, rgb_color_t color, uint8_t intensity_perc)
{
    p_fsm->color = color;
    p_fsm->intensity_perc = intensity_perc;
    p_fsm->new_color = true;
}
/*This function is used to fire the RGB light FSM. 
It is used to check the transitions and execute the actions 
of the RGB light FSM.*/
void fsm_rgb_light_fire (fsm_rgb_light_t *p_fsm)
{
    fsm_fire(p_fsm);
}

bool fsm_rgb_light_get_status (fsm_rgb_light_t *p_fsm)
{
    return p_fsm->status;
}
 
void fsm_rgb_light_set_status (fsm_rgb_light_t *p_fsm, bool pause)
{
    p_fsm->status = pause;
}
 
bool fsm_rgb_light_check_activity (fsm_rgb_light_t *p_fsm)
{
   return !p_fsm->idle && p_fsm->status;
}