/*!
    \file    init_rom.c
    \brief   Rom init function for GD32VW55x SDK.

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

#include "wrapper_os.h"
#include "init_rom.h"
#include "rom_export_mbedtls.h"
#include "mbedtls/entropy_poll.h"
#include "mbedtls/platform_time.h"
#include "mbedtls/platform.h"
#include "trng.h"

struct rom_api_t *p_rom_api = (struct rom_api_t *)ROM_API_ARRAY_BASE;

static mbedtls_time_t my_time_get(mbedtls_time_t *p)
{
    return (sys_os_now(false) * OS_MS_PER_TICK);
}

static void mebdtls_platform_init(void)
{
#if 1
    /* Reconfig function pointers for MbedTLS */
    mbedtls_platform_set_calloc_free((void *(*)(size_t,  size_t))sys_calloc, sys_mfree);
    mbedtls_platform_set_snprintf(snprintf);
    mbedtls_platform_set_printf(printf);
    mbedtls_platform_set_time(my_time_get);
    trng_close(1);
#else
    /* Reconfig function pointers for MbedTLS */
    mbedtls_platform_set_calloc_free((void *(*)(size_t,  size_t))sys_calloc, sys_mfree);
    mbedtls_platform_set_snprintf(snprintf);
    mbedtls_platform_set_printf(printf);
    mbedtls_platform_set_time(my_time_get);
    trng_close(1);
    mbedtls_hwpka_flag_set(0
                        | MBEDTLS_HW_EXP_MOD
                        | MBEDTLS_HW_RSA_PRIVATE
                        // | MBEDTLS_HW_ECDSA_SIGN
                        // | MBEDTLS_HW_ECDSA_VERIFY
                        | MBEDTLS_HW_ECP_MUL
                        | MBEDTLS_HW_ECP_CHECK
                        | MBEDTLS_HW_MPI_MUL);
    mbedtls_platform_set_hardware_poll(gd_hardware_poll);

    /* Others */
    mbedtls_ecp_curve_val_init();
#endif
}

/*!
    \brief      initialize rom
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rom_init(void)
{
    mebdtls_platform_init();
}


int mbedtls_platform_entropy_poll( void * data,
                                   unsigned char * output,
                                   size_t len,
                                   size_t * olen )
{
    return gd_hardware_poll(data, output, len, olen);
}
