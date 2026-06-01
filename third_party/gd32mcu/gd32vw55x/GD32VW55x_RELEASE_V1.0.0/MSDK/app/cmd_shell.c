/*!
    \file    cmd_shell.c
    \brief   Command shell for GD32VW55x SDK.

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

#include "app_cfg.h"
#include "wlan_config.h"
#include "build_config.h"
#include "gd32vw55x.h"
#include "lwip/igmp.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "wifi_vif.h"
#include "wifi_net_ip.h"
#include "wifi_management.h"
#include "wifi_export.h"
#include "wifi_init.h"
#include "cmd_shell.h"
#include "dbg_print.h"
#include "uart.h"
#include "uart_config.h"
#include "ping.h"
#include "net_iperf.h"
#ifdef CONFIG_OTA_DEMO_SUPPORT
#include "ota_demo.h"
#endif
#ifdef CONFIG_ALICLOUD_SUPPORT
#include "alicloud_entry.h"
#endif
#ifdef CONFIG_MQTT
#include "mqtt_cmd.h"
#endif

#ifdef CFG_TRACE
#include "trace_ext.h"
#endif

#if defined(CONFIG_RF_TEST_SUPPORT) || defined(CONFIG_INTERNAL_DEBUG)
#include "cmd_rftest.h"
#endif

#ifdef CONFIG_ATCMD
#include "atcmd.c"
#endif

#if defined(CONFIG_INTERNAL_DEBUG)
#include "cmd_inner.c"
#endif

// CLI task message queue size
#define CLI_QUEUE_SIZE 3

struct cmd_module_info {
    enum cmd_mode_type cmd_mode;
    struct cmd_module_reg_info cmd_reg_infos[CMD_MODULE_MAX];
};

static os_queue_t cmd_queue;
static struct cmd_module_info cmd_info;
static const struct cmd_entry cmd_table[];

int cmd_info_send(int id, void *msg_data, uint16_t len);
int cli_parse_ip4(char *str, uint32_t *ip, uint32_t *mask);

#ifdef CONFIG_IPERF_TEST
extern void cmd_iperf(int argc, char **argv);
#endif /* CONFIG_IPERF_TEST */

#ifdef CONFIG_IPERF3_TEST
extern void cmd_iperf3(int argc, char **argv);
#endif

#ifdef CONFIG_SSL_TEST
extern void cmd_ssl_client(int argc, char **argv);
#endif

#ifdef CONFIG_LWIP_SOCKETS_TEST
extern void cmd_lwip_sockets_client(int argc, char **argv);
extern void cmd_lwip_sockets_server(int argc, char **argv);
extern void cmd_lwip_sockets_close(int argc, char **argv);
extern void cmd_lwip_sockets_get_status(int argc, char **argv);
#endif

static void uart_cmd_rx_handle_done(cyclic_buf_t *uart_cyc_buf, uint8_t *buf, uint16_t *len)
{
    if (*len > cyclic_buf_count(uart_cyc_buf)) {
        *len = cyclic_buf_count(uart_cyc_buf);
    }

    if (buf == NULL) {
        cyclic_buf_drop(uart_cyc_buf, *len);
    } else {
        cyclic_buf_read(uart_cyc_buf, buf, *len);
    }
}

/**
 ****************************************************************************************
 * @brief Convert string containing ip address
 *
 * The string may should be of the form a.b.c.d/e (/e being optional)
 *
 * @param[in]  str   String to parse
 * @param[out] ip    Updated with the numerical value of the ip address
 * @param[out] mask  Updated with the numerical value of the network mask
 *                   (or 32 if not present)
 * @return 0 if string contained what looks like a valid ip address and -1 otherwise
 ****************************************************************************************
 */
int cli_parse_ip4(char *str, uint32_t *ip, uint32_t *mask)
{
    char *token;
    uint32_t a, i, j;

    #define check_is_num(_str)  for (j = 0; j < strlen(_str); j++) \
        {                                                          \
            if (_str[j] < '0' || _str[j] > '9')                    \
            return -1;                                             \
        }

    // Check if mask is present
    token = strchr(str, '/');
    if (token && mask) {
        *token++ = '\0';
        check_is_num(token);
        a = atoi(token);
        if ((a == 0) || (a > 32))
            return -1;
        *mask = (1 << a) - 1;
    } else if (mask) {
        *mask = 0xffffffff;
    }

    // parse the ip part
    *ip = 0;
    for (i = 0; i < 4; i++) {
        if (i < 3) {
            token = strchr(str, '.');
            if (!token)
                return -1;
            *token++ = '\0';
        }
        check_is_num(str);
        a = atoi(str);
        if (a > 255)
            return -1;
        str = token;
        *ip += (a << (i * 8));
    }

    return 0;
}

/**
 ****************************************************************************************
 * @brief Convert string mac address
 *
 * The string may should be of the form xx:xx:xx:xx:xx:xx
 *
 * @param[in]  str   String to parse
 * @param[out] bssid BSSID
 * @return 0 if string contained what looks like a valid ip address and -1 otherwise
 ****************************************************************************************
 */
