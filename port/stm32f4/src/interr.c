/**
 * @file interr.c
 * @brief Interrupt service routines for the STM32F4 platform.
 * @author SDG2. Román Cárdenas (r.cardenas@upm.es) and Josué Pagán (j.pagan@upm.es)
 * @date 2026-01-01
 */
// Include HW dependencies:
#include "port_system.h"
#include "stm32f4_system.h"

// Include headers of different port elements:
#include "port_button.h"
#include "stm32f4_button.h"
#include "port_keyboard.h"
#include "stm32f4_keyboard.h"
//#include "port_simone.h"


void _check_column_interrupt(uint8_t column_index)
{
    stm32f4_keyboard_hw_t *p_keyboard = &keyboards_arr[PORT_KEYBOARD_MAIN_ID];
    GPIO_TypeDef *p_port = p_keyboard->p_col_ports[column_index];
    uint8_t pin = p_keyboard->p_col_pins[column_index];
    bool condicion = stm32f4_system_gpio_read(p_port,pin);

    p_keyboard->flag_key_pressed = condicion;
    p_keyboard->col_idx_interrupt = column_index;
    EXTI->PR = BIT_POS_TO_MASK(p_keyboard->p_col_pins[column_index]);
}
//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
//------------------------------------------------------
/**
 * @brief Interrupt service routine for the System tick timer (SysTick).
 *
 * @note This ISR is called when the SysTick timer generates an interrupt.
 * The program flow jumps to this ISR and increments the tick counter by one millisecond.
 *
 * > **TO-DO alumnos:**
 * >
 * > ✅ 1. **Increment the System tick counter `msTicks` in 1 count.** To do so, use the function `port_system_get_millis()` and `port_system_set_millis()`.
 *
 * @warning **The variable `msTicks` must be declared volatile!** Just because it is modified by a call of an ISR, in order to avoid [*race conditions*](https://en.wikipedia.org/wiki/Race_condition). **Added to the definition** after *static*.
 *
 */
void SysTick_Handler(void)
{
    //Incremet the System ticks by one
    uint32_t ticks = port_system_get_millis();
    port_system_set_millis(ticks+1);
}

/**
 * @brief This function handles the interruption from the user button 
 * 
 */
void EXTI15_10_IRQHandler(void)
{
    /*ISR user button*/
    if (EXTI->PR & BIT_POS_TO_MASK(buttons_arr[PORT_USER_BUTTON_ID].pin))
    {
        GPIO_TypeDef *p_port = buttons_arr[PORT_USER_BUTTON_ID].p_port;
        uint8_t pin = buttons_arr[PORT_USER_BUTTON_ID].pin;
        bool condition = stm32f4_system_gpio_read(p_port,pin);

        //if the is released changes the flag
        buttons_arr[PORT_USER_BUTTON_ID].flag_pressed = !condition;

        // cleans the pendig register
        EXTI->PR = BIT_POS_TO_MASK(pin);
    }
    /*Keyboard*/
    if(EXTI->PR & BIT_POS_TO_MASK(keyboards_arr[PORT_KEYBOARD_MAIN_ID].p_col_pins[PORT_KEYBOARD_COL_1]))
    {
        _check_column_interrupt(PORT_KEYBOARD_COL_1);
    }
}

void EXTI9_5_IRQHandler(){
    if(EXTI->PR & BIT_POS_TO_MASK(keyboards_arr[PORT_KEYBOARD_MAIN_ID].p_col_pins[PORT_KEYBOARD_COL_0]))
    {
        _check_column_interrupt(PORT_KEYBOARD_COL_0);
    }
    if(EXTI->PR & BIT_POS_TO_MASK(keyboards_arr[PORT_KEYBOARD_MAIN_ID].p_col_pins[PORT_KEYBOARD_COL_3]))
    {
        _check_column_interrupt(PORT_KEYBOARD_COL_3);
    }
}

void EXTI4_IRQHandler()
{
    if(EXTI->PR & BIT_POS_TO_MASK(keyboards_arr[PORT_KEYBOARD_MAIN_ID].p_col_pins[PORT_KEYBOARD_COL_2]))
    {
        _check_column_interrupt(PORT_KEYBOARD_COL_2);
    }
}

void TIM5_IRQHandler()
{
    TIM5->SR &= ~TIM_SR_UIF;
    port_keyboard_set_row_timeout_status(PORT_KEYBOARD_MAIN_ID,true);
}