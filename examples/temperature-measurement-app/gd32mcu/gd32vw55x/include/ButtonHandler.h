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
        BUTTON_TRIGGER = 0,
        BUTTON_FACTORY
    } Button_Type;

    static void Init(Button_Type_t button_type);
    typedef void (*ButtonHandlerCallback)(Button_Type_t button_type);
    void SetButtonHandlerCallback(ButtonHandlerCallback button_callback);
};
