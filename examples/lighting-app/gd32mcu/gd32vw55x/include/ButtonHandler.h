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

#pragma once

#include <stdint.h>

class ButtonHandler
{
public:
    enum Button_Type_t
    {
        // 主功能键：EXTI0 / KEY_TAMPER_WAKEUP
        BUTTON_LED_TOGGLE = 0,

        // ✅ 兼容 sensor-app 的命名：同一个按键，不增加按键数量
        BUTTON_TRIGGER = BUTTON_LED_TOGGLE,

        // 工厂复位键：EXTI14 / PC14
        BUTTON_FACTORY
    } Button_Type;

    static void Init(Button_Type_t button_type);
    typedef void (*ButtonHandlerCallback)(Button_Type_t button_type);
    void SetButtonHandlerCallback(ButtonHandlerCallback button_callback);
};