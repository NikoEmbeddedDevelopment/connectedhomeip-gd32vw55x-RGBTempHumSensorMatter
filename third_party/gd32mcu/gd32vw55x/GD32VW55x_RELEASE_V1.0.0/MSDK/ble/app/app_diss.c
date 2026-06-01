/**
 ****************************************************************************************
 *
 * @file app_diss.c
 *
 * @brief Device Information Service Server Application Module entry point.
 *
 * Copyright (C) GigaDevice 2023 - 2023
 *
 ****************************************************************************************
 */

#include "ble_profile_config.h"

#if BLE_PROFILE_DIS_SERVER
#include "app_diss.h"

/**@breif Init APP device information service server module.
 *
 */
void ble_app_diss_init(void)
{
    ble_diss_param_t diss_param;

    uint8_t manufat_name[] = "GigaDevice";
    uint8_t model_name[] = "GD32VW55x";
    uint8_t serial_num[] = "HDM1";
    uint8_t hw_rev[] = "HW ver 1.0";
    uint8_t fw_rev[] = "FW ver 1.0";
    uint8_t sw_rev[] = "SW ver 1.0";
    uint8_t ieee_data[] = {BLE_DIS_IEEE_11073_BODY_EXP, 0, 'e', 'x', 'p', 'e', 'r', 'i', 'm', 'e', 'n', 't', 'a', 'l'};
    ble_dis_sys_id_t sys_id;
    ble_dis_pnp_id_t pnp_id;

    sys_id.manufact_id = 0x000C2B0C2B;
    sys_id.oui = 0x00010203;

    pnp_id.vendor_id_source = BLE_DIS_VND_ID_SRC_BLUETOOTH_SIG;
    pnp_id.vendor_id = 0x0C2B;
    pnp_id.product_id = 0x01;
    pnp_id.product_version = 0x01;

    diss_param.manufact_name.p_data = manufat_name;
    diss_param.manufact_name.len = sizeof(manufat_name);
    diss_param.model_num.p_data = model_name;
    diss_param.model_num.len = sizeof(model_name);
    diss_param.serial_num.p_data = serial_num;
    diss_param.serial_num.len = sizeof(serial_num);
    diss_param.hw_rev.p_data = hw_rev;
    diss_param.hw_rev.len = sizeof(hw_rev);
    diss_param.fw_rev.p_data = fw_rev;
    diss_param.fw_rev.len = sizeof(fw_rev);
    diss_param.sw_rev.p_data = sw_rev;
    diss_param.sw_rev.len = sizeof(sw_rev);
    diss_param.ieee_data.p_data = ieee_data;
    diss_param.ieee_data.len = sizeof(ieee_data);
    diss_param.p_sys_id = &sys_id;
    diss_param.p_pnp_id = &pnp_id;

    ble_diss_init(&diss_param);
}
#endif

