/*!
    \file    atcmd.c
    \brief   AT command for GD32VW55x SDK

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

#include "version.h"
#include "_build_date.h"
#include "atcmd.h"
#include "atcmd_wifi.h"
#include "atcmd_tcpip.h"

extern uart_config_t log_uart_conf;
uart_config_t log_uart_conf = {
    .usart_periph = LOG_UART,
    .baudrate = 115200U,
    .databits = USART_WL_8BIT,
    .stopbits = USART_STB_1BIT,
    .parity = USART_PM_NONE,
    .flow_ctrl = (USART_CTS_DISABLE | USART_RTS_DISABLE)
};

/*!
    \brief      convert command to serial port configuration parameters
    \param[in]  cmd: serial port configuration parameters included in the command
    \param[out] gd: serial port configuration parameters
    \retval     none
*/
static void uart_param_at2gd(uart_config_t *cmd, uart_config_t *gd)
{
    gd->baudrate = cmd->baudrate;
    if (cmd->databits == 8)
        gd->databits = USART_WL_8BIT;
    else
        gd->databits = USART_WL_9BIT;

    if (cmd->stopbits == 0)
        gd->stopbits = USART_STB_0_5BIT;
    else if (cmd->stopbits == 1)
        gd->stopbits = USART_STB_1BIT;
    else if (cmd->stopbits == 2)
        gd->stopbits = USART_STB_1_5BIT;
    else
        gd->stopbits = USART_STB_2BIT;

    if (cmd->parity == 0)
        gd->parity = USART_PM_NONE;
    else if (cmd->parity == 1)
        gd->parity = USART_PM_ODD;
    else
        gd->parity = USART_PM_EVEN;

    if (cmd->flow_ctrl == 0)
        gd->flow_ctrl = 0;
    else if (cmd->flow_ctrl == 1)
        gd->flow_ctrl = USART_RTS_ENABLE;
    else if (cmd->flow_ctrl == 2)
        gd->flow_ctrl = USART_CTS_ENABLE;
    else
        gd->flow_ctrl = (USART_CTS_ENABLE | USART_RTS_ENABLE);
}

/*!
    \brief      convert serial port configuration parameters to command
    \param[in]  gd: serial port configuration parameters
    \param[out] cmd: serial port configuration parameters included in the command
    \retval     none
*/
static void uart_param_gd2at(uart_config_t *gd, uart_config_t *cmd)
{
    cmd->baudrate = gd->baudrate;
    if (gd->databits == USART_WL_8BIT)
        cmd->databits = 8;
    else
        cmd->databits = 9;
    if (gd->stopbits == USART_STB_0_5BIT)
        cmd->stopbits = 0;
    else if (gd->stopbits == USART_STB_1BIT)
        cmd->stopbits = 1;
    else if (gd->stopbits == USART_STB_1_5BIT)
        cmd->stopbits = 2;
    else
        cmd->stopbits = 3;
    if (gd->parity == USART_PM_NONE)
        cmd->parity = 0;
    else if (gd->parity == USART_PM_ODD)
        cmd->parity = 1;
    else
        cmd->parity = 2;
    cmd->flow_ctrl = 0;
    if ((gd->flow_ctrl & USART_RTS_ENABLE) == USART_RTS_ENABLE)
        cmd->flow_ctrl |= BIT(0);
    if ((gd->flow_ctrl & USART_CTS_ENABLE) == USART_CTS_ENABLE)
        cmd->flow_ctrl |= BIT(1);
}

/*!
    \brief      the AT command replies OK
    \param[in]  none
    \param[out] none
    \retval     none
*/
void at_rsp_ok(void)
{
    AT_RSP("OK\r\n");
}

/*!
    \brief      the AT command replies ERROR
    \param[in]  none
    \param[out] none
    \retval     none
*/
void at_rsp_error(void)
{
    AT_RSP("ERROR\r\n");
}

/*!
    \brief      parse string
    \param[in]  param: pointer to a string waiting to be parsed
    \param[out] none
    \retval     none
*/
char *at_string_parse(char *param)
{
    char *find = strchr(param, AT_QUOTE);
    char *head = NULL, *ptr, *tail = NULL;

    if (find == NULL)
        return NULL;
    head = ptr = ++find;
    while (1) {
        find = strchr(ptr, AT_QUOTE);
        if (find == NULL)
            break;
        else
            tail = find;
        ptr = find + 1;
    }
    if (tail != NULL)
        *tail = '\0';
    else
        return NULL;
    if (*head == '\0')
        head = NULL;
    return head;
}

/*!
    \brief      enter AT command mode
    \param[in]  argc: number of parameters
    \param[in]  argv: the pointer to the array that holds the parameters
    \param[out] none
    \retval     none
*/
static void at_entry(int argc, char **argv)
{
    cmd_mode_type_set(CMD_MODE_TYPE_AT);
    cip_info_init();
    at_rsp_ok();
}