int cli_parse_macaddr(char *str, uint8_t *bssid)
{
    char *token;
    char *endptr = NULL;
    uint32_t a, i, j;

    #define check_is_hex(_str)  for (j = 0; j < 2; j++)            \
        {                                                          \
            if (!((_str[j] >= '0' && _str[j] <= '9')               \
                || (_str[j] >= 'a' && _str[j] <= 'f')              \
                || (_str[j] >= 'A' && _str[j] <= 'F'))) {          \
                return -1;                                         \
            }                                                      \
        }

    // parse the mac address
    for (i = 0; i < 6; i++) {
        if (i < 5) {
            token = strchr(str, ':');
            if (!token)
                return -1;
            *token++ = '\0';
        }
        check_is_hex(str);
        a = (uint32_t)strtoul(str, &endptr, 16);
        if (a > 255)
            return -1;
        bssid[i] = a;
        str = token;
    }

    return 0;
}

/**
 ****************************************************************************************
 * @brief Process function for 'list_cmd' command
 *
 * Simply list the commands sending a print message.
 *
 * @param[in] params Not used
 *
 * @return 0 on success and !=0 if error occurred
 ****************************************************************************************
 */
static void cmd_help(int argc, char **argv)
{
    uint8_t i;

    for (i = 0; cmd_table[i].function != NULL; i++) {
        app_print("%s\n", cmd_table[i].command);
    }

    return;
}

static void cmd_reboot(int argc, char **argv)
{
    SysTimer_SoftwareReset();
}

static void cmd_task_list(int argc, char **argv)
{
    app_print("TaskName\t\tState\tPri\tStack\tID\tStackBase\r\n");
    app_print("--------------------------------------------------\r\n");
    sys_task_list(NULL);
}

/**
 ****************************************************************************************
 * @brief Process function for 'free' command
 *
 * Provides information about memory usage.
 *
 * @param[in] params Not used
 * @return 0 on success and !=0 if error occurred
 ****************************************************************************************
 */
static void cmd_free(int argc, char **argv)
{
    extern void dump_mem_block_list(void);

    int total, used, free, max_used;

    sys_heap_info(&total, &free, &max_used);
    used = total - free;
    max_used = total - max_used;

    app_print("RTOS HEAP: free=%d used=%d max_used=%d/%d\n",
                free, used, max_used, total);

    dump_mem_block_list();

    return;
}

/**
 ****************************************************************************************
 * @brief Process function for 'cpu_stats' command
 *
 * @param[in] params Not used
 * @return 0 on success and !=0 if error occurred
 ****************************************************************************************
 */
static void cmd_cpu_stats(int argc, char **argv)
{
#if configUSE_TRACE_FACILITY && configGENERATE_RUN_TIME_STATS
    char *pcWriteBuffer;

    pcWriteBuffer = sys_zalloc(500);

    if (pcWriteBuffer) {
        app_print("TaskName\t\tRunTime\tPercentage\r\n");
        app_print("--------------------------------------\r\n");
        vTaskGetRunTimeStats(pcWriteBuffer);
        app_print("%s\r\n", pcWriteBuffer);
        sys_mfree(pcWriteBuffer);
    }
#else
    app_print("configUSE_TRACE_FACILITY && configGENERATE_RUN_TIME_STATS MUST BE 1\r\n");
#endif
    return;
}

static void cmd_group_join(int argc, char **argv)
{
    if (argc == 2) {
        int vif_idx = WIFI_VIF_INDEX_DEFAULT;
        ip4_addr_t group_ip;
        struct netif *net_if = NULL;

        if (inet_aton((char *)argv[1], (struct in_addr*)&group_ip) == 0) {
            app_print("\rCan not join group because of group IP error\r\n");
            goto usage;
        }

        if (!ip4_addr_ismulticast(&group_ip)) {
            app_print("ip is not a multicast ip\r\n");
            goto usage;
        }

        net_if = (struct netif *)vif_idx_to_net_if(vif_idx);
        if (!net_if) {
            app_print("no netif found for interface:%d", vif_idx);
            return;
        }
        if (net_dhcp_address_obtained(net_if) || net_if_is_static_ip()) {
            igmp_joingroup((const ip4_addr_t *)&net_if->ip_addr, (const ip4_addr_t *)&group_ip);
        } else {
            app_print("Can not join group because IP not got\r\n");
        }
        return;
    }
usage:
    app_print("Usage: join_group <group ip eg:224.0.0.5>");
}

static void cmd_sys_ps(int argc, char **argv)
{
    uint8_t ps_mode;

    if (argc != 2) {
        app_print("Usage: sys_ps [mode]\n\r");
        app_print("\tmode: 0: None, 1: CPU Deep Sleep\r\n");
    } else {
        ps_mode = atoi(argv[1]);
        sys_ps_set(ps_mode);
    }

    ps_mode = sys_ps_get();

    app_print("Current power save mode: %d\n\r", ps_mode);
}

static void cmd_wifi_debug(int argc, char **argv)
{
    char *endptr = NULL;
    uint32_t enable = 0;

    if (argc == 2) {
        enable = (uint32_t)strtoul((const char *)argv[1], &endptr, 0);
        if (*endptr != '\0') {
            goto usage;
        }
        if (enable != 0 && enable != 1)
            goto usage;
        if (enable)
            wifi_netlink_dbg_open();
        else
            wifi_netlink_dbg_close();
    } else {
        goto usage;
    }
    return;

usage:
    app_print("Usage: wifi_debug <0 or 1>\r\n");
}

static void cmd_wifi_open(int argc, char **argv)
{
    wifi_netlink_wifi_open();
}

static void cmd_wifi_close(int argc, char **argv)
{
    wifi_netlink_wifi_close();
}

