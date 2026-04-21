/**
 * @file stm32f4_rgb_light.c
 * @brief Portable functions to interact with the RGB light system FSM library. All portable functions must be implemented in this file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Standard C includes */
#include <stdio.h>
#include <math.h>

/* HW dependent includes */
#include "port_rgb_light.h"
#include "port_system.h"

/* Microcontroller dependent includes */
#include "stm32f4_system.h"
#include "stm32f4_rgb_light.h"

/* Defines --------------------------------------------------------------------*/

/* Typedefs --------------------------------------------------------------------*/

/* Global variables */
stm32f4_rgb_light_hw_t 	rgb_lights_arr [] = {
    [PORT_RGB_LIGHT_ID] = {
        .p_port_red = STM32F4_RGB_LIGHT_R_GPIO,
        .pin_red = STM32F4_RGB_LIGHT_R_PIN,
        .p_port_green = STM32F4_RGB_LIGHT_G_GPIO,
        .pin_green = STM32F4_RGB_LIGHT_G_PIN,
        .p_port_blue = STM32F4_RGB_LIGHT_B_GPIO,
        .pin_blue = STM32F4_RGB_LIGHT_B_PIN
    },
};

/* Private functions -----------------------------------------------------------*/
stm32f4_rgb_light_hw_t *_stm32f4_rgb_light_get(uint8_t rgb_light_id)
{
    if(rgb_light_id < sizeof(rgb_lights_arr) / sizeof(rgb_lights_arr[0])){
        return &rgb_lights_arr[rgb_light_id];
    }
    else
    {
        return NULL;
    }
}

void 	_timer_pwm_config (uint8_t rgb_light_id)
{
    /*Enable the clock of the timer that controls the column scanning.*/
    RCC -> APB1ENR|=RCC_APB1ENR_TIM4EN;
    /*Disable the counter of the timer*/
    TIM4 -> CR1 &= ~TIM_CR1_CEN;
    /*Enable the autoreload preload*/
    TIM4 -> CR1 |= TIM_CR1_ARPE;
    /*Set the counter of the timer to 0*/
    TIM4 -> CNT = 0;

    double SystemClock = (double)SystemCoreClock;
    double t_inter = (double)PORT_RGB_LIGHT_PWM_PERIOD_MS / 1000.0;

    double psc = round((SystemClock*t_inter)/(65535.0+1.0)-1.0); // uint32_t psc = (uint32_t)(((SystemClock * t_inter) / 65536.0) - 1.0 + 0.5);
    double arr = round((SystemClock*t_inter)/(psc + 1.0)-1.0);
    if(arr > 65535.0){
        psc += 1.0;
        arr = round((SystemClock*t_inter)/(psc + 1.0)-1.0);
    }
    TIM4 -> ARR = arr;
    TIM4 -> PSC = psc;

    /*Disable the output of the corresponding channels*/
    TIM4 -> CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E);

    /*Clear the P and NP bits*/
    TIM4 -> CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P | TIM_CCER_CC3P | TIM_CCER_CC4P);
    TIM4 -> CCER &= ~(TIM_CCER_CC1NP | TIM_CCER_CC2NP | TIM_CCER_CC3NP | TIM_CCER_CC4NP);

    /*Set PWM mode*/
    TIM4 -> CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC2M);
    TIM4 -> CCMR1 &= ~(TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);
    TIM4 -> CCMR2 &= ~(TIM_CCMR2_OC3M | TIM_CCMR2_OC4M);
    TIM4 -> CCMR2 &= ~(TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE);
    

    /*trigger an update event to set the arr and psc values*/
    TIM4 -> EGR = TIM_EGR_UG;
}

/* Public functions -----------------------------------------------------------*/
void 	port_rgb_light_init (uint8_t rgb_light_id)
{
    stm32f4_rgb_light_hw_t *p_rgb_light = _stm32f4_rgb_light_get(rgb_light_id);

    /*config alternate mode no pull*/
    stm32f4_system_gpio_config(p_rgb_light->p_port_red,p_rgb_light->pin_red,STM32F4_GPIO_MODE_AF,STM32F4_GPIO_PUPDR_NOPULL);
    stm32f4_system_gpio_config(p_rgb_light->p_port_green,p_rgb_light->pin_green,STM32F4_GPIO_MODE_AF,STM32F4_GPIO_PUPDR_NOPULL);
    stm32f4_system_gpio_config(p_rgb_light->p_port_blue,p_rgb_light->pin_blue,STM32F4_GPIO_MODE_AF,STM32F4_GPIO_PUPDR_NOPULL);

    /*alternate function*/
    stm32f4_system_gpio_config_alternate(p_rgb_light->p_port_red,p_rgb_light->pin_red,STM32F4_AF2);
    stm32f4_system_gpio_config_alternate(p_rgb_light->p_port_green,p_rgb_light->pin_green,STM32F4_AF2);
    stm32f4_system_gpio_config_alternate(p_rgb_light->p_port_blue,p_rgb_light->pin_blue,STM32F4_AF2);

    _timer_pwm_config(rgb_light_id);
    port_rgb_light_set_rgb(rgb_light_id,color_off);
}
void 	port_rgb_light_set_rgb (uint8_t rgb_light_id, rgb_color_t color)
{
    if (rgb_light_id == PORT_RGB_LIGHT_ID)
    {
        /*disable the timer*/
        TIM4->CR1 &= ~TIM_CR1_CEN;

        uint32_t arr = TIM4->ARR;

        if (color.r != 0 || color.g != 0 || color.b != 0)
        {
            // ================= RED =======================
            if (color.r == 0)
            {
                //disable the chanel
                TIM4 -> CCER &= ~(TIM_CCER_CC1E);
            }
            else
            {
            uint32_t duty_r = (color.r * arr) / COLOR_RGB_MAX_VALUE; //scale o "normalization" to RGB_MAX_VALUE
            TIM4->CCR1 = duty_r;
            TIM4->CCER |= TIM_CCER_CC1E; //enable the chanel
            }
            // ================= GREEN =======================
            if (color.g == 0)
            {
                TIM4 -> CCER &= ~(TIM_CCER_CC2E);
            }
            else
            {
            uint32_t duty_g = (color.g * arr) / COLOR_RGB_MAX_VALUE;
            TIM4->CCR2 = duty_g;
            TIM4->CCER |= TIM_CCER_CC2E;
            }
            // ================= BLUE =======================
            if (color.b == 0)
            {
                TIM4 -> CCER &= ~(TIM_CCER_CC3E);
            }
            else
            {
            uint32_t duty_b = (color.b * arr) / COLOR_RGB_MAX_VALUE;
            TIM4->CCR3 = duty_b;
            TIM4->CCER |= TIM_CCER_CC3E;
            }
            /*force ARR to update*/
            TIM4->EGR |= TIM_EGR_UG;

            TIM4->CR1 |= TIM_CR1_CEN;
        }
        else 
        {
            /*Disable the capture/compare register for all the chanells*/
            TIM4 -> CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E);
        }
    }
}