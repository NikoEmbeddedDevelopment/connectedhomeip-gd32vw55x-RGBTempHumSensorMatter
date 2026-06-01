/*!
    \file    alicloud_entry.c
    \brief   Alicloud entry functions.

    \version 2023-07-20, V1.0.0, firmware for GD32VW55x
*/

/*
    Copyright (c) 2023, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "app_cfg.h"
#include "wrapper_os.h"
#include "dbg_print.h"
#include "alicloud_entry.h"
#include "ali_linkkit_sdk_include.h"
#include "cmd_shell.h"

#ifdef CONFIG_ALICLOUD_SUPPORT
static void alicloud_task(void *argv)
{
    app_main_paras_t *paras = (app_main_paras_t *)argv;
    linkkit_main(paras->argc, paras->argv);
    printf("alicloud task exit.\r\n");
    sys_task_delete(NULL);
}

void cmd_alicloud_linkkit(int argc, char **argv)
{
    void *handle;
    app_main_paras_t paras;

    if (argc != 2) {
        printf("Usage: ali_cloud <mode>\r\n");
        printf("<mode>: 1 - smart config, 2 - softap config\r\n");
        return;
    }

    paras.argc = argc;
    paras.argv = argv;

    handle = sys_task_create_dynamic((const uint8_t *)"alicloud_task",
                    ALICLOUD_STACK_SIZE, OS_TASK_PRIORITY(ALICLOUD_TASK_PRIO),
                    (task_func_t)alicloud_task, (void *)&paras);
    if (handle == NULL) {
        printf("ERROR: create alicloud task failed.\r\n");
        return;
    }

    sys_ms_sleep(1000);
    awss_config_press();

    return;
}
#endif