static void cmd_wifi_mac_addr(int argc, char **argv)
{
    uint8_t *addr;
    uint8_t user_addr[WIFI_ALEN];

    if(argc == 1) {
        addr = wifi_vif_mac_addr_get(WIFI_VIF_INDEX_DEFAULT);
        app_print("Current MAC: "MAC_FMT"\r\n", MAC_ARG_UINT8(addr));
        goto Usage;
    } else if(argc == 2) {
        cli_parse_macaddr(argv[1], user_addr);
        app_print("User MAC: "MAC_FMT"\r\n", MAC_ARG_UINT8(user_addr));
        wifi_netlink_wifi_close();
        wifi_vif_user_addr_set(user_addr);
        wifi_netlink_wifi_open();
        addr = wifi_vif_mac_addr_get(WIFI_VIF_INDEX_DEFAULT);
        app_print("Current MAC: "MAC_FMT"\r\n", MAC_ARG_UINT8(addr));
    } else {
        goto Usage;
    }
    return;
Usage:
    app_print("\rUsage: wifi_mac_addr [xx:xx:xx:xx:xx:xx]\r\n");
}

#ifdef CFG_WIFI_CONCURRENT
static void cmd_wifi_concurrent(int argc, char **argv)
{
    char *endptr = NULL;
    uint32_t enable = 0;

    if (argc == 2) {
        enable = (uint32_t)strtoul((const char *)argv[1], &endptr, 0);
        if (*endptr != '\0') {
            goto usage;
        }
        if (enable != 0 && enable != 1) {
            goto usage;
        }
        wifi_management_concurrent_set(enable);
    } else if (argc == 1) {
        app_print("wifi concurrent mode %d\r\n", wifi_concurrent_mode);
    } else {
        goto usage;
    }
    return;

usage:
    app_print("Usage: wifi_concurrent [0 or 1]\r\n");
}
#endif /* CFG_WIFI_CONCURRENT */

static void cb_scan_done(void *eloop_data, void *user_ctx)
{
    app_print("WIFI_SCAN: done\r\n");
    wifi_netlink_scan_results_print(WIFI_VIF_INDEX_DEFAULT, wifi_netlink_scan_result_print);
    eloop_event_unregister(WIFI_MGMT_EVENT_SCAN_DONE);
    eloop_event_unregister(WIFI_MGMT_EVENT_SCAN_FAIL);
}

static void cb_scan_fail(void *eloop_data, void *user_ctx)
{
    app_print("WIFI_SCAN: failed\r\n");
    eloop_event_unregister(WIFI_MGMT_EVENT_SCAN_DONE);
    eloop_event_unregister(WIFI_MGMT_EVENT_SCAN_FAIL);
}

static void cmd_wifi_scan(int argc, char **argv)
{
#if 1
    eloop_event_register(WIFI_MGMT_EVENT_SCAN_DONE, cb_scan_done, NULL, NULL);
    eloop_event_register(WIFI_MGMT_EVENT_SCAN_FAIL, cb_scan_fail, NULL, NULL);

    if (wifi_management_scan(false, NULL)) {
        eloop_event_unregister(WIFI_MGMT_EVENT_SCAN_DONE);
        eloop_event_unregister(WIFI_MGMT_EVENT_SCAN_FAIL);
        app_print("Wifi scan failed\r\n");
    }
#else
    if (wifi_management_scan(true, NULL) != 0) {
        app_print("start wifi_scan failed\r\n");
    } else {
        wifi_netlink_scan_results_print(WIFI_VIF_INDEX_DEFAULT, wifi_netlink_scan_result_print);
    }
#endif
}

/**
 ****************************************************************************************
 * @brief Process function for 'connect' command
 *
 * Start connection to an AP in a separated task.
 * @verbatim
   wifi_connect <SSID> [password]
   @endverbatim
 *
 * @param[in] params <ssid> [password]
 * @return
 ****************************************************************************************
 */
static void cb_connect_success(void *eloop_data, void *user_ctx)
{
    app_print("WIFI_CONNECT: success\r\n");

    eloop_event_unregister(WIFI_MGMT_EVENT_CONNECT_SUCCESS);
    eloop_event_unregister(WIFI_MGMT_EVENT_CONNECT_FAIL);
}

static void cb_connect_fail(void *eloop_data, void *user_ctx)
{
    app_print("WIFI_CONNECT: fail\r\n");

    eloop_event_unregister(WIFI_MGMT_EVENT_CONNECT_SUCCESS);
    eloop_event_unregister(WIFI_MGMT_EVENT_CONNECT_FAIL);
}

static void cmd_wifi_connect(int argc, char **argv)
{
    int status = 0;
    char *ssid;
    char *password;

    if(argc == 2) {
        ssid = argv[1];
        password = NULL;
    } else if(argc == 3) {
        ssid = argv[1];
        password = argv[2];
    } else {
        app_print("\rUsage: wifi_connect <SSID> [PASSWORD]\r\n");
        return;
    }
#if 1
    status = wifi_management_connect(ssid, password, false);
#else
    eloop_event_register(WIFI_MGMT_EVENT_CONNECT_SUCCESS, cb_connect_success, NULL, NULL);
    eloop_event_register(WIFI_MGMT_EVENT_CONNECT_FAIL, cb_connect_fail, NULL, NULL);

    status = wifi_management_connect(ssid, password, false);
#endif

    if (status != 0) {
        app_print("start wifi_connect failed %d\r\n", status);
    }
}

