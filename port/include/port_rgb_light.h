/**
 * @file port_rgb_light.h
 * @brief Header for the portable functions to interact with the HW of the RGB light system. The functions must be implemented in the platform-specific code.
 * @author alejandro suarez suarez
 * @author norberto de los rios gutierrez
 * @date fecha
 */
#ifndef PORT_RGB_LIGHT_SYSTEM_H_
#define PORT_RGB_LIGHT_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include "rgb_colors.h"
/* Typedefs --------------------------------------------------------------------*/

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define PORT_RGB_LIGHT_ID 0
/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Configure the HW specifications of a given RGB light.
 * 
 * @param rgb_light_id RGB light ID. This index is used to select the element of the rgb_lights_arr[] array
 */
void port_rgb_light_init(uint8_t rgb_light_id);

/**
 * @brief Set the Capture/Compare register values for each channel of the RGB LED given a color.
 * 
 * @param rgb_light_id RGB light system identifier number.
 * @param color RGB color to set.
 */
void port_rgb_light_set_rgb(uint8_t rgb_light_id, rgb_color_t color);

#endif /* PORT_RGB_LIGHT_SYSTEM_H_ */