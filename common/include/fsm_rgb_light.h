/**
 * @file fsm_rgb_light.h
 * @brief Header for fsm_rgb_light.c file.
 * @author Alejandro Suárez Suárez
 * @author Norberto de los Ríos Gutierrez
 * @date fecha
 */

#ifndef FSM_RGB_LIGHT_SYSTEM_H_
#define FSM_RGB_LIGHT_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
/* Defines and enums ----------------------------------------------------------*/
#include "fsm.h"
#include "rgb_colors.h"
/* Enums */
enum  	FSM_RGB_LIGHT_SYSTEM {
    IDLE_RGB = 0,
    SET_COLOR
};
/* Defines and enums ----------------------------------------------------------*/
#define MAX_LEVEL_INTENSITY 100
/* Typedefs --------------------------------------------------------------------*/
typedef struct 
{
    fsm_t f;
    rgb_color_t color;
    uint8_t intensity_perc;
    bool new_color;
    bool status;
    bool idle;
    uint32_t rgb_light_id;
}fsm_rgb_light_t;

/* Function prototypes and explanation -------------------------------------------------*/
fsm_rgb_light_t * fsm_rgb_light_new (uint8_t rgb_light_id);

void fsm_rgb_light_destroy (fsm_rgb_light_t *p_fsm);

void fsm_rgb_light_set_color_intensity (fsm_rgb_light_t *p_fsm, rgb_color_t color, uint8_t intensity_perc);
 
void fsm_rgb_light_fire (fsm_rgb_light_t *p_fsm);

bool fsm_rgb_light_get_status (fsm_rgb_light_t *p_fsm); 
 
void fsm_rgb_light_set_status (fsm_rgb_light_t *p_fsm, bool pause);
 
bool fsm_rgb_light_check_activity (fsm_rgb_light_t *p_fsm);
 
 
#endif /* FSM_RGB_LIGHT_SYSTEM_H_ */