static void cmd_wifi_connect_bssid(int argc, char **argv)
{
    int status = 0;
    char *string_bssid;
    uint8_t bssid[WIFI_ALEN];
    char *password;

    if(argc == 2) {
        string_bssid = argv[1];
        password = NULL;
    } else if(argc == 3) {
        string_bssid = argv[1];
        password = argv[2];
    } else {
        app_print("\rUsage: wifi_connect_bssid <BSSID> [PASSWORD]\r\n");
        return;
    }
    cli_parse_macaddr(string_bssid, bssid);
    app_print("bssid: "MAC_FMT"\r\n", MAC_ARG_UINT8(bssid));
    status = wifi_management_connect_with_bssid(bssid, password, true);

    if (status != 0) {
        app_print("start wifi_connect_bssid failed %d\r\n", status);
    }
}

static void cmd_wifi_disconnect(int argc, char **argv)
{
    wifi_management_disconnect();
    return;
}

/**
 ****************************************************************************************
 * @brief Process function for 'wifi_status' command
 *
 * @param[in] params not used
 *
 * @return
 ****************************************************************************************
 */
static void cmd_wifi_status(int argc, char **argv)
{
    wifi_netlink_status_print();
}

/**
 ****************************************************************************************
 * @brief Process function for 'wifi_set_ip' command
 *
   @verbatim
      wifi_set_ip dhcp
      wifi_set_ip <ip> <gw>
   @endverbatim
 *
 * @param[in] params 'dhcp' | <ip> <gw>
 * @return
 ****************************************************************************************
 */
static void cmd_wifi_ip_set(int argc, char **argv)
{
    int vif_idx = WIFI_VIF_INDEX_DEFAULT;
    struct wifi_vif_tag *wvif = vif_idx_to_wvif(vif_idx);
    struct wifi_ip_addr_cfg ip_cfg;

    if (argc == 2) {
        if (strcmp(argv[1], "dhcp") == 0) {
            /* Only STA mode */
            if (wvif->wvif_type != WVIF_STA) {
                app_print("wifi_set_ip: only for STA mode\r\n");
                goto usage;
            }
            /* if ip has been get before, clear it and get a new one */
            net_if_use_static_ip(false);
            ip_cfg.mode = IP_ADDR_NONE;
            wifi_set_vif_ip(vif_idx, &ip_cfg);

            ip_cfg.mode = IP_ADDR_DHCP_CLIENT;
            ip_cfg.default_output = true;
            ip_cfg.dhcp.to_ms = VIF_DHCP_TIMEOUT;

            if (wifi_set_vif_ip(vif_idx, &ip_cfg)) {
                app_print("wifi_set_ip: dhcpc failed\n");
            }
        } else {
            goto usage;
        }
    } else if (argc == 3) {
        /* Only STA mode */
        if (wvif->wvif_type != WVIF_STA) {
            app_print("wifi_set_ip: only for STA mode\r\n");
            goto usage;
        }
        app_print("wifi_set_ip: set ip addr:%s, gate_way:%s\r\n", (char *)argv[1], (char *)argv[2]);
        ip_cfg.mode = IP_ADDR_STATIC_IPV4;
        net_if_use_static_ip(true);

        if (cli_parse_ip4((char *)argv[1], &ip_cfg.ipv4.addr, &ip_cfg.ipv4.mask) != 0)
            goto usage;

        if (cli_parse_ip4((char *)argv[2], &ip_cfg.ipv4.gw, NULL))
            goto usage;

        if (wifi_set_vif_ip(vif_idx, &ip_cfg)) {
            app_print("wifi_set_ip: failed to set ip\r\n");
        }
    } else if (argc == 4) {
        if (strcmp(argv[1], "dhcpd") == 0) {
            /* Only SoftAP mode */
            if (wvif->wvif_type != WVIF_AP) {
                app_print("wifi_set_ip: only for AP mode\r\n");
                goto usage;
            }

            ip_cfg.mode = IP_ADDR_DHCP_SERVER;
            if (cli_parse_ip4((char *)argv[2], &ip_cfg.ipv4.addr, &ip_cfg.ipv4.mask) != 0)
                goto usage;

            if (cli_parse_ip4((char *)argv[3], &ip_cfg.ipv4.gw, NULL))
                goto usage;

            if (wifi_set_vif_ip(vif_idx, &ip_cfg)) {
                app_print("wifi_set_ip: failed to set dhcpd\r\n");
            }
        } else {
            goto err_input;
        }
    } else {
        goto err_input;
    }
    return;

err_input:
    app_print("wifi_set_ip: invalid input\r\n");
usage:
    app_print("Usage: wifi_set_ip dhcp | <ip_addr/mask_bits> <gate_way> | dhcpd <ip_addr/mask_bits> <gate_way>\r\n");
    app_print("\tdhcp: get ip by start dhcp, only for STA mode\r\n");
    app_print("\tip_addr: ipv4 addr to set.\r\n");
    app_print("\tgate_way: gate way to set.\r\n");
    app_print("\tdhcpd: use new ip addr to restart dhcp server, only for SoftAP mode\r\n");
    app_print("Example: wifi_set_ip 192.168.0.123/24 192.168.0.1\r\n");
    app_print("         wifi_set_ip dhcp\r\n");
    app_print("         wifi_set_ip dhcpd 192.168.0.1/24 192.168.0.1\r\n");
    return;
}

/**
 ****************************************************************************************
 * @brief Process function for 'wifi_auto_conn' command
 *
   @verbatim
      wifi_auto_conn 1/0 or wifi_auto_conn
   @endverbatim
 *
 * @param[in] [auto_conn]
 * @return
 ****************************************************************************************
 */
