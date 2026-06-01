/*!
    \file    ble_init.c
    \brief   Implementation of the BLE initialization.

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

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ble_app_config.h"
#include "ble_profile_config.h"

#include "ble_init.h"
#include "ble_export.h"

#include "ble_adapter.h"
#include "ble_adv.h"
#include "ble_scan.h"
#include "ble_conn.h"
#include "ble_l2cap_coc.h"
#include "ble_sec.h"

#include "app_adapter_mgr.h"
#include "ble_storage.h"
#include "wrapper_os.h"
#include "ble_gatts.h"
#if (BLE_APP_GATT_CLIENT_SUPPORT)
#include "ble_gattc.h"
#if (BLE_APP_GATTC_CO_ITF_SUPPORT)
#include "ble_gattc_co.h"
#endif
#endif
#include "app_cmd.h"
#include "app_adv_mgr.h"
#include "app_scan_mgr.h"
#include "app_list_mgr.h"
#include "app_sec_mgr.h"
#include "app_dev_mgr.h"
#include "app_conn_mgr.h"
#include "app_per_sync_mgr.h"
#include "app_iso_mgr.h"
#include "app_l2cap.h"
#include "nvds_flash.h"
#include "gd32vw55x_platform.h"
#include "dbg_print.h"


#if BLE_PROFILE_DIS_SERVER
#include "app_diss.h"
#endif

#if BLE_PROFILE_SAMPLE_SERVER
#include "ble_sample_srv.h"
#endif

#if BLE_PROFILE_SAMPLE_CLIENT
#include "ble_sample_cli.h"
#endif

#ifdef CFG_VIRTUAL_HCI_MODE
#include "app_virtual_hci.h"
#endif

#if BLE_PROFILE_HPS_SERVER
#include "app_http_proxy_server.h"
#endif

#if BLE_PROFILE_HPS_CLIENT
#include "app_http_proxy_client.h"
#endif

#if BLE_PROFILE_PROX_SERVER
#include "app_prox_rpt.h"
#endif

#if BLE_PROFILE_PROX_CLIENT
#include "app_prox_monitor.h"
#endif

/*
 * DEFINITIONS
 ****************************************************************************************
 */
/** @brief Definitions of the different ble task priorities. */
enum
{
    BLE_STACK_TASK_PRIORITY = OS_TASK_PRIORITY(2),      /**< Priority of the BLE stack task. */
    BLE_APP_TASK_PRIORITY   = OS_TASK_PRIORITY(1),      /**< Priority of the BLE APP task. */
};

/** @brief Definitions of the different ble task stack size requirements. */
enum
{
    BLE_STACK_TASK_STACK_SIZE = 768,        /**< BLE stack task stack size. */
    BLE_APP_TASK_STACK_SIZE   = 512,        /**< BLE APP task stack size. */
};

/*
 * GLOBAL VARIABLES
 ****************************************************************************************
 */
/** @brief BLE sleep mode when flash erase need to execute. */
uint8_t flash_erase_sleep_mode = 0;

/** @brief Semaphore signaled when all ble tasks are ready. */
static os_sema_t ble_ready_sem;

/**@brief Function to notify other modules that BLE module is in ready state.
 *
 */
void ble_task_ready(void)
{
    sys_sema_up(&ble_ready_sem);
}

/**@brief Function for other modules to wait BLE into ready state.
 *
 * return   0 if enter successfully wait, otherwise -1
 */
int ble_wait_ready(void)
{
    if (ble_ready_sem == NULL)
        return -1;

    if (sys_sema_down(&ble_ready_sem, 0))
        return -1;

    // always re-signal the semaphore in case it is called by several tasks.
    sys_sema_up(&ble_ready_sem);
    return 0;
}

/**@brief Function to handle before/after flash erase execute.
 *
 * param[in] type       Erase handle type
 */
void ble_flash_erase_handler(nvds_erase_type_t type)
{
    if (type == NVDS_ERASE_TYPE_PRE_HANDLE) {
        flash_erase_sleep_mode = ble_sleep_mode_get();
        if (flash_erase_sleep_mode != 0) {
            ble_sleep_mode_set(0);
            ble_stack_task_resume(false);
            // wait ble pmu on, timeout 10ms
            ble_wait_pmu_on(10);
        }
    } else {
        if (flash_erase_sleep_mode != 0) {
            ble_sleep_mode_set(flash_erase_sleep_mode);
        }
    }
}

/**@brief Init BLE tasks.
 *
 */