/*!
    \brief      exit AT command mode
    \param[in]  argc: number of parameters
    \param[in]  argv: the pointer to the array that holds the parameters
    \param[out] none
    \retval     none
*/
static void at_exit(int argc, char **argv)
{
    extern void sys_msleep(u32_t ms);
    cip_info_reset();
    sys_msleep(500);
    cmd_mode_type_set(CMD_MODE_TYPE_NORMAL);
    at_rsp_ok();
}

/*!
    \brief      the AT command reset system
    \param[in]  argc: number of parameters
    \param[in]  argv: the pointer to the array that holds the parameters
    \param[out] none
    \retval     none
*/
static void at_reset(int argc, char **argv)
{
    usart_tx_idle_wait(log_uart_conf.usart_periph);
    rcu_deinit();
    SysTimer_SoftwareReset();
    at_rsp_ok();
}

/*!
    \brief      the AT command show version of the software
    \param[in]  argc: number of parameters
    \param[in]  argv: the pointer to the array that holds the parameters
    \param[out] none
    \retval     none
*/
static void at_version(int argc, char **argv)
{
    AT_RSP("==================================\r\n");
    AT_RSP("SDK revsion: v%d.%d.%d\r\n", WIFI_VERSION_MAJ, WIFI_VERSION_MIN, WIFI_VERSION_REL);
#ifdef WIFI_GIT_REVISION
    AT_RSP("SDK git reversion: %s\r\n", WIFI_GIT_REVISION);
#endif
    AT_RSP("SDK build date: %s\r\n", SDK_BUILD_DATE);
    at_rsp_ok();
}

/*!
    \brief      the AT command show all of the tasks
    \param[in]  argc: number of parameters
    \param[in]  argv: the pointer to the array that holds the parameters
    \param[out] none
    \retval     none
*/
static void at_task(int argc, char **argv)
{
    AT_RSP("==================================\r\n");
    sys_task_list(NULL);
    at_rsp_ok();
}

/*!
    \brief      the AT command show heap usage
    \param[in]  argc: number of parameters
    \param[in]  argv: the pointer to the array that holds the parameters
    \param[out] none
    \retval     none
*/
static void at_heap(int argc, char **argv)
{
    AT_RSP("==================================\r\n");
    AT_RSP("Total free heap size = %d\r\n", sys_free_heap_size());
    AT_RSP("Total min free heap size = %d\r\n", sys_min_free_heap_size());
    at_rsp_ok();
}

/*!
    \brief      the AT command show system ram usage
    \param[in]  argc: number of parameters
    \param[in]  argv: the pointer to the array that holds the parameters
    \param[out] none
    \retval     none
*/
static void at_sysram(int argc, char **argv)
{
    uint32_t free_sram = sys_free_heap_size();
    AT_RSP("==================================\r\n");
    AT_RSP("Free SRAM size = %u\r\n", free_sram);

    at_rsp_ok();
}

/*!
    \brief      the AT command show  the configuration of log usart or conifgurate log usart
    \param[in]  argc: number of parameters
    \param[in]  argv: the pointer to the array that holds the parameters
    \param[out] none
    \retval     none
*/
static void at_log_uart(int argc, char **argv)
{
    uart_config_t cmd;
    if (argc == 1) {
        if (argv[0][strlen(argv[0]) - 1] == AT_QUESTION) {
            uart_param_gd2at(&log_uart_conf, &cmd);
            AT_RSP("+UART: %d, %d, %d, %d, %d\r\n",
                cmd.baudrate, cmd.databits, cmd.stopbits, cmd.parity, cmd.flow_ctrl);
        } else {
            goto Error;
        }
    } else if (argc == 2) {
        if (argv[1][0] == AT_QUESTION)
            goto Usage;
        else
            goto Error;
    } else if (argc == 6) {
        char *endptr = NULL;
        cmd.baudrate = (uint32_t)strtoul((const char *)argv[1], &endptr, 10);
        if (*endptr != '\0') {
            goto Error;
        }
        cmd.databits = (uint32_t)strtoul((const char *)argv[2], &endptr, 10);
        if (*endptr != '\0') {
            goto Error;
        }
        cmd.stopbits = (uint32_t)strtoul((const char *)argv[3], &endptr, 10);
        if (*endptr != '\0') {
            goto Error;
        }
        cmd.parity = (uint32_t)strtoul((const char *)argv[4], &endptr, 10);
        if (*endptr != '\0') {
            goto Error;
        }
        cmd.flow_ctrl = (uint32_t)strtoul((const char *)argv[5], &endptr, 10);
        if (*endptr != '\0') {
            goto Error;
        }
        if ((cmd.databits != 8 && cmd.databits != 9)
            || (cmd.stopbits > 3) || (cmd.parity > 2) || (cmd.flow_ctrl > 3))
            goto Error;

        uart_param_at2gd(&cmd, &log_uart_conf);
        uart_config(log_uart_conf.usart_periph, log_uart_conf.baudrate, log_uart_conf.flow_ctrl, false, false);
    } else {
        goto Error;
    }

    at_rsp_ok();
    return;

Error:
    at_rsp_error();
    return;
Usage:
    AT_RSP("+UART=<baudrate>,<databits>,<stopbits>,<parity>,<flow control>\r\n");
    at_rsp_ok();
    return;
}