static void cmd_wifi_auto_conn(int argc, char **argv)
{
    char *endptr = NULL;
    uint32_t enable = 0;

    if (argc == 2) {
        enable = (uint32_t)strtoul((const char *)argv[1], &endptr, 0);
        if (*endptr != '\0') {
            goto usage;
        }
        if (enable != 0 && enable != 1) {
            goto usage;
        }
        wifi_netlink_auto_conn_set(enable);
    } else if (argc == 1) {
        app_print("Current wifi auto conn %d\r\n", wifi_netlink_auto_conn_get());
        return;
    } else {
        goto usage;
    }
    return;

usage:
    app_print("Usage: wifi_auto_conn [0 or 1]\r\n");
}

/**
 ****************************************************************************************
 * @brief Process function for 'wifi_ps' command
 *
 * Enable/disable power save mode.
 * @verbatim
   wifi_ps <mode>
   @endverbatim
 *
 * @param[in] params  PS parameters
 * @return none
 ****************************************************************************************
 */
static void cmd_wifi_ps(int argc, char **argv)
{
    uint32_t mode_set;

    do {
        if (argc != 2)
            break;

        mode_set = atoi(argv[1]);
        if (wifi_netlink_ps_mode_set(WIFI_VIF_INDEX_DEFAULT, mode_set))
            app_print("wifi_set_ps: set failed\r\n");
        return;
    } while(0);

    app_print("Usage: wifi_ps <mode>\n\r");
    app_print("\tmode: 0: off, 1: always on, 2: dynamic on\r\n");
}

/**
 ****************************************************************************************
 * @brief Process function for 'wifi_ap' command
 *
 * Start an AP
 * @verbatim
   wifi_ap <ssid> <password> <channel> [-a <akm>[,<akm 2>]] [-hide <hide_ap>]
   @endverbatim
 * @param[in] params <ssid> <password> <channel> [-a <akm>[,<akm 2>]] [-hide <hide_ap>]
 * @return
 ****************************************************************************************
 */
#ifdef CFG_SOFTAP
static void cmd_wifi_ap(int argc, char **argv)
{
    char *ssid = NULL;
    char *password = NULL;
    uint32_t passwd_len = 0;
    uint8_t channel = 1, is_hidden = 0;
    wifi_ap_auth_mode_t auth_mode = AUTH_MODE_UNKNOWN;
    char *option, *akm_str = NULL;
    int arg_idx = 4;

    if ((argc < 4) || (argc & 1) == 1) {
        goto usage;
    }
    ssid = argv[1];
    password = argv[2];
    channel = atoi(argv[3]);

    while (arg_idx < argc) {
        option = argv[arg_idx];
        if (!strcmp("-a", option)) {
            akm_str = argv[arg_idx + 1];
        } else if (!strcmp("-hide", option)) {
            is_hidden = atoi(argv[arg_idx + 1]) > 0 ? 1 : 0;
        }
        arg_idx += 2;
    }

    passwd_len = strlen(password);
    if ((strlen(ssid) > WIFI_SSID_MAX_LEN)
        || (passwd_len < WPA_MIN_PSK_LEN && strcmp("NULL", password))
        || (passwd_len > WPA_MAX_PSK_LEN))
        goto usage;

    if (channel > 13 || channel < 1)
        goto usage;

    if (akm_str) {
        if (!strcmp(akm_str, "open"))
            auth_mode = AUTH_MODE_OPEN;
        else if (!strcmp(akm_str, "wpa2"))
            auth_mode = AUTH_MODE_WPA2;
        else if (!strcmp(akm_str, "wpa3"))
            auth_mode = AUTH_MODE_WPA3;
        else if (!strcmp(akm_str, "wpa2,wpa3") || !strcmp(akm_str, "wpa3,wpa2"))
            auth_mode = AUTH_MODE_WPA2_WPA3;
        else
            goto usage;
    }

    if (!strcmp(password, "NULL")) {
        auth_mode = AUTH_MODE_OPEN;
        password = NULL;
    } else if (auth_mode == AUTH_MODE_UNKNOWN || auth_mode == AUTH_MODE_OPEN) {
        // default use wpa2 mode;
        // or password has been entered, but a '-a open' followed, ignore -a option.
        auth_mode = AUTH_MODE_WPA2;
    }

    if (wifi_management_ap_start(ssid, password, channel, auth_mode, is_hidden)) {
        app_print("Failed to start AP, check your configuration.\r\n");
        return;
    }

    app_print("SoftAP successfully started!\r\n");
    return;
usage:
    app_print("Usage: wifi_ap <ssid> <password> <channel> [-a <akm>[,<akm 2>]] [-hide <hide_ap>]\r\n");
    app_print("<ssid>: The length should be between 1 and 32.\r\n");
    app_print("<password>: The length should be between 8 and 63, but can be \"NULL\" indicates open ap.\r\n");
    app_print("<channel>: 1~13.\r\n");
    app_print("[-a <akm>[,<akm 2>]]: only support following 5 AKM units: open; wpa2; wpa3; wpa2,wpa3 or wpa3,wpa2.\r\n");
    app_print("[-hide <hide_ap>]: 0 or 1, default 0.\r\n");
    app_print("For example:\r\n");
    app_print("    wifi_ap test_ap 12345678 1 -a wpa3 -hide 0, means a wpa3 ap in channel 1 and can broadcast ssid.\r\n");
    app_print("    wifi_ap test_ap NULL 5, means an open ap in channel 5.\r\n");
    app_print("    wifi_ap test_ap 12345678 11, means a wpa2 ap in channel 11, default wpa2.\r\n");
}

