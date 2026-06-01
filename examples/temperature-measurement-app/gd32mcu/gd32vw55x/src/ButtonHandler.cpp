/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "ButtonHandler.h"
#include "AppConfig.h"
#include "AppTask.h"

static ButtonHandler::ButtonHandlerCallback button_press_handler = nullptr;

void ButtonHandler::Init(Button_Type_t button_type)
{
    if(button_type == ButtonHandler::BUTTON_TRIGGER)
    {
        gd_eval_key_init(KEY_TAMPER_WAKEUP, KEY_MODE_EXTI);
    }
    else if(button_type == ButtonHandler::BUTTON_FACTORY)
    {
        //configure PC14 as factory reset key
        /* enable the key clock */
        rcu_periph_clock_enable(RCU_GPIOC);
        rcu_periph_clock_enable(RCU_SYSCFG);

        /* configure button pin as input */
        gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_14);

        /* enable and set key EXTI interrupt to the lowest priority */
        eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL3_PRIO1);
        eclic_irq_enable(EXTI10_15_IRQn,1, 1);

        /* connect key EXTI line to key GPIO pin */
        syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN14);

        /* configure key EXTI line */
        exti_init(EXTI_14, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(EXTI_14);
    }

}

void ButtonHandler::SetButtonHandlerCallback(ButtonHandlerCallback button_callback)
{
    if (button_callback != nullptr)
    {
        button_press_handler = button_callback;
    }
}

/*!
    \brief      this function handles external lines 0 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
extern "C" void EXTI0_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(EXTI_0)){
        exti_interrupt_flag_clear(EXTI_0);
        button_press_handler(ButtonHandler::BUTTON_TRIGGER);
    }
}

/*!
    \brief      this function handles external lines 10 to 15 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
extern "C" void EXTI10_15_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(EXTI_14)){
        exti_interrupt_flag_clear(EXTI_14);
        button_press_handler(ButtonHandler::BUTTON_FACTORY);
    }
}


