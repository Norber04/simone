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
#include "math.h"
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
    p_color->r = (uint8_t)((((float)p_color->r * intensity_perc) / 100.0f) + 0.5f);
    p_color->g = (uint8_t)((((float)p_color->g * intensity_perc) / 100.0f) + 0.5f);
    p_color->b = (uint8_t)((((float)p_color->b * intensity_perc) / 100.0f) + 0.5f);
}		
/* State machine input or transition functions */
static bool check_active(fsm_t * p_this)
{
    fsm_rgb_light_t *p_rgb_light =(fsm_rgb_light_t *)p_this;
    return p_rgb_light->status;
}	


static bool check_off(fsm_t * p_this)
{
    fsm_rgb_light_t *p_rgb_light =(fsm_rgb_light_t *)p_this;
    return !p_rgb_light->status;
}


static bool check_set_new_color	(fsm_t * p_this)
{
    fsm_rgb_light_t *p_rgb_light =(fsm_rgb_light_t *)p_this;
    return p_rgb_light->new_color;
}

/* State machine output or action functions */


static void do_set_color(fsm_t * p_this)
{
    fsm_rgb_light_t *p_rgb_light =(fsm_rgb_light_t *)p_this;
    _correct_rgb_light_levels(&p_rgb_light->color,p_rgb_light->intensity_perc);
    port_rgb_light_set_rgb(p_rgb_light->rgb_light_id,p_rgb_light->color);
    p_rgb_light->new_color = false;
    p_rgb_light->idle = true;
}

static void do_set_off(	fsm_t * p_this)	
{
    fsm_rgb_light_t *p_rgb_light =(fsm_rgb_light_t *)p_this;
    port_rgb_light_set_rgb(p_rgb_light->rgb_light_id,color_off);
    p_rgb_light->idle = false;
}


static void do_set_on(fsm_t * p_this)
{
    fsm_rgb_light_t *p_rgb_light =(fsm_rgb_light_t *)p_this;
    port_rgb_light_set_rgb(p_rgb_light->rgb_light_id,color_off);
}


fsm_trans_t fsm_trans_rgb_light[] = {
    {IDLE_RGB,      check_active,           SET_COLOR,      do_set_on},
    {SET_COLOR,     check_set_new_color,    SET_COLOR,      do_set_color},
    {SET_COLOR,     check_off,              IDLE_RGB,       do_set_off},
    {-1,            NULL,                   -1,             NULL}
};
/* Other auxiliary functions */
static void fsm_rgb_light_init(	fsm_rgb_light_t * p_fsm_rgb_light, uint8_t rgb_light_id)
{
    fsm_init(&p_fsm_rgb_light->f ,fsm_trans_rgb_light);

    p_fsm_rgb_light->rgb_light_id = rgb_light_id;
    p_fsm_rgb_light->intensity_perc = MAX_LEVEL_INTENSITY;
    p_fsm_rgb_light->color = color_off;
    p_fsm_rgb_light->new_color = false;
    p_fsm_rgb_light->idle = false;
    p_fsm_rgb_light->status = false;

    port_rgb_light_init(p_fsm_rgb_light->rgb_light_id);
}	

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
    fsm_fire(&p_fsm->f);
}

bool fsm_rgb_light_get_status (fsm_rgb_light_t *p_fsm)
{
    return p_fsm->status;
}
 
void fsm_rgb_light_set_status (fsm_rgb_light_t *p_fsm, bool pause)
{
    p_fsm->status = pause;
}

bool fsm_rgb_light_check_activity(fsm_rgb_light_t * p_fsm)
{
   return p_fsm->status && !(p_fsm->idle == IDLE_RGB);
}