// func cmd_wifi_ap_adv is used for auto test, it will be removed in future.
static void cmd_wifi_ap_adv(int argc, char **argv)
{
    cmd_wifi_ap(argc, argv);
}

static void cmd_wifi_ap_stop(int argc, char **argv)
{
    wifi_management_ap_stop();
}
#endif // CFG_SOFTAP

/**
 ****************************************************************************************
 * @brief Process function for 'wifi_monitor' command
 *
 * monitor command can be used to start the monitor mode
 *
   @verbatim
     wifi_monitor start <channel>
     wifi_monitor stop
   @endverbatim
 *
 * @param[in] params monitor_start/stop commands above
 ****************************************************************************************
 */
static void cmd_wifi_monitor(int argc, char **argv)
{
    char *option;
    uint8_t channel;

    if (argc == 3) {
        option = argv[1];
        if (!strcmp("start", option)) {
            channel = atoi(argv[2]);
            if (channel < 1 || channel > 14) {
                goto usage;
            }
            wifi_management_monitor_start(channel, NULL);
            return;
        }
    } else if (argc == 2) {
        option = argv[1];
        if (!strcmp("stop", option)) {
            wifi_management_sta_start();
            return;
        }
    }

usage:
    app_print("Usage: wifi_monitor stop | start <channel>\r\n");
    app_print("start: start the monitor mode.\r\n");
    app_print("<channel>: 1~14.\r\n");
    app_print("stop: stop the monitor mode.\r\n");
}

#ifdef CONFIG_OTA_DEMO_SUPPORT
static void cmd_ota_demo(int argc, char **argv)
{
    char *ssid;
    char *password;
    char *srv_addr;
    char *image_url;
    size_t key_len;
    size_t ssid_len;

    if (argc == 4) {
        ssid_len = strlen(argv[1]);
        if (ssid_len <= MAC_SSID_LEN)
            ssid = argv[1];
        else
            goto usage;

        password = NULL;
        srv_addr = argv[2];
        image_url = argv[3];
    } else if (argc == 5) {
        ssid_len = strlen(argv[1]);
        if (ssid_len <= MAC_SSID_LEN)
            ssid = argv[1];
        else
            goto usage;

        key_len = strlen(argv[2]);
        if (key_len <= WPA_MAX_PSK_LEN && key_len >= WPA_MIN_PSK_LEN)
            password = argv[2];
        else
            goto usage;

        srv_addr = argv[3];
        image_url = argv[4];
    } else {
        goto usage;
    }

    if (wifi_management_connect(ssid, password, true)) {
        app_print("WiFi connect failed, OTA demo abort\r\n");
        return;
    }

    if (ota_demo_cfg_init(srv_addr, image_url))
        goto usage;

    ota_demo_start();

    return;
usage:
    app_print("Usage: ota_demo <ssid> [password] <srvaddr> <imageurl>\r\n");
    app_print("<ssid>: The length should be between 1 and 32.\r\n");
    app_print("[password]: The length should be between 8 and 63, but can be empty indicates open ap.\r\n");
    app_print("<srvaddr>: IPv4 address of remote OTA server needded to set. eg: 192.168.0.123.\r\n");
    app_print("<imageurl>: The length should be between 1 and 127.\r\n");
    app_print("for example:\r\n");
    app_print("    ota_dmeo test_ap 192.168.3.100 image-ota.bin, means connect to an open AP\r\n");
    app_print("\t\t\tand update the image-ota.bin from 192.168.3.100.\r\n");
}
#endif

// Array of supported CLI command
static const struct cmd_entry cmd_table[] =
{
    {"help", cmd_help},
    {"reboot", cmd_reboot},

#ifdef CONFIG_BASECMD
    {"tasks", cmd_task_list},
    {"free", cmd_free},
    {"cpu_stats", cmd_cpu_stats},
    {"sys_ps", cmd_sys_ps},

#ifdef CFG_WLAN_SUPPORT
    {"ping", cmd_ping},
    {"join_group", cmd_group_join},
#ifdef CONFIG_SSL_TEST
    {"ssl_client", cmd_ssl_client},
#endif
#ifdef CONFIG_IPERF_TEST
    {"iperf", cmd_iperf},
#endif
#ifdef CONFIG_IPERF3_TEST
    {"iperf3", cmd_iperf3},
#endif
#ifdef CONFIG_OTA_DEMO_SUPPORT
    {"ota_demo", cmd_ota_demo},
#endif
#ifdef CONFIG_ALICLOUD_SUPPORT
    {"ali_cloud", cmd_alicloud_linkkit},
#endif
    {"wifi_debug", cmd_wifi_debug},
    {"wifi_open", cmd_wifi_open},
    {"wifi_close", cmd_wifi_close},
    {"wifi_mac_addr", cmd_wifi_mac_addr},
#ifdef CFG_WIFI_CONCURRENT
    {"wifi_concurrent", cmd_wifi_concurrent},
#endif
    {"wifi_auto_conn", cmd_wifi_auto_conn},
    {"wifi_scan", cmd_wifi_scan},
    {"wifi_connect", cmd_wifi_connect},
    {"wifi_connect_bssid", cmd_wifi_connect_bssid},
    {"wifi_disconnect", cmd_wifi_disconnect},
    {"wifi_status", cmd_wifi_status},
    {"wifi_set_ip", cmd_wifi_ip_set},
#ifdef CFG_LPS
    {"wifi_ps", cmd_wifi_ps},
#endif
    {"wifi_monitor", cmd_wifi_monitor},
#ifdef CFG_SOFTAP
    {"wifi_ap", cmd_wifi_ap},
    {"wifi_ap_adv", cmd_wifi_ap_adv},
    {"wifi_stop_ap", cmd_wifi_ap_stop},
#endif /* CFG_SOFTAP */
#ifdef CONFIG_MQTT
    {"mqtt_connect", cmd_mqtt_connect_server},
    {"mqtt_publish", cmd_mqtt_msg_pub},
    {"mqtt_subscribe", cmd_mqtt_msg_sub},
    {"mqtt_auto_reconnect", cmd_mqtt_set_auto_reconnect},
    {"mqtt_disconnect", cmd_mqtt_disconnect},
#endif /*CONFIG_MQTT*/
#ifdef CONFIG_LWIP_SOCKETS_TEST
    {"socket_client", cmd_lwip_sockets_client},
    {"socket_server", cmd_lwip_sockets_server},
    {"socket_close", cmd_lwip_sockets_close},
    {"socket_get_status", cmd_lwip_sockets_get_status},
#endif
#endif /* CFG_WLAN_SUPPORT */
#endif /* CONFIG_BASECMD */
    {"", NULL}
};