#include "atcmd_tcpip.c"
#include "atcmd_wifi.c"
static void at_help(int argc, char **argv);
const struct atcmd_entry atcmd_table[] = {
    /* ====== Base ====== */
    {"AT", at_entry},
    {"ATQ", at_exit},
    {"AT+HELP", at_help},
    {"AT+RST", at_reset},
    {"AT+GMR", at_version},
    {"AT+TASK", at_task},
    {"AT+HEAP", at_heap},
    {"AT+SYSRAM", at_sysram},
    {"AT+UART", at_log_uart},
    /* ====== WLAN ====== */
#ifdef CFG_WLAN_SUPPORT
    {"AT+CWMODE_CUR", at_cw_mode_cur},
    {"AT+CWJAP_CUR", at_cw_ap_cur_join},
    {"AT+CWLAP", at_cw_ap_list},
    {"AT+CWSTATUS", at_cw_status},
    {"AT+CWQAP", at_cw_ap_quit},
    {"AT+CWSAP_CUR", at_cw_ap_cur_start},
    {"AT+CWLIF", at_cw_ap_client_list},
    {"AT+CWAUTOCONN", at_cw_auto_connect},
#endif
    /* ====== TCPIP ====== */
    {"AT+PING", at_cip_ping},
    {"AT+CIPSTA", at_cip_sta_ip},
    {"AT+CIPSTART", at_cip_start},
    {"AT+CIPSEND", at_cip_send},
    {"AT+CIPSERVER", at_cip_server},
    {"AT+CIPCLOSE", at_cip_close},
    {"AT+CIPSTATUS", at_cip_status},
    {"AT+CIFSR", at_cip_ip_addr_get},
    {"", NULL}
};

/*!
    \brief      the AT command show AT command list
    \param[in]  argc: number of parameters
    \param[in]  argv: the pointer to the array that holds the parameters
    \param[out] none
    \retval     none
*/
static void at_help(int argc, char **argv)
{
    int i;
    AT_RSP("\rAT COMMAND LIST:\r\n");
    AT_RSP("==============================\r\n");

    for (i = 0; atcmd_table[i].exec != NULL; i++)
    {
        AT_RSP("    %s\r\n", atcmd_table[i].name);
    }
    AT_RSP("\r\nOK\r\n");
}

static void atcmd_common_help_cb(void)
{
    AT_RSP("\t%s\n", atcmd_table[0].name);
    return;
}

static uint8_t atcmd_common_handle(void *data, void **cmd)
{
    const struct atcmd_entry *w_cmd = atcmd_table;
    char *data_tmp = (char *)data;

    if (cmd_mode_type_get() != CMD_MODE_TYPE_AT && strcmp(data_tmp, "AT"))
    {
        return CLI_UNKWN_CMD;
    }

    while (w_cmd->exec) {
        if (!strcmp(data_tmp, w_cmd->name) ||
           (!strncmp(data_tmp, w_cmd->name, strlen(w_cmd->name)) && data_tmp[strlen(w_cmd->name)] == AT_QUESTION))
        {
            *cmd = w_cmd->exec;
            break;
        }
        w_cmd++;
    }
    if (w_cmd->exec == NULL) {
        at_rsp_error();
        return CLI_UNKWN_CMD;
    }
    return CLI_SUCCESS;
}

/*!
    \brief      parse AT command
    \param[in]  atcmd: the pointer of AT command
    \param[in]  argv: the pointer to the array that holds the parameters
    \param[out] none
    \retval     none
*/
static int atcmd_parse(char *atcmd, char **argv)
{
    int32_t i;
    int32_t argc = 0;

    if (atcmd == NULL) {
        return -1;
    }
    char *buf = atcmd;
    uint32_t size = strlen(atcmd);

    /* Get command */
    argv[argc] = buf;
    for (i = 0; i < size; i++) {
        if (*buf == AT_EQU) {
            break;
        } else if ((*buf == AT_CR) || (*buf == AT_LF)) {
            goto Exit;
        } else {
            buf++;
        }
    }
    if (i == size)
        goto Exit;

    /* Get parameters */
    *buf++ = '\0';
    argc++;
    while((argc < AT_MAX_ARGC) && (*buf != '\0')) {
        argv[argc] = buf;
        argc++;
        buf++;

        while ((*buf != AT_SEPARATE) && (*buf != '\0')) {
            buf++;
        }
        if (*buf != '\0') {
            *buf = '\0';
            buf ++;
        }
        while (*buf == AT_SPACE) {
            *buf = '\0';
            buf ++;
        }
    }

    return argc;

Exit:
    *buf = '\0';
    argc++;
    return argc;
}

int atcmd_init(void)
{
   return cmd_module_reg(CMD_MODULE_ATCMD, NULL, atcmd_common_handle, atcmd_common_help_cb, atcmd_parse);
}