static void ble_task_init(void)
{
    ble_stack_task_init(BLE_STACK_TASK_STACK_SIZE, BLE_STACK_TASK_PRIORITY);

#if (BLE_APP_SUPPORT)
    ble_app_task_init(BLE_APP_TASK_STACK_SIZE, BLE_APP_TASK_PRIORITY);
#endif

    sys_sema_init_ext(&ble_ready_sem, 1, 0);
}

#if (BLE_APP_SUPPORT)
/**@brief Init BLE component modules needed.
 *
 */
void ble_module_init(void)
{
    ble_storage_init();

    if (ble_adp_init() != BLE_ERR_NO_ERROR) {
        dbg_print(NOTICE, "adapter init fail! check and restart again or reload image if necessary\r\n");
        return;
    }

#if (BLE_CFG_ROLE & (BLE_CFG_ROLE_BROADCASTER | BLE_CFG_ROLE_PERIPHERAL))
    ble_adv_init();
#endif

#if (BLE_CFG_ROLE & (BLE_CFG_ROLE_OBSERVER | BLE_CFG_ROLE_CENTRAL))
    ble_scan_init(BLE_GAP_LOCAL_ADDR_STATIC);
#endif

#if (BLE_CFG_ROLE & (BLE_CFG_ROLE_PERIPHERAL | BLE_CFG_ROLE_CENTRAL))
    ble_sec_init();
    ble_l2cap_init();
    ble_conn_init();
#endif

#if (BLE_CFG_ROLE & (BLE_CFG_ROLE_PERIPHERAL | BLE_CFG_ROLE_CENTRAL))
    ble_gatts_init();
#if (BLE_APP_GATT_CLIENT_SUPPORT)
    ble_gattc_init();
#if (BLE_APP_GATTC_CO_ITF_SUPPORT)
    ble_gattc_co_init();
#endif
#endif
#endif

}

/**@brief Init BLE profiles needed.
 *
 */
void ble_profile_init(void)
{
#if BLE_PROFILE_DIS_SERVER
    ble_app_diss_init();
#endif

#if BLE_PROFILE_SAMPLE_SERVER
    ble_sample_srv_init();
#endif

#if BLE_PROFILE_SAMPLE_CLIENT
    ble_sample_cli_init();
#endif

#if BLE_PROFILE_HPS_SERVER
    app_hpss_init();
#endif

#if BLE_PROFILE_HPS_CLIENT
    app_hpsc_init();
#endif

#if BLE_PROFILE_PROX_SERVER
    app_prox_rpt_init();
#endif

#if BLE_PROFILE_PROX_CLIENT
    app_proxm_init();
#endif

}

/**@brief Init BLE application modules needed.
 *
 */
void ble_app_init(void)
{
#if (BLE_APP_CMD_SUPPORT)
    ble_cli_init();
#endif

    app_adapter_init();

#if (BLE_CFG_ROLE & (BLE_CFG_ROLE_BROADCASTER | BLE_CFG_ROLE_PERIPHERAL))
    app_adv_mgr_init();
#endif

#if (BLE_CFG_ROLE & (BLE_CFG_ROLE_OBSERVER | BLE_CFG_ROLE_CENTRAL))
    app_scan_mgr_init();
#endif

    app_l2cap_mgr_init();

    app_dm_init();

#if (BLE_CFG_ROLE & (BLE_CFG_ROLE_PERIPHERAL | BLE_CFG_ROLE_CENTRAL))
    app_conn_mgr_init();
    app_sec_mgr_init();
#endif

#if (BLE_APP_PER_ADV_SUPPORT)
    app_per_sync_mgr_init();
#endif

    app_list_mgr_init();

#if (BLE_APP_BIS_SUPPORT || BLE_APP_CIS_SUPPORT)
    app_iso_mgr_init();
#endif // (BLE_APP_BIS_SUPPORT || BLE_APP_CIS_SUPPORT)

    ble_profile_init();
}
#endif // (BLE_APP_SUPPORT)

/**@brief Initialization of the ble module.
 *
 * @details This function allocates all the resources needed by the different BLE sub-modules.
 *          This function initialize the command processing.
 *          This function will use the wrapper_os API to create tasks, semaphores, etc.
 */
void ble_init(void)
{
    ble_power_on();

#ifdef CFG_VIRTUAL_HCI_MODE
    app_virtual_hci_init();
#endif

    ble_stack_init();

    ble_task_init();

#ifdef CFG_VIRTUAL_HCI_MODE
    app_virtual_hci_enable();
#endif

#if (BLE_APP_SUPPORT)
    ble_module_init();

    ble_app_init();
#endif // (BLE_APP_SUPPORT)

    /* ble need to close deep sleep before flash erase */
    nvds_flash_erase_handler_register(ble_flash_erase_handler);
}