#ifdef CONFIG_ATCMD
static void at_cmd_exec(struct cmd_msg *msg)
{
    int argc;
    char *argv[MAX_ARGC];
    char *command;
    cmd_handle_cb handle_cb = NULL;
    struct cmd_module_reg_info * atcmd_tab_info = &(cmd_info.cmd_reg_infos[CMD_MODULE_ATCMD]);

    command = sys_malloc(msg->len);
    uart_cmd_rx_handle_done((cyclic_buf_t *)msg->data, (uint8_t *)command, &msg->len);

    if (command == NULL)
    {
        app_print("No buffer alloc for at cmd ! \r\n");
        return;
    }

    if ((atcmd_tab_info->parse_cb == NULL) ||
        (argc = atcmd_tab_info->parse_cb(command, argv)) <= 0) {
        at_rsp_error();
        goto done;
    }

    if (atcmd_tab_info->get_handle_cb != NULL &&
        (atcmd_tab_info->prefix == NULL ||
        !memcmp(command, atcmd_tab_info->prefix, strlen(atcmd_tab_info->prefix)))) {
        if (atcmd_tab_info->get_handle_cb(command, (void **)&handle_cb) == CLI_SUCCESS) {
            handle_cb(argc, argv);
        }
    }

done:
    sys_mfree(command);
    app_print("# ");
}
#endif

static int parse_cmd(char *buf, char **argv)
{
    int argc = 0;

    if (buf == NULL)
        return 0;
    while((argc < MAX_ARGC) && (*buf != '\0')) {
        argv[argc] = buf;
        argc ++;
        buf ++;

        while((*buf != ' ') && (*buf != '\0'))
            buf ++;

        while(*buf == ' ') {
            *buf = '\0';
            buf ++;
        }
        // Don't replace space
        if(argc == 1){
            if(strcmp(argv[0], "iwpriv") == 0){
                if(*buf != '\0'){
                    argv[1] = buf;
                    argc ++;
                }
                break;
            }
        }
    }

    return argc;
}

static void cmd_common_help(void)
{
#if (!defined(CONFIG_RF_TEST_SUPPORT)) && defined(CONFIG_BASECMD)
    uint8_t i;
    for (i = 0; cmd_table[i].function != NULL; i++) {
        app_print("\t%s\n", cmd_table[i].command);
    }
#endif

#if defined(CONFIG_RF_TEST_SUPPORT) || defined(CONFIG_INTERNAL_DEBUG)
    app_print("==============================\r\n");
    wifi_rftest_cmd_help();
#endif

#if defined(CONFIG_INTERNAL_DEBUG)
    app_print("==============================\r\n");
    wifi_inner_cmd_help();
#endif
    return;
}

static uint8_t cmd_common_handle(void *data, void **cmd)
{
    const struct cmd_entry *w_cmd = cmd_table;

    while (w_cmd->function) {
        if (!strcmp((char *)data, w_cmd->command)) {
            *cmd = w_cmd->function;
            break;
        }
        w_cmd++;
    }

#if defined(CONFIG_RF_TEST_SUPPORT) || defined(CONFIG_INTERNAL_DEBUG)
    if (w_cmd->function == NULL) {
        w_cmd = wifi_rftest_get_handle_cb(data, cmd);
    }
#endif

#if defined(CONFIG_INTERNAL_DEBUG)
    if (w_cmd->function == NULL) {
        w_cmd = wifi_inner_get_handle_cb(data, cmd);
    }
#endif

    return w_cmd->function == NULL ? CLI_UNKWN_CMD : CLI_SUCCESS;
}

/**
 ****************************************************************************************
 * @brief separate the param and cmd from the msg separated by symbol of ' '
 *
 * @param[in] command          point to msg
 * @param[in] command_len      length of msg
 * @param[out] param           point to the param of cmd
 ****************************************************************************************
 */
static char* cmd_param_separate(char *command, uint16_t command_len)
{
    char *param = strchr(command, ' ');

    if (param) {
        *param++ = '\0';
        while (*param == ' ') {
            param++;
        }
    } else {
         command[command_len - 1] = '\0'; //be sure to have \0 in command
    }
    return param;
}

static void cmn_cmd_exec(struct cmd_msg *msg)
{
    char *command, *param;
    uint32_t res = CLI_UNKWN_CMD;
    cmd_handle_cb handle_cb = NULL;
    cmd_parse_cb parse_cb = parse_cmd; // default parse function.
    char *argv[MAX_ARGC];
    uint32_t argc;
    uint8_t idx;

    command = sys_malloc(msg->len);
    uart_cmd_rx_handle_done((cyclic_buf_t *)msg->data, (uint8_t *)command, &msg->len);

    if (command == NULL)
    {
        app_print("No buffer alloc for cmd ! \r\n");
        return;
    }

    param = cmd_param_separate(command, msg->len);

    if (!strcmp(command, "help")) {
        for (idx = 0; idx < CMD_MODULE_MAX; idx++) {
            if (cmd_info.cmd_reg_infos[idx].help_cb != NULL) {
                app_print("==============================\r\n");
                cmd_info.cmd_reg_infos[idx].help_cb();
            }
        }
        goto symbol_print;
    }

    for (idx = 0; idx < CMD_MODULE_MAX; idx++) {
        if (cmd_info.cmd_reg_infos[idx].get_handle_cb != NULL &&
            (cmd_info.cmd_reg_infos[idx].prefix == NULL ||
            !memcmp(command, cmd_info.cmd_reg_infos[idx].prefix, strlen(cmd_info.cmd_reg_infos[idx].prefix)))) {
            res = cmd_info.cmd_reg_infos[idx].get_handle_cb(command, (void **)&handle_cb);
            if (res == CLI_SUCCESS) {
                parse_cb = cmd_info.cmd_reg_infos[idx].parse_cb != NULL ? cmd_info.cmd_reg_infos[idx].parse_cb : parse_cb;
                break;
            } else if (res == CLI_ERROR) {
                break;
            }
        }
    }

    switch (res) {
    case CLI_SUCCESS :
        argv[0] = command;
        argc = parse_cb(param, &argv[1]) + 1;
        handle_cb(argc, argv);
        break;

    case CLI_UNKWN_CMD :
        app_print("Unknown command - %s!\r\n", command);
        break;

    case  CLI_ERROR:
    default :
        app_print("Error!\r\n");
        break;
    }

symbol_print:
    sys_mfree(command);
    app_print("# ");
    return;
}

static void cmd_msg_process(struct cmd_msg *msg)
{
    switch (cmd_mode_type_get()) {
#ifdef CONFIG_ATCMD
    case CMD_MODE_TYPE_AT:
        at_cmd_exec(msg);
        break;
#endif
    case CMD_MODE_TYPE_NORMAL:
    default :
        cmn_cmd_exec(msg);
        break;
    }

    return;
}

uint8_t cmd_module_reg(enum cmd_module_id id, char *prefix, cmd_module_get_handle_cb get_handle_cb,
    cmd_module_help_cb help_cb, cmd_parse_cb parse_cb)
{
    if (id >= CMD_MODULE_MAX || get_handle_cb == NULL)
        return CLI_ERROR;

    cmd_info.cmd_reg_infos[id].prefix = prefix;
    cmd_info.cmd_reg_infos[id].get_handle_cb = get_handle_cb;
    cmd_info.cmd_reg_infos[id].help_cb = help_cb;
    cmd_info.cmd_reg_infos[id].parse_cb = parse_cb;

    return CLI_SUCCESS;
}

void cmd_mode_type_set(enum cmd_mode_type cmd_mode)
{
    cmd_info.cmd_mode = cmd_mode;
}

enum cmd_mode_type cmd_mode_type_get(void)
{
    return cmd_info.cmd_mode;
}

/**
 ****************************************************************************************
 * @brief CLI task main loop
 *
 * CLI task may received command and process it
 ****************************************************************************************
 */
static void cmd_cli_task(void *param)
{
    struct cmd_msg msg;

#ifdef CFG_WLAN_SUPPORT
    wifi_wait_ready();
#endif

    for (;;) {
        sys_queue_read(&cmd_queue, &msg, -1, false);
        cmd_msg_process(&msg);
    }
}

int cmd_shell_init(void)
{
    if (sys_task_create_dynamic((const uint8_t *)"CLI task", CLI_STACK_SIZE, CLI_PRIORITY, cmd_cli_task, NULL) == NULL) {
        return -1;
    }

    if (sys_queue_init(&cmd_queue, CLI_QUEUE_SIZE, sizeof(struct cmd_msg))) {
        return -2;
    }

    sys_memset(&cmd_info, 0, sizeof(struct cmd_module_info));
    cmd_mode_type_set(CMD_MODE_TYPE_NORMAL);
    if (cmd_module_reg(CMD_MODULE_COMMON, NULL, cmd_common_handle, cmd_common_help, NULL))
        return -1;

#ifdef CONFIG_ATCMD
    if (atcmd_init())
        return -1;
#endif

    return 0;
}

int cmd_info_send(int id, void *msg_data, uint16_t len)
{
    struct cmd_msg msg;

    msg.id   = CMD_MSG_ID(0, id);
    msg.len  = len;
    msg.data = msg_data;

    return sys_queue_write(&cmd_queue, &msg, 0, true);
}
