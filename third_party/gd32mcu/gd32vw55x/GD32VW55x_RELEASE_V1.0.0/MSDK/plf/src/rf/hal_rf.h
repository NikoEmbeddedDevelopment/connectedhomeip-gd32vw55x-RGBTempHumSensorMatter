/*!
    \file    hal_rf.h
    \brief   Header file for WLAN HAL RF.

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

#ifndef __HAL_RF_H
#define __HAL_RF_H

#if CONFIG_RF_TYPE == RF_GDM32103
#include "reg_access.h"
#endif

#define NUM_24G_CHANNELS    (14)
#define NUM_5G_CHANNELS     (8)       //modify it if support more

#if CONFIG_RF_TYPE == RF_GDM32106
/* ========  RF Register Definitions ========  */
#define RFIP_ADDR_GCFG                                                  0x0000
#define RFIP_ADDR_RX_STATE_DELAY_CFG1                                   0x0004
#define RFIP_ADDR_RX_STATE_DELAY_CFG2                                   0x0008
#define RFIP_ADDR_TX_STATE_DELAY_CFG1                                   0x000C
#define RFIP_ADDR_RXOFF_CFG                                             0x0010
#define RFIP_ADDR_RXANA_CFG                                             0x0014
#define RFIP_ADDR_RXIDLE_CFG                                            0x0018
#define RFIP_ADDR_RXACTIVE_CFG                                          0x001C
#define RFIP_ADDR_TXOFF_CFG                                             0x0020
#define RFIP_ADDR_TXANA_CFG                                             0x0024
#define RFIP_ADDR_TXON_CFG                                              0x0028
#define RFIP_ADDR_TXRFOFF_CFG                                           0x002C
#define RFIP_ADDR_LDO_CFG                                               0x0058
#define RFIP_ADDR_XTAL_CFG                                              0x005C
#define RFIP_ADDR_PLLS_CFG1                                             0x0060
#define RFIP_ADDR_PLLS_CFG2                                             0x0064
#define RFIP_ADDR_RFPLL_LOOP_CFG                                        0x0068
#define RFIP_ADDR_RFPLL_DIG_CFG1                                        0x006C
#define RFIP_ADDR_RFPLL_DIG_CFG2                                        0x0070
#define RFIP_ADDR_RFPLL_DIG_INT_FRAC_CFG                                0x0074
#define RFIP_ADDR_RFPLL_DIG_VCOCAL_CFG                                  0x0078
#define RFIP_ADDR_MDLL_CFG                                              0x007C
#define RFIP_ADDR_RX_LPF2_CFG                                           0x0080
#define RFIP_ADDR_SDMADC_CFG                                            0x0084
#define RFIP_ADDR_SDMADC_DIG_CFG                                        0x0088
#define RFIP_ADDR_SDMADC_CIC_CFG                                        0x008C
#define RFIP_ADDR_SARADC_CFG                                            0x0090
#define RFIP_ADDR_AUXADC_CFG                                            0x0094
#define RFIP_ADDR_RCCAL_CFG                                             0x0098
#define RFIP_ADDR_RCCAL_OFFSET_TEST_CFG                                 0x009C
#define RFIP_ADDR_MUX_CFG                                               0x00A0
#define RFIP_ADDR_LDO_ANA_CFG                                           0x00A4
#define RFIP_ADDR_LDO_VCO_CFG                                           0x00A8
#define RFIP_ADDR_LNA_CFG                                               0x00AC
#define RFIP_ADDR_RXMIX_CFG                                             0x00B0
#define RFIP_ADDR_RXFILTER1_CFG1                                        0x00B4
#define RFIP_ADDR_RXFILTER1_CFG2                                        0x00B8
#define RFIP_ADDR_RXPKDET_CFG                                           0x00BC
#define RFIP_ADDR_VCO_CFG1                                              0x00C0
#define RFIP_ADDR_VCO_CFG2                                              0x00C4
#define RFIP_ADDR_RFPLL_CFG                                             0x00C8
#define RFIP_ADDR_PA_CFG                                                0x00CC
#define RFIP_ADDR_TXRF_CFG1                                             0x00D0
#define RFIP_ADDR_TXRF_CFG2                                             0x00D4
#define RFIP_ADDR_TXDAC_CFG                                             0x00D8
#define RFIP_ADDR_TXFILT_CFG                                            0x00DC
#define RFIP_ADDR_BG_CFG                                                0x00E0
#define RFIP_ADDR_TEMP_CFG                                              0x00E4
#define RFIP_ADDR_LO_CAL_CFG                                            0x00E8
#define RFIP_ADDR_LDO_ANALOG_CFG                                        0x00EC
#define RFIP_ADDR_RX_GAIN_CFG                                           0x00F0
#define RFIP_ADDR_RF_RESERVED                                           0x00FC
#define RFIP_ADDR_TX_RF_GAIN_CFG0                                       0x0100
#define RFIP_ADDR_TX_RF_GAIN_CFG1                                       0x0104
#define RFIP_ADDR_TX_RF_GAIN_CFG2                                       0x0108
#define RFIP_ADDR_TX_RF_GAIN_CFG3                                       0x010C
#define RFIP_ADDR_TX_RF_GAIN_CFG4                                       0x0110
#define RFIP_ADDR_TX_RF_GAIN_CFG5                                       0x0114
#define RFIP_ADDR_TX_RF_GAIN_CFG6                                       0x0118
#define RFIP_ADDR_TX_RF_GAIN_CFG7                                       0x011C
#define RFIP_ADDR_TX_RF_GAIN_CFG8                                       0x0120
#define RFIP_ADDR_TX_RF_GAIN_CFG9                                       0x0124
#define RFIP_ADDR_TX_RF_GAIN_CFG10                                      0x0128
#define RFIP_ADDR_TX_RF_GAIN_CFG11                                      0x012C
#define RFIP_ADDR_TX_RF_GAIN_CFG12                                      0x0130
#define RFIP_ADDR_TX_RF_GAIN_CFG13                                      0x0134
#define RFIP_ADDR_TX_RF_GAIN_CFG14                                      0x0138
#define RFIP_ADDR_TX_RF_GAIN_CFG15                                      0x013C
#define RFIP_ADDR_TX_DIG_GAIN_CFG0                                      0x0140
#define RFIP_ADDR_TX_DIG_GAIN_CFG1                                      0x0144
#define RFIP_ADDR_TX_DIG_GAIN_CFG2                                      0x0148
#define RFIP_ADDR_TX_DIG_GAIN_CFG3                                      0x014C
#define RFIP_ADDR_TX_DIG_GAIN_CFG4                                      0x0150
#define RFIP_ADDR_TX_DIG_GAIN_CFG5                                      0x0154
#define RFIP_ADDR_TX_DIG_GAIN_CFG6                                      0x0158
#define RFIP_ADDR_TX_TEMP_INDEX_CFG0                                    0x0160
#define RFIP_ADDR_TX_TEMP_INDEX_CFG1                                    0x0164
#define RFIP_ADDR_TX_TEMP_INDEX_CFG2                                    0x0168
#define RFIP_ADDR_TX_TEMP_INDEX_CFG3                                    0x016C
#define RFIP_ADDR_TX_TEMP_INDEX_CFG4                                    0x0170
#define RFIP_ADDR_TX_TEMP_INDEX_CFG5                                    0x0174
#define RFIP_ADDR_TX_TEMP_INDEX_CFG6                                    0x0178
#define RFIP_ADDR_TX_TEMP_INDEX_CFG7                                    0x017C
#define RFIP_ADDR_TX_TEMP_INDEX_CFG8                                    0x0180
#define RFIP_ADDR_TX_TEMP_INDEX_CFG9                                    0x0184
#define RFIP_ADDR_TX_TEMP_INDEX_CFG10                                   0x0188
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG0                               0x0190
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG1                               0x0194
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG2                               0x0198
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG3                               0x019C
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG4                               0x01A0
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG5                               0x01A4
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG6                               0x01A8
#define RFIP_ADDR_TPC_COMP_LSB_CFG0                                     0x01B0
#define RFIP_ADDR_TPC_COMP_LSB_CFG1                                     0x01B4
#define RFIP_ADDR_TPC_COMP_LSB_CFG2                                     0x01B8
#define RFIP_ADDR_TPC_COMP_LSB_CFG3                                     0x01BC
#define RFIP_ADDR_TPC_COMP_LSB_CFG4                                     0x01C0
#define RFIP_ADDR_TPC_COMP_LSB_CFG5                                     0x01C4
#define RFIP_ADDR_TPC_COMP_LSB_CFG6                                     0x01C8
#define RFIP_ADDR_LOFT_COMP_I_CFG0                                      0x01D0
#define RFIP_ADDR_LOFT_COMP_I_CFG1                                      0x01D4
#define RFIP_ADDR_LOFT_COMP_I_CFG2                                      0x01D8
#define RFIP_ADDR_LOFT_COMP_I_CFG3                                      0x01DC
#define RFIP_ADDR_LOFT_COMP_I_CFG4                                      0x01E0
#define RFIP_ADDR_LOFT_COMP_I_CFG5                                      0x01E4
#define RFIP_ADDR_LOFT_COMP_I_CFG6                                      0x01E8
#define RFIP_ADDR_LOFT_COMP_I_CFG7                                      0x01EC
#define RFIP_ADDR_LOFT_COMP_Q_CFG0                                      0x01F0
#define RFIP_ADDR_LOFT_COMP_Q_CFG1                                      0x01F4
#define RFIP_ADDR_LOFT_COMP_Q_CFG2                                      0x01F8
#define RFIP_ADDR_LOFT_COMP_Q_CFG3                                      0x01FC
#define RFIP_ADDR_LOFT_COMP_Q_CFG4                                      0x0200
#define RFIP_ADDR_LOFT_COMP_Q_CFG5                                      0x0204
#define RFIP_ADDR_LOFT_COMP_Q_CFG6                                      0x0208
#define RFIP_ADDR_LOFT_COMP_Q_CFG7                                      0x020C
#define RFIP_ADDR_LOFT_SFT_CFG_I_CFG0                                   0x0210
#define RFIP_ADDR_LOFT_SFT_CFG_I_CFG1                                   0x0214
#define RFIP_ADDR_LOFT_SFT_CFG_Q_CFG0                                   0x0218
#define RFIP_ADDR_LOFT_SFT_CFG_Q_CFG1                                   0x021C
#define RFIP_ADDR_TX_DC_OFFSET_CFG                                      0x0220
#define RFIP_ADDR_TX_CTRL_CFG                                           0x0224
#define RFIP_ADDR_TX_SRAM_TEST_CFG                                      0x0228
#define RFIP_ADDR_TX_SRAM_TEST_DR                                       0x022C
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG0                                0x0230
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG1                                0x0234
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG2                                0x0238
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG3                                0x023C
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG4                                0x0240
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG5                                0x0244
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG6                                0x0248
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG7                                0x024C
#define RFIP_ADDR_GPIO_MODE_CFG0                                        0x0300
#define RFIP_ADDR_GPIO_MODE_CFG1                                        0x0304
#define RFIP_ADDR_GPIO_RX_FORCE_CFG                                     0x0308
#define RFIP_ADDR_GPIO_TX_FORCE_CFG                                     0x030C
#define RFIP_ADDR_GPIO_FORCE_EN_CFG                                     0x0310
#define RFIP_ADDR_TX_GAIN_FORCE_CFG                                     0x03E4
#define RFIP_ADDR_TX_COMP_FORCE_CFG                                     0x03E8
#define RFIP_ADDR_RFIPSTATE                                             0x03EC
#define RFIP_ADDR_RFIPCALIN                                             0x03F0
#define RFIP_ADDR_RFPLLDEBUGCFG                                         0x03F4
#define RFIP_ADDR_RFPLLDEBUGCFG2                                        0x03F8
#define RFIP_ADDR_TXRX_DEBUG                                            0x03FC

/* RFIP_ADDR_GCFG */
#define FORCESTATE                                                      BIT1 // Force Off State
#define SOFT_RESET                                                      BIT0 // Soft reset all registers

/* RFIP_ADDR_RX_STATE_DELAY_CFG1 */
#define RX_ANA_ON_DELAY_MASK                                            CO_BITS(0, 15) // delay time from RX_ANA_ON to RX_IDLE
#define RX_ANA_ON_DELAY_SHIFT                                           0
#define RX_IDLE_DELAY_MASK                                              CO_BITS(16, 31) // delay time from RX_IDLE to RX_ACTIVE
#define RX_IDLE_DELAY_SHIFT                                             16

/* RFIP_ADDR_RX_STATE_DELAY_CFG2 */
#define RX_FASTON_DELAY_MASK                                            CO_BITS(0, 15) // delay time from RX_FAST_ON to RX_OFF
#define RX_FASTON_DELAY_SHIFT                                           0

/* RFIP_ADDR_TX_STATE_DELAY_CFG1 */
#define TX_ANA_ON_DELAY_MASK                                            CO_BITS(0, 15) // delay time from TX_ANA_ON to TX_ACTIVE
#define TX_ANA_ON_DELAY_SHIFT                                           0
#define TX_RFOFF_DELAY_MASK                                             CO_BITS(16, 31) // delay time from TX_RF_OFF to TX_OFF
#define TX_RFOFF_DELAY_SHIFT                                            16

/* RFIP_ADDR_RXOFF_CFG */
#define PU_SARADC                                                       BIT13
#define PU_SDMADC                                                       BIT12
#define RX_ADC_RST_CFG                                                  BIT11
#define PU_RXFLT_POST                                                   BIT10
#define PLLS_CLK4SAR_160_EN                                             BIT9
#define VCO_RXBUF_EN                                                    BIT8
#define PU_RXPKDET2                                                     BIT7
#define PU_RXPKDET1                                                     BIT6
#define RXFILTER_OS_EN                                                  BIT5
#define PUP_FILTER_PRE                                                  BIT4
#define PU_RXLO                                                         BIT3
#define PU_TIA                                                          BIT2
#define PU_RMIX                                                         BIT1
#define PU_LNA                                                          BIT0

/* RFIP_ADDR_RXANA_CFG */
#define PU_SARADC                                                       BIT13
#define PU_SDMADC                                                       BIT12
#define RX_ADC_RST_CFG                                                  BIT11
#define PU_RXFLT_POST                                                   BIT10
#define PLLS_CLK4SAR_160_EN                                             BIT9
#define VCO_RXBUF_EN                                                    BIT8
#define PU_RXPKDET2                                                     BIT7
#define PU_RXPKDET1                                                     BIT6
#define RXFILTER_OS_EN                                                  BIT5
#define PUP_FILTER_PRE                                                  BIT4
#define PU_RXLO                                                         BIT3
#define PU_TIA                                                          BIT2
#define PU_RMIX                                                         BIT1
#define PU_LNA                                                          BIT0

/* RFIP_ADDR_RXIDLE_CFG */
#define PU_SARADC                                                       BIT13
#define PU_SDMADC                                                       BIT12
#define RX_ADC_RST_CFG                                                  BIT11
#define PU_RXFLT_POST                                                   BIT10
#define PLLS_CLK4SAR_160_EN                                             BIT9
#define VCO_RXBUF_EN                                                    BIT8
#define PU_RXPKDET2                                                     BIT7
#define PU_RXPKDET1                                                     BIT6
#define RXFILTER_OS_EN                                                  BIT5
#define PUP_FILTER_PRE                                                  BIT4
#define PU_RXLO                                                         BIT3
#define PU_TIA                                                          BIT2
#define PU_RMIX                                                         BIT1
#define PU_LNA                                                          BIT0

/* RFIP_ADDR_RXACTIVE_CFG */
#define PU_SARADC                                                       BIT13
#define PU_SDMADC                                                       BIT12
#define RX_ADC_RST_CFG                                                  BIT11
#define PU_RXFLT_POST                                                   BIT10
#define PLLS_CLK4SAR_160_EN                                             BIT9
#define VCO_RXBUF_EN                                                    BIT8
#define PU_RXPKDET2                                                     BIT7
#define PU_RXPKDET1                                                     BIT6
#define RXFILTER_OS_EN                                                  BIT5
#define PUP_FILTER_PRE                                                  BIT4
#define PU_RXLO                                                         BIT3
#define PU_TIA                                                          BIT2
#define PU_RMIX                                                         BIT1
#define PU_LNA                                                          BIT0

/* RFIP_ADDR_TXOFF_CFG */
#define PU_TXFLT                                                        BIT8
#define PU_TXDAC                                                        BIT7
#define PU_TXAUXL                                                       BIT6
#define PU_TXAUXH                                                       BIT5
#define PU_TXMIXER                                                      BIT4
#define PU_TXLO                                                         BIT3
#define PU_PA                                                           BIT2
#define VCO_TXBUF_EN                                                    BIT1
#define EN_TX                                                           BIT0

/* RFIP_ADDR_TXANA_CFG */
#define PU_TXFLT                                                        BIT8
#define PU_TXDAC                                                        BIT7
#define PU_TXAUXL                                                       BIT6
#define PU_TXAUXH                                                       BIT5
#define PU_TXMIXER                                                      BIT4
#define PU_TXLO                                                         BIT3
#define PU_PA                                                           BIT2
#define VCO_TXBUF_EN                                                    BIT1
#define EN_TX                                                           BIT0

/* RFIP_ADDR_TXON_CFG */
#define PU_TXFLT                                                        BIT8
#define PU_TXDAC                                                        BIT7
#define PU_TXAUXL                                                       BIT6
#define PU_TXAUXH                                                       BIT5
#define PU_TXMIXER                                                      BIT4
#define PU_TXLO                                                         BIT3
#define PU_PA                                                           BIT2
#define VCO_TXBUF_EN                                                    BIT1
#define EN_TX                                                           BIT0

/* RFIP_ADDR_TXRFOFF_CFG */
#define PU_TXFLT                                                        BIT8
#define PU_TXDAC                                                        BIT7
#define PU_TXAUXL                                                       BIT6
#define PU_TXAUXH                                                       BIT5
#define PU_TXMIXER                                                      BIT4
#define PU_TXLO                                                         BIT3
#define PU_PA                                                           BIT2
#define VCO_TXBUF_EN                                                    BIT1
#define EN_TX                                                           BIT0

/* RFIP_ADDR_LDO_CFG */
#define LDOCLK_BYP                                                      BIT31
#define LDOCLK_CAL_MASK                                                 CO_BITS(28, 30)
#define LDOCLK_CAL_SHIFT                                                28
#define LDOCLK_LQB                                                      BIT27
#define LDOCLK_TEST                                                     BIT26
#define PU_LDOCLK                                                       BIT25

/* RFIP_ADDR_XTAL_CFG */
#define PU_XTAL_DIG_REGSTR                                              BIT31
#define REG_XTAL_VBIT_MASK                                              CO_BITS(27, 30)
#define REG_XTAL_VBIT_SHIFT                                             27
#define XCLK4FPGA_EN_LV                                                 BIT26
#define XCLK4MCU_EN_LV                                                  BIT25
#define XCLK4PLLCAL_EN_LV                                               BIT24
#define XCLK4PLLS_EN_LV                                                 BIT23
#define XTAL_EN_OSC                                                     BIT22
#define XTAL_EN_RFPLL_BUFF                                              BIT21
#define XTAL_FREQ_TUNING_MASK                                           CO_BITS(14, 20)
#define XTAL_FREQ_TUNING_SHIFT                                          14
#define XTAL_OSC_IBIT_MASK                                              CO_BITS(9, 13)
#define XTAL_OSC_IBIT_SHIFT                                             9
#define PU_XTAL_REGSTR                                                  BIT8

/* RFIP_ADDR_PLLS_CFG1 */
#define PLLS_CLK4DIG_160_EN                                             BIT31
#define PLLS_CLK_320_EN                                                 BIT30
#define PLLS_CLK_480_EN                                                 BIT29
#define PLLS_CLK_960_EN                                                 BIT28
#define PLLS_DITHER_LV_MASK                                             CO_BITS(26, 27)
#define PLLS_DITHER_LV_SHIFT                                            26
#define PLLS_FORCELOCK_LV                                               BIT25
#define PLLS_FRAC_CONTROL_LV                                            BIT24
#define PLLS_IDF_LV_MASK                                                CO_BITS(20, 23)
#define PLLS_IDF_LV_SHIFT                                               20
#define PLLS_NDIV_LV_MASK                                               CO_BITS(12, 19)
#define PLLS_NDIV_LV_SHIFT                                              12
#define PLLS_ENABLE_LV                                                  BIT11
#define PU_PLLS_LDO_LV                                                  BIT10
#define PLLS_LOCK_LV_DD                                                 BIT9

/* RFIP_ADDR_PLLS_CFG2 */
#define PLLS_FRAC_LV_MASK                                               CO_BITS(16, 31)
#define PLLS_FRAC_LV_SHIFT                                              16

/* RFIP_ADDR_RFPLL_LOOP_CFG */
#define LDO_RFPLL_BYP_REGSTR                                            BIT31
#define LDO_RFPLL_CAL_REGSTR_MASK                                       CO_BITS(28, 30)
#define LDO_RFPLL_CAL_REGSTR_SHIFT                                      28
#define LDO_RFPLL_LQB_REGSTR                                            BIT27
#define LDO_RFPLL_PU_REGSTR                                             BIT26
#define LDO_RFPLL_TST_REGSTR                                            BIT25
#define RFPLL_REG_LOOP_VBIT_V22_MASK                                    CO_BITS(21, 24)
#define RFPLL_REG_LOOP_VBIT_V22_SHIFT                                   21
#define RFPLL_CPIBIT_V22_MASK                                           CO_BITS(18, 20)
#define RFPLL_CPIBIT_V22_SHIFT                                          18
#define RFPLL_CPRBIT_MASK                                               CO_BITS(15, 17)
#define RFPLL_CPRBIT_SHIFT                                              15
#define RFPLL_DIS_LF                                                    BIT14
#define RFPLL_DIS_PDT                                                   BIT13
#define RFPLL_DIS_VCTRL                                                 BIT12
#define RFPLL_SMODE                                                     BIT11
#define RFPLL_VCO_GAIN_MASK                                             CO_BITS(7, 10)
#define RFPLL_VCO_GAIN_SHIFT                                            7
#define PU_RFPLL_LOOP_V22                                               BIT6
#define REG_RFPLL_PFDEN_DR                                              BIT0

/* RFIP_ADDR_RFPLL_DIG_CFG1 */
#define RFPLL_CO_AMP_BYPASS                                             BIT26
#define RFPLL_MASHFIL_CKEN                                              BIT25
#define RFPLL_XRST_MASH                                                 BIT24
#define RFPLL_XRST_SD                                                   BIT23
#define RFPLL_SDCLK_EN                                                  BIT22
#define RFPLL_XRST_SDFILT                                               BIT21
#define RFPLL_SD_MOD_MASK                                               CO_BITS(19, 20)
#define RFPLL_SD_MOD_SHIFT                                              19
#define RFPLL_SD_FILT                                                   BIT18
#define RFPLL_FINE_AMP_BYPASS                                           BIT17
#define RFPLL_CO_FREQ_BYPASS                                            BIT16
#define RFPLL_NDIVIDER_REG_MASK                                         CO_BITS(1, 15)
#define RFPLL_NDIVIDER_REG_SHIFT                                        1
#define RFPLL_NDIVIDER_REG_EN                                           BIT0

/* RFIP_ADDR_RFPLL_DIG_CFG2 */
#define RFPLL_SEL_FREQPLLIN_MASK                                        CO_BITS(28, 31)
#define RFPLL_SEL_FREQPLLIN_SHIFT                                       28
#define RFPLL_SEL_FCOMPMES_MASK                                         CO_BITS(24, 27)
#define RFPLL_SEL_FCOMPMES_SHIFT                                        24
#define RFPLL_ORDER_FWP_MASK                                            CO_BITS(17, 23)
#define RFPLL_ORDER_FWP_SHIFT                                           17
#define RFPLL_XPD_PIPELINE_LB                                           BIT16
#define RFPLL_FREQ_CAL_ACCURACY_MASK                                    CO_BITS(12, 15)
#define RFPLL_FREQ_CAL_ACCURACY_SHIFT                                   12
#define RFPLL_FREQ_NUM_PERIOD_SET_MASK                                  CO_BITS(9, 11)
#define RFPLL_FREQ_NUM_PERIOD_SET_SHIFT                                 9
#define RFPLL_WAIT_FINE_F_MASK                                          CO_BITS(6, 8)
#define RFPLL_WAIT_FINE_F_SHIFT                                         6
#define RFPLL_XRST_SYNCH                                                BIT5
#define RFPLL_XRST_CLK_CTRL                                             BIT4
#define RFPLL_XRST_DIG                                                  BIT3
#define RFPLL_XRST_CMOS_DIVIDER                                         BIT2

/* RFIP_ADDR_RFPLL_DIG_INT_FRAC_CFG */
#define RFPLL_INT_MASK                                                  CO_BITS(21, 29)
#define RFPLL_INT_SHIFT                                                 21
#define RFPLL_FRAC_MASK                                                 CO_BITS(0, 20)
#define RFPLL_FRAC_SHIFT                                                0

/* RFIP_ADDR_RFPLL_DIG_VCOCAL_CFG */
#define RFPLL_LOCK_DET_LV_DD                                            BIT31
#define RFPLL_LOCK_NUM_MASK                                             CO_BITS(23, 30)
#define RFPLL_LOCK_NUM_SHIFT                                            23
#define RFPLL_CAL_REQ                                                   BIT19
#define RFPLL_XRST_CALIBRATOR                                           BIT18
#define RFPLL_CALFREQ_EXT_MASK                                          CO_BITS(10, 17)
#define RFPLL_CALFREQ_EXT_SHIFT                                         10
#define RFPLL_CALFREQ_OUT_MASK                                          CO_BITS(2, 9)
#define RFPLL_CALFREQ_OUT_SHIFT                                         2
#define RFPLL_CALFREQ_EXT_SEL                                           BIT1
#define RFPLL_END_CAL_DD                                                BIT0

/* RFIP_ADDR_MDLL_CFG */
#define MDLL_CP_IBIT_MASK                                               CO_BITS(29, 31)
#define MDLL_CP_IBIT_SHIFT                                              29
#define MDLL_IVBIT_MASK                                                 CO_BITS(26, 28)
#define MDLL_IVBIT_SHIFT                                                26
#define MDLL_PDT_MODE                                                   BIT25
#define MDLL_CKX2_CBIT_MASK                                             CO_BITS(22, 24)
#define MDLL_CKX2_CBIT_SHIFT                                            22
#define MDLL_CKX2_EN                                                    BIT21
#define MDLL_CKX2_RBIT                                                  BIT20
#define MDLL_CLK_TEST_EN                                                BIT19
#define MDLL_DIV_NUM_MASK                                               CO_BITS(15, 18)
#define MDLL_DIV_NUM_SHIFT                                              15
#define MDLL_CBIT_MASK                                                  CO_BITS(12, 14)
#define MDLL_CBIT_SHIFT                                                 12
#define MDLL_RBIT                                                       BIT11
#define MDLL_CLK_RFPLL_EN                                               BIT10
#define RSTN_MDLL                                                       BIT9
#define PU_MDLL                                                         BIT8

/* RFIP_ADDR_RX_LPF2_CFG */
#define RXFLT_POST_CCW_MASK                                             CO_BITS(26, 31)
#define RXFLT_POST_CCW_SHIFT                                            26
#define RXFLT_POST_BW_MODE                                              BIT25

/* RFIP_ADDR_SDMADC_CFG */
#define SDMADC_DWAORCYCLIC                                              BIT31
#define SDMADC_IAOP_CTRL_MASK                                           CO_BITS(28, 30)
#define SDMADC_IAOP_CTRL_SHIFT                                          28
#define SDMADC_ICMP_CTRL_MASK                                           CO_BITS(25, 27)
#define SDMADC_ICMP_CTRL_SHIFT                                          25
#define SDMADC_NODEM                                                    BIT24
#define SDMADC_VDIGSEL_MASK                                             CO_BITS(22, 23)
#define SDMADC_VDIGSEL_SHIFT                                            22
#define SDMADC_CAL_INPUT                                                BIT21
#define SDMADC_CLK_EN                                                   BIT20

/* RFIP_ADDR_SDMADC_DIG_CFG */
#define RXINVADCCLK                                                     BIT31               // RXADC sample clk inv enable
#define RXDIVIDERINIT_MASK                                              CO_BITS(29, 30) // RX clk divider init data
#define RXDIVIDERINIT_SHIFT                                             29
#define RX_ADCCAL_START_OFFSET_MASK                                     CO_BITS(17, 20)
#define RX_ADCCAL_START_OFFSET_SHIFT                                    17
#define RX_ADCCAL_THRESHOLD_MASK                                        CO_BITS(9, 16)
#define RX_ADCCAL_THRESHOLD_SHIFT                                       9
#define RX_ADCCAL_CNT2_MASK                                             CO_BITS(6, 8)
#define RX_ADCCAL_CNT2_SHIFT                                            6
#define RX_ADCCAL_CNT1_MASK                                             CO_BITS(3, 5)
#define RX_ADCCAL_CNT1_SHIFT                                            3
#define RX_ADCCAL_IQSEL                                                 BIT2
#define RX_ADCCAL_SWRST                                                 BIT1
#define ADCCAL_START                                                    BIT0

/* RFIP_ADDR_SDMADC_CIC_CFG */
#define RXCICEN_CFG                                                     BIT31  // RXCIC enable
#define RXCICRESETEN                                                    BIT30 // RXCIC reset enable when Sat
#define RXCICSATTHRESHOLD_MASK                                          CO_BITS(22, 29)
#define RXCICSATTHRESHOLD_SHIFT                                         22
#define RXCICOFFSET_EN                                                  BIT21

/* RFIP_ADDR_SARADC_CFG */
#define SARADC_LDO_SEL_MASK                                             CO_BITS(1, 2)
#define SARADC_LDO_SEL_SHIFT                                            1
#define SEL_SAR_ADC_CFG                                                 BIT0

/* RFIP_ADDR_AUXADC_CFG */
#define AUXADC_ON                                                       BIT31
#define AUXADC_IN_CHSEL_MASK                                            CO_BITS(29, 30)
#define AUXADC_IN_CHSEL_SHIFT                                           29
#define AUXADC_N_SAMP_MASK                                              CO_BITS(23, 28)
#define AUXADC_N_SAMP_SHIFT                                             23
#define AUXADC_BUFFER_MODE                                              BIT22
#define AUXADC_SPEED                                                    BIT21
#define LDO_AUXADC_TEST_EN                                              BIT20
#define LDO_AUXADC_VBIT_MASK                                            CO_BITS(18, 19)
#define LDO_AUXADC_VBIT_SHIFT                                           18
#define PU_LDO_AUXADC                                                   BIT17
#define AUXADC_DATA_MASK                                                CO_BITS(1, 10)
#define AUXADC_DATA_SHIFT                                               1
#define AUXADC_DATA_VALID_FLAG                                          BIT0

/* RFIP_ADDR_RCCAL_CFG */
#define RCCAL_BW_MODE                                                   BIT31
#define RCCAL_BW_CTRL_MASK                                              CO_BITS(29, 30)
#define RCCAL_BW_CTRL_SHIFT                                             29
#define RCCAL_CSAMP_DEL_MASK                                            CO_BITS(14, 22)
#define RCCAL_CSAMP_DEL_SHIFT                                           14
#define RCCAL_CFORCE                                                    BIT13
#define RCCAL_CTMAX_MASK                                                CO_BITS(7, 12)
#define RCCAL_CTMAX_SHIFT                                               7
#define TX_LPF_CCAL_FO                                                  BIT6
#define RX_LPF_CCAL_FO                                                  BIT5
#define TX_LPF_RC_UPDATE_CFG                                            BIT4
#define RX_LPF_RC_UPDATE_CFG                                            BIT3
#define RCCAL_DEBUG                                                     BIT2
#define RC_CAL_CEN                                                      BIT0

/* RFIP_ADDR_RCCAL_OFFSET_TEST_CFG */
#define RX_LPF_CCAL_OFFSET_MASK                                         CO_BITS(26, 31)
#define RX_LPF_CCAL_OFFSET_SHIFT                                        26
#define TX_LPF_CCAL_OFFSET_MASK                                         CO_BITS(20, 25)
#define TX_LPF_CCAL_OFFSET_SHIFT                                        20
#define RCCAL_CFORCE_VAL_MASK                                           CO_BITS(6, 11)
#define RCCAL_CFORCE_VAL_SHIFT                                          6
#define RCCAL_CFORCE_ADD_MASK                                           CO_BITS(0, 5)
#define RCCAL_CFORCE_ADD_SHIFT                                          0

/* RFIP_ADDR_MUX_CFG */
#define PLLS_TEST_EN                                                    BIT31

/* RFIP_ADDR_LDO_ANA_CFG */
#define LDO_ANA_PUP_REGSTR                                              BIT31
#define LDO_ANA_LQB_REGSTR                                              BIT30
#define LDO_ANA_CAL_REGSTR_MASK                                         CO_BITS(27, 29)
#define LDO_ANA_CAL_REGSTR_SHIFT                                        27
#define LDO_ANA_BYP_REGSTR                                              BIT26
#define LDO_ANA_TST_REGSTR                                              BIT25

/* RFIP_ADDR_LDO_VCO_CFG */
#define LDO_VCO_PUP_REGSTR                                              BIT31
#define LDO_VCO_LQB_REGSTR                                              BIT30
#define LDO_VCO_CAL_REGSTR_MASK                                         CO_BITS(27, 29)
#define LDO_VCO_CAL_REGSTR_SHIFT                                        27
#define LDO_VCO_BYP_REGSTR                                              BIT26
#define LDO_VCO_TST_REGSTR                                              BIT25

/* RFIP_ADDR_LNA_CFG */
#define LNA_REG_TRIM_MASK                                               CO_BITS(28, 31)
#define LNA_REG_TRIM_SHIFT                                              28
#define LNA_CAL_MASK                                                    CO_BITS(26, 27)
#define LNA_CAL_SHIFT                                                   26
#define LNA_RBIAS_TRIM_MASK                                             CO_BITS(22, 25)
#define LNA_RBIAS_TRIM_SHIFT                                            22
#define LNA_IBIAS_TRIM_MASK                                             CO_BITS(18, 21)
#define LNA_IBIAS_TRIM_SHIFT                                            18
#define RX_MATCH_C_MASK                                                 CO_BITS(14, 17)
#define RX_MATCH_C_SHIFT                                                14

/* RFIP_ADDR_RXMIX_CFG */
#define RXLO_VCM_MASK                                                   CO_BITS(29, 31)
#define RXLO_VCM_SHIFT                                                  29

/* RFIP_ADDR_RXFILTER1_CFG1 */
#define RXFILTER_AUXI_EN                                                BIT31
#define RXFILTER_AUXQ_EN                                                BIT30
#define RXFILTER_BW_MODE                                                BIT29
#define RXFILTER_CCW_MASK                                               CO_BITS(23, 28)
#define RXFILTER_CCW_SHIFT                                              23
#define RXFILTER_OS_I_EN                                                BIT22
#define RXFILTER_OS_Q_EN                                                BIT21
#define RXFILTER_OS_MODE                                                BIT20

/* RFIP_ADDR_RXFILTER1_CFG2 */
#define RXFILTER_OS_I_FORCE_VALUE_MASK                                  CO_BITS(23, 31) // from RX_DC_OFFSET Or force value
#define RXFILTER_OS_I_FORCE_VALUE_SHIFT                                 23
#define RXFILTER_OS_Q_FORCE_VALUE_MASK                                  CO_BITS(14, 22)
#define RXFILTER_OS_Q_FORCE_VALUE_SHIFT                                 14
#define RXFILTER_OS_FORCE_EN                                            BIT13

/* RFIP_ADDR_RXPKDET_CFG */
#define PKDET1_VREF_MASK                                                CO_BITS(3, 4)
#define PKDET1_VREF_SHIFT                                               3
#define PKDET2_VREF_MASK                                                CO_BITS(0, 2)
#define PKDET2_VREF_SHIFT                                               0

/* RFIP_ADDR_VCO_CFG1 */
#define PU_VCO                                                          BIT31
#define PU_DIV                                                          BIT30
#define REG_VCO_BIT_MASK                                                CO_BITS(26, 29)
#define REG_VCO_BIT_SHIFT                                               26
#define REG_VCOBUF_BIT_MASK                                             CO_BITS(22, 25)
#define REG_VCOBUF_BIT_SHIFT                                            22
#define VCO_DIVBIAS_BIT_MASK                                            CO_BITS(20, 21)
#define VCO_DIVBIAS_BIT_SHIFT                                           20
#define VCO_PKDET_EN                                                    BIT19
#define VCO_PKREF_BIT_MASK                                              CO_BITS(16, 18)
#define VCO_PKREF_BIT_SHIFT                                             16
#define VCO_VAR_BIT_MASK                                                CO_BITS(13, 15)
#define VCO_VAR_BIT_SHIFT                                               13
#define VCO_VCOM_BIT_MASK                                               CO_BITS(10, 12)
#define VCO_VCOM_BIT_SHIFT                                              10
#define VCO_VAR_SWAP                                                    BIT9
#define VCO_PK_OVLD_DD                                                  BIT8

/* RFIP_ADDR_VCO_CFG2 */
#define VCO_BAND_FORCE_EN                                               BIT31
#define VCO_BAND_MASK                                                   CO_BITS(23, 30)
#define VCO_BAND_SHIFT                                                  23

/* RFIP_ADDR_RFPLL_CFG */
#define PU_RFPLL_REGDIV2                                                BIT31
#define PU_RFPLL_REGPRESC                                               BIT30
#define RFPLL_REG_DIV2_VBIT_MASK                                        CO_BITS(26, 29)
#define RFPLL_REG_DIV2_VBIT_SHIFT                                       26
#define RFPLL_REG_PRESC_VBIT_MASK                                       CO_BITS(22, 25)
#define RFPLL_REG_PRESC_VBIT_SHIFT                                      22
#define RFPLL_PRESC_DIV23_EN                                            BIT21
#define RFPLL_SDMCLK_SEL                                                BIT20
#define RFPLL_FBC_SEL                                                   BIT19

/* RFIP_ADDR_PA_CFG */
#define REG_PAIBCAS_BIT_MASK                                            CO_BITS(6, 8)
#define REG_PAIBCAS_BIT_SHIFT                                           6
#define REG_PAIB_BIT_MASK                                               CO_BITS(3, 5)
#define REG_PAIB_BIT_SHIFT                                              3
#define REG_PAVBCS_BIT_MASK                                             CO_BITS(0, 2)
#define REG_PAVBCS_BIT_SHIFT                                            0

/* RFIP_ADDR_TXRF_CFG1 */
#define REG_TXCAPTUNE_MASK                                              CO_BITS(21, 24)
#define REG_TXCAPTUNE_SHIFT                                             21
#define REG_TXVBCAL_BIT_MASK                                            CO_BITS(19, 20)
#define REG_TXVBCAL_BIT_SHIFT                                           19
#define REG_TXVBCAS_BIT_MASK                                            CO_BITS(17, 18)
#define REG_TXVBCAS_BIT_SHIFT                                           17
#define REG_TXVBLO_BIT_MASK                                             CO_BITS(15, 16)
#define REG_TXVBLO_BIT_SHIFT                                            15
#define REG_TXIBIAS_BIT_MASK                                            CO_BITS(13, 14)
#define REG_TXIBIAS_BIT_SHIFT                                           13
#define REG_DEQ_BIT_MASK                                                CO_BITS(11, 12)
#define REG_DEQ_BIT_SHIFT                                               11
#define REG_TXCAL_DCI_EN                                                BIT10
#define REG_TXCAL_DCV_EN                                                BIT9
#define REG_TXCAL_EDGE                                                  BIT8
#define REG_TXCAL_SWAP                                                  BIT7
#define TXCAL_CLK_SEL_MASK                                              CO_BITS(5, 6)
#define TXCAL_CLK_SEL_SHIFT                                             5

/* RFIP_ADDR_TXRF_CFG2 */
#define REG_TXCALI_BIT_MASK                                             CO_BITS(23, 31)
#define REG_TXCALI_BIT_SHIFT                                            23
#define REG_TXCALQ_BIT_MASK                                             CO_BITS(14, 22)
#define REG_TXCALQ_BIT_SHIFT                                            14

/* RFIP_ADDR_TXDAC_CFG */
#define REG_TXDAC_MODE_MASK                                             CO_BITS(29, 31)
#define REG_TXDAC_MODE_SHIFT                                            29
#define REG_TXDAC_CMBIT_MASK                                            CO_BITS(26, 28)
#define REG_TXDAC_CMBIT_SHIFT                                           26
#define REG_TXDAC_LPMODE                                                BIT25
#define REG_TXDAC_RANGE_MASK                                            CO_BITS(23, 24)
#define REG_TXDAC_RANGE_SHIFT                                           23
#define REG_TXDAC_SEL_EDGE                                              BIT22
#define REG_TXDAC_VDVDD_MASK                                            CO_BITS(20, 21)
#define REG_TXDAC_VDVDD_SHIFT                                           20
#define REG_TXDAC_VLOWCTRL_MASK                                         CO_BITS(18, 19)
#define REG_TXDAC_VLOWCTRL_SHIFT                                        18
#define TXDC_CALOUT_I_DD                                                BIT17
#define TXDC_CALOUT_Q_DD                                                BIT16

/* RFIP_ADDR_TXFILT_CFG */
#define REG_TXFLT_CCW_MASK                                              CO_BITS(26, 31)
#define REG_TXFLT_CCW_SHIFT                                             26
#define REG_TXFLT_GAIN_MASK                                             CO_BITS(23, 25)
#define REG_TXFLT_GAIN_SHIFT                                            23
#define REG_TXFLT_MODE                                                  BIT22
#define REG_TXFLT_VCAS_MASK                                             CO_BITS(20, 21)
#define REG_TXFLT_VCAS_SHIFT                                            20

/* RFIP_ADDR_BG_CFG */
#define REG_BG_B_MASK                                                   CO_BITS(27, 31)
#define REG_BG_B_SHIFT                                                  27

/* RFIP_ADDR_TEMP_CFG */
#define PU_TEMP                                                         BIT31
#define TEMP_I_MASK                                                     CO_BITS(0, 9)
#define TEMP_I_SHIFT                                                    0

/* RFIP_ADDR_LO_CAL_CFG */
#define REG_LOCALATT_BIT_MASK                                           CO_BITS(30, 31)
#define REG_LOCALATT_BIT_SHIFT                                          30
#define EN_LOCAL                                                        BIT29

/* RFIP_ADDR_LDO_ANALOG_CFG */
#define CAL_LDODIG_L_MASK                                               CO_BITS(29, 31)
#define CAL_LDODIG_L_SHIFT                                              29
#define COMP_PU_L                                                       BIT28

/* RFIP_ADDR_RX_GAIN_CFG */
#define RX_GAIN_FORCE_EN                                                BIT31
#define LNA_GC_MASK                                                     CO_BITS(28, 30)
#define LNA_GC_SHIFT                                                    28
#define LNA_RLOAD                                                       BIT27
#define LNA_RMATCH                                                      BIT26
#define LPF1_GC_MASK                                                    CO_BITS(23, 25)
#define LPF1_GC_SHIFT                                                   23
#define LPF2_GC_MASK                                                    CO_BITS(19, 22)
#define LPF2_GC_SHIFT                                                   19
#define LPF3_GC_MASK                                                    CO_BITS(16, 18)
#define LPF3_GC_SHIFT                                                   16

/* RFIP_ADDR_RF_RESERVED */
#define REG_RESERVED_IN_MASK                                            CO_BITS(16, 31)
#define REG_RESERVED_IN_SHIFT                                           16
#define REG_RESERVED_OUT_MASK                                           CO_BITS(0, 3)
#define REG_RESERVED_OUT_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG0 */
#define TX_RF_GAIN_CFG1_MASK                                            CO_BITS(11, 21)
#define TX_RF_GAIN_CFG1_SHIFT                                           11
#define TX_RF_GAIN_CFG0_MASK                                            CO_BITS(0, 10)
#define TX_RF_GAIN_CFG0_SHIFT                                           0

/* RFIP_ADDR_TX_RF_GAIN_CFG1 */
#define TX_RF_GAIN_CFG3_MASK                                            CO_BITS(11, 21)
#define TX_RF_GAIN_CFG3_SHIFT                                           11
#define TX_RF_GAIN_CFG2_MASK                                            CO_BITS(0, 10)
#define TX_RF_GAIN_CFG2_SHIFT                                           0

/* RFIP_ADDR_TX_RF_GAIN_CFG2 */
#define TX_RF_GAIN_CFG5_MASK                                            CO_BITS(11, 21)
#define TX_RF_GAIN_CFG5_SHIFT                                           11
#define TX_RF_GAIN_CFG4_MASK                                            CO_BITS(0, 10)
#define TX_RF_GAIN_CFG4_SHIFT                                           0

/* RFIP_ADDR_TX_RF_GAIN_CFG3 */
#define TX_RF_GAIN_CFG7_MASK                                            CO_BITS(11, 21)
#define TX_RF_GAIN_CFG7_SHIFT                                           11
#define TX_RF_GAIN_CFG6_MASK                                            CO_BITS(0, 10)
#define TX_RF_GAIN_CFG6_SHIFT                                           0

/* RFIP_ADDR_TX_RF_GAIN_CFG4 */
#define TX_RF_GAIN_CFG9_MASK                                            CO_BITS(11, 21)
#define TX_RF_GAIN_CFG9_SHIFT                                           11
#define TX_RF_GAIN_CFG8_MASK                                            CO_BITS(0, 10)
#define TX_RF_GAIN_CFG8_SHIFT                                           0

/* RFIP_ADDR_TX_RF_GAIN_CFG5 */
#define TX_RF_GAIN_CFG11_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG11_SHIFT                                          11
#define TX_RF_GAIN_CFG10_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG10_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG6 */
#define TX_RF_GAIN_CFG13_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG13_SHIFT                                          11
#define TX_RF_GAIN_CFG12_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG12_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG7 */
#define TX_RF_GAIN_CFG15_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG15_SHIFT                                          11
#define TX_RF_GAIN_CFG14_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG14_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG8 */
#define TX_RF_GAIN_CFG17_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG17_SHIFT                                          11
#define TX_RF_GAIN_CFG16_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG16_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG9 */
#define TX_RF_GAIN_CFG19_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG19_SHIFT                                          11
#define TX_RF_GAIN_CFG18_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG18_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG10 */
#define TX_RF_GAIN_CFG21_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG21_SHIFT                                          11
#define TX_RF_GAIN_CFG20_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG20_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG11 */
#define TX_RF_GAIN_CFG23_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG23_SHIFT                                          11
#define TX_RF_GAIN_CFG22_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG22_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG12 */
#define TX_RF_GAIN_CFG25_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG25_SHIFT                                          11
#define TX_RF_GAIN_CFG24_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG24_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG13 */
#define TX_RF_GAIN_CFG27_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG27_SHIFT                                          11
#define TX_RF_GAIN_CFG26_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG26_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG14 */
#define TX_RF_GAIN_CFG29_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG29_SHIFT                                          11
#define TX_RF_GAIN_CFG28_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG28_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG15 */
#define TX_RF_GAIN_CFG31_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG31_SHIFT                                          11
#define TX_RF_GAIN_CFG30_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG30_SHIFT                                          0

/* RFIP_ADDR_TX_DIG_GAIN_CFG0 */
#define TX_DIG_GAIN_CFG4_MASK                                           CO_BITS(24, 29)
#define TX_DIG_GAIN_CFG4_SHIFT                                          24
#define TX_DIG_GAIN_CFG3_MASK                                           CO_BITS(18, 23)
#define TX_DIG_GAIN_CFG3_SHIFT                                          18
#define TX_DIG_GAIN_CFG2_MASK                                           CO_BITS(12, 17)
#define TX_DIG_GAIN_CFG2_SHIFT                                          12
#define TX_DIG_GAIN_CFG1_MASK                                           CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG1_SHIFT                                          6
#define TX_DIG_GAIN_CFG0_MASK                                           CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG0_SHIFT                                          0

/* RFIP_ADDR_TX_DIG_GAIN_CFG1 */
#define TX_DIG_GAIN_CFG9_MASK                                           CO_BITS(24, 29)
#define TX_DIG_GAIN_CFG9_SHIFT                                          24
#define TX_DIG_GAIN_CFG8_MASK                                           CO_BITS(18, 23)
#define TX_DIG_GAIN_CFG8_SHIFT                                          18
#define TX_DIG_GAIN_CFG7_MASK                                           CO_BITS(12, 17)
#define TX_DIG_GAIN_CFG7_SHIFT                                          12
#define TX_DIG_GAIN_CFG6_MASK                                           CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG6_SHIFT                                          6
#define TX_DIG_GAIN_CFG5_MASK                                           CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG5_SHIFT                                          0

/* RFIP_ADDR_TX_DIG_GAIN_CFG2 */
#define TX_DIG_GAIN_CFG14_MASK                                          CO_BITS(24, 29)
#define TX_DIG_GAIN_CFG14_SHIFT                                         24
#define TX_DIG_GAIN_CFG13_MASK                                          CO_BITS(18, 23)
#define TX_DIG_GAIN_CFG13_SHIFT                                         18
#define TX_DIG_GAIN_CFG12_MASK                                          CO_BITS(12, 17)
#define TX_DIG_GAIN_CFG12_SHIFT                                         12
#define TX_DIG_GAIN_CFG11_MASK                                          CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG11_SHIFT                                         6
#define TX_DIG_GAIN_CFG10_MASK                                          CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG10_SHIFT                                         0

/* RFIP_ADDR_TX_DIG_GAIN_CFG3 */
#define TX_DIG_GAIN_CFG19_MASK                                          CO_BITS(24, 29)
#define TX_DIG_GAIN_CFG19_SHIFT                                         24
#define TX_DIG_GAIN_CFG18_MASK                                          CO_BITS(18, 23)
#define TX_DIG_GAIN_CFG18_SHIFT                                         18
#define TX_DIG_GAIN_CFG17_MASK                                          CO_BITS(12, 17)
#define TX_DIG_GAIN_CFG17_SHIFT                                         12
#define TX_DIG_GAIN_CFG16_MASK                                          CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG16_SHIFT                                         6
#define TX_DIG_GAIN_CFG15_MASK                                          CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG15_SHIFT                                         0

/* RFIP_ADDR_TX_DIG_GAIN_CFG4 */
#define TX_DIG_GAIN_CFG24_MASK                                          CO_BITS(24, 29)
#define TX_DIG_GAIN_CFG24_SHIFT                                         24
#define TX_DIG_GAIN_CFG23_MASK                                          CO_BITS(18, 23)
#define TX_DIG_GAIN_CFG23_SHIFT                                         18
#define TX_DIG_GAIN_CFG22_MASK                                          CO_BITS(12, 17)
#define TX_DIG_GAIN_CFG22_SHIFT                                         12
#define TX_DIG_GAIN_CFG21_MASK                                          CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG21_SHIFT                                         6
#define TX_DIG_GAIN_CFG20_MASK                                          CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG20_SHIFT                                         0

/* RFIP_ADDR_TX_DIG_GAIN_CFG5 */
#define TX_DIG_GAIN_CFG29_MASK                                          CO_BITS(24, 29)
#define TX_DIG_GAIN_CFG29_SHIFT                                         24
#define TX_DIG_GAIN_CFG28_MASK                                          CO_BITS(18, 23)
#define TX_DIG_GAIN_CFG28_SHIFT                                         18
#define TX_DIG_GAIN_CFG27_MASK                                          CO_BITS(12, 17)
#define TX_DIG_GAIN_CFG27_SHIFT                                         12
#define TX_DIG_GAIN_CFG26_MASK                                          CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG26_SHIFT                                         6
#define TX_DIG_GAIN_CFG25_MASK                                          CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG25_SHIFT                                         0

/* RFIP_ADDR_TX_DIG_GAIN_CFG6 */
#define SCALE_FACTOR_MASK                                               CO_BITS(21, 31)
#define SCALE_FACTOR_SHIFT                                              21
#define TX_DIG_GAIN_DB_MASK                                             CO_BITS(12, 20)
#define TX_DIG_GAIN_DB_SHIFT                                            12
#define TX_DIG_GAIN_CFG31_MASK                                          CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG31_SHIFT                                         6
#define TX_DIG_GAIN_CFG30_MASK                                          CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG30_SHIFT                                         0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG0 */
#define TEMP_INDEX2_MASK                                                CO_BITS(20, 29)
#define TEMP_INDEX2_SHIFT                                               20
#define TEMP_INDEX1_MASK                                                CO_BITS(10, 19)
#define TEMP_INDEX1_SHIFT                                               10
#define TEMP_INDEX0_MASK                                                CO_BITS(0, 9)
#define TEMP_INDEX0_SHIFT                                               0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG1 */
#define TEMP_INDEX5_MASK                                                CO_BITS(20, 29)
#define TEMP_INDEX5_SHIFT                                               20
#define TEMP_INDEX4_MASK                                                CO_BITS(10, 19)
#define TEMP_INDEX4_SHIFT                                               10
#define TEMP_INDEX3_MASK                                                CO_BITS(0, 9)
#define TEMP_INDEX3_SHIFT                                               0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG2 */
#define TEMP_INDEX8_MASK                                                CO_BITS(20, 29)
#define TEMP_INDEX8_SHIFT                                               20
#define TEMP_INDEX7_MASK                                                CO_BITS(10, 19)
#define TEMP_INDEX7_SHIFT                                               10
#define TEMP_INDEX6_MASK                                                CO_BITS(0, 9)
#define TEMP_INDEX6_SHIFT                                               0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG3 */
#define TEMP_INDEX11_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX11_SHIFT                                              20
#define TEMP_INDEX10_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX10_SHIFT                                              10
#define TEMP_INDEX9_MASK                                                CO_BITS(0, 9)
#define TEMP_INDEX9_SHIFT                                               0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG4 */
#define TEMP_INDEX14_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX14_SHIFT                                              20
#define TEMP_INDEX13_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX13_SHIFT                                              10
#define TEMP_INDEX12_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX12_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG5 */
#define TEMP_INDEX17_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX17_SHIFT                                              20
#define TEMP_INDEX16_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX16_SHIFT                                              10
#define TEMP_INDEX15_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX15_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG6 */
#define TEMP_INDEX20_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX20_SHIFT                                              20
#define TEMP_INDEX19_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX19_SHIFT                                              10
#define TEMP_INDEX18_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX18_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG7 */
#define TEMP_INDEX23_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX23_SHIFT                                              20
#define TEMP_INDEX22_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX22_SHIFT                                              10
#define TEMP_INDEX21_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX21_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG8 */
#define TEMP_INDEX26_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX26_SHIFT                                              20
#define TEMP_INDEX25_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX25_SHIFT                                              10
#define TEMP_INDEX24_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX24_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG9 */
#define TEMP_INDEX29_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX29_SHIFT                                              20
#define TEMP_INDEX28_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX28_SHIFT                                              10
#define TEMP_INDEX27_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX27_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG10 */
#define TEMP_INDEX31_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX31_SHIFT                                              10
#define TEMP_INDEX30_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX30_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG0 */
#define TEMP_COMP_INDEX_CFG4_MASK                                       CO_BITS(24, 29)
#define TEMP_COMP_INDEX_CFG4_SHIFT                                      24
#define TEMP_COMP_INDEX_CFG3_MASK                                       CO_BITS(18, 23)
#define TEMP_COMP_INDEX_CFG3_SHIFT                                      18
#define TEMP_COMP_INDEX_CFG2_MASK                                       CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG2_SHIFT                                      12
#define TEMP_COMP_INDEX_CFG1_MASK                                       CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG1_SHIFT                                      6
#define TEMP_COMP_INDEX_CFG0_MASK                                       CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG0_SHIFT                                      0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG1 */
#define TEMP_COMP_INDEX_CFG9_MASK                                       CO_BITS(24, 29)
#define TEMP_COMP_INDEX_CFG9_SHIFT                                      24
#define TEMP_COMP_INDEX_CFG8_MASK                                       CO_BITS(18, 23)
#define TEMP_COMP_INDEX_CFG8_SHIFT                                      18
#define TEMP_COMP_INDEX_CFG7_MASK                                       CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG7_SHIFT                                      12
#define TEMP_COMP_INDEX_CFG6_MASK                                       CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG6_SHIFT                                      6
#define TEMP_COMP_INDEX_CFG5_MASK                                       CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG5_SHIFT                                      0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG2 */
#define TEMP_COMP_INDEX_CFG14_MASK                                      CO_BITS(24, 29)
#define TEMP_COMP_INDEX_CFG14_SHIFT                                     24
#define TEMP_COMP_INDEX_CFG13_MASK                                      CO_BITS(18, 23)
#define TEMP_COMP_INDEX_CFG13_SHIFT                                     18
#define TEMP_COMP_INDEX_CFG12_MASK                                      CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG12_SHIFT                                     12
#define TEMP_COMP_INDEX_CFG11_MASK                                      CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG11_SHIFT                                     6
#define TEMP_COMP_INDEX_CFG10_MASK                                      CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG10_SHIFT                                     0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG3 */
#define TEMP_COMP_INDEX_CFG19_MASK                                      CO_BITS(24, 29)
#define TEMP_COMP_INDEX_CFG19_SHIFT                                     24
#define TEMP_COMP_INDEX_CFG18_MASK                                      CO_BITS(18, 23)
#define TEMP_COMP_INDEX_CFG18_SHIFT                                     18
#define TEMP_COMP_INDEX_CFG17_MASK                                      CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG17_SHIFT                                     12
#define TEMP_COMP_INDEX_CFG16_MASK                                      CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG16_SHIFT                                     6
#define TEMP_COMP_INDEX_CFG15_MASK                                      CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG15_SHIFT                                     0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG4 */
#define TEMP_COMP_INDEX_CFG24_MASK                                      CO_BITS(24, 29)
#define TEMP_COMP_INDEX_CFG24_SHIFT                                     24
#define TEMP_COMP_INDEX_CFG23_MASK                                      CO_BITS(18, 23)
#define TEMP_COMP_INDEX_CFG23_SHIFT                                     18
#define TEMP_COMP_INDEX_CFG22_MASK                                      CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG22_SHIFT                                     12
#define TEMP_COMP_INDEX_CFG21_MASK                                      CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG21_SHIFT                                     6
#define TEMP_COMP_INDEX_CFG20_MASK                                      CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG20_SHIFT                                     0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG5 */
#define TEMP_COMP_INDEX_CFG29_MASK                                      CO_BITS(24, 29)
#define TEMP_COMP_INDEX_CFG29_SHIFT                                     24
#define TEMP_COMP_INDEX_CFG28_MASK                                      CO_BITS(18, 23)
#define TEMP_COMP_INDEX_CFG28_SHIFT                                     18
#define TEMP_COMP_INDEX_CFG27_MASK                                      CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG27_SHIFT                                     12
#define TEMP_COMP_INDEX_CFG26_MASK                                      CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG26_SHIFT                                     6
#define TEMP_COMP_INDEX_CFG25_MASK                                      CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG25_SHIFT                                     0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG6 */
#define TEMP_COMP_INDEX_CFG32_MASK                                      CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG32_SHIFT                                     12
#define TEMP_COMP_INDEX_CFG31_MASK                                      CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG31_SHIFT                                     6
#define TEMP_COMP_INDEX_CFG30_MASK                                      CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG30_SHIFT                                     0

/* RFIP_ADDR_TPC_COMP_LSB_CFG0 */
#define TPC_COMP_LSB_CFG4_MASK                                          CO_BITS(24, 29)
#define TPC_COMP_LSB_CFG4_SHIFT                                         24
#define TPC_COMP_LSB_CFG3_MASK                                          CO_BITS(18, 23)
#define TPC_COMP_LSB_CFG3_SHIFT                                         18
#define TPC_COMP_LSB_CFG2_MASK                                          CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG2_SHIFT                                         12
#define TPC_COMP_LSB_CFG1_MASK                                          CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG1_SHIFT                                         6
#define TPC_COMP_LSB_CFG0_MASK                                          CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG0_SHIFT                                         0

/* RFIP_ADDR_TPC_COMP_LSB_CFG1 */
#define TPC_COMP_LSB_CFG9_MASK                                          CO_BITS(24, 29)
#define TPC_COMP_LSB_CFG9_SHIFT                                         24
#define TPC_COMP_LSB_CFG8_MASK                                          CO_BITS(18, 23)
#define TPC_COMP_LSB_CFG8_SHIFT                                         18
#define TPC_COMP_LSB_CFG7_MASK                                          CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG7_SHIFT                                         12
#define TPC_COMP_LSB_CFG6_MASK                                          CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG6_SHIFT                                         6
#define TPC_COMP_LSB_CFG5_MASK                                          CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG5_SHIFT                                         0

/* RFIP_ADDR_TPC_COMP_LSB_CFG2 */
#define TPC_COMP_LSB_CFG14_MASK                                         CO_BITS(24, 29)
#define TPC_COMP_LSB_CFG14_SHIFT                                        24
#define TPC_COMP_LSB_CFG13_MASK                                         CO_BITS(18, 23)
#define TPC_COMP_LSB_CFG13_SHIFT                                        18
#define TPC_COMP_LSB_CFG12_MASK                                         CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG12_SHIFT                                        12
#define TPC_COMP_LSB_CFG11_MASK                                         CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG11_SHIFT                                        6
#define TPC_COMP_LSB_CFG10_MASK                                         CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG10_SHIFT                                        0

/* RFIP_ADDR_TPC_COMP_LSB_CFG3 */
#define TPC_COMP_LSB_CFG19_MASK                                         CO_BITS(24, 29)
#define TPC_COMP_LSB_CFG19_SHIFT                                        24
#define TPC_COMP_LSB_CFG18_MASK                                         CO_BITS(18, 23)
#define TPC_COMP_LSB_CFG18_SHIFT                                        18
#define TPC_COMP_LSB_CFG17_MASK                                         CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG17_SHIFT                                        12
#define TPC_COMP_LSB_CFG16_MASK                                         CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG16_SHIFT                                        6
#define TPC_COMP_LSB_CFG15_MASK                                         CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG15_SHIFT                                        0

/* RFIP_ADDR_TPC_COMP_LSB_CFG4 */
#define TPC_COMP_LSB_CFG24_MASK                                         CO_BITS(24, 29)
#define TPC_COMP_LSB_CFG24_SHIFT                                        24
#define TPC_COMP_LSB_CFG23_MASK                                         CO_BITS(18, 23)
#define TPC_COMP_LSB_CFG23_SHIFT                                        18
#define TPC_COMP_LSB_CFG22_MASK                                         CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG22_SHIFT                                        12
#define TPC_COMP_LSB_CFG21_MASK                                         CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG21_SHIFT                                        6
#define TPC_COMP_LSB_CFG20_MASK                                         CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG20_SHIFT                                        0

/* RFIP_ADDR_TPC_COMP_LSB_CFG5 */
#define TPC_COMP_LSB_CFG29_MASK                                         CO_BITS(24, 29)
#define TPC_COMP_LSB_CFG29_SHIFT                                        24
#define TPC_COMP_LSB_CFG28_MASK                                         CO_BITS(18, 23)
#define TPC_COMP_LSB_CFG28_SHIFT                                        18
#define TPC_COMP_LSB_CFG27_MASK                                         CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG27_SHIFT                                        12
#define TPC_COMP_LSB_CFG26_MASK                                         CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG26_SHIFT                                        6
#define TPC_COMP_LSB_CFG25_MASK                                         CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG25_SHIFT                                        0

/* RFIP_ADDR_TPC_COMP_LSB_CFG6 */
#define TPC_COMP_LSB_CFG32_MASK                                         CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG32_SHIFT                                        12
#define TPC_COMP_LSB_CFG31_MASK                                         CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG31_SHIFT                                        6
#define TPC_COMP_LSB_CFG30_MASK                                         CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG30_SHIFT                                        0

/* RFIP_ADDR_LOFT_COMP_I_CFG0 */
#define LOFT_COMP_I_CFG3_MASK                                           CO_BITS(18, 23)
#define LOFT_COMP_I_CFG3_SHIFT                                          18
#define LOFT_COMP_I_CFG2_MASK                                           CO_BITS(12, 17)
#define LOFT_COMP_I_CFG2_SHIFT                                          12
#define LOFT_COMP_I_CFG1_MASK                                           CO_BITS(6, 11)
#define LOFT_COMP_I_CFG1_SHIFT                                          6
#define LOFT_COMP_I_CFG0_MASK                                           CO_BITS(0, 5)
#define LOFT_COMP_I_CFG0_SHIFT                                          0

/* RFIP_ADDR_LOFT_COMP_I_CFG1 */
#define LOFT_COMP_I_CFG7_MASK                                           CO_BITS(18, 23)
#define LOFT_COMP_I_CFG7_SHIFT                                          18
#define LOFT_COMP_I_CFG6_MASK                                           CO_BITS(12, 17)
#define LOFT_COMP_I_CFG6_SHIFT                                          12
#define LOFT_COMP_I_CFG5_MASK                                           CO_BITS(6, 11)
#define LOFT_COMP_I_CFG5_SHIFT                                          6
#define LOFT_COMP_I_CFG4_MASK                                           CO_BITS(0, 5)
#define LOFT_COMP_I_CFG4_SHIFT                                          0

/* RFIP_ADDR_LOFT_COMP_I_CFG2 */
#define LOFT_COMP_I_CFG11_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_I_CFG11_SHIFT                                         18
#define LOFT_COMP_I_CFG10_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_I_CFG10_SHIFT                                         12
#define LOFT_COMP_I_CFG9_MASK                                           CO_BITS(6, 11)
#define LOFT_COMP_I_CFG9_SHIFT                                          6
#define LOFT_COMP_I_CFG8_MASK                                           CO_BITS(0, 5)
#define LOFT_COMP_I_CFG8_SHIFT                                          0

/* RFIP_ADDR_LOFT_COMP_I_CFG3 */
#define LOFT_COMP_I_CFG15_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_I_CFG15_SHIFT                                         18
#define LOFT_COMP_I_CFG14_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_I_CFG14_SHIFT                                         12
#define LOFT_COMP_I_CFG13_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_I_CFG13_SHIFT                                         6
#define LOFT_COMP_I_CFG12_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_I_CFG12_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_I_CFG4 */
#define LOFT_COMP_I_CFG19_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_I_CFG19_SHIFT                                         18
#define LOFT_COMP_I_CFG18_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_I_CFG18_SHIFT                                         12
#define LOFT_COMP_I_CFG17_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_I_CFG17_SHIFT                                         6
#define LOFT_COMP_I_CFG16_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_I_CFG16_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_I_CFG5 */
#define LOFT_COMP_I_CFG23_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_I_CFG23_SHIFT                                         18
#define LOFT_COMP_I_CFG22_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_I_CFG22_SHIFT                                         12
#define LOFT_COMP_I_CFG21_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_I_CFG21_SHIFT                                         6
#define LOFT_COMP_I_CFG20_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_I_CFG20_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_I_CFG6 */
#define LOFT_COMP_I_CFG27_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_I_CFG27_SHIFT                                         18
#define LOFT_COMP_I_CFG26_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_I_CFG26_SHIFT                                         12
#define LOFT_COMP_I_CFG25_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_I_CFG25_SHIFT                                         6
#define LOFT_COMP_I_CFG24_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_I_CFG24_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_I_CFG7 */
#define LOFT_COMP_I_CFG31_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_I_CFG31_SHIFT                                         18
#define LOFT_COMP_I_CFG30_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_I_CFG30_SHIFT                                         12
#define LOFT_COMP_I_CFG29_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_I_CFG29_SHIFT                                         6
#define LOFT_COMP_I_CFG28_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_I_CFG28_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_Q_CFG0 */
#define LOFT_COMP_Q_CFG3_MASK                                           CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG3_SHIFT                                          18
#define LOFT_COMP_Q_CFG2_MASK                                           CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG2_SHIFT                                          12
#define LOFT_COMP_Q_CFG1_MASK                                           CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG1_SHIFT                                          6
#define LOFT_COMP_Q_CFG0_MASK                                           CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG0_SHIFT                                          0

/* RFIP_ADDR_LOFT_COMP_Q_CFG1 */
#define LOFT_COMP_Q_CFG7_MASK                                           CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG7_SHIFT                                          18
#define LOFT_COMP_Q_CFG6_MASK                                           CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG6_SHIFT                                          12
#define LOFT_COMP_Q_CFG5_MASK                                           CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG5_SHIFT                                          6
#define LOFT_COMP_Q_CFG4_MASK                                           CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG4_SHIFT                                          0

/* RFIP_ADDR_LOFT_COMP_Q_CFG2 */
#define LOFT_COMP_Q_CFG11_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG11_SHIFT                                         18
#define LOFT_COMP_Q_CFG10_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG10_SHIFT                                         12
#define LOFT_COMP_Q_CFG9_MASK                                           CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG9_SHIFT                                          6
#define LOFT_COMP_Q_CFG8_MASK                                           CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG8_SHIFT                                          0

/* RFIP_ADDR_LOFT_COMP_Q_CFG3 */
#define LOFT_COMP_Q_CFG15_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG15_SHIFT                                         18
#define LOFT_COMP_Q_CFG14_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG14_SHIFT                                         12
#define LOFT_COMP_Q_CFG13_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG13_SHIFT                                         6
#define LOFT_COMP_Q_CFG12_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG12_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_Q_CFG4 */
#define LOFT_COMP_Q_CFG19_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG19_SHIFT                                         18
#define LOFT_COMP_Q_CFG18_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG18_SHIFT                                         12
#define LOFT_COMP_Q_CFG17_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG17_SHIFT                                         6
#define LOFT_COMP_Q_CFG16_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG16_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_Q_CFG5 */
#define LOFT_COMP_Q_CFG23_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG23_SHIFT                                         18
#define LOFT_COMP_Q_CFG22_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG22_SHIFT                                         12
#define LOFT_COMP_Q_CFG21_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG21_SHIFT                                         6
#define LOFT_COMP_Q_CFG20_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG20_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_Q_CFG6 */
#define LOFT_COMP_Q_CFG27_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG27_SHIFT                                         18
#define LOFT_COMP_Q_CFG26_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG26_SHIFT                                         12
#define LOFT_COMP_Q_CFG25_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG25_SHIFT                                         6
#define LOFT_COMP_Q_CFG24_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG24_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_Q_CFG7 */
#define LOFT_COMP_Q_CFG31_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG31_SHIFT                                         18
#define LOFT_COMP_Q_CFG30_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG30_SHIFT                                         12
#define LOFT_COMP_Q_CFG29_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG29_SHIFT                                         6
#define LOFT_COMP_Q_CFG28_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG28_SHIFT                                         0

/* RFIP_ADDR_LOFT_SFT_CFG_I_CFG0 */
#define LOFT_COMP_SFT_CFG_I_CFG15_MASK                                  CO_BITS(30, 31)
#define LOFT_COMP_SFT_CFG_I_CFG15_SHIFT                                 30
#define LOFT_COMP_SFT_CFG_I_CFG14_MASK                                  CO_BITS(28, 29)
#define LOFT_COMP_SFT_CFG_I_CFG14_SHIFT                                 28
#define LOFT_COMP_SFT_CFG_I_CFG13_MASK                                  CO_BITS(26, 27)
#define LOFT_COMP_SFT_CFG_I_CFG13_SHIFT                                 26
#define LOFT_COMP_SFT_CFG_I_CFG12_MASK                                  CO_BITS(24, 25)
#define LOFT_COMP_SFT_CFG_I_CFG12_SHIFT                                 24
#define LOFT_COMP_SFT_CFG_I_CFG11_MASK                                  CO_BITS(22, 23)
#define LOFT_COMP_SFT_CFG_I_CFG11_SHIFT                                 22
#define LOFT_COMP_SFT_CFG_I_CFG10_MASK                                  CO_BITS(20, 21)
#define LOFT_COMP_SFT_CFG_I_CFG10_SHIFT                                 20
#define LOFT_COMP_SFT_CFG_I_CFG9_MASK                                   CO_BITS(18, 19)
#define LOFT_COMP_SFT_CFG_I_CFG9_SHIFT                                  18
#define LOFT_COMP_SFT_CFG_I_CFG8_MASK                                   CO_BITS(16, 17)
#define LOFT_COMP_SFT_CFG_I_CFG8_SHIFT                                  16
#define LOFT_COMP_SFT_CFG_I_CFG7_MASK                                   CO_BITS(14, 15)
#define LOFT_COMP_SFT_CFG_I_CFG7_SHIFT                                  14
#define LOFT_COMP_SFT_CFG_I_CFG6_MASK                                   CO_BITS(12, 13)
#define LOFT_COMP_SFT_CFG_I_CFG6_SHIFT                                  12
#define LOFT_COMP_SFT_CFG_I_CFG5_MASK                                   CO_BITS(10, 11)
#define LOFT_COMP_SFT_CFG_I_CFG5_SHIFT                                  10
#define LOFT_COMP_SFT_CFG_I_CFG4_MASK                                   CO_BITS(8, 9)
#define LOFT_COMP_SFT_CFG_I_CFG4_SHIFT                                  8
#define LOFT_COMP_SFT_CFG_I_CFG3_MASK                                   CO_BITS(6, 7)
#define LOFT_COMP_SFT_CFG_I_CFG3_SHIFT                                  6
#define LOFT_COMP_SFT_CFG_I_CFG2_MASK                                   CO_BITS(4, 5)
#define LOFT_COMP_SFT_CFG_I_CFG2_SHIFT                                  4
#define LOFT_COMP_SFT_CFG_I_CFG1_MASK                                   CO_BITS(2, 3)
#define LOFT_COMP_SFT_CFG_I_CFG1_SHIFT                                  2
#define LOFT_COMP_SFT_CFG_I_CFG0_MASK                                   CO_BITS(0, 1)
#define LOFT_COMP_SFT_CFG_I_CFG0_SHIFT                                  0

/* RFIP_ADDR_LOFT_SFT_CFG_I_CFG1 */
#define LOFT_COMP_SFT_CFG_I_CFG31_MASK                                  CO_BITS(30, 31)
#define LOFT_COMP_SFT_CFG_I_CFG31_SHIFT                                 30
#define LOFT_COMP_SFT_CFG_I_CFG30_MASK                                  CO_BITS(28, 29)
#define LOFT_COMP_SFT_CFG_I_CFG30_SHIFT                                 28
#define LOFT_COMP_SFT_CFG_I_CFG29_MASK                                  CO_BITS(26, 27)
#define LOFT_COMP_SFT_CFG_I_CFG29_SHIFT                                 26
#define LOFT_COMP_SFT_CFG_I_CFG28_MASK                                  CO_BITS(24, 25)
#define LOFT_COMP_SFT_CFG_I_CFG28_SHIFT                                 24
#define LOFT_COMP_SFT_CFG_I_CFG27_MASK                                  CO_BITS(22, 23)
#define LOFT_COMP_SFT_CFG_I_CFG27_SHIFT                                 22
#define LOFT_COMP_SFT_CFG_I_CFG26_MASK                                  CO_BITS(20, 21)
#define LOFT_COMP_SFT_CFG_I_CFG26_SHIFT                                 20
#define LOFT_COMP_SFT_CFG_I_CFG25_MASK                                  CO_BITS(18, 19)
#define LOFT_COMP_SFT_CFG_I_CFG25_SHIFT                                 18
#define LOFT_COMP_SFT_CFG_I_CFG24_MASK                                  CO_BITS(16, 17)
#define LOFT_COMP_SFT_CFG_I_CFG24_SHIFT                                 16
#define LOFT_COMP_SFT_CFG_I_CFG23_MASK                                  CO_BITS(14, 15)
#define LOFT_COMP_SFT_CFG_I_CFG23_SHIFT                                 14
#define LOFT_COMP_SFT_CFG_I_CFG22_MASK                                  CO_BITS(12, 13)
#define LOFT_COMP_SFT_CFG_I_CFG22_SHIFT                                 12
#define LOFT_COMP_SFT_CFG_I_CFG21_MASK                                  CO_BITS(10, 11)
#define LOFT_COMP_SFT_CFG_I_CFG21_SHIFT                                 10
#define LOFT_COMP_SFT_CFG_I_CFG20_MASK                                  CO_BITS(8, 9)
#define LOFT_COMP_SFT_CFG_I_CFG20_SHIFT                                 8
#define LOFT_COMP_SFT_CFG_I_CFG19_MASK                                  CO_BITS(6, 7)
#define LOFT_COMP_SFT_CFG_I_CFG19_SHIFT                                 6
#define LOFT_COMP_SFT_CFG_I_CFG18_MASK                                  CO_BITS(4, 5)
#define LOFT_COMP_SFT_CFG_I_CFG18_SHIFT                                 4
#define LOFT_COMP_SFT_CFG_I_CFG17_MASK                                  CO_BITS(2, 3)
#define LOFT_COMP_SFT_CFG_I_CFG17_SHIFT                                 2
#define LOFT_COMP_SFT_CFG_I_CFG16_MASK                                  CO_BITS(0, 1)
#define LOFT_COMP_SFT_CFG_I_CFG16_SHIFT                                 0

/* RFIP_ADDR_LOFT_SFT_CFG_Q_CFG0 */
#define LOFT_COMP_SFT_CFG_Q_CFG15_MASK                                  CO_BITS(30, 31)
#define LOFT_COMP_SFT_CFG_Q_CFG15_SHIFT                                 30
#define LOFT_COMP_SFT_CFG_Q_CFG14_MASK                                  CO_BITS(28, 29)
#define LOFT_COMP_SFT_CFG_Q_CFG14_SHIFT                                 28
#define LOFT_COMP_SFT_CFG_Q_CFG13_MASK                                  CO_BITS(26, 27)
#define LOFT_COMP_SFT_CFG_Q_CFG13_SHIFT                                 26
#define LOFT_COMP_SFT_CFG_Q_CFG12_MASK                                  CO_BITS(24, 25)
#define LOFT_COMP_SFT_CFG_Q_CFG12_SHIFT                                 24
#define LOFT_COMP_SFT_CFG_Q_CFG11_MASK                                  CO_BITS(22, 23)
#define LOFT_COMP_SFT_CFG_Q_CFG11_SHIFT                                 22
#define LOFT_COMP_SFT_CFG_Q_CFG10_MASK                                  CO_BITS(20, 21)
#define LOFT_COMP_SFT_CFG_Q_CFG10_SHIFT                                 20
#define LOFT_COMP_SFT_CFG_Q_CFG9_MASK                                   CO_BITS(18, 19)
#define LOFT_COMP_SFT_CFG_Q_CFG9_SHIFT                                  18
#define LOFT_COMP_SFT_CFG_Q_CFG8_MASK                                   CO_BITS(16, 17)
#define LOFT_COMP_SFT_CFG_Q_CFG8_SHIFT                                  16
#define LOFT_COMP_SFT_CFG_Q_CFG7_MASK                                   CO_BITS(14, 15)
#define LOFT_COMP_SFT_CFG_Q_CFG7_SHIFT                                  14
#define LOFT_COMP_SFT_CFG_Q_CFG6_MASK                                   CO_BITS(12, 13)
#define LOFT_COMP_SFT_CFG_Q_CFG6_SHIFT                                  12
#define LOFT_COMP_SFT_CFG_Q_CFG5_MASK                                   CO_BITS(10, 11)
#define LOFT_COMP_SFT_CFG_Q_CFG5_SHIFT                                  10
#define LOFT_COMP_SFT_CFG_Q_CFG4_MASK                                   CO_BITS(8, 9)
#define LOFT_COMP_SFT_CFG_Q_CFG4_SHIFT                                  8
#define LOFT_COMP_SFT_CFG_Q_CFG3_MASK                                   CO_BITS(6, 7)
#define LOFT_COMP_SFT_CFG_Q_CFG3_SHIFT                                  6
#define LOFT_COMP_SFT_CFG_Q_CFG2_MASK                                   CO_BITS(4, 5)
#define LOFT_COMP_SFT_CFG_Q_CFG2_SHIFT                                  4
#define LOFT_COMP_SFT_CFG_Q_CFG1_MASK                                   CO_BITS(2, 3)
#define LOFT_COMP_SFT_CFG_Q_CFG1_SHIFT                                  2
#define LOFT_COMP_SFT_CFG_Q_CFG0_MASK                                   CO_BITS(0, 1)
#define LOFT_COMP_SFT_CFG_Q_CFG0_SHIFT                                  0

/* RFIP_ADDR_LOFT_SFT_CFG_Q_CFG1 */
#define LOFT_COMP_SFT_CFG_Q_CFG31_MASK                                  CO_BITS(30, 31)
#define LOFT_COMP_SFT_CFG_Q_CFG31_SHIFT                                 30
#define LOFT_COMP_SFT_CFG_Q_CFG30_MASK                                  CO_BITS(28, 29)
#define LOFT_COMP_SFT_CFG_Q_CFG30_SHIFT                                 28
#define LOFT_COMP_SFT_CFG_Q_CFG29_MASK                                  CO_BITS(26, 27)
#define LOFT_COMP_SFT_CFG_Q_CFG29_SHIFT                                 26
#define LOFT_COMP_SFT_CFG_Q_CFG28_MASK                                  CO_BITS(24, 25)
#define LOFT_COMP_SFT_CFG_Q_CFG28_SHIFT                                 24
#define LOFT_COMP_SFT_CFG_Q_CFG27_MASK                                  CO_BITS(22, 23)
#define LOFT_COMP_SFT_CFG_Q_CFG27_SHIFT                                 22
#define LOFT_COMP_SFT_CFG_Q_CFG26_MASK                                  CO_BITS(20, 21)
#define LOFT_COMP_SFT_CFG_Q_CFG26_SHIFT                                 20
#define LOFT_COMP_SFT_CFG_Q_CFG25_MASK                                  CO_BITS(18, 19)
#define LOFT_COMP_SFT_CFG_Q_CFG25_SHIFT                                 18
#define LOFT_COMP_SFT_CFG_Q_CFG24_MASK                                  CO_BITS(16, 17)
#define LOFT_COMP_SFT_CFG_Q_CFG24_SHIFT                                 16
#define LOFT_COMP_SFT_CFG_Q_CFG23_MASK                                  CO_BITS(14, 15)
#define LOFT_COMP_SFT_CFG_Q_CFG23_SHIFT                                 14
#define LOFT_COMP_SFT_CFG_Q_CFG22_MASK                                  CO_BITS(12, 13)
#define LOFT_COMP_SFT_CFG_Q_CFG22_SHIFT                                 12
#define LOFT_COMP_SFT_CFG_Q_CFG21_MASK                                  CO_BITS(10, 11)
#define LOFT_COMP_SFT_CFG_Q_CFG21_SHIFT                                 10
#define LOFT_COMP_SFT_CFG_Q_CFG20_MASK                                  CO_BITS(8, 9)
#define LOFT_COMP_SFT_CFG_Q_CFG20_SHIFT                                 8
#define LOFT_COMP_SFT_CFG_Q_CFG19_MASK                                  CO_BITS(6, 7)
#define LOFT_COMP_SFT_CFG_Q_CFG19_SHIFT                                 6
#define LOFT_COMP_SFT_CFG_Q_CFG18_MASK                                  CO_BITS(4, 5)
#define LOFT_COMP_SFT_CFG_Q_CFG18_SHIFT                                 4
#define LOFT_COMP_SFT_CFG_Q_CFG17_MASK                                  CO_BITS(2, 3)
#define LOFT_COMP_SFT_CFG_Q_CFG17_SHIFT                                 2
#define LOFT_COMP_SFT_CFG_Q_CFG16_MASK                                  CO_BITS(0, 1)
#define LOFT_COMP_SFT_CFG_Q_CFG16_SHIFT                                 0

/* RFIP_ADDR_TX_DC_OFFSET_CFG */
#define TX_DC_OFFSET_Q_CFG_MASK                                         CO_BITS(7, 13)
#define TX_DC_OFFSET_Q_CFG_SHIFT                                        7
#define TX_DC_OFFSET_I_CFG_MASK                                         CO_BITS(0, 6)
#define TX_DC_OFFSET_I_CFG_SHIFT                                        0

/* RFIP_ADDR_TX_CTRL_CFG */
#define CLIP_FACTOR_CFG_MASK                                            CO_BITS(20, 31)
#define CLIP_FACTOR_CFG_SHIFT                                           20
#define TPC_CAL_OFFSET_CFG_MASK                                         CO_BITS(14, 19)
#define TPC_CAL_OFFSET_CFG_SHIFT                                        14
#define TPC_CAL_LSB_CFG_MASK                                            CO_BITS(8, 13)
#define TPC_CAL_LSB_CFG_SHIFT                                           8
#define UP_CONVERTER_SEL_X4_CFG                                         BIT7
#define RX2TX_TEST_EN_CFG                                               BIT6
#define TX_RX_BRIDGE_RST_CFG                                            BIT5
#define TX_RF_IF_SWRST_CFG                                              BIT4
#define TX_UPCONVERT_BYPASS_N                                           BIT3
#define TX_SRAM_TEST_EN                                                 BIT0

/* RFIP_ADDR_TX_SRAM_TEST_CFG */
#define TX_SRAM_TEST_FLUSH                                              BIT31

/* RFIP_ADDR_TX_SRAM_TEST_DR */
#define TX_SRAM_Q_DIN_MASK                                              CO_BITS(12, 23)
#define TX_SRAM_Q_DIN_SHIFT                                             12
#define TX_SRAM_I_DIN_MASK                                              CO_BITS(0, 11)
#define TX_SRAM_I_DIN_SHIFT                                             0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG0 */
#define IN11N_CBW20_TX_COEFF2_MASK                                      CO_BITS(20, 29)
#define IN11N_CBW20_TX_COEFF2_SHIFT                                     20
#define IN11N_CBW20_TX_COEFF1_MASK                                      CO_BITS(10, 19)
#define IN11N_CBW20_TX_COEFF1_SHIFT                                     10
#define IN11N_CBW20_TX_COEFF0_MASK                                      CO_BITS(0, 9)
#define IN11N_CBW20_TX_COEFF0_SHIFT                                     0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG1 */
#define IN11N_CBW20_TX_COEFF5_MASK                                      CO_BITS(20, 29)
#define IN11N_CBW20_TX_COEFF5_SHIFT                                     20
#define IN11N_CBW20_TX_COEFF4_MASK                                      CO_BITS(10, 19)
#define IN11N_CBW20_TX_COEFF4_SHIFT                                     10
#define IN11N_CBW20_TX_COEFF3_MASK                                      CO_BITS(0, 9)
#define IN11N_CBW20_TX_COEFF3_SHIFT                                     0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG2 */
#define IN11N_CBW20_TX_COEFF8_MASK                                      CO_BITS(20, 29)
#define IN11N_CBW20_TX_COEFF8_SHIFT                                     20
#define IN11N_CBW20_TX_COEFF7_MASK                                      CO_BITS(10, 19)
#define IN11N_CBW20_TX_COEFF7_SHIFT                                     10
#define IN11N_CBW20_TX_COEFF6_MASK                                      CO_BITS(0, 9)
#define IN11N_CBW20_TX_COEFF6_SHIFT                                     0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG3 */
#define IN11N_CBW20_TX_COEFF10_MASK                                     CO_BITS(10, 19)
#define IN11N_CBW20_TX_COEFF10_SHIFT                                    10
#define IN11N_CBW20_TX_COEFF9_MASK                                      CO_BITS(0, 9)
#define IN11N_CBW20_TX_COEFF9_SHIFT                                     0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG4 */
#define IN11B_TX_COEFF2_MASK                                            CO_BITS(20, 29)
#define IN11B_TX_COEFF2_SHIFT                                           20
#define IN11B_TX_COEFF1_MASK                                            CO_BITS(10, 19)
#define IN11B_TX_COEFF1_SHIFT                                           10
#define IN11B_TX_COEFF0_MASK                                            CO_BITS(0, 9)
#define IN11B_TX_COEFF0_SHIFT                                           0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG5 */
#define IN11B_TX_COEFF5_MASK                                            CO_BITS(20, 29)
#define IN11B_TX_COEFF5_SHIFT                                           20
#define IN11B_TX_COEFF4_MASK                                            CO_BITS(10, 19)
#define IN11B_TX_COEFF4_SHIFT                                           10
#define IN11B_TX_COEFF3_MASK                                            CO_BITS(0, 9)
#define IN11B_TX_COEFF3_SHIFT                                           0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG6 */
#define IN11B_TX_COEFF8_MASK                                            CO_BITS(20, 29)
#define IN11B_TX_COEFF8_SHIFT                                           20
#define IN11B_TX_COEFF7_MASK                                            CO_BITS(10, 19)
#define IN11B_TX_COEFF7_SHIFT                                           10
#define IN11B_TX_COEFF6_MASK                                            CO_BITS(0, 9)
#define IN11B_TX_COEFF6_SHIFT                                           0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG7 */
#define IN11B_TX_COEFF10_MASK                                           CO_BITS(10, 19)
#define IN11B_TX_COEFF10_SHIFT                                          10
#define IN11B_TX_COEFF9_MASK                                            CO_BITS(0, 9)
#define IN11B_TX_COEFF9_SHIFT                                           0

/* RFIP_ADDR_GPIO_MODE_CFG0 */
#define RXEN_MODE_CFG                                                   BIT31
#define RXIF_EN_MODE_CFG                                                BIT30
#define RX_GAIN0_MODE_CFG                                               BIT29
#define RX_GAIN1_MODE_CFG                                               BIT28
#define RX_GAIN2_MODE_CFG                                               BIT27
#define RX_GAIN3_MODE_CFG                                               BIT26
#define RX_GAIN4_MODE_CFG                                               BIT25
#define RX_GAIN5_MODE_CFG                                               BIT24
#define RX_GAIN6_MODE_CFG                                               BIT23
#define RX_GAIN7_MODE_CFG                                               BIT22
#define RFPKDET0_MODE_CFG                                               BIT21
#define RFPKDET1_MODE_CFG                                               BIT20
#define RXDATA0_MODE_CFG                                                BIT19
#define RXDATA1_MODE_CFG                                                BIT18
#define RXDATA2_MODE_CFG                                                BIT17
#define RXDATA3_MODE_CFG                                                BIT16
#define RXDATA4_MODE_CFG                                                BIT15
#define RXDATA5_MODE_CFG                                                BIT14
#define RXDATA6_MODE_CFG                                                BIT13
#define RXDATA7_MODE_CFG                                                BIT12
#define RXDATA8_MODE_CFG                                                BIT11
#define RXDATA9_MODE_CFG                                                BIT10
#define RXDATA10_MODE_CFG                                               BIT9
#define RXDATA11_MODE_CFG                                               BIT8
#define RXCLK_MODE_CFG                                                  BIT7

/* RFIP_ADDR_GPIO_MODE_CFG1 */
#define TXEN_MODE_CFG                                                   BIT31
#define TXIF_EN_MODE_CFG                                                BIT30
#define TXDATA0_MODE_CFG                                                BIT29
#define TXDATA1_MODE_CFG                                                BIT28
#define TXDATA2_MODE_CFG                                                BIT27
#define TXDATA3_MODE_CFG                                                BIT26
#define TXDATA4_MODE_CFG                                                BIT25
#define TXDATA5_MODE_CFG                                                BIT24
#define TXDATA6_MODE_CFG                                                BIT23
#define TXDATA7_MODE_CFG                                                BIT22
#define TXDATA8_MODE_CFG                                                BIT21
#define TXDATA9_MODE_CFG                                                BIT20
#define TXDATA10_MODE_CFG                                               BIT19
#define TXDATA11_MODE_CFG                                               BIT18
#define TXCLK_MODE_CFG                                                  BIT17
#define GPIO_MODE_SEL                                                   BIT0

/* RFIP_ADDR_GPIO_RX_FORCE_CFG */
#define RXRF_ENABLE_VALUE                                               BIT31
#define RXIF_ENABLE_VALUE                                               BIT30
#define RX_GAIN_INDEX_VALUE_MASK                                        CO_BITS(22, 29)
#define RX_GAIN_INDEX_VALUE_SHIFT                                       22
#define RX_DCOFFSETI_VALUE_MASK                                         CO_BITS(13, 21)
#define RX_DCOFFSETI_VALUE_SHIFT                                        13
#define RX_DCOFFSETQ_VALUE_MASK                                         CO_BITS(4, 12)
#define RX_DCOFFSETQ_VALUE_SHIFT                                        4

/* RFIP_ADDR_GPIO_TX_FORCE_CFG */
#define TXRF_ENABLE_VALUE                                               BIT31
#define TXIF_ENABLE_VALUE                                               BIT30
#define TX_GAIN_INDEX_VALUE_MASK                                        CO_BITS(24, 29)
#define TX_GAIN_INDEX_VALUE_SHIFT                                       24
#define TX_DATA_VALUE_MASK                                              CO_BITS(12, 23)
#define TX_DATA_VALUE_SHIFT                                             12

/* RFIP_ADDR_GPIO_FORCE_EN_CFG */
#define RXRF_ENABLE_FORCE_EN                                            BIT31
#define RXIF_ENABLE_FORCE_EN                                            BIT30
#define RX_GAIN_INDEX_FORCE_EN                                          BIT29
#define RX_DCOFFSET_FORCE_EN                                            BIT28
#define TXRF_ENABLE_FORCE_EN                                            BIT3
#define TXIF_ENABLE_FORCE_EN                                            BIT2
#define TX_GAIN_INDEX_FORCE_EN                                          BIT1
#define TX_DATA_FORCE_EN                                                BIT0

/* RFIP_ADDR_TX_GAIN_FORCE_CFG */
#define TX_DIG_GAIN_FORCE_EN                                            BIT31
#define TX_DIG_GAIN_FORCE_VALUE_MASK                                    CO_BITS(20, 30)
#define TX_DIG_GAIN_FORCE_VALUE_SHIFT                                   20
#define TX_RF_GAIN_FORCE_EN                                             BIT15
#define TX_RF_GAIN_FORCE_VALUE_MASK                                     CO_BITS(4, 14)
#define TX_RF_GAIN_FORCE_VALUE_SHIFT                                    4

/* RFIP_ADDR_TX_COMP_FORCE_CFG */
#define TX_I_COMP_FORCE_EN                                              BIT31
#define TX_I_COMP_FORCE_VALUE_MASK                                      CO_BITS(19, 30)
#define TX_I_COMP_FORCE_VALUE_SHIFT                                     19
#define TX_Q_COMP_FORCE_EN                                              BIT15
#define TX_Q_COMP_FORCE_VALUE_MASK                                      CO_BITS(3, 14)
#define TX_Q_COMP_FORCE_VALUE_SHIFT                                     3

/* RFIP_ADDR_RFIPSTATE */
#define RCCAL_CSTATE_MASK                                               CO_BITS(27, 31)
#define RCCAL_CSTATE_SHIFT                                              27
#define RX_STATE_MASK                                                   CO_BITS(3, 5)
#define RX_STATE_SHIFT                                                  3
#define TX_STATE_MASK                                                   CO_BITS(0, 2)
#define TX_STATE_SHIFT                                                  0

/* RFIP_ADDR_RFIPCALIN */
#define RCCAL_CIN_MASK                                                  CO_BITS(26, 31)
#define RCCAL_CIN_SHIFT                                                 26
#define ADCCAL_ACC_MASK                                                 CO_BITS(7, 25)
#define ADCCAL_ACC_SHIFT                                                7
#define ADCCAL_M_MASK                                                   CO_BITS(3, 6)
#define ADCCAL_M_SHIFT                                                  3
#define ADCCAL_BUG                                                      BIT2
#define ADCCAL_SIGN                                                     BIT1
#define ADCCAL_ENABLE                                                   BIT0

/* RFIP_ADDR_RFPLLDEBUGCFG */
#define RFPLL_MSB_RF_COUNTER_OUT                                        BIT30
#define RFPLL_RF_COUNTER_DONE_OUT_MASK                                  CO_BITS(16, 29)
#define RFPLL_RF_COUNTER_DONE_OUT_SHIFT                                 16
#define RFPLL_FREQ_CALIBRE_MEM_M_OUT_MASK                               CO_BITS(8, 15)
#define RFPLL_FREQ_CALIBRE_MEM_M_OUT_SHIFT                              8
#define RFPLL_FREQ_CALIBRE_MEM_P_OUT_MASK                               CO_BITS(0, 7)
#define RFPLL_FREQ_CALIBRE_MEM_P_OUT_SHIFT                              0

/* RFIP_ADDR_RFPLLDEBUGCFG2 */
#define RFPLL_DIFF_MEM_M_OUT_MASK                                       CO_BITS(14, 27)
#define RFPLL_DIFF_MEM_M_OUT_SHIFT                                      14
#define RFPLL_DIFF_MEM_P_OUT_MASK                                       CO_BITS(0, 13)
#define RFPLL_DIFF_MEM_P_OUT_SHIFT                                      0

/* RFIP_ADDR_TXRX_DEBUG */
#define TX_AFIFO_ERR                                                    BIT1
#define RX_AFIFO_ERR                                                    BIT0
uint32_t read_rf_reg(uint32_t regaddr);
void write_rf_reg(uint32_t regaddr, uint32_t data);
#elif CONFIG_RF_TYPE == RF_GDM32110
/* ========  RF Register Definitions ========  */
#define RFIP_ADDR_GCFG                                                  0x0000
#define RFIP_ADDR_RX_STATE_DELAY_CFG1                                   0x0004
#define RFIP_ADDR_RX_STATE_DELAY_CFG2                                   0x0008
#define RFIP_ADDR_TX_STATE_DELAY_CFG1                                   0x000C
#define RFIP_ADDR_RXOFF_CFG                                             0x0010
#define RFIP_ADDR_RXANA_CFG                                             0x0014
#define RFIP_ADDR_RXIDLE_CFG                                            0x0018
#define RFIP_ADDR_RXACTIVE_CFG                                          0x001C
#define RFIP_ADDR_TXOFF_CFG                                             0x0020
#define RFIP_ADDR_TXANA_CFG                                             0x0024
#define RFIP_ADDR_TXON_CFG                                              0x0028
#define RFIP_ADDR_TXRFOFF_CFG                                           0x002C
#define RFIP_ADDR_LDO_CFG                                               0x0058
#define RFIP_ADDR_XTAL_CFG                                              0x005C
#define RFIP_ADDR_PLLS_CFG1                                             0x0060
#define RFIP_ADDR_PLLS_CFG2                                             0x0064
#define RFIP_ADDR_RFPLL_LOOP_CFG                                        0x0068
#define RFIP_ADDR_RFPLL_DIG_CFG1                                        0x006C
#define RFIP_ADDR_RFPLL_DIG_CFG2                                        0x0070
#define RFIP_ADDR_RFPLL_DIG_INT_FRAC_CFG                                0x0074
#define RFIP_ADDR_RFPLL_DIG_VCOCAL_CFG                                  0x0078
#define RFIP_ADDR_MDLL_CFG                                              0x007C
#define RFIP_ADDR_RX_LPF2_CFG                                           0x0080
#define RFIP_ADDR_SDMADC_CFG                                            0x0084
#define RFIP_ADDR_SDMADC_DIG_CFG                                        0x0088
#define RFIP_ADDR_SDMADC_CIC_CFG                                        0x008C
#define RFIP_ADDR_SARADC_CFG                                            0x0090
#define RFIP_ADDR_AUXADC_CFG                                            0x0094
#define RFIP_ADDR_RCCAL_CFG                                             0x0098
#define RFIP_ADDR_RCCAL_OFFSET_TEST_CFG                                 0x009C
#define RFIP_ADDR_MUX_CFG                                               0x00A0
#define RFIP_ADDR_LDO_ANA_CFG                                           0x00A4
#define RFIP_ADDR_LDO_VCO_CFG                                           0x00A8
#define RFIP_ADDR_LNA_CFG                                               0x00AC
#define RFIP_ADDR_RXMIX_CFG                                             0x00B0
#define RFIP_ADDR_RXFILTER1_CFG1                                        0x00B4
#define RFIP_ADDR_RXFILTER1_CFG2                                        0x00B8
#define RFIP_ADDR_RXPKDET_CFG                                           0x00BC
#define RFIP_ADDR_VCO_CFG1                                              0x00C0
#define RFIP_ADDR_VCO_CFG2                                              0x00C4
#define RFIP_ADDR_RFPLL_CFG                                             0x00C8
#define RFIP_ADDR_PA_CFG                                                0x00CC
#define RFIP_ADDR_TXRF_CFG1                                             0x00D0
#define RFIP_ADDR_TXRF_CFG2                                             0x00D4
#define RFIP_ADDR_TXDAC_CFG                                             0x00D8
#define RFIP_ADDR_TXFILT_CFG                                            0x00DC
#define RFIP_ADDR_BG_CFG                                                0x00E0
#define RFIP_ADDR_TEMP_CFG                                              0x00E4
#define RFIP_ADDR_LO_CAL_CFG                                            0x00E8
#define RFIP_ADDR_LDO_ANALOG_CFG                                        0x00EC
#define RFIP_ADDR_RX_GAIN_CFG                                           0x00F0
#define RFIP_ADDR_RF_RESERVED                                           0x00FC
#define RFIP_ADDR_TX_RF_GAIN_CFG0                                       0x0100
#define RFIP_ADDR_TX_RF_GAIN_CFG1                                       0x0104
#define RFIP_ADDR_TX_RF_GAIN_CFG2                                       0x0108
#define RFIP_ADDR_TX_RF_GAIN_CFG3                                       0x010C
#define RFIP_ADDR_TX_RF_GAIN_CFG4                                       0x0110
#define RFIP_ADDR_TX_RF_GAIN_CFG5                                       0x0114
#define RFIP_ADDR_TX_RF_GAIN_CFG6                                       0x0118
#define RFIP_ADDR_TX_RF_GAIN_CFG7                                       0x011C
#define RFIP_ADDR_TX_RF_GAIN_CFG8                                       0x0120
#define RFIP_ADDR_TX_RF_GAIN_CFG9                                       0x0124
#define RFIP_ADDR_TX_RF_GAIN_CFG10                                      0x0128
#define RFIP_ADDR_TX_RF_GAIN_CFG11                                      0x012C
#define RFIP_ADDR_TX_RF_GAIN_CFG12                                      0x0130
#define RFIP_ADDR_TX_RF_GAIN_CFG13                                      0x0134
#define RFIP_ADDR_TX_RF_GAIN_CFG14                                      0x0138
#define RFIP_ADDR_TX_RF_GAIN_CFG15                                      0x013C
#define RFIP_ADDR_TX_DIG_GAIN_CFG0                                      0x0140
#define RFIP_ADDR_TX_DIG_GAIN_CFG1                                      0x0144
#define RFIP_ADDR_TX_DIG_GAIN_CFG2                                      0x0148
#define RFIP_ADDR_TX_DIG_GAIN_CFG3                                      0x014C
#define RFIP_ADDR_TX_DIG_GAIN_CFG4                                      0x0150
#define RFIP_ADDR_TX_DIG_GAIN_CFG5                                      0x0154
#define RFIP_ADDR_TX_DIG_GAIN_CFG6                                      0x0158
#define RFIP_ADDR_TX_TEMP_INDEX_CFG0                                    0x0160
#define RFIP_ADDR_TX_TEMP_INDEX_CFG1                                    0x0164
#define RFIP_ADDR_TX_TEMP_INDEX_CFG2                                    0x0168
#define RFIP_ADDR_TX_TEMP_INDEX_CFG3                                    0x016C
#define RFIP_ADDR_TX_TEMP_INDEX_CFG4                                    0x0170
#define RFIP_ADDR_TX_TEMP_INDEX_CFG5                                    0x0174
#define RFIP_ADDR_TX_TEMP_INDEX_CFG6                                    0x0178
#define RFIP_ADDR_TX_TEMP_INDEX_CFG7                                    0x017C
#define RFIP_ADDR_TX_TEMP_INDEX_CFG8                                    0x0180
#define RFIP_ADDR_TX_TEMP_INDEX_CFG9                                    0x0184
#define RFIP_ADDR_TX_TEMP_INDEX_CFG10                                   0x0188
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG0                               0x0190
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG1                               0x0194
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG2                               0x0198
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG3                               0x019C
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG4                               0x01A0
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG5                               0x01A4
#define RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG6                               0x01A8
#define RFIP_ADDR_TPC_COMP_LSB_CFG0                                     0x01B0
#define RFIP_ADDR_TPC_COMP_LSB_CFG1                                     0x01B4
#define RFIP_ADDR_TPC_COMP_LSB_CFG2                                     0x01B8
#define RFIP_ADDR_TPC_COMP_LSB_CFG3                                     0x01BC
#define RFIP_ADDR_TPC_COMP_LSB_CFG4                                     0x01C0
#define RFIP_ADDR_TPC_COMP_LSB_CFG5                                     0x01C4
#define RFIP_ADDR_TPC_COMP_LSB_CFG6                                     0x01C8
#define RFIP_ADDR_LOFT_COMP_I_CFG0                                      0x01D0
#define RFIP_ADDR_LOFT_COMP_I_CFG1                                      0x01D4
#define RFIP_ADDR_LOFT_COMP_I_CFG2                                      0x01D8
#define RFIP_ADDR_LOFT_COMP_I_CFG3                                      0x01DC
#define RFIP_ADDR_LOFT_COMP_I_CFG4                                      0x01E0
#define RFIP_ADDR_LOFT_COMP_I_CFG5                                      0x01E4
#define RFIP_ADDR_LOFT_COMP_I_CFG6                                      0x01E8
#define RFIP_ADDR_LOFT_COMP_I_CFG7                                      0x01EC
#define RFIP_ADDR_LOFT_COMP_Q_CFG0                                      0x01F0
#define RFIP_ADDR_LOFT_COMP_Q_CFG1                                      0x01F4
#define RFIP_ADDR_LOFT_COMP_Q_CFG2                                      0x01F8
#define RFIP_ADDR_LOFT_COMP_Q_CFG3                                      0x01FC
#define RFIP_ADDR_LOFT_COMP_Q_CFG4                                      0x0200
#define RFIP_ADDR_LOFT_COMP_Q_CFG5                                      0x0204
#define RFIP_ADDR_LOFT_COMP_Q_CFG6                                      0x0208
#define RFIP_ADDR_LOFT_COMP_Q_CFG7                                      0x020C
#define RFIP_ADDR_LOFT_SFT_CFG_I_CFG0                                   0x0210
#define RFIP_ADDR_LOFT_SFT_CFG_I_CFG1                                   0x0214
#define RFIP_ADDR_LOFT_SFT_CFG_Q_CFG0                                   0x0218
#define RFIP_ADDR_LOFT_SFT_CFG_Q_CFG1                                   0x021C
#define RFIP_ADDR_TX_DC_OFFSET_CFG                                      0x0220
#define RFIP_ADDR_TX_CTRL_CFG                                           0x0224
#define RFIP_ADDR_TX_SRAM_TEST_CFG                                      0x0228
#define RFIP_ADDR_TX_SRAM_TEST_DR                                       0x022C
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG0                                0x0230
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG1                                0x0234
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG2                                0x0238
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG3                                0x023C
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG4                                0x0240
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG5                                0x0244
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG6                                0x0248
#define RFIP_ADDR_TX_UPCONVER_COEFF_CFG7                                0x024C
#define RFIP_ADDR_GPIO_MODE_CFG0                                        0x0300
#define RFIP_ADDR_GPIO_MODE_CFG1                                        0x0304
#define RFIP_ADDR_GPIO_RX_FORCE_CFG                                     0x0308
#define RFIP_ADDR_GPIO_TX_FORCE_CFG                                     0x030C
#define RFIP_ADDR_GPIO_FORCE_EN_CFG                                     0x0310
#define RFIP_ADDR_TX_GAIN_FORCE_CFG                                     0x03E4
#define RFIP_ADDR_TX_COMP_FORCE_CFG                                     0x03E8
#define RFIP_ADDR_RFIPSTATE                                             0x03EC
#define RFIP_ADDR_RFIPCALIN                                             0x03F0
#define RFIP_ADDR_RFPLLDEBUGCFG                                         0x03F4
#define RFIP_ADDR_RFPLLDEBUGCFG2                                        0x03F8
#define RFIP_ADDR_TXRX_DEBUG                                            0x03FC

/* RFIP_ADDR_GCFG */
#define FORCESTATE                                                      BIT1                  // Force Off State
#define SOFT_RESET                                                      BIT0                  // Soft reset all registers

/* RFIP_ADDR_RX_STATE_DELAY_CFG1 */
#define RX_ANA_ON_DELAY_MASK                                            CO_BITS(0, 15) // delay time from RX_ANA_ON to RX_IDLE
#define RX_ANA_ON_DELAY_SHIFT                                           0
#define RX_IDLE_DELAY_MASK                                              CO_BITS(16, 31) // delay time from RX_IDLE to RX_ACTIVE
#define RX_IDLE_DELAY_SHIFT                                             16

/* RFIP_ADDR_RX_STATE_DELAY_CFG2 */
#define RX_FASTON_DELAY_MASK                                            CO_BITS(0, 15) // delay time from RX_FAST_ON to RX_OFF
#define RX_FASTON_DELAY_SHIFT                                           0

/* RFIP_ADDR_TX_STATE_DELAY_CFG1 */
#define TX_ANA_ON_DELAY_MASK                                            CO_BITS(0, 15) // delay time from TX_ANA_ON to TX_ACTIVE
#define TX_ANA_ON_DELAY_SHIFT                                           0
#define TX_RFOFF_DELAY_MASK                                             CO_BITS(16, 31) // delay time from TX_RF_OFF to TX_OFF
#define TX_RFOFF_DELAY_SHIFT                                            16

/* RFIP_ADDR_RXOFF_CFG */
#define PU_SARADC                                                       BIT13
#define PU_SDMADC                                                       BIT12
#define RX_ADC_RST_CFG                                                  BIT11
#define PU_RXFLT_POST                                                   BIT10
#define PLLS_CLK4SAR_160_EN                                             BIT9
#define VCO_RXBUF_EN                                                    BIT8
#define PU_RXPKDET2                                                     BIT7
#define PU_RXPKDET1                                                     BIT6
#define RXFILTER_OS_EN                                                  BIT5
#define PUP_FILTER_PRE                                                  BIT4
#define PU_RXLO                                                         BIT3
#define PU_TIA                                                          BIT2
#define PU_RMIX                                                         BIT1
#define PU_LNA                                                          BIT0

/* RFIP_ADDR_RXANA_CFG */
#define PU_SARADC                                                       BIT13
#define PU_SDMADC                                                       BIT12
#define RX_ADC_RST_CFG                                                  BIT11
#define PU_RXFLT_POST                                                   BIT10
#define PLLS_CLK4SAR_160_EN                                             BIT9
#define VCO_RXBUF_EN                                                    BIT8
#define PU_RXPKDET2                                                     BIT7
#define PU_RXPKDET1                                                     BIT6
#define RXFILTER_OS_EN                                                  BIT5
#define PUP_FILTER_PRE                                                  BIT4
#define PU_RXLO                                                         BIT3
#define PU_TIA                                                          BIT2
#define PU_RMIX                                                         BIT1
#define PU_LNA                                                          BIT0

/* RFIP_ADDR_RXIDLE_CFG */
#define PU_SARADC                                                       BIT13
#define PU_SDMADC                                                       BIT12
#define RX_ADC_RST_CFG                                                  BIT11
#define PU_RXFLT_POST                                                   BIT10
#define PLLS_CLK4SAR_160_EN                                             BIT9
#define VCO_RXBUF_EN                                                    BIT8
#define PU_RXPKDET2                                                     BIT7
#define PU_RXPKDET1                                                     BIT6
#define RXFILTER_OS_EN                                                  BIT5
#define PUP_FILTER_PRE                                                  BIT4
#define PU_RXLO                                                         BIT3
#define PU_TIA                                                          BIT2
#define PU_RMIX                                                         BIT1
#define PU_LNA                                                          BIT0

/* RFIP_ADDR_RXACTIVE_CFG */
#define PU_SARADC                                                       BIT13
#define PU_SDMADC                                                       BIT12
#define RX_ADC_RST_CFG                                                  BIT11
#define PU_RXFLT_POST                                                   BIT10
#define PLLS_CLK4SAR_160_EN                                             BIT9
#define VCO_RXBUF_EN                                                    BIT8
#define PU_RXPKDET2                                                     BIT7
#define PU_RXPKDET1                                                     BIT6
#define RXFILTER_OS_EN                                                  BIT5
#define PUP_FILTER_PRE                                                  BIT4
#define PU_RXLO                                                         BIT3
#define PU_TIA                                                          BIT2
#define PU_RMIX                                                         BIT1
#define PU_LNA                                                          BIT0

/* RFIP_ADDR_TXOFF_CFG */
#define PU_TXFLT                                                        BIT8
#define PU_TXDAC                                                        BIT7
#define PU_TXAUXL                                                       BIT6
#define PU_TXAUXH                                                       BIT5
#define PU_TXMIXER                                                      BIT4
#define PU_TXLO                                                         BIT3
#define PU_PA                                                           BIT2
#define VCO_TXBUF_EN                                                    BIT1
#define EN_TX                                                           BIT0

/* RFIP_ADDR_TXANA_CFG */
#define PU_TXFLT                                                        BIT8
#define PU_TXDAC                                                        BIT7
#define PU_TXAUXL                                                       BIT6
#define PU_TXAUXH                                                       BIT5
#define PU_TXMIXER                                                      BIT4
#define PU_TXLO                                                         BIT3
#define PU_PA                                                           BIT2
#define VCO_TXBUF_EN                                                    BIT1
#define EN_TX                                                           BIT0

/* RFIP_ADDR_TXON_CFG */
#define PU_TXFLT                                                        BIT8
#define PU_TXDAC                                                        BIT7
#define PU_TXAUXL                                                       BIT6
#define PU_TXAUXH                                                       BIT5
#define PU_TXMIXER                                                      BIT4
#define PU_TXLO                                                         BIT3
#define PU_PA                                                           BIT2
#define VCO_TXBUF_EN                                                    BIT1
#define EN_TX                                                           BIT0

/* RFIP_ADDR_TXRFOFF_CFG */
#define PU_TXFLT                                                        BIT8
#define PU_TXDAC                                                        BIT7
#define PU_TXAUXL                                                       BIT6
#define PU_TXAUXH                                                       BIT5
#define PU_TXMIXER                                                      BIT4
#define PU_TXLO                                                         BIT3
#define PU_PA                                                           BIT2
#define VCO_TXBUF_EN                                                    BIT1
#define EN_TX                                                           BIT0

/* RFIP_ADDR_LDO_CFG */
#define LDOCLK_BYP                                                      BIT31
#define LDOCLK_CAL_MASK                                                 CO_BITS(28, 30)
#define LDOCLK_CAL_SHIFT                                                28
#define LDOCLK_LQB                                                      BIT27
#define LDOCLK_TEST                                                     BIT26
#define PU_LDOCLK                                                       BIT25

/* RFIP_ADDR_XTAL_CFG */
#define PU_XTAL_DIG_REGSTR                                              BIT31
#define REG_XTAL_VBIT_MASK                                              CO_BITS(27, 30)
#define REG_XTAL_VBIT_SHIFT                                             27
#define XCLK4FPGA_EN_LV                                                 BIT26
#define XCLK4MCU_EN_LV                                                  BIT25
#define XCLK4PLLCAL_EN_LV                                               BIT24
#define XCLK4PLLS_EN_LV                                                 BIT23
#define XTAL_EN_OSC                                                     BIT22
#define XTAL_EN_RFPLL_BUFF                                              BIT21
#define XTAL_FREQ_TUNING_MASK                                           CO_BITS(14, 20)
#define XTAL_FREQ_TUNING_SHIFT                                          14
#define XTAL_OSC_IBIT_MASK                                              CO_BITS(9, 13)
#define XTAL_OSC_IBIT_SHIFT                                             9
#define PU_XTAL_REGSTR                                                  BIT8

/* RFIP_ADDR_PLLS_CFG1 */
#define PLLS_CLK4DIG_160_EN                                             BIT31
#define PLLS_CLK_320_EN                                                 BIT30
#define PLLS_CLK_480_EN                                                 BIT29
#define PLLS_CLK_960_EN                                                 BIT28
#define PLLS_DITHER_LV_MASK                                             CO_BITS(26, 27)
#define PLLS_DITHER_LV_SHIFT                                            26
#define PLLS_FORCELOCK_LV                                               BIT25
#define PLLS_FRAC_CONTROL_LV                                            BIT24
#define PLLS_IDF_LV_MASK                                                CO_BITS(20, 23)
#define PLLS_IDF_LV_SHIFT                                               20
#define PLLS_NDIV_LV_MASK                                               CO_BITS(12, 19)
#define PLLS_NDIV_LV_SHIFT                                              12
#define PLLS_ENABLE_LV                                                  BIT11
#define PU_PLLS_LDO_LV                                                  BIT10
#define PLLS_LOCK_LV_DD                                                 BIT9

/* RFIP_ADDR_PLLS_CFG2 */
#define PLLS_FRAC_LV_MASK                                               CO_BITS(16, 31)
#define PLLS_FRAC_LV_SHIFT                                              16

/* RFIP_ADDR_RFPLL_LOOP_CFG */
#define LDO_RFPLL_BYP_REGSTR                                            BIT31
#define LDO_RFPLL_CAL_REGSTR_MASK                                       CO_BITS(28, 30)
#define LDO_RFPLL_CAL_REGSTR_SHIFT                                      28
#define LDO_RFPLL_LQB_REGSTR                                            BIT27
#define LDO_RFPLL_PU_REGSTR                                             BIT26
#define LDO_RFPLL_TST_REGSTR                                            BIT25
#define RFPLL_REG_LOOP_VBIT_V22_MASK                                    CO_BITS(21, 24)
#define RFPLL_REG_LOOP_VBIT_V22_SHIFT                                   21
#define RFPLL_CPIBIT_V22_MASK                                           CO_BITS(18, 20)
#define RFPLL_CPIBIT_V22_SHIFT                                          18
#define RFPLL_CPRBIT_MASK                                               CO_BITS(15, 17)
#define RFPLL_CPRBIT_SHIFT                                              15
#define RFPLL_DIS_LF                                                    BIT14
#define RFPLL_DIS_PDT                                                   BIT13
#define RFPLL_DIS_VCTRL                                                 BIT12
#define RFPLL_SMODE                                                     BIT11
#define RFPLL_VCO_GAIN_MASK                                             CO_BITS(7, 10)
#define RFPLL_VCO_GAIN_SHIFT                                            7
#define PU_RFPLL_LOOP_V22                                               BIT6
#define REG_RFPLL_PFDEN_DR                                              BIT0

/* RFIP_ADDR_RFPLL_DIG_CFG1 */
#define RFPLL_CO_AMP_BYPASS                                             BIT26
#define RFPLL_MASHFIL_CKEN                                              BIT25
#define RFPLL_XRST_MASH                                                 BIT24
#define RFPLL_XRST_SD                                                   BIT23
#define RFPLL_SDCLK_EN                                                  BIT22
#define RFPLL_XRST_SDFILT                                               BIT21
#define RFPLL_SD_MOD_MASK                                               CO_BITS(19, 20)
#define RFPLL_SD_MOD_SHIFT                                              19
#define RFPLL_SD_FILT                                                   BIT18
#define RFPLL_FINE_AMP_BYPASS                                           BIT17
#define RFPLL_CO_FREQ_BYPASS                                            BIT16
#define RFPLL_NDIVIDER_REG_MASK                                         CO_BITS(1, 15)
#define RFPLL_NDIVIDER_REG_SHIFT                                        1
#define RFPLL_NDIVIDER_REG_EN                                           BIT0

/* RFIP_ADDR_RFPLL_DIG_CFG2 */
#define RFPLL_SEL_FREQPLLIN_MASK                                        CO_BITS(28, 31)
#define RFPLL_SEL_FREQPLLIN_SHIFT                                       28
#define RFPLL_SEL_FCOMPMES_MASK                                         CO_BITS(24, 27)
#define RFPLL_SEL_FCOMPMES_SHIFT                                        24
#define RFPLL_ORDER_FWP_MASK                                            CO_BITS(17, 23)
#define RFPLL_ORDER_FWP_SHIFT                                           17
#define RFPLL_XPD_PIPELINE_LB                                           BIT16
#define RFPLL_FREQ_CAL_ACCURACY_MASK                                    CO_BITS(12, 15)
#define RFPLL_FREQ_CAL_ACCURACY_SHIFT                                   12
#define RFPLL_FREQ_NUM_PERIOD_SET_MASK                                  CO_BITS(9, 11)
#define RFPLL_FREQ_NUM_PERIOD_SET_SHIFT                                 9
#define RFPLL_WAIT_FINE_F_MASK                                          CO_BITS(6, 8)
#define RFPLL_WAIT_FINE_F_SHIFT                                         6
#define RFPLL_XRST_SYNCH                                                BIT5
#define RFPLL_XRST_CLK_CTRL                                             BIT4
#define RFPLL_XRST_DIG                                                  BIT3
#define RFPLL_XRST_CMOS_DIVIDER                                         BIT2

/* RFIP_ADDR_RFPLL_DIG_INT_FRAC_CFG */
#define RFPLL_INT_MASK                                                  CO_BITS(21, 29)
#define RFPLL_INT_SHIFT                                                 21
#define RFPLL_FRAC_MASK                                                 CO_BITS(0, 20)
#define RFPLL_FRAC_SHIFT                                                0

/* RFIP_ADDR_RFPLL_DIG_VCOCAL_CFG */
#define RFPLL_LOCK_DET_LV_DD                                            BIT31
#define RFPLL_LOCK_NUM_MASK                                             CO_BITS(23, 30)
#define RFPLL_LOCK_NUM_SHIFT                                            23
#define RFPLL_CAL_REQ                                                   BIT19
#define RFPLL_XRST_CALIBRATOR                                           BIT18
#define RFPLL_CALFREQ_EXT_MASK                                          CO_BITS(10, 17)
#define RFPLL_CALFREQ_EXT_SHIFT                                         10
#define RFPLL_CALFREQ_OUT_MASK                                          CO_BITS(2, 9)
#define RFPLL_CALFREQ_OUT_SHIFT                                         2
#define RFPLL_CALFREQ_EXT_SEL                                           BIT1
#define RFPLL_END_CAL_DD                                                BIT0

/* RFIP_ADDR_MDLL_CFG */
#define MDLL_CP_IBIT_MASK                                               CO_BITS(29, 31)
#define MDLL_CP_IBIT_SHIFT                                              29
#define MDLL_IVBIT_MASK                                                 CO_BITS(26, 28)
#define MDLL_IVBIT_SHIFT                                                26
#define MDLL_PDT_MODE                                                   BIT25
#define MDLL_CKX2_CBIT_MASK                                             CO_BITS(22, 24)
#define MDLL_CKX2_CBIT_SHIFT                                            22
#define MDLL_CKX2_EN                                                    BIT21
#define MDLL_CKX2_RBIT                                                  BIT20
#define MDLL_CLK_TEST_EN                                                BIT19
#define MDLL_DIV_NUM_MASK                                               CO_BITS(15, 18)
#define MDLL_DIV_NUM_SHIFT                                              15
#define MDLL_CBIT_MASK                                                  CO_BITS(12, 14)
#define MDLL_CBIT_SHIFT                                                 12
#define MDLL_RBIT                                                       BIT11
#define MDLL_CLK_RFPLL_EN                                               BIT10
#define RSTN_MDLL                                                       BIT9
#define PU_MDLL                                                         BIT8

/* RFIP_ADDR_RX_LPF2_CFG */
#define RXFLT_POST_CCW_MASK                                             CO_BITS(26, 31)
#define RXFLT_POST_CCW_SHIFT                                            26
#define RXFLT_POST_BW_MODE                                              BIT25

/* RFIP_ADDR_SDMADC_CFG */
#define SDMADC_DWAORCYCLIC                                              BIT31
#define SDMADC_IAOP_CTRL_MASK                                           CO_BITS(28, 30)
#define SDMADC_IAOP_CTRL_SHIFT                                          28
#define SDMADC_ICMP_CTRL_MASK                                           CO_BITS(25, 27)
#define SDMADC_ICMP_CTRL_SHIFT                                          25
#define SDMADC_NODEM                                                    BIT24
#define SDMADC_VDIGSEL_MASK                                             CO_BITS(22, 23)
#define SDMADC_VDIGSEL_SHIFT                                            22
#define SDMADC_CAL_INPUT                                                BIT21
#define SDMADC_CLK_EN                                                   BIT20

/* RFIP_ADDR_SDMADC_DIG_CFG */
#define RXINVADCCLK                                                     BIT31               // RXADC sample clk inv enable
#define RXDIVIDERINIT_MASK                                              CO_BITS(29, 30) // RX clk divider init data
#define RXDIVIDERINIT_SHIFT                                             29
#define RX_ADCCAL_START_OFFSET_MASK                                     CO_BITS(17, 20)
#define RX_ADCCAL_START_OFFSET_SHIFT                                    17
#define RX_ADCCAL_THRESHOLD_MASK                                        CO_BITS(9, 16)
#define RX_ADCCAL_THRESHOLD_SHIFT                                       9
#define RX_ADCCAL_CNT2_MASK                                             CO_BITS(6, 8)
#define RX_ADCCAL_CNT2_SHIFT                                            6
#define RX_ADCCAL_CNT1_MASK                                             CO_BITS(3, 5)
#define RX_ADCCAL_CNT1_SHIFT                                            3
#define RX_ADCCAL_IQSEL                                                 BIT2
#define RX_ADCCAL_SWRST                                                 BIT1
#define ADCCAL_START                                                    BIT0

/* RFIP_ADDR_SDMADC_CIC_CFG */
#define RXCICEN_CFG                                                     BIT31  // RXCIC enable
#define RXCICRESETEN                                                    BIT30 // RXCIC reset enable when Sat
#define RXCICSATTHRESHOLD_MASK                                          CO_BITS(22, 29)
#define RXCICSATTHRESHOLD_SHIFT                                         22
#define RXCICOFFSET_EN                                                  BIT21

/* RFIP_ADDR_SARADC_CFG */
#define SARADC_LDO_SEL_MASK                                             CO_BITS(1, 2)
#define SARADC_LDO_SEL_SHIFT                                            1
#define SEL_SAR_ADC_CFG                                                 BIT0

/* RFIP_ADDR_AUXADC_CFG */
#define AUXADC_ON                                                       BIT31
#define AUXADC_IN_CHSEL_MASK                                            CO_BITS(29, 30)
#define AUXADC_IN_CHSEL_SHIFT                                           29
#define AUXADC_N_SAMP_MASK                                              CO_BITS(23, 28)
#define AUXADC_N_SAMP_SHIFT                                             23
#define AUXADC_BUFFER_MODE                                              BIT22
#define AUXADC_SPEED                                                    BIT21
#define LDO_AUXADC_TEST_EN                                              BIT20
#define LDO_AUXADC_VBIT_MASK                                            CO_BITS(18, 19)
#define LDO_AUXADC_VBIT_SHIFT                                           18
#define PU_LDO_AUXADC                                                   BIT17
#define AUXADC_DATA_MASK                                                CO_BITS(1, 10)
#define AUXADC_DATA_SHIFT                                               1
#define AUXADC_DATA_VALID_FLAG                                          BIT0

/* RFIP_ADDR_RCCAL_CFG */
#define RCCAL_BW_MODE                                                   BIT31
#define RCCAL_BW_CTRL_MASK                                              CO_BITS(29, 30)
#define RCCAL_BW_CTRL_SHIFT                                             29
#define RCCAL_CSAMP_DEL_MASK                                            CO_BITS(14, 22)
#define RCCAL_CSAMP_DEL_SHIFT                                           14
#define RCCAL_CFORCE                                                    BIT13
#define RCCAL_CTMAX_MASK                                                CO_BITS(7, 12)
#define RCCAL_CTMAX_SHIFT                                               7
#define TX_LPF_CCAL_FO                                                  BIT6
#define RX_LPF_CCAL_FO                                                  BIT5
#define TX_LPF_RC_UPDATE_CFG                                            BIT4
#define RX_LPF_RC_UPDATE_CFG                                            BIT3
#define RCCAL_DEBUG                                                     BIT2
#define RC_CAL_CEN                                                      BIT0

/* RFIP_ADDR_RCCAL_OFFSET_TEST_CFG */
#define RX_LPF_CCAL_OFFSET_MASK                                         CO_BITS(26, 31)
#define RX_LPF_CCAL_OFFSET_SHIFT                                        26
#define TX_LPF_CCAL_OFFSET_MASK                                         CO_BITS(20, 25)
#define TX_LPF_CCAL_OFFSET_SHIFT                                        20
#define RCCAL_CFORCE_VAL_MASK                                           CO_BITS(6, 11)
#define RCCAL_CFORCE_VAL_SHIFT                                          6
#define RCCAL_CFORCE_ADD_MASK                                           CO_BITS(0, 5)
#define RCCAL_CFORCE_ADD_SHIFT                                          0

/* RFIP_ADDR_MUX_CFG */
#define PLLS_TEST_EN                                                    BIT31

/* RFIP_ADDR_LDO_ANA_CFG */
#define LDO_ANA_PUP_REGSTR                                              BIT31
#define LDO_ANA_LQB_REGSTR                                              BIT30
#define LDO_ANA_CAL_REGSTR_MASK                                         CO_BITS(27, 29)
#define LDO_ANA_CAL_REGSTR_SHIFT                                        27
#define LDO_ANA_BYP_REGSTR                                              BIT26
#define LDO_ANA_TST_REGSTR                                              BIT25

/* RFIP_ADDR_LDO_VCO_CFG */
#define LDO_VCO_PUP_REGSTR                                              BIT31
#define LDO_VCO_LQB_REGSTR                                              BIT30
#define LDO_VCO_CAL_REGSTR_MASK                                         CO_BITS(27, 29)
#define LDO_VCO_CAL_REGSTR_SHIFT                                        27
#define LDO_VCO_BYP_REGSTR                                              BIT26
#define LDO_VCO_TST_REGSTR                                              BIT25

/* RFIP_ADDR_LNA_CFG */
#define LNA_REG_TRIM_MASK                                               CO_BITS(28, 31)
#define LNA_REG_TRIM_SHIFT                                              28
#define LNA_CAL_MASK                                                    CO_BITS(26, 27)
#define LNA_CAL_SHIFT                                                   26
#define LNA_RBIAS_TRIM_MASK                                             CO_BITS(22, 25)
#define LNA_RBIAS_TRIM_SHIFT                                            22
#define LNA_IBIAS_TRIM_MASK                                             CO_BITS(18, 21)
#define LNA_IBIAS_TRIM_SHIFT                                            18
#define RX_MATCH_C_MASK                                                 CO_BITS(14, 17)
#define RX_MATCH_C_SHIFT                                                14

/* RFIP_ADDR_RXMIX_CFG */
#define RXLO_VCM_MASK                                                   CO_BITS(29, 31)
#define RXLO_VCM_SHIFT                                                  29

/* RFIP_ADDR_RXFILTER1_CFG1 */
#define RXFILTER_AUXI_EN                                                BIT31
#define RXFILTER_AUXQ_EN                                                BIT30
#define RXFILTER_BW_MODE                                                BIT29
#define RXFILTER_CCW_MASK                                               CO_BITS(23, 28)
#define RXFILTER_CCW_SHIFT                                              23
#define RXFILTER_OS_I_EN                                                BIT22
#define RXFILTER_OS_Q_EN                                                BIT21
#define RXFILTER_OS_MODE                                                BIT20

/* RFIP_ADDR_RXFILTER1_CFG2 */
#define RXFILTER_OS_I_FORCE_VALUE_MASK                                  CO_BITS(23, 31) // from RX_DC_OFFSET Or force value
#define RXFILTER_OS_I_FORCE_VALUE_SHIFT                                 23
#define RXFILTER_OS_Q_FORCE_VALUE_MASK                                  CO_BITS(14, 22)
#define RXFILTER_OS_Q_FORCE_VALUE_SHIFT                                 14
#define RXFILTER_OS_FORCE_EN                                            BIT13

/* RFIP_ADDR_RXPKDET_CFG */
#define PKDET1_VREF_MASK                                                CO_BITS(3, 4)
#define PKDET1_VREF_SHIFT                                               3
#define PKDET2_VREF_MASK                                                CO_BITS(0, 2)
#define PKDET2_VREF_SHIFT                                               0

/* RFIP_ADDR_VCO_CFG1 */
#define PU_VCO                                                          BIT31
#define PU_DIV                                                          BIT30
#define REG_VCO_BIT_MASK                                                CO_BITS(26, 29)
#define REG_VCO_BIT_SHIFT                                               26
#define REG_VCOBUF_BIT_MASK                                             CO_BITS(22, 25)
#define REG_VCOBUF_BIT_SHIFT                                            22
#define VCO_DIVBIAS_BIT_MASK                                            CO_BITS(20, 21)
#define VCO_DIVBIAS_BIT_SHIFT                                           20
#define VCO_PKDET_EN                                                    BIT19
#define VCO_PKREF_BIT_MASK                                              CO_BITS(16, 18)
#define VCO_PKREF_BIT_SHIFT                                             16
#define VCO_VAR_BIT_MASK                                                CO_BITS(13, 15)
#define VCO_VAR_BIT_SHIFT                                               13
#define VCO_VCOM_BIT_MASK                                               CO_BITS(10, 12)
#define VCO_VCOM_BIT_SHIFT                                              10
#define VCO_VAR_SWAP                                                    BIT9
#define VCO_PK_OVLD_DD                                                  BIT8

/* RFIP_ADDR_VCO_CFG2 */
#define VCO_BAND_FORCE_EN                                               BIT31
#define VCO_BAND_MASK                                                   CO_BITS(23, 30)
#define VCO_BAND_SHIFT                                                  23

/* RFIP_ADDR_RFPLL_CFG */
#define PU_RFPLL_REGDIV2                                                BIT31
#define PU_RFPLL_REGPRESC                                               BIT30
#define RFPLL_REG_DIV2_VBIT_MASK                                        CO_BITS(26, 29)
#define RFPLL_REG_DIV2_VBIT_SHIFT                                       26
#define RFPLL_REG_PRESC_VBIT_MASK                                       CO_BITS(22, 25)
#define RFPLL_REG_PRESC_VBIT_SHIFT                                      22
#define RFPLL_PRESC_DIV23_EN                                            BIT21
#define RFPLL_SDMCLK_SEL                                                BIT20
#define RFPLL_FBC_SEL                                                   BIT19

/* RFIP_ADDR_PA_CFG */
#define REG_PAIBCAS_BIT_MASK                                            CO_BITS(6, 8)
#define REG_PAIBCAS_BIT_SHIFT                                           6
#define REG_PAIB_BIT_MASK                                               CO_BITS(3, 5)
#define REG_PAIB_BIT_SHIFT                                              3
#define REG_PAVBCS_BIT_MASK                                             CO_BITS(0, 2)
#define REG_PAVBCS_BIT_SHIFT                                            0

/* RFIP_ADDR_TXRF_CFG1 */
#define REG_TXCAPTUNE_MASK                                              CO_BITS(21, 24)
#define REG_TXCAPTUNE_SHIFT                                             21
#define REG_TXVBCAL_BIT_MASK                                            CO_BITS(19, 20)
#define REG_TXVBCAL_BIT_SHIFT                                           19
#define REG_TXVBCAS_BIT_MASK                                            CO_BITS(17, 18)
#define REG_TXVBCAS_BIT_SHIFT                                           17
#define REG_TXVBLO_BIT_MASK                                             CO_BITS(15, 16)
#define REG_TXVBLO_BIT_SHIFT                                            15
#define REG_TXIBIAS_BIT_MASK                                            CO_BITS(13, 14)
#define REG_TXIBIAS_BIT_SHIFT                                           13
#define REG_DEQ_BIT_MASK                                                CO_BITS(11, 12)
#define REG_DEQ_BIT_SHIFT                                               11
#define REG_TXCAL_DCI_EN                                                BIT10
#define REG_TXCAL_DCV_EN                                                BIT9
#define REG_TXCAL_EDGE                                                  BIT8
#define REG_TXCAL_SWAP                                                  BIT7
#define TXCAL_CLK_SEL_MASK                                              CO_BITS(5, 6)
#define TXCAL_CLK_SEL_SHIFT                                             5

/* RFIP_ADDR_TXRF_CFG2 */
#define REG_TXCALI_BIT_MASK                                             CO_BITS(23, 31)
#define REG_TXCALI_BIT_SHIFT                                            23
#define REG_TXCALQ_BIT_MASK                                             CO_BITS(14, 22)
#define REG_TXCALQ_BIT_SHIFT                                            14

/* RFIP_ADDR_TXDAC_CFG */
#define REG_TXDAC_MODE_MASK                                             CO_BITS(29, 31)
#define REG_TXDAC_MODE_SHIFT                                            29
#define REG_TXDAC_CMBIT_MASK                                            CO_BITS(26, 28)
#define REG_TXDAC_CMBIT_SHIFT                                           26
#define REG_TXDAC_LPMODE                                                BIT25
#define REG_TXDAC_RANGE_MASK                                            CO_BITS(23, 24)
#define REG_TXDAC_RANGE_SHIFT                                           23
#define REG_TXDAC_SEL_EDGE                                              BIT22
#define REG_TXDAC_VDVDD_MASK                                            CO_BITS(20, 21)
#define REG_TXDAC_VDVDD_SHIFT                                           20
#define REG_TXDAC_VLOWCTRL_MASK                                         CO_BITS(18, 19)
#define REG_TXDAC_VLOWCTRL_SHIFT                                        18
#define TXDC_CALOUT_I_DD                                                BIT17
#define TXDC_CALOUT_Q_DD                                                BIT16

/* RFIP_ADDR_TXFILT_CFG */
#define REG_TXFLT_CCW_MASK                                              CO_BITS(26, 31)
#define REG_TXFLT_CCW_SHIFT                                             26
#define REG_TXFLT_GAIN_MASK                                             CO_BITS(23, 25)
#define REG_TXFLT_GAIN_SHIFT                                            23
#define REG_TXFLT_MODE                                                  BIT22
#define REG_TXFLT_VCAS_MASK                                             CO_BITS(20, 21)
#define REG_TXFLT_VCAS_SHIFT                                            20

/* RFIP_ADDR_BG_CFG */
#define REG_BG_START_N                                                  BIT29
#define REG_BG_VBIT_MASK                                                CO_BITS(26, 28)
#define REG_BG_VBIT_SHIFT                                               26
#define REG_BG_EN_BGCR_PLL                                              BIT25
#define REG_BG_EN_BGCR_RX                                               BIT24
#define REG_BG_EN_BGCR_TX                                               BIT23
#define REG_BG_EN_BGCR_CAL                                              BIT22

/* RFIP_ADDR_TEMP_CFG */
#define PU_TEMP                                                         BIT31
#define TEMP_I_MASK                                                     CO_BITS(0, 9)
#define TEMP_I_SHIFT                                                    0

/* RFIP_ADDR_LO_CAL_CFG */
#define REG_LOCALATT_BIT_MASK                                           CO_BITS(30, 31)
#define REG_LOCALATT_BIT_SHIFT                                          30
#define EN_IQCAL                                                        BIT23
#define EN_PDET                                                         BIT22

/* RFIP_ADDR_LDO_ANALOG_CFG */
#define CAL_LDODIG_L_MASK                                               CO_BITS(29, 31)
#define CAL_LDODIG_L_SHIFT                                              29
#define COMP_PU_L                                                       BIT28

/* RFIP_ADDR_RX_GAIN_CFG */
#define RX_GAIN_FORCE_EN                                                BIT31
#define LNA_GC_MASK                                                     CO_BITS(28, 30)
#define LNA_GC_SHIFT                                                    28
#define LNA_RLOAD                                                       BIT27
#define LNA_RMATCH                                                      BIT26
#define LPF1_GC_MASK                                                    CO_BITS(23, 25)
#define LPF1_GC_SHIFT                                                   23
#define LPF2_GC_MASK                                                    CO_BITS(19, 22)
#define LPF2_GC_SHIFT                                                   19
#define LPF3_GC_MASK                                                    CO_BITS(16, 18)
#define LPF3_GC_SHIFT                                                   16

/* RFIP_ADDR_RF_RESERVED */
#define REG_RESERVED_IN_MASK                                            CO_BITS(16, 31)
#define REG_RESERVED_IN_SHIFT                                           16
#define REG_RESERVED_OUT_MASK                                           CO_BITS(0, 3)
#define REG_RESERVED_OUT_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG0 */
#define TX_RF_GAIN_CFG1_MASK                                            CO_BITS(11, 21)
#define TX_RF_GAIN_CFG1_SHIFT                                           11
#define TX_RF_GAIN_CFG0_MASK                                            CO_BITS(0, 10)
#define TX_RF_GAIN_CFG0_SHIFT                                           0

/* RFIP_ADDR_TX_RF_GAIN_CFG1 */
#define TX_RF_GAIN_CFG3_MASK                                            CO_BITS(11, 21)
#define TX_RF_GAIN_CFG3_SHIFT                                           11
#define TX_RF_GAIN_CFG2_MASK                                            CO_BITS(0, 10)
#define TX_RF_GAIN_CFG2_SHIFT                                           0

/* RFIP_ADDR_TX_RF_GAIN_CFG2 */
#define TX_RF_GAIN_CFG5_MASK                                            CO_BITS(11, 21)
#define TX_RF_GAIN_CFG5_SHIFT                                           11
#define TX_RF_GAIN_CFG4_MASK                                            CO_BITS(0, 10)
#define TX_RF_GAIN_CFG4_SHIFT                                           0

/* RFIP_ADDR_TX_RF_GAIN_CFG3 */
#define TX_RF_GAIN_CFG7_MASK                                            CO_BITS(11, 21)
#define TX_RF_GAIN_CFG7_SHIFT                                           11
#define TX_RF_GAIN_CFG6_MASK                                            CO_BITS(0, 10)
#define TX_RF_GAIN_CFG6_SHIFT                                           0

/* RFIP_ADDR_TX_RF_GAIN_CFG4 */
#define TX_RF_GAIN_CFG9_MASK                                            CO_BITS(11, 21)
#define TX_RF_GAIN_CFG9_SHIFT                                           11
#define TX_RF_GAIN_CFG8_MASK                                            CO_BITS(0, 10)
#define TX_RF_GAIN_CFG8_SHIFT                                           0

/* RFIP_ADDR_TX_RF_GAIN_CFG5 */
#define TX_RF_GAIN_CFG11_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG11_SHIFT                                          11
#define TX_RF_GAIN_CFG10_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG10_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG6 */
#define TX_RF_GAIN_CFG13_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG13_SHIFT                                          11
#define TX_RF_GAIN_CFG12_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG12_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG7 */
#define TX_RF_GAIN_CFG15_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG15_SHIFT                                          11
#define TX_RF_GAIN_CFG14_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG14_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG8 */
#define TX_RF_GAIN_CFG17_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG17_SHIFT                                          11
#define TX_RF_GAIN_CFG16_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG16_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG9 */
#define TX_RF_GAIN_CFG19_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG19_SHIFT                                          11
#define TX_RF_GAIN_CFG18_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG18_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG10 */
#define TX_RF_GAIN_CFG21_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG21_SHIFT                                          11
#define TX_RF_GAIN_CFG20_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG20_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG11 */
#define TX_RF_GAIN_CFG23_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG23_SHIFT                                          11
#define TX_RF_GAIN_CFG22_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG22_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG12 */
#define TX_RF_GAIN_CFG25_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG25_SHIFT                                          11
#define TX_RF_GAIN_CFG24_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG24_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG13 */
#define TX_RF_GAIN_CFG27_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG27_SHIFT                                          11
#define TX_RF_GAIN_CFG26_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG26_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG14 */
#define TX_RF_GAIN_CFG29_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG29_SHIFT                                          11
#define TX_RF_GAIN_CFG28_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG28_SHIFT                                          0

/* RFIP_ADDR_TX_RF_GAIN_CFG15 */
#define TX_RF_GAIN_CFG31_MASK                                           CO_BITS(11, 21)
#define TX_RF_GAIN_CFG31_SHIFT                                          11
#define TX_RF_GAIN_CFG30_MASK                                           CO_BITS(0, 10)
#define TX_RF_GAIN_CFG30_SHIFT                                          0

/* RFIP_ADDR_TX_DIG_GAIN_CFG0 */
#define TX_DIG_GAIN_CFG4_MASK                                           CO_BITS(24, 29)
#define TX_DIG_GAIN_CFG4_SHIFT                                          24
#define TX_DIG_GAIN_CFG3_MASK                                           CO_BITS(18, 23)
#define TX_DIG_GAIN_CFG3_SHIFT                                          18
#define TX_DIG_GAIN_CFG2_MASK                                           CO_BITS(12, 17)
#define TX_DIG_GAIN_CFG2_SHIFT                                          12
#define TX_DIG_GAIN_CFG1_MASK                                           CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG1_SHIFT                                          6
#define TX_DIG_GAIN_CFG0_MASK                                           CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG0_SHIFT                                          0

/* RFIP_ADDR_TX_DIG_GAIN_CFG1 */
#define TX_DIG_GAIN_CFG9_MASK                                           CO_BITS(24, 29)
#define TX_DIG_GAIN_CFG9_SHIFT                                          24
#define TX_DIG_GAIN_CFG8_MASK                                           CO_BITS(18, 23)
#define TX_DIG_GAIN_CFG8_SHIFT                                          18
#define TX_DIG_GAIN_CFG7_MASK                                           CO_BITS(12, 17)
#define TX_DIG_GAIN_CFG7_SHIFT                                          12
#define TX_DIG_GAIN_CFG6_MASK                                           CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG6_SHIFT                                          6
#define TX_DIG_GAIN_CFG5_MASK                                           CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG5_SHIFT                                          0

/* RFIP_ADDR_TX_DIG_GAIN_CFG2 */
#define TX_DIG_GAIN_CFG14_MASK                                          CO_BITS(24, 29)
#define TX_DIG_GAIN_CFG14_SHIFT                                         24
#define TX_DIG_GAIN_CFG13_MASK                                          CO_BITS(18, 23)
#define TX_DIG_GAIN_CFG13_SHIFT                                         18
#define TX_DIG_GAIN_CFG12_MASK                                          CO_BITS(12, 17)
#define TX_DIG_GAIN_CFG12_SHIFT                                         12
#define TX_DIG_GAIN_CFG11_MASK                                          CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG11_SHIFT                                         6
#define TX_DIG_GAIN_CFG10_MASK                                          CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG10_SHIFT                                         0

/* RFIP_ADDR_TX_DIG_GAIN_CFG3 */
#define TX_DIG_GAIN_CFG19_MASK                                          CO_BITS(24, 29)
#define TX_DIG_GAIN_CFG19_SHIFT                                         24
#define TX_DIG_GAIN_CFG18_MASK                                          CO_BITS(18, 23)
#define TX_DIG_GAIN_CFG18_SHIFT                                         18
#define TX_DIG_GAIN_CFG17_MASK                                          CO_BITS(12, 17)
#define TX_DIG_GAIN_CFG17_SHIFT                                         12
#define TX_DIG_GAIN_CFG16_MASK                                          CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG16_SHIFT                                         6
#define TX_DIG_GAIN_CFG15_MASK                                          CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG15_SHIFT                                         0

/* RFIP_ADDR_TX_DIG_GAIN_CFG4 */
#define TX_DIG_GAIN_CFG24_MASK                                          CO_BITS(24, 29)
#define TX_DIG_GAIN_CFG24_SHIFT                                         24
#define TX_DIG_GAIN_CFG23_MASK                                          CO_BITS(18, 23)
#define TX_DIG_GAIN_CFG23_SHIFT                                         18
#define TX_DIG_GAIN_CFG22_MASK                                          CO_BITS(12, 17)
#define TX_DIG_GAIN_CFG22_SHIFT                                         12
#define TX_DIG_GAIN_CFG21_MASK                                          CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG21_SHIFT                                         6
#define TX_DIG_GAIN_CFG20_MASK                                          CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG20_SHIFT                                         0

/* RFIP_ADDR_TX_DIG_GAIN_CFG5 */
#define TX_DIG_GAIN_CFG29_MASK                                          CO_BITS(24, 29)
#define TX_DIG_GAIN_CFG29_SHIFT                                         24
#define TX_DIG_GAIN_CFG28_MASK                                          CO_BITS(18, 23)
#define TX_DIG_GAIN_CFG28_SHIFT                                         18
#define TX_DIG_GAIN_CFG27_MASK                                          CO_BITS(12, 17)
#define TX_DIG_GAIN_CFG27_SHIFT                                         12
#define TX_DIG_GAIN_CFG26_MASK                                          CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG26_SHIFT                                         6
#define TX_DIG_GAIN_CFG25_MASK                                          CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG25_SHIFT                                         0

/* RFIP_ADDR_TX_DIG_GAIN_CFG6 */
#define SCALE_FACTOR_MASK                                               CO_BITS(21, 31)
#define SCALE_FACTOR_SHIFT                                              21
#define TX_DIG_GAIN_DB_MASK                                             CO_BITS(12, 20)
#define TX_DIG_GAIN_DB_SHIFT                                            12
#define TX_DIG_GAIN_CFG31_MASK                                          CO_BITS(6, 11)
#define TX_DIG_GAIN_CFG31_SHIFT                                         6
#define TX_DIG_GAIN_CFG30_MASK                                          CO_BITS(0, 5)
#define TX_DIG_GAIN_CFG30_SHIFT                                         0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG0 */
#define TEMP_INDEX2_MASK                                                CO_BITS(20, 29)
#define TEMP_INDEX2_SHIFT                                               20
#define TEMP_INDEX1_MASK                                                CO_BITS(10, 19)
#define TEMP_INDEX1_SHIFT                                               10
#define TEMP_INDEX0_MASK                                                CO_BITS(0, 9)
#define TEMP_INDEX0_SHIFT                                               0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG1 */
#define TEMP_INDEX5_MASK                                                CO_BITS(20, 29)
#define TEMP_INDEX5_SHIFT                                               20
#define TEMP_INDEX4_MASK                                                CO_BITS(10, 19)
#define TEMP_INDEX4_SHIFT                                               10
#define TEMP_INDEX3_MASK                                                CO_BITS(0, 9)
#define TEMP_INDEX3_SHIFT                                               0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG2 */
#define TEMP_INDEX8_MASK                                                CO_BITS(20, 29)
#define TEMP_INDEX8_SHIFT                                               20
#define TEMP_INDEX7_MASK                                                CO_BITS(10, 19)
#define TEMP_INDEX7_SHIFT                                               10
#define TEMP_INDEX6_MASK                                                CO_BITS(0, 9)
#define TEMP_INDEX6_SHIFT                                               0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG3 */
#define TEMP_INDEX11_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX11_SHIFT                                              20
#define TEMP_INDEX10_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX10_SHIFT                                              10
#define TEMP_INDEX9_MASK                                                CO_BITS(0, 9)
#define TEMP_INDEX9_SHIFT                                               0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG4 */
#define TEMP_INDEX14_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX14_SHIFT                                              20
#define TEMP_INDEX13_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX13_SHIFT                                              10
#define TEMP_INDEX12_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX12_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG5 */
#define TEMP_INDEX17_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX17_SHIFT                                              20
#define TEMP_INDEX16_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX16_SHIFT                                              10
#define TEMP_INDEX15_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX15_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG6 */
#define TEMP_INDEX20_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX20_SHIFT                                              20
#define TEMP_INDEX19_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX19_SHIFT                                              10
#define TEMP_INDEX18_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX18_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG7 */
#define TEMP_INDEX23_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX23_SHIFT                                              20
#define TEMP_INDEX22_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX22_SHIFT                                              10
#define TEMP_INDEX21_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX21_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG8 */
#define TEMP_INDEX26_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX26_SHIFT                                              20
#define TEMP_INDEX25_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX25_SHIFT                                              10
#define TEMP_INDEX24_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX24_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG9 */
#define TEMP_INDEX29_MASK                                               CO_BITS(20, 29)
#define TEMP_INDEX29_SHIFT                                              20
#define TEMP_INDEX28_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX28_SHIFT                                              10
#define TEMP_INDEX27_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX27_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_INDEX_CFG10 */
#define TEMP_INDEX31_MASK                                               CO_BITS(10, 19)
#define TEMP_INDEX31_SHIFT                                              10
#define TEMP_INDEX30_MASK                                               CO_BITS(0, 9)
#define TEMP_INDEX30_SHIFT                                              0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG0 */
#define TEMP_COMP_INDEX_CFG4_MASK                                       CO_BITS(24, 29)
#define TEMP_COMP_INDEX_CFG4_SHIFT                                      24
#define TEMP_COMP_INDEX_CFG3_MASK                                       CO_BITS(18, 23)
#define TEMP_COMP_INDEX_CFG3_SHIFT                                      18
#define TEMP_COMP_INDEX_CFG2_MASK                                       CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG2_SHIFT                                      12
#define TEMP_COMP_INDEX_CFG1_MASK                                       CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG1_SHIFT                                      6
#define TEMP_COMP_INDEX_CFG0_MASK                                       CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG0_SHIFT                                      0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG1 */
#define TEMP_COMP_INDEX_CFG9_MASK                                       CO_BITS(24, 29)
#define TEMP_COMP_INDEX_CFG9_SHIFT                                      24
#define TEMP_COMP_INDEX_CFG8_MASK                                       CO_BITS(18, 23)
#define TEMP_COMP_INDEX_CFG8_SHIFT                                      18
#define TEMP_COMP_INDEX_CFG7_MASK                                       CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG7_SHIFT                                      12
#define TEMP_COMP_INDEX_CFG6_MASK                                       CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG6_SHIFT                                      6
#define TEMP_COMP_INDEX_CFG5_MASK                                       CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG5_SHIFT                                      0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG2 */
#define TEMP_COMP_INDEX_CFG14_MASK                                      CO_BITS(24, 29)
#define TEMP_COMP_INDEX_CFG14_SHIFT                                     24
#define TEMP_COMP_INDEX_CFG13_MASK                                      CO_BITS(18, 23)
#define TEMP_COMP_INDEX_CFG13_SHIFT                                     18
#define TEMP_COMP_INDEX_CFG12_MASK                                      CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG12_SHIFT                                     12
#define TEMP_COMP_INDEX_CFG11_MASK                                      CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG11_SHIFT                                     6
#define TEMP_COMP_INDEX_CFG10_MASK                                      CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG10_SHIFT                                     0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG3 */
#define TEMP_COMP_INDEX_CFG19_MASK                                      CO_BITS(24, 29)
#define TEMP_COMP_INDEX_CFG19_SHIFT                                     24
#define TEMP_COMP_INDEX_CFG18_MASK                                      CO_BITS(18, 23)
#define TEMP_COMP_INDEX_CFG18_SHIFT                                     18
#define TEMP_COMP_INDEX_CFG17_MASK                                      CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG17_SHIFT                                     12
#define TEMP_COMP_INDEX_CFG16_MASK                                      CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG16_SHIFT                                     6
#define TEMP_COMP_INDEX_CFG15_MASK                                      CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG15_SHIFT                                     0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG4 */
#define TEMP_COMP_INDEX_CFG24_MASK                                      CO_BITS(24, 29)
#define TEMP_COMP_INDEX_CFG24_SHIFT                                     24
#define TEMP_COMP_INDEX_CFG23_MASK                                      CO_BITS(18, 23)
#define TEMP_COMP_INDEX_CFG23_SHIFT                                     18
#define TEMP_COMP_INDEX_CFG22_MASK                                      CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG22_SHIFT                                     12
#define TEMP_COMP_INDEX_CFG21_MASK                                      CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG21_SHIFT                                     6
#define TEMP_COMP_INDEX_CFG20_MASK                                      CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG20_SHIFT                                     0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG5 */
#define TEMP_COMP_INDEX_CFG29_MASK                                      CO_BITS(24, 29)
#define TEMP_COMP_INDEX_CFG29_SHIFT                                     24
#define TEMP_COMP_INDEX_CFG28_MASK                                      CO_BITS(18, 23)
#define TEMP_COMP_INDEX_CFG28_SHIFT                                     18
#define TEMP_COMP_INDEX_CFG27_MASK                                      CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG27_SHIFT                                     12
#define TEMP_COMP_INDEX_CFG26_MASK                                      CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG26_SHIFT                                     6
#define TEMP_COMP_INDEX_CFG25_MASK                                      CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG25_SHIFT                                     0

/* RFIP_ADDR_TX_TEMP_COMP_INDEX_CFG6 */
#define TEMP_COMP_INDEX_CFG32_MASK                                      CO_BITS(12, 17)
#define TEMP_COMP_INDEX_CFG32_SHIFT                                     12
#define TEMP_COMP_INDEX_CFG31_MASK                                      CO_BITS(6, 11)
#define TEMP_COMP_INDEX_CFG31_SHIFT                                     6
#define TEMP_COMP_INDEX_CFG30_MASK                                      CO_BITS(0, 5)
#define TEMP_COMP_INDEX_CFG30_SHIFT                                     0

/* RFIP_ADDR_TPC_COMP_LSB_CFG0 */
#define TPC_COMP_LSB_CFG4_MASK                                          CO_BITS(24, 29)
#define TPC_COMP_LSB_CFG4_SHIFT                                         24
#define TPC_COMP_LSB_CFG3_MASK                                          CO_BITS(18, 23)
#define TPC_COMP_LSB_CFG3_SHIFT                                         18
#define TPC_COMP_LSB_CFG2_MASK                                          CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG2_SHIFT                                         12
#define TPC_COMP_LSB_CFG1_MASK                                          CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG1_SHIFT                                         6
#define TPC_COMP_LSB_CFG0_MASK                                          CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG0_SHIFT                                         0

/* RFIP_ADDR_TPC_COMP_LSB_CFG1 */
#define TPC_COMP_LSB_CFG9_MASK                                          CO_BITS(24, 29)
#define TPC_COMP_LSB_CFG9_SHIFT                                         24
#define TPC_COMP_LSB_CFG8_MASK                                          CO_BITS(18, 23)
#define TPC_COMP_LSB_CFG8_SHIFT                                         18
#define TPC_COMP_LSB_CFG7_MASK                                          CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG7_SHIFT                                         12
#define TPC_COMP_LSB_CFG6_MASK                                          CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG6_SHIFT                                         6
#define TPC_COMP_LSB_CFG5_MASK                                          CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG5_SHIFT                                         0

/* RFIP_ADDR_TPC_COMP_LSB_CFG2 */
#define TPC_COMP_LSB_CFG14_MASK                                         CO_BITS(24, 29)
#define TPC_COMP_LSB_CFG14_SHIFT                                        24
#define TPC_COMP_LSB_CFG13_MASK                                         CO_BITS(18, 23)
#define TPC_COMP_LSB_CFG13_SHIFT                                        18
#define TPC_COMP_LSB_CFG12_MASK                                         CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG12_SHIFT                                        12
#define TPC_COMP_LSB_CFG11_MASK                                         CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG11_SHIFT                                        6
#define TPC_COMP_LSB_CFG10_MASK                                         CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG10_SHIFT                                        0

/* RFIP_ADDR_TPC_COMP_LSB_CFG3 */
#define TPC_COMP_LSB_CFG19_MASK                                         CO_BITS(24, 29)
#define TPC_COMP_LSB_CFG19_SHIFT                                        24
#define TPC_COMP_LSB_CFG18_MASK                                         CO_BITS(18, 23)
#define TPC_COMP_LSB_CFG18_SHIFT                                        18
#define TPC_COMP_LSB_CFG17_MASK                                         CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG17_SHIFT                                        12
#define TPC_COMP_LSB_CFG16_MASK                                         CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG16_SHIFT                                        6
#define TPC_COMP_LSB_CFG15_MASK                                         CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG15_SHIFT                                        0

/* RFIP_ADDR_TPC_COMP_LSB_CFG4 */
#define TPC_COMP_LSB_CFG24_MASK                                         CO_BITS(24, 29)
#define TPC_COMP_LSB_CFG24_SHIFT                                        24
#define TPC_COMP_LSB_CFG23_MASK                                         CO_BITS(18, 23)
#define TPC_COMP_LSB_CFG23_SHIFT                                        18
#define TPC_COMP_LSB_CFG22_MASK                                         CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG22_SHIFT                                        12
#define TPC_COMP_LSB_CFG21_MASK                                         CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG21_SHIFT                                        6
#define TPC_COMP_LSB_CFG20_MASK                                         CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG20_SHIFT                                        0

/* RFIP_ADDR_TPC_COMP_LSB_CFG5 */
#define TPC_COMP_LSB_CFG29_MASK                                         CO_BITS(24, 29)
#define TPC_COMP_LSB_CFG29_SHIFT                                        24
#define TPC_COMP_LSB_CFG28_MASK                                         CO_BITS(18, 23)
#define TPC_COMP_LSB_CFG28_SHIFT                                        18
#define TPC_COMP_LSB_CFG27_MASK                                         CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG27_SHIFT                                        12
#define TPC_COMP_LSB_CFG26_MASK                                         CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG26_SHIFT                                        6
#define TPC_COMP_LSB_CFG25_MASK                                         CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG25_SHIFT                                        0

/* RFIP_ADDR_TPC_COMP_LSB_CFG6 */
#define TPC_COMP_LSB_CFG32_MASK                                         CO_BITS(12, 17)
#define TPC_COMP_LSB_CFG32_SHIFT                                        12
#define TPC_COMP_LSB_CFG31_MASK                                         CO_BITS(6, 11)
#define TPC_COMP_LSB_CFG31_SHIFT                                        6
#define TPC_COMP_LSB_CFG30_MASK                                         CO_BITS(0, 5)
#define TPC_COMP_LSB_CFG30_SHIFT                                        0

/* RFIP_ADDR_LOFT_COMP_I_CFG0 */
#define LOFT_COMP_I_CFG3_MASK                                           CO_BITS(18, 23)
#define LOFT_COMP_I_CFG3_SHIFT                                          18
#define LOFT_COMP_I_CFG2_MASK                                           CO_BITS(12, 17)
#define LOFT_COMP_I_CFG2_SHIFT                                          12
#define LOFT_COMP_I_CFG1_MASK                                           CO_BITS(6, 11)
#define LOFT_COMP_I_CFG1_SHIFT                                          6
#define LOFT_COMP_I_CFG0_MASK                                           CO_BITS(0, 5)
#define LOFT_COMP_I_CFG0_SHIFT                                          0

/* RFIP_ADDR_LOFT_COMP_I_CFG1 */
#define LOFT_COMP_I_CFG7_MASK                                           CO_BITS(18, 23)
#define LOFT_COMP_I_CFG7_SHIFT                                          18
#define LOFT_COMP_I_CFG6_MASK                                           CO_BITS(12, 17)
#define LOFT_COMP_I_CFG6_SHIFT                                          12
#define LOFT_COMP_I_CFG5_MASK                                           CO_BITS(6, 11)
#define LOFT_COMP_I_CFG5_SHIFT                                          6
#define LOFT_COMP_I_CFG4_MASK                                           CO_BITS(0, 5)
#define LOFT_COMP_I_CFG4_SHIFT                                          0

/* RFIP_ADDR_LOFT_COMP_I_CFG2 */
#define LOFT_COMP_I_CFG11_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_I_CFG11_SHIFT                                         18
#define LOFT_COMP_I_CFG10_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_I_CFG10_SHIFT                                         12
#define LOFT_COMP_I_CFG9_MASK                                           CO_BITS(6, 11)
#define LOFT_COMP_I_CFG9_SHIFT                                          6
#define LOFT_COMP_I_CFG8_MASK                                           CO_BITS(0, 5)
#define LOFT_COMP_I_CFG8_SHIFT                                          0

/* RFIP_ADDR_LOFT_COMP_I_CFG3 */
#define LOFT_COMP_I_CFG15_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_I_CFG15_SHIFT                                         18
#define LOFT_COMP_I_CFG14_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_I_CFG14_SHIFT                                         12
#define LOFT_COMP_I_CFG13_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_I_CFG13_SHIFT                                         6
#define LOFT_COMP_I_CFG12_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_I_CFG12_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_I_CFG4 */
#define LOFT_COMP_I_CFG19_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_I_CFG19_SHIFT                                         18
#define LOFT_COMP_I_CFG18_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_I_CFG18_SHIFT                                         12
#define LOFT_COMP_I_CFG17_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_I_CFG17_SHIFT                                         6
#define LOFT_COMP_I_CFG16_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_I_CFG16_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_I_CFG5 */
#define LOFT_COMP_I_CFG23_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_I_CFG23_SHIFT                                         18
#define LOFT_COMP_I_CFG22_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_I_CFG22_SHIFT                                         12
#define LOFT_COMP_I_CFG21_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_I_CFG21_SHIFT                                         6
#define LOFT_COMP_I_CFG20_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_I_CFG20_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_I_CFG6 */
#define LOFT_COMP_I_CFG27_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_I_CFG27_SHIFT                                         18
#define LOFT_COMP_I_CFG26_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_I_CFG26_SHIFT                                         12
#define LOFT_COMP_I_CFG25_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_I_CFG25_SHIFT                                         6
#define LOFT_COMP_I_CFG24_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_I_CFG24_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_I_CFG7 */
#define LOFT_COMP_I_CFG31_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_I_CFG31_SHIFT                                         18
#define LOFT_COMP_I_CFG30_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_I_CFG30_SHIFT                                         12
#define LOFT_COMP_I_CFG29_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_I_CFG29_SHIFT                                         6
#define LOFT_COMP_I_CFG28_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_I_CFG28_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_Q_CFG0 */
#define LOFT_COMP_Q_CFG3_MASK                                           CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG3_SHIFT                                          18
#define LOFT_COMP_Q_CFG2_MASK                                           CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG2_SHIFT                                          12
#define LOFT_COMP_Q_CFG1_MASK                                           CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG1_SHIFT                                          6
#define LOFT_COMP_Q_CFG0_MASK                                           CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG0_SHIFT                                          0

/* RFIP_ADDR_LOFT_COMP_Q_CFG1 */
#define LOFT_COMP_Q_CFG7_MASK                                           CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG7_SHIFT                                          18
#define LOFT_COMP_Q_CFG6_MASK                                           CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG6_SHIFT                                          12
#define LOFT_COMP_Q_CFG5_MASK                                           CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG5_SHIFT                                          6
#define LOFT_COMP_Q_CFG4_MASK                                           CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG4_SHIFT                                          0

/* RFIP_ADDR_LOFT_COMP_Q_CFG2 */
#define LOFT_COMP_Q_CFG11_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG11_SHIFT                                         18
#define LOFT_COMP_Q_CFG10_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG10_SHIFT                                         12
#define LOFT_COMP_Q_CFG9_MASK                                           CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG9_SHIFT                                          6
#define LOFT_COMP_Q_CFG8_MASK                                           CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG8_SHIFT                                          0

/* RFIP_ADDR_LOFT_COMP_Q_CFG3 */
#define LOFT_COMP_Q_CFG15_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG15_SHIFT                                         18
#define LOFT_COMP_Q_CFG14_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG14_SHIFT                                         12
#define LOFT_COMP_Q_CFG13_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG13_SHIFT                                         6
#define LOFT_COMP_Q_CFG12_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG12_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_Q_CFG4 */
#define LOFT_COMP_Q_CFG19_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG19_SHIFT                                         18
#define LOFT_COMP_Q_CFG18_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG18_SHIFT                                         12
#define LOFT_COMP_Q_CFG17_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG17_SHIFT                                         6
#define LOFT_COMP_Q_CFG16_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG16_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_Q_CFG5 */
#define LOFT_COMP_Q_CFG23_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG23_SHIFT                                         18
#define LOFT_COMP_Q_CFG22_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG22_SHIFT                                         12
#define LOFT_COMP_Q_CFG21_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG21_SHIFT                                         6
#define LOFT_COMP_Q_CFG20_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG20_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_Q_CFG6 */
#define LOFT_COMP_Q_CFG27_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG27_SHIFT                                         18
#define LOFT_COMP_Q_CFG26_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG26_SHIFT                                         12
#define LOFT_COMP_Q_CFG25_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG25_SHIFT                                         6
#define LOFT_COMP_Q_CFG24_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG24_SHIFT                                         0

/* RFIP_ADDR_LOFT_COMP_Q_CFG7 */
#define LOFT_COMP_Q_CFG31_MASK                                          CO_BITS(18, 23)
#define LOFT_COMP_Q_CFG31_SHIFT                                         18
#define LOFT_COMP_Q_CFG30_MASK                                          CO_BITS(12, 17)
#define LOFT_COMP_Q_CFG30_SHIFT                                         12
#define LOFT_COMP_Q_CFG29_MASK                                          CO_BITS(6, 11)
#define LOFT_COMP_Q_CFG29_SHIFT                                         6
#define LOFT_COMP_Q_CFG28_MASK                                          CO_BITS(0, 5)
#define LOFT_COMP_Q_CFG28_SHIFT                                         0

/* RFIP_ADDR_LOFT_SFT_CFG_I_CFG0 */
#define LOFT_COMP_SFT_CFG_I_CFG15_MASK                                  CO_BITS(30, 31)
#define LOFT_COMP_SFT_CFG_I_CFG15_SHIFT                                 30
#define LOFT_COMP_SFT_CFG_I_CFG14_MASK                                  CO_BITS(28, 29)
#define LOFT_COMP_SFT_CFG_I_CFG14_SHIFT                                 28
#define LOFT_COMP_SFT_CFG_I_CFG13_MASK                                  CO_BITS(26, 27)
#define LOFT_COMP_SFT_CFG_I_CFG13_SHIFT                                 26
#define LOFT_COMP_SFT_CFG_I_CFG12_MASK                                  CO_BITS(24, 25)
#define LOFT_COMP_SFT_CFG_I_CFG12_SHIFT                                 24
#define LOFT_COMP_SFT_CFG_I_CFG11_MASK                                  CO_BITS(22, 23)
#define LOFT_COMP_SFT_CFG_I_CFG11_SHIFT                                 22
#define LOFT_COMP_SFT_CFG_I_CFG10_MASK                                  CO_BITS(20, 21)
#define LOFT_COMP_SFT_CFG_I_CFG10_SHIFT                                 20
#define LOFT_COMP_SFT_CFG_I_CFG9_MASK                                   CO_BITS(18, 19)
#define LOFT_COMP_SFT_CFG_I_CFG9_SHIFT                                  18
#define LOFT_COMP_SFT_CFG_I_CFG8_MASK                                   CO_BITS(16, 17)
#define LOFT_COMP_SFT_CFG_I_CFG8_SHIFT                                  16
#define LOFT_COMP_SFT_CFG_I_CFG7_MASK                                   CO_BITS(14, 15)
#define LOFT_COMP_SFT_CFG_I_CFG7_SHIFT                                  14
#define LOFT_COMP_SFT_CFG_I_CFG6_MASK                                   CO_BITS(12, 13)
#define LOFT_COMP_SFT_CFG_I_CFG6_SHIFT                                  12
#define LOFT_COMP_SFT_CFG_I_CFG5_MASK                                   CO_BITS(10, 11)
#define LOFT_COMP_SFT_CFG_I_CFG5_SHIFT                                  10
#define LOFT_COMP_SFT_CFG_I_CFG4_MASK                                   CO_BITS(8, 9)
#define LOFT_COMP_SFT_CFG_I_CFG4_SHIFT                                  8
#define LOFT_COMP_SFT_CFG_I_CFG3_MASK                                   CO_BITS(6, 7)
#define LOFT_COMP_SFT_CFG_I_CFG3_SHIFT                                  6
#define LOFT_COMP_SFT_CFG_I_CFG2_MASK                                   CO_BITS(4, 5)
#define LOFT_COMP_SFT_CFG_I_CFG2_SHIFT                                  4
#define LOFT_COMP_SFT_CFG_I_CFG1_MASK                                   CO_BITS(2, 3)
#define LOFT_COMP_SFT_CFG_I_CFG1_SHIFT                                  2
#define LOFT_COMP_SFT_CFG_I_CFG0_MASK                                   CO_BITS(0, 1)
#define LOFT_COMP_SFT_CFG_I_CFG0_SHIFT                                  0

/* RFIP_ADDR_LOFT_SFT_CFG_I_CFG1 */
#define LOFT_COMP_SFT_CFG_I_CFG31_MASK                                  CO_BITS(30, 31)
#define LOFT_COMP_SFT_CFG_I_CFG31_SHIFT                                 30
#define LOFT_COMP_SFT_CFG_I_CFG30_MASK                                  CO_BITS(28, 29)
#define LOFT_COMP_SFT_CFG_I_CFG30_SHIFT                                 28
#define LOFT_COMP_SFT_CFG_I_CFG29_MASK                                  CO_BITS(26, 27)
#define LOFT_COMP_SFT_CFG_I_CFG29_SHIFT                                 26
#define LOFT_COMP_SFT_CFG_I_CFG28_MASK                                  CO_BITS(24, 25)
#define LOFT_COMP_SFT_CFG_I_CFG28_SHIFT                                 24
#define LOFT_COMP_SFT_CFG_I_CFG27_MASK                                  CO_BITS(22, 23)
#define LOFT_COMP_SFT_CFG_I_CFG27_SHIFT                                 22
#define LOFT_COMP_SFT_CFG_I_CFG26_MASK                                  CO_BITS(20, 21)
#define LOFT_COMP_SFT_CFG_I_CFG26_SHIFT                                 20
#define LOFT_COMP_SFT_CFG_I_CFG25_MASK                                  CO_BITS(18, 19)
#define LOFT_COMP_SFT_CFG_I_CFG25_SHIFT                                 18
#define LOFT_COMP_SFT_CFG_I_CFG24_MASK                                  CO_BITS(16, 17)
#define LOFT_COMP_SFT_CFG_I_CFG24_SHIFT                                 16
#define LOFT_COMP_SFT_CFG_I_CFG23_MASK                                  CO_BITS(14, 15)
#define LOFT_COMP_SFT_CFG_I_CFG23_SHIFT                                 14
#define LOFT_COMP_SFT_CFG_I_CFG22_MASK                                  CO_BITS(12, 13)
#define LOFT_COMP_SFT_CFG_I_CFG22_SHIFT                                 12
#define LOFT_COMP_SFT_CFG_I_CFG21_MASK                                  CO_BITS(10, 11)
#define LOFT_COMP_SFT_CFG_I_CFG21_SHIFT                                 10
#define LOFT_COMP_SFT_CFG_I_CFG20_MASK                                  CO_BITS(8, 9)
#define LOFT_COMP_SFT_CFG_I_CFG20_SHIFT                                 8
#define LOFT_COMP_SFT_CFG_I_CFG19_MASK                                  CO_BITS(6, 7)
#define LOFT_COMP_SFT_CFG_I_CFG19_SHIFT                                 6
#define LOFT_COMP_SFT_CFG_I_CFG18_MASK                                  CO_BITS(4, 5)
#define LOFT_COMP_SFT_CFG_I_CFG18_SHIFT                                 4
#define LOFT_COMP_SFT_CFG_I_CFG17_MASK                                  CO_BITS(2, 3)
#define LOFT_COMP_SFT_CFG_I_CFG17_SHIFT                                 2
#define LOFT_COMP_SFT_CFG_I_CFG16_MASK                                  CO_BITS(0, 1)
#define LOFT_COMP_SFT_CFG_I_CFG16_SHIFT                                 0

/* RFIP_ADDR_LOFT_SFT_CFG_Q_CFG0 */
#define LOFT_COMP_SFT_CFG_Q_CFG15_MASK                                  CO_BITS(30, 31)
#define LOFT_COMP_SFT_CFG_Q_CFG15_SHIFT                                 30
#define LOFT_COMP_SFT_CFG_Q_CFG14_MASK                                  CO_BITS(28, 29)
#define LOFT_COMP_SFT_CFG_Q_CFG14_SHIFT                                 28
#define LOFT_COMP_SFT_CFG_Q_CFG13_MASK                                  CO_BITS(26, 27)
#define LOFT_COMP_SFT_CFG_Q_CFG13_SHIFT                                 26
#define LOFT_COMP_SFT_CFG_Q_CFG12_MASK                                  CO_BITS(24, 25)
#define LOFT_COMP_SFT_CFG_Q_CFG12_SHIFT                                 24
#define LOFT_COMP_SFT_CFG_Q_CFG11_MASK                                  CO_BITS(22, 23)
#define LOFT_COMP_SFT_CFG_Q_CFG11_SHIFT                                 22
#define LOFT_COMP_SFT_CFG_Q_CFG10_MASK                                  CO_BITS(20, 21)
#define LOFT_COMP_SFT_CFG_Q_CFG10_SHIFT                                 20
#define LOFT_COMP_SFT_CFG_Q_CFG9_MASK                                   CO_BITS(18, 19)
#define LOFT_COMP_SFT_CFG_Q_CFG9_SHIFT                                  18
#define LOFT_COMP_SFT_CFG_Q_CFG8_MASK                                   CO_BITS(16, 17)
#define LOFT_COMP_SFT_CFG_Q_CFG8_SHIFT                                  16
#define LOFT_COMP_SFT_CFG_Q_CFG7_MASK                                   CO_BITS(14, 15)
#define LOFT_COMP_SFT_CFG_Q_CFG7_SHIFT                                  14
#define LOFT_COMP_SFT_CFG_Q_CFG6_MASK                                   CO_BITS(12, 13)
#define LOFT_COMP_SFT_CFG_Q_CFG6_SHIFT                                  12
#define LOFT_COMP_SFT_CFG_Q_CFG5_MASK                                   CO_BITS(10, 11)
#define LOFT_COMP_SFT_CFG_Q_CFG5_SHIFT                                  10
#define LOFT_COMP_SFT_CFG_Q_CFG4_MASK                                   CO_BITS(8, 9)
#define LOFT_COMP_SFT_CFG_Q_CFG4_SHIFT                                  8
#define LOFT_COMP_SFT_CFG_Q_CFG3_MASK                                   CO_BITS(6, 7)
#define LOFT_COMP_SFT_CFG_Q_CFG3_SHIFT                                  6
#define LOFT_COMP_SFT_CFG_Q_CFG2_MASK                                   CO_BITS(4, 5)
#define LOFT_COMP_SFT_CFG_Q_CFG2_SHIFT                                  4
#define LOFT_COMP_SFT_CFG_Q_CFG1_MASK                                   CO_BITS(2, 3)
#define LOFT_COMP_SFT_CFG_Q_CFG1_SHIFT                                  2
#define LOFT_COMP_SFT_CFG_Q_CFG0_MASK                                   CO_BITS(0, 1)
#define LOFT_COMP_SFT_CFG_Q_CFG0_SHIFT                                  0

/* RFIP_ADDR_LOFT_SFT_CFG_Q_CFG1 */
#define LOFT_COMP_SFT_CFG_Q_CFG31_MASK                                  CO_BITS(30, 31)
#define LOFT_COMP_SFT_CFG_Q_CFG31_SHIFT                                 30
#define LOFT_COMP_SFT_CFG_Q_CFG30_MASK                                  CO_BITS(28, 29)
#define LOFT_COMP_SFT_CFG_Q_CFG30_SHIFT                                 28
#define LOFT_COMP_SFT_CFG_Q_CFG29_MASK                                  CO_BITS(26, 27)
#define LOFT_COMP_SFT_CFG_Q_CFG29_SHIFT                                 26
#define LOFT_COMP_SFT_CFG_Q_CFG28_MASK                                  CO_BITS(24, 25)
#define LOFT_COMP_SFT_CFG_Q_CFG28_SHIFT                                 24
#define LOFT_COMP_SFT_CFG_Q_CFG27_MASK                                  CO_BITS(22, 23)
#define LOFT_COMP_SFT_CFG_Q_CFG27_SHIFT                                 22
#define LOFT_COMP_SFT_CFG_Q_CFG26_MASK                                  CO_BITS(20, 21)
#define LOFT_COMP_SFT_CFG_Q_CFG26_SHIFT                                 20
#define LOFT_COMP_SFT_CFG_Q_CFG25_MASK                                  CO_BITS(18, 19)
#define LOFT_COMP_SFT_CFG_Q_CFG25_SHIFT                                 18
#define LOFT_COMP_SFT_CFG_Q_CFG24_MASK                                  CO_BITS(16, 17)
#define LOFT_COMP_SFT_CFG_Q_CFG24_SHIFT                                 16
#define LOFT_COMP_SFT_CFG_Q_CFG23_MASK                                  CO_BITS(14, 15)
#define LOFT_COMP_SFT_CFG_Q_CFG23_SHIFT                                 14
#define LOFT_COMP_SFT_CFG_Q_CFG22_MASK                                  CO_BITS(12, 13)
#define LOFT_COMP_SFT_CFG_Q_CFG22_SHIFT                                 12
#define LOFT_COMP_SFT_CFG_Q_CFG21_MASK                                  CO_BITS(10, 11)
#define LOFT_COMP_SFT_CFG_Q_CFG21_SHIFT                                 10
#define LOFT_COMP_SFT_CFG_Q_CFG20_MASK                                  CO_BITS(8, 9)
#define LOFT_COMP_SFT_CFG_Q_CFG20_SHIFT                                 8
#define LOFT_COMP_SFT_CFG_Q_CFG19_MASK                                  CO_BITS(6, 7)
#define LOFT_COMP_SFT_CFG_Q_CFG19_SHIFT                                 6
#define LOFT_COMP_SFT_CFG_Q_CFG18_MASK                                  CO_BITS(4, 5)
#define LOFT_COMP_SFT_CFG_Q_CFG18_SHIFT                                 4
#define LOFT_COMP_SFT_CFG_Q_CFG17_MASK                                  CO_BITS(2, 3)
#define LOFT_COMP_SFT_CFG_Q_CFG17_SHIFT                                 2
#define LOFT_COMP_SFT_CFG_Q_CFG16_MASK                                  CO_BITS(0, 1)
#define LOFT_COMP_SFT_CFG_Q_CFG16_SHIFT                                 0

/* RFIP_ADDR_TX_DC_OFFSET_CFG */
#define TX_DC_OFFSET_Q_CFG_MASK                                         CO_BITS(7, 13)
#define TX_DC_OFFSET_Q_CFG_SHIFT                                        7
#define TX_DC_OFFSET_I_CFG_MASK                                         CO_BITS(0, 6)
#define TX_DC_OFFSET_I_CFG_SHIFT                                        0

/* RFIP_ADDR_TX_CTRL_CFG */
#define CLIP_FACTOR_CFG_MASK                                            CO_BITS(20, 31)
#define CLIP_FACTOR_CFG_SHIFT                                           20
#define TPC_CAL_OFFSET_CFG_MASK                                         CO_BITS(14, 19)
#define TPC_CAL_OFFSET_CFG_SHIFT                                        14
#define TPC_CAL_LSB_CFG_MASK                                            CO_BITS(8, 13)
#define TPC_CAL_LSB_CFG_SHIFT                                           8
#define UP_CONVERTER_SEL_X4_CFG                                         BIT7
#define RX2TX_TEST_EN_CFG                                               BIT6
#define TX_RX_BRIDGE_RST_CFG                                            BIT5
#define TX_RF_IF_SWRST_CFG                                              BIT4
#define TX_UPCONVERT_BYPASS_N                                           BIT3
#define TX_SRAM_TEST_EN                                                 BIT0

/* RFIP_ADDR_TX_SRAM_TEST_CFG */
#define TX_SRAM_TEST_FLUSH                                              BIT31

/* RFIP_ADDR_TX_SRAM_TEST_DR */
#define TX_SRAM_Q_DIN_MASK                                              CO_BITS(12, 23)
#define TX_SRAM_Q_DIN_SHIFT                                             12
#define TX_SRAM_I_DIN_MASK                                              CO_BITS(0, 11)
#define TX_SRAM_I_DIN_SHIFT                                             0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG0 */
#define IN11N_CBW20_TX_COEFF2_MASK                                      CO_BITS(20, 29)
#define IN11N_CBW20_TX_COEFF2_SHIFT                                     20
#define IN11N_CBW20_TX_COEFF1_MASK                                      CO_BITS(10, 19)
#define IN11N_CBW20_TX_COEFF1_SHIFT                                     10
#define IN11N_CBW20_TX_COEFF0_MASK                                      CO_BITS(0, 9)
#define IN11N_CBW20_TX_COEFF0_SHIFT                                     0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG1 */
#define IN11N_CBW20_TX_COEFF5_MASK                                      CO_BITS(20, 29)
#define IN11N_CBW20_TX_COEFF5_SHIFT                                     20
#define IN11N_CBW20_TX_COEFF4_MASK                                      CO_BITS(10, 19)
#define IN11N_CBW20_TX_COEFF4_SHIFT                                     10
#define IN11N_CBW20_TX_COEFF3_MASK                                      CO_BITS(0, 9)
#define IN11N_CBW20_TX_COEFF3_SHIFT                                     0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG2 */
#define IN11N_CBW20_TX_COEFF8_MASK                                      CO_BITS(20, 29)
#define IN11N_CBW20_TX_COEFF8_SHIFT                                     20
#define IN11N_CBW20_TX_COEFF7_MASK                                      CO_BITS(10, 19)
#define IN11N_CBW20_TX_COEFF7_SHIFT                                     10
#define IN11N_CBW20_TX_COEFF6_MASK                                      CO_BITS(0, 9)
#define IN11N_CBW20_TX_COEFF6_SHIFT                                     0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG3 */
#define IN11N_CBW20_TX_COEFF10_MASK                                     CO_BITS(10, 19)
#define IN11N_CBW20_TX_COEFF10_SHIFT                                    10
#define IN11N_CBW20_TX_COEFF9_MASK                                      CO_BITS(0, 9)
#define IN11N_CBW20_TX_COEFF9_SHIFT                                     0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG4 */
#define IN11B_TX_COEFF2_MASK                                            CO_BITS(20, 29)
#define IN11B_TX_COEFF2_SHIFT                                           20
#define IN11B_TX_COEFF1_MASK                                            CO_BITS(10, 19)
#define IN11B_TX_COEFF1_SHIFT                                           10
#define IN11B_TX_COEFF0_MASK                                            CO_BITS(0, 9)
#define IN11B_TX_COEFF0_SHIFT                                           0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG5 */
#define IN11B_TX_COEFF5_MASK                                            CO_BITS(20, 29)
#define IN11B_TX_COEFF5_SHIFT                                           20
#define IN11B_TX_COEFF4_MASK                                            CO_BITS(10, 19)
#define IN11B_TX_COEFF4_SHIFT                                           10
#define IN11B_TX_COEFF3_MASK                                            CO_BITS(0, 9)
#define IN11B_TX_COEFF3_SHIFT                                           0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG6 */
#define IN11B_TX_COEFF8_MASK                                            CO_BITS(20, 29)
#define IN11B_TX_COEFF8_SHIFT                                           20
#define IN11B_TX_COEFF7_MASK                                            CO_BITS(10, 19)
#define IN11B_TX_COEFF7_SHIFT                                           10
#define IN11B_TX_COEFF6_MASK                                            CO_BITS(0, 9)
#define IN11B_TX_COEFF6_SHIFT                                           0

/* RFIP_ADDR_TX_UPCONVER_COEFF_CFG7 */
#define IN11B_TX_COEFF10_MASK                                           CO_BITS(10, 19)
#define IN11B_TX_COEFF10_SHIFT                                          10
#define IN11B_TX_COEFF9_MASK                                            CO_BITS(0, 9)
#define IN11B_TX_COEFF9_SHIFT                                           0

/* RFIP_ADDR_GPIO_MODE_CFG0 */
#define RXEN_MODE_CFG                                                   BIT31
#define RXIF_EN_MODE_CFG                                                BIT30
#define RX_GAIN0_MODE_CFG                                               BIT29
#define RX_GAIN1_MODE_CFG                                               BIT28
#define RX_GAIN2_MODE_CFG                                               BIT27
#define RX_GAIN3_MODE_CFG                                               BIT26
#define RX_GAIN4_MODE_CFG                                               BIT25
#define RX_GAIN5_MODE_CFG                                               BIT24
#define RX_GAIN6_MODE_CFG                                               BIT23
#define RX_GAIN7_MODE_CFG                                               BIT22
#define RFPKDET0_MODE_CFG                                               BIT21
#define RFPKDET1_MODE_CFG                                               BIT20
#define RXDATA0_MODE_CFG                                                BIT19
#define RXDATA1_MODE_CFG                                                BIT18
#define RXDATA2_MODE_CFG                                                BIT17
#define RXDATA3_MODE_CFG                                                BIT16
#define RXDATA4_MODE_CFG                                                BIT15
#define RXDATA5_MODE_CFG                                                BIT14
#define RXDATA6_MODE_CFG                                                BIT13
#define RXDATA7_MODE_CFG                                                BIT12
#define RXDATA8_MODE_CFG                                                BIT11
#define RXDATA9_MODE_CFG                                                BIT10
#define RXDATA10_MODE_CFG                                               BIT9
#define RXDATA11_MODE_CFG                                               BIT8
#define RXCLK_MODE_CFG                                                  BIT7

/* RFIP_ADDR_GPIO_MODE_CFG1 */
#define TXEN_MODE_CFG                                                   BIT31
#define TXIF_EN_MODE_CFG                                                BIT30
#define TXDATA0_MODE_CFG                                                BIT29
#define TXDATA1_MODE_CFG                                                BIT28
#define TXDATA2_MODE_CFG                                                BIT27
#define TXDATA3_MODE_CFG                                                BIT26
#define TXDATA4_MODE_CFG                                                BIT25
#define TXDATA5_MODE_CFG                                                BIT24
#define TXDATA6_MODE_CFG                                                BIT23
#define TXDATA7_MODE_CFG                                                BIT22
#define TXDATA8_MODE_CFG                                                BIT21
#define TXDATA9_MODE_CFG                                                BIT20
#define TXDATA10_MODE_CFG                                               BIT19
#define TXDATA11_MODE_CFG                                               BIT18
#define TXCLK_MODE_CFG                                                  BIT17
#define GPIO_MODE_SEL                                                   BIT0

/* RFIP_ADDR_GPIO_RX_FORCE_CFG */
#define RXRF_ENABLE_VALUE                                               BIT31
#define RXIF_ENABLE_VALUE                                               BIT30
#define RX_GAIN_INDEX_VALUE_MASK                                        CO_BITS(22, 29)
#define RX_GAIN_INDEX_VALUE_SHIFT                                       22
#define RX_DCOFFSETI_VALUE_MASK                                         CO_BITS(13, 21)
#define RX_DCOFFSETI_VALUE_SHIFT                                        13
#define RX_DCOFFSETQ_VALUE_MASK                                         CO_BITS(4, 12)
#define RX_DCOFFSETQ_VALUE_SHIFT                                        4

/* RFIP_ADDR_GPIO_TX_FORCE_CFG */
#define TXRF_ENABLE_VALUE                                               BIT31
#define TXIF_ENABLE_VALUE                                               BIT30
#define TX_GAIN_INDEX_VALUE_MASK                                        CO_BITS(24, 29)
#define TX_GAIN_INDEX_VALUE_SHIFT                                       24
#define TX_DATA_VALUE_MASK                                              CO_BITS(12, 23)
#define TX_DATA_VALUE_SHIFT                                             12

/* RFIP_ADDR_GPIO_FORCE_EN_CFG */
#define RXRF_ENABLE_FORCE_EN                                            BIT31
#define RXIF_ENABLE_FORCE_EN                                            BIT30
#define RX_GAIN_INDEX_FORCE_EN                                          BIT29
#define RX_DCOFFSET_FORCE_EN                                            BIT28
#define TXRF_ENABLE_FORCE_EN                                            BIT3
#define TXIF_ENABLE_FORCE_EN                                            BIT2
#define TX_GAIN_INDEX_FORCE_EN                                          BIT1
#define TX_DATA_FORCE_EN                                                BIT0

/* RFIP_ADDR_TX_GAIN_FORCE_CFG */
#define TX_DIG_GAIN_FORCE_EN                                            BIT31
#define TX_DIG_GAIN_FORCE_VALUE_MASK                                    CO_BITS(20, 30)
#define TX_DIG_GAIN_FORCE_VALUE_SHIFT                                   20
#define TX_RF_GAIN_FORCE_EN                                             BIT15
#define TX_RF_GAIN_FORCE_VALUE_MASK                                     CO_BITS(4, 14)
#define TX_RF_GAIN_FORCE_VALUE_SHIFT                                    4

/* RFIP_ADDR_RCCAL_OFFSET_TEST_CFG */
#define RCCAL_ASSIGN_DIS_MASK                                           CO_BITS(21, 22)
#define RCCAL_ASSIGN_DIS_SHIFT                                          21
#define RCCAL_CLK_CFG3_MASK                                             CO_BITS(15, 20)
#define RCCAL_CLK_CFG3_SHIFT                                            15
#define RCCAL_CLK_CFG2_MASK                                             CO_BITS(9, 14)
#define RCCAL_CLK_CFG2_SHIFT                                            9
#define RCCAL_CLK_CFG1_MASK                                             CO_BITS(3, 8)
#define RCCAL_CLK_CFG1_SHIFT                                            3
#define RCCAL_NUM_MASK                                                  CO_BITS(1, 2)
#define RCCAL_NUM_SHIFT                                                 1
#define RCCAL_REQ                                                       BIT0

/* RFIP_ADDR_RCCAL_SR */
#define RCCAL_FINISH_D3                                                 BIT31
#define RCCAL_RESULT3_MASK                                              CO_BITS(27, 30)
#define RCCAL_RESULT3_SHIFT                                             27
#define RCCAL_RESULT2_MASK                                              CO_BITS(23, 26)
#define RCCAL_RESULT2_SHIFT                                             23
#define RCCAL_RESULT1_MASK                                              CO_BITS(19, 22)
#define RCCAL_RESULT1_SHIFT                                             19

/* RFIP_ADDR_TX_COMP_FORCE_CFG */
#define TX_I_COMP_FORCE_EN                                              BIT31
#define TX_I_COMP_FORCE_VALUE_MASK                                      CO_BITS(19, 30)
#define TX_I_COMP_FORCE_VALUE_SHIFT                                     19
#define TX_Q_COMP_FORCE_EN                                              BIT15
#define TX_Q_COMP_FORCE_VALUE_MASK                                      CO_BITS(3, 14)
#define TX_Q_COMP_FORCE_VALUE_SHIFT                                     3

/* RFIP_ADDR_RFIPSTATE */
#define RCCAL_CSTATE_MASK                                               CO_BITS(27, 31)
#define RCCAL_CSTATE_SHIFT                                              27
#define RX_STATE_MASK                                                   CO_BITS(3, 5)
#define RX_STATE_SHIFT                                                  3
#define TX_STATE_MASK                                                   CO_BITS(0, 2)
#define TX_STATE_SHIFT                                                  0

/* RFIP_ADDR_RFIPCALIN */
#define RCCAL_CIN_MASK                                                  CO_BITS(26, 31)
#define RCCAL_CIN_SHIFT                                                 26
#define ADCCAL_ACC_MASK                                                 CO_BITS(7, 25)
#define ADCCAL_ACC_SHIFT                                                7
#define ADCCAL_M_MASK                                                   CO_BITS(3, 6)
#define ADCCAL_M_SHIFT                                                  3
#define ADCCAL_BUG                                                      BIT2
#define ADCCAL_SIGN                                                     BIT1
#define ADCCAL_ENABLE                                                   BIT0

/* RFIP_ADDR_RFPLLDEBUGCFG */
#define RFPLL_MSB_RF_COUNTER_OUT                                        BIT30
#define RFPLL_RF_COUNTER_DONE_OUT_MASK                                  CO_BITS(16, 29)
#define RFPLL_RF_COUNTER_DONE_OUT_SHIFT                                 16
#define RFPLL_FREQ_CALIBRE_MEM_M_OUT_MASK                               CO_BITS(8, 15)
#define RFPLL_FREQ_CALIBRE_MEM_M_OUT_SHIFT                              8
#define RFPLL_FREQ_CALIBRE_MEM_P_OUT_MASK                               CO_BITS(0, 7)
#define RFPLL_FREQ_CALIBRE_MEM_P_OUT_SHIFT                              0

/* RFIP_ADDR_RFPLLDEBUGCFG2 */
#define RFPLL_DIFF_MEM_M_OUT_MASK                                       CO_BITS(14, 27)
#define RFPLL_DIFF_MEM_M_OUT_SHIFT                                      14
#define RFPLL_DIFF_MEM_P_OUT_MASK                                       CO_BITS(0, 13)
#define RFPLL_DIFF_MEM_P_OUT_SHIFT                                      0

/* RFIP_ADDR_TXRX_DEBUG */
#define TX_AFIFO_ERR                                                    BIT1
#define RX_AFIFO_ERR                                                    BIT0

uint32_t read_rf_reg(uint32_t regaddr);
void write_rf_reg(uint32_t regaddr, uint32_t data);

#elif CONFIG_RF_TYPE == RF_GDM32103
/* ========  RF Register Definitions ========  */
#define RFIP_ADDR_GCFG                                                  0x0000
#define RFIP_ADDR_XTAL_CFG1                                             0x0004
#define RFIP_ADDR_MDLL_CFG1                                             0x0008
#define RFIP_ADDR_RFPLL_CFG1                                            0x000C
#define RFIP_ADDR_VCO_CFG1                                              0x0010
#define RFIP_ADDR_PLLS_CFG                                              0x0014
#define RFIP_ADDR_PLLS_DIVNUM                                           0x0018
#define RFIP_ADDR_TX_DAC_CFG                                            0x001C
#define RFIP_ADDR_TX_FILTER_CFG                                         0x0020
#define RFIP_ADDR_TX_MIXER_CFG                                          0x0024
#define RFIP_ADDR_TX_PAD_CFG                                            0x0028
#define RFIP_ADDR_TX_PA_CFG                                             0x002C
#define RFIP_ADDR_TX_CAL_ENABLE                                         0x0030
#define RFIP_ADDR_LNA_CAL_RX_DC                                         0x0038
#define RFIP_ADDR_RX_FILTER_PKDET                                       0x003C
#define RFIP_ADDR_SAR_ADC                                               0x0040
#define RFIP_ADDR_LDO_CFG1                                              0x0044
#define RFIP_ADDR_AUXADC_TEMP                                           0x0048
#define RFIP_ADDR_XTAL_FREQ_TUNING                                      0x004C
#define RFIP_ADDR_MDLL_CFG2                                             0x0050
#define RFIP_ADDR_RX_RO_RESULT                                          0x0054
#define RFIP_ADDR_RO_STATE_RESULT                                       0x0058
#define RFIP_ADDR_FORCE_STATE_ENABLE                                    0x005C
#define RFIP_ADDR_PLLS_CFG1                                             0x0060
#define RFIP_ADDR_RFPLL_CFG2                                            0x0064
#define RFIP_ADDR_PORT_FORCE_VALUE1                                     0x0068
#define RFIP_ADDR_PORT_FORCE_VALUE2                                     0x006C
#define RFIP_ADDR_TX_DATA                                               0x0070
#define RFIP_ADDR_FORCE_RX_GAIN_INDEX                                   0x0074
#define RFIP_ADDR_FORCE_RX_GAIN_VALUE                                   0x0078
#define RFIP_ADDR_RX_LNA_GAIN_TABLE                                     0x007C
#define RFIP_ADDR_RX_PGA_GAIN_TABLE                                     0x0080
#define RFIP_ADDR_RX_LPF_GAIN_TABLE1                                    0x0084
#define RFIP_ADDR_RX_LPF_GAIN_TABLE2                                    0x0088
#define RFIP_ADDR_FORCE_TX_GAIN_INDEX                                   0x008C
#define RFIP_ADDR_FORCE_TX_GAIN_VALUE                                   0x0090
#define RFIP_ADDR_RO_TX_DIG_GAIN_VALUE                                  0x0094
#define RFIP_ADDR_FORCE_TX_COMP                                         0x0098
#define RFIP_ADDR_RFPLL_FSM_STATE_OFF_LDOON                             0x009C
#define RFIP_ADDR_RFPLL_FSM_STATE_VCOON_MDLL                            0x00A0
#define RFIP_ADDR_RFPLL_FSM_STATE_VCOCAL_ACTIVE                         0x00A4
#define RFIP_ADDR_RX_FSM_STATE_OFF_ANAON                                0x00A8
#define RFIP_ADDR_RX_FSM_STATE_IDLE_ACTIVE                              0x00AC
#define RFIP_ADDR_TX_FSM_STATE_OFF_ANAON                                0x00B0
#define RFIP_ADDR_TX_FSM_STATE_ACTIVE_RFOFF                             0x00B4
#define RFIP_ADDR_TX_FSM_STATE_DRON                                     0x00B8
#define RFIP_ADDR_RO_RX_GAIN_VALUE                                      0x00BC
#define RFIP_ADDR_VCO_CFG2                                              0x00C8
#define RFIP_ADDR_RFPLL_DIG_CFG1                                        0x00CC
#define RFIP_ADDR_RFPLL_DIG_CFG2                                        0x00D0
#define RFIP_ADDR_RFPLL_VCOCAL_CFG1                                     0x00D4
#define RFIP_ADDR_PLLS_CFG2                                             0x00D8
#define RFIP_ADDR_RO_FILTER_CCW                                         0x00DC
#define RFIP_ADDR_PA_BIAS_11N_CFG                                       0x00E0
#define RFIP_ADDR_PA_BIAS_11B_CFG                                       0x00E4
#define RFIP_ADDR_PA_BIAS_BT_CFG                                        0x00E8
#define RFIP_ADDR_TX_CALCOMP_WIFI                                       0x00EC
#define RFIP_ADDR_TX_CALCOMP_BT                                         0x00F0
#define RFIP_ADDR_TX_UPCONVERTER                                        0x00F4
#define RFIP_ADDR_TX_DC_CFG1                                            0x00F8
#define RFIP_ADDR_DELAY_TX_RX_STATE                                     0x00FC
#define RFIP_ADDR_DELAY_RFPLL_STATE                                     0x0100
#define RFIP_ADDR_PLLS_CFG3                                             0x0104
#define RFIP_ADDR_TX_UPCONVERTER_COEF0                                  0x0108
#define RFIP_ADDR_TX_UPCONVERTER_COEF1                                  0x010C
#define RFIP_ADDR_TX_UPCONVERTER_COEF2                                  0x0110
#define RFIP_ADDR_TX_UPCONVERTER_COEF3                                  0x0114
#define RFIP_ADDR_TX_UPCONVERTER_COEF4                                  0x0118
#define RFIP_ADDR_TX_UPCONVERTER_COEF5                                  0x011C
#define RFIP_ADDR_TX_UPCONVERTER_COEF6                                  0x0120
#define RFIP_ADDR_TX_UPCONVERTER_COEF7                                  0x0124
#define RFIP_ADDR_RO_RFPLL_DEBUG_STATUS1                                0x0128
#define RFIP_ADDR_RO_RFPLL_DEBUG_STATUS2                                0x012C
#define RFIP_ADDR_RO_TX_RF_DEBUG_STATUS                                 0x0130
#define RFIP_ADDR_AUXADC_CFG                                            0x0134
#define RFIP_ADDR_RC_CAL_CFG                                            0x0138
#define RFIP_ADDR_RC_CAL_RESULT                                         0x013C
#define RFIP_ADDR_FILTER_CFG                                            0x0140
#define RFIP_ADDR_LOFT_COMP_REG0                                        0x0144
#define RFIP_ADDR_LOFT_COMP_REG1                                        0x0148
#define RFIP_ADDR_LOFT_COMP_REG2                                        0x014C
#define RFIP_ADDR_LOFT_COMP_REG3                                        0x0150
#define RFIP_ADDR_LOFT_COMP_REG4                                        0x0154
#define RFIP_ADDR_LOFT_COMP_REG5                                        0x0158
#define RFIP_ADDR_TX_RF_GAIN_CFG0                                       0x015C
#define RFIP_ADDR_TX_RF_GAIN_CFG1                                       0x0160
#define RFIP_ADDR_TX_RF_GAIN_CFG2                                       0x0164
#define RFIP_ADDR_TX_RF_GAIN_CFG3                                       0x0168
#define RFIP_ADDR_TX_RF_GAIN_CFG4                                       0x016C
#define RFIP_ADDR_TX_RF_GAIN_CFG5                                       0x0170
#define RFIP_ADDR_TX_RF_GAIN_CFG6                                       0x0174
#define RFIP_ADDR_TX_RF_GAIN_CFG7                                       0x0178
#define RFIP_ADDR_TX_RF_GAIN_CFG8                                       0x017C
#define RFIP_ADDR_TX_RF_GAIN_CFG9                                       0x0180
#define RFIP_ADDR_TX_RF_GAIN_CFG10                                      0x0184
#define RFIP_ADDR_TX_RF_GAIN_CFG11                                      0x0188
#define RFIP_ADDR_TX_RF_GAIN_CFG12                                      0x018C
#define RFIP_ADDR_TX_RF_GAIN_CFG13                                      0x0190
#define RFIP_ADDR_TX_DIG_GAIN_CFG0                                      0x0194
#define RFIP_ADDR_TX_DIG_GAIN_CFG1                                      0x0198
#define RFIP_ADDR_TX_DIG_GAIN_CFG2                                      0x019C
#define RFIP_ADDR_TX_DIG_GAIN_CFG3                                      0x01A0
#define RFIP_ADDR_TX_DIG_GAIN_CFG4                                      0x01A4
#define RFIP_ADDR_TX_DIG_GAIN_CFG5                                      0x01A8
#define RFIP_ADDR_TX_DIG_GAIN_CFG6                                      0x01AC
#define RFIP_ADDR_TX_DIG_GAIN_CFG7                                      0x01B0
#define RFIP_ADDR_TX_DIG_GAIN_CFG8                                      0x01B4
#define RFIP_ADDR_TX_DIG_GAIN_CFG9                                      0x01B8
#define RFIP_ADDR_TX_DIG_GAIN_CFG10                                     0x01BC
#define RFIP_ADDR_TX_DIG_GAIN_CFG11                                     0x01C0
#define RFIP_ADDR_TX_DIG_GAIN_CFG12                                     0x01C4
#define RFIP_ADDR_TX_DIG_GAIN_CFG13                                     0x01C8
#define RFIP_ADDR_TX_DIG_GAIN_CFG14                                     0x01CC
#define RFIP_ADDR_TX_DIG_GAIN_CFG15                                     0x01D0
#define RFIP_ADDR_TX_DIG_GAIN_CFG16                                     0x01D4
#define RFIP_ADDR_TX_DIG_GAIN_CFG17                                     0x01D8
#define RFIP_ADDR_TX_DIG_GAIN_CFG18                                     0x01DC
#define RFIP_ADDR_TX_DIG_GAIN_CFG19                                     0x01E0
#define RFIP_ADDR_TX_DIG_GAIN_CFG20                                     0x01E4
#define RFIP_ADDR_TX_DIG_GAIN_CFG21                                     0x01E8
#define RFIP_ADDR_TX_DIG_GAIN_CFG22                                     0x01EC
#define RFIP_ADDR_TX_DIG_GAIN_CFG23                                     0x01F0
#define RFIP_ADDR_TX_DIG_GAIN_CFG24                                     0x01F4
#define RFIP_ADDR_TX_DIG_GAIN_CFG25                                     0x01F8
#define RFIP_ADDR_TX_DIG_GAIN_CFG26                                     0x01FC
#define RFIP_ADDR_TX_DIG_GAIN_CFG27                                     0x0200
#define RFIP_ADDR_TX_DIG_GAIN_CFG28                                     0x0204
#define RFIP_ADDR_TX_DIG_GAIN_CFG29                                     0x0208
#define RFIP_ADDR_TX_DIG_GAIN_CFG30                                     0x020C
#define RFIP_ADDR_TX_DIG_GAIN_CFG31                                     0x0210
#define RFIP_ADDR_TX_CURRENT_CFG0                                       0x0214
#define RFIP_ADDR_TX_CURRENT_CFG1                                       0x0218
#define RFIP_ADDR_TX_CURRENT_CFG2                                       0x021C
#define RFIP_ADDR_TX_CURRENT_CFG3                                       0x0220


/* RFIP_ADDR_XTAL_CFG1 */
#define XTAL_CFG1_XTAL_BYPASS_DIGBUF_EN                                 BIT11
#define XTAL_CFG1_PU_XTAL_DIG                                           BIT9
#define XTAL_CFG1_DEBUG_EN_CLK2DIG_XTAL                                 BIT8
#define XTAL_CFG1_REG_XTAL_VBIT_MASK                                    CO_BITS(4, 7)
#define XTAL_CFG1_REG_XTAL_VBIT_SHIFT                                   4
#define XTAL_CFG1_XTAL_OSC_IBIT_MASK                                    CO_BITS(0, 3)
#define XTAL_CFG1_XTAL_OSC_IBIT_SHIFT                                   0

/* RFIP_ADDR_MDLL_CFG1 */
#define MDLL_CFG1_MDLL_CKX2_CBIT_MASK                                   CO_BITS(16, 18)
#define MDLL_CFG1_MDLL_CKX2_CBIT_SHIFT                                  16
#define MDLL_CFG1_MDLL_IVBIT_MASK                                       CO_BITS(12, 14)
#define MDLL_CFG1_MDLL_IVBIT_SHIFT                                      12
#define MDLL_CFG1_MDLL_CP_IBIT_MASK                                     CO_BITS(8, 10)
#define MDLL_CFG1_MDLL_CP_IBIT_SHIFT                                    8
#define MDLL_CFG1_MDLL_CPOPA_EN                                         BIT4
#define MDLL_CFG1_MDLL_CKX2_RBIT                                        BIT3
#define MDLL_CFG1_MDLL_PDT_MODE                                         BIT2
#define MDLL_CFG1_EN_CLK_MDLL_RFPLL                                     BIT0

/* RFIP_ADDR_RFPLL_CFG1 */
#define RFPLL_CFG1_RFPLL_VCTRL_PDT_LATCH                                BIT31
#define RFPLL_CFG1_RFPLL_VCTRL_PDT_RBIT_MASK                            CO_BITS(28, 30)
#define RFPLL_CFG1_RFPLL_VCTRL_PDT_RBIT_SHIFT                           28
#define RFPLL_CFG1_RFPLL_PRESC_REG_VBIT_MASK                            CO_BITS(24, 27)
#define RFPLL_CFG1_RFPLL_PRESC_REG_VBIT_SHIFT                           24
#define RFPLL_CFG1_RFPLL_PRESC_DIV23_EN                                 BIT23
#define RFPLL_CFG1_RFPLL_PRESC_FBC_SEL                                  BIT22
#define RFPLL_CFG1_RFPLL_PRESC_SDMCLK_SEL                               BIT21
#define RFPLL_CFG1_RFPLL_LOOP_VCTRL_GND                                 BIT20
#define RFPLL_CFG1_RFPLL_LOOP_NOTCH_ENB                                 BIT19
#define RFPLL_CFG1_RFPLL_LOOP_CALI_NOTCH_ENB                            BIT18
#define RFPLL_CFG1_RFPLL_LOOP_LDO_VBIT_MASK                             CO_BITS(16, 17)
#define RFPLL_CFG1_RFPLL_LOOP_LDO_VBIT_SHIFT                            16
#define RFPLL_CFG1_RFPLL_LOOP_REG_VBIT_MASK                             CO_BITS(12, 15)
#define RFPLL_CFG1_RFPLL_LOOP_REG_VBIT_SHIFT                            12
#define RFPLL_CFG1_RFPLL_LOOP_LDO_BYP                                   BIT11
#define RFPLL_CFG1_RFPLL_LOOP_CPIBIT_MASK                               CO_BITS(6, 10)
#define RFPLL_CFG1_RFPLL_LOOP_CPIBIT_SHIFT                              6
#define RFPLL_CFG1_RFPLL_LOOP_CPRBIT_MASK                               CO_BITS(4, 5)
#define RFPLL_CFG1_RFPLL_LOOP_CPRBIT_SHIFT                              4
#define RFPLL_CFG1_PU_RFPLL_VCTRL_PDT                                   BIT3
#define RFPLL_CFG1_RFPLL_LOOP_VCO_GAIN_MASK                             CO_BITS(0, 2)
#define RFPLL_CFG1_RFPLL_LOOP_VCO_GAIN_SHIFT                            0

/* RFIP_ADDR_VCO_CFG1 */
#define VCO_CFG1_VCO_PK_OVLD_DD                                         BIT31
#define VCO_CFG1_VCO_PKDET_EN_CFG                                       BIT30
#define VCO_CFG1_VCO_REG_BIT_CFG_MASK                                   CO_BITS(26, 29)
#define VCO_CFG1_VCO_REG_BIT_CFG_SHIFT                                  26
#define VCO_CFG1_RFPLL_REG_RESERVED_IN_MASK                             CO_BITS(24, 25)
#define VCO_CFG1_RFPLL_REG_RESERVED_IN_SHIFT                            24
#define VCO_CFG1_RFPLL_VCTRL_TEST_EN_MASK                               CO_BITS(22, 23)
#define VCO_CFG1_RFPLL_VCTRL_TEST_EN_SHIFT                              22
#define VCO_CFG1_RFPLL_PKDET_UP_EN                                      BIT21
#define VCO_CFG1_VCO_REGBUF_BIT_MASK                                    CO_BITS(16, 19)
#define VCO_CFG1_VCO_REGBUF_BIT_SHIFT                                   16
#define VCO_CFG1_VCO_DIVBIAS_BIT_MASK                                   CO_BITS(12, 13)
#define VCO_CFG1_VCO_DIVBIAS_BIT_SHIFT                                  12
#define VCO_CFG1_VCO_VCOM_BIT_MASK                                      CO_BITS(8, 10)
#define VCO_CFG1_VCO_VCOM_BIT_SHIFT                                     8
#define VCO_CFG1_VCO_VAR_SWAP                                           BIT7
#define VCO_CFG1_VCO_VAR_BIT_MASK                                       CO_BITS(4, 6)
#define VCO_CFG1_VCO_VAR_BIT_SHIFT                                      4
#define VCO_CFG1_VCO_PKREF_BIT_MASK                                     CO_BITS(0, 2)
#define VCO_CFG1_VCO_PKREF_BIT_SHIFT                                    0

/* RFIP_ADDR_PLLS_CFG */
#define PLLS_CFG_PLLS_PFD_DLYCTRL                                       BIT27
#define PLLS_CFG_PLLS_VREF_SEL_BG                                       BIT26
#define PLLS_CFG_PLLS_LDO_0P9V                                          BIT25
#define PLLS_CFG_PLLS_CLK_FBC_EN                                        BIT19
#define PLLS_CFG_PLLS_PFD_EN                                            BIT18
#define PLLS_CFG_PLLS_RSTN_REG                                          BIT17
#define PLLS_CFG_PLLS_CPOPA_EN                                          BIT16
#define PLLS_CFG_PLLS_CLK4SAR_DCSEL_MASK                                CO_BITS(14, 15)
#define PLLS_CFG_PLLS_CLK4SAR_DCSEL_SHIFT                               14
#define PLLS_CFG_PLLS_REF_SEL_HSI_ANA                                   BIT13
#define PLLS_CFG_PLLS_BYPSS_LOCK2                                       BIT12
#define PLLS_CFG_PLLS_REFX2_RBIT                                        BIT11
#define PLLS_CFG_PLLS_RBIT_MASK                                         CO_BITS(9, 10)
#define PLLS_CFG_PLLS_RBIT_SHIFT                                        9
#define PLLS_CFG_PLLS_INT_MODE                                          BIT8
#define PLLS_CFG_PLLS_FORCELOCK_LV                                      BIT7
#define PLLS_CFG_PLLS_CP_IBIT_MASK                                      CO_BITS(4, 6)
#define PLLS_CFG_PLLS_CP_IBIT_SHIFT                                     4
#define PLLS_CFG_DEBUG_EN_CLK2DIG_PLLS                                  BIT3
#define PLLS_CFG_EN_CLK_PLLS_80_DIG                                     BIT2
#define PLLS_CFG_PLLS_REFX2_MODE                                        BIT1
#define PLLS_CFG_EN_PLLS_LDO33TO12                                      BIT0

/* RFIP_ADDR_PLLS_DIVNUM */
#define PLLS_DIVNUM_PLLS_FORCE_DIVNUM_EN                                BIT10
#define PLLS_DIVNUM_PLLS_FORCE_DIVNUM_VALUE_MASK                        CO_BITS(0, 9)
#define PLLS_DIVNUM_PLLS_FORCE_DIVNUM_VALUE_SHIFT                       0

/* RFIP_ADDR_TX_DAC_CFG */
#define TX_DAC_CFG_REG_TXDAC_VLOWCTRL_MASK                              CO_BITS(16, 17)
#define TX_DAC_CFG_REG_TXDAC_VLOWCTRL_SHIFT                             16
#define TX_DAC_CFG_REG_TXDAC_VDVDD_MASK                                 CO_BITS(14, 15)
#define TX_DAC_CFG_REG_TXDAC_VDVDD_SHIFT                                14
#define TX_DAC_CFG_REG_TXDAC_SEL_EDGE                                   BIT13
#define TX_DAC_CFG_REG_TXDAC_LPMODE                                     BIT10
#define TX_DAC_CFG_REG_TXDAC_CMBIT_MASK                                 CO_BITS(7, 9)
#define TX_DAC_CFG_REG_TXDAC_CMBIT_SHIFT                                7
#define TX_DAC_CFG_TXDAC_ICTRL_MASK                                     CO_BITS(4, 6)
#define TX_DAC_CFG_TXDAC_ICTRL_SHIFT                                    4
#define TX_DAC_CFG_REG_TXDAC_MODE_MASK                                  CO_BITS(0, 2)
#define TX_DAC_CFG_REG_TXDAC_MODE_SHIFT                                 0

/* RFIP_ADDR_TX_FILTER_CFG */
#define TX_FILTER_CFG_REG_TXFLT_BWMODE                                  BIT3
#define TX_FILTER_CFG_REG_TXFLT_VCAS_MASK                               CO_BITS(0, 2)
#define TX_FILTER_CFG_REG_TXFLT_VCAS_SHIFT                              0

/* RFIP_ADDR_TX_MIXER_CFG */
#define TX_MIXER_CFG_REG_MXDEQ_BIT_MASK                                 CO_BITS(14, 15)
#define TX_MIXER_CFG_REG_MXDEQ_BIT_SHIFT                                14
#define TX_MIXER_CFG_REG_MXCAPTUNE_MASK                                 CO_BITS(10, 13)
#define TX_MIXER_CFG_REG_MXCAPTUNE_SHIFT                                10
#define TX_MIXER_CFG_REG_MXVBCAL_BIT_MASK                               CO_BITS(8, 9)
#define TX_MIXER_CFG_REG_MXVBCAL_BIT_SHIFT                              8
#define TX_MIXER_CFG_REG_MXVBCAS_BIT_MASK                               CO_BITS(6, 7)
#define TX_MIXER_CFG_REG_MXVBCAS_BIT_SHIFT                              6
#define TX_MIXER_CFG_REG_MXVBLO_BIT_MASK                                CO_BITS(4, 5)
#define TX_MIXER_CFG_REG_MXVBLO_BIT_SHIFT                               4
#define TX_MIXER_CFG_REG_CAL_EDGE                                       BIT3
#define TX_MIXER_CFG_REG_CAL_SWAP                                       BIT0

/* RFIP_ADDR_TX_PAD_CFG */
#define TX_PAD_CFG_REG_PADDQ_BIT_MASK                                   CO_BITS(15, 16)
#define TX_PAD_CFG_REG_PADDQ_BIT_SHIFT                                  15
#define TX_PAD_CFG_REG_PADTEMP_BIT_MASK                                 CO_BITS(12, 14)
#define TX_PAD_CFG_REG_PADTEMP_BIT_SHIFT                                12
#define TX_PAD_CFG_REG_PADCAP_BIT_MASK                                  CO_BITS(9, 11)
#define TX_PAD_CFG_REG_PADCAP_BIT_SHIFT                                 9
#define TX_PAD_CFG_REG_PADIBATT_BIT_MASK                                CO_BITS(6, 8)
#define TX_PAD_CFG_REG_PADIBATT_BIT_SHIFT                               6
#define TX_PAD_CFG_REG_PADBIAS_SET1_MASK                                CO_BITS(2, 3)
#define TX_PAD_CFG_REG_PADBIAS_SET1_SHIFT                               2
#define TX_PAD_CFG_REG_PADBIAS_SET2_MASK                                CO_BITS(0, 1)
#define TX_PAD_CFG_REG_PADBIAS_SET2_SHIFT                               0

/* RFIP_ADDR_TX_PA_CFG */
#define TX_PA_CFG_REG_TXPOWER_MODE_MASK                                 CO_BITS(23, 24)
#define TX_PA_CFG_REG_TXPOWER_MODE_SHIFT                                23
#define TX_PA_CFG_REG_PACAPX2_BIT_MASK                                  CO_BITS(20, 22)
#define TX_PA_CFG_REG_PACAPX2_BIT_SHIFT                                 20
#define TX_PA_CFG_REG_PAIB_TRIM_MASK                                    CO_BITS(16, 19)
#define TX_PA_CFG_REG_PAIB_TRIM_SHIFT                                   16
#define TX_PA_CFG_REG_PATEMP_BIT_MASK                                   CO_BITS(13, 15)
#define TX_PA_CFG_REG_PATEMP_BIT_SHIFT                                  13
#define TX_PA_CFG_REG_PAIBATT_BIT_MASK                                  CO_BITS(6, 8)
#define TX_PA_CFG_REG_PAIBATT_BIT_SHIFT                                 6
#define TX_PA_CFG_REG_PABIAS_SET1_MASK                                  CO_BITS(2, 3)
#define TX_PA_CFG_REG_PABIAS_SET1_SHIFT                                 2
#define TX_PA_CFG_REG_PABIAS_SET2_MASK                                  CO_BITS(0, 1)
#define TX_PA_CFG_REG_PABIAS_SET2_SHIFT                                 0

/* RFIP_ADDR_TX_CAL_ENABLE */
#define TX_CAL_ENABLE_EN_CAL_DCV                                        BIT16
#define TX_CAL_ENABLE_EN_CAL_DCI                                        BIT15
#define TX_CAL_ENABLE_REG_CALGAIN_BIT_MASK                              CO_BITS(6, 7)
#define TX_CAL_ENABLE_REG_CALGAIN_BIT_SHIFT                             6
#define TX_CAL_ENABLE_REG_PSATTH_BIT_MASK                               CO_BITS(4, 5)
#define TX_CAL_ENABLE_REG_PSATTH_BIT_SHIFT                              4
#define TX_CAL_ENABLE_REG_LOCALATT_BIT_MASK                             CO_BITS(2, 3)
#define TX_CAL_ENABLE_REG_LOCALATT_BIT_SHIFT                            2
#define TX_CAL_ENABLE_EN_TX_IQCAL                                       BIT1
#define TX_CAL_ENABLE_EN_TX_PDET                                        BIT0

/* RFIP_ADDR_LNA_CAL_RX_DC */
#define LNA_CAL_RX_DC_LNA_VBIAS_TRIM_MASK                               CO_BITS(24, 27)
#define LNA_CAL_RX_DC_LNA_VBIAS_TRIM_SHIFT                              24
#define LNA_CAL_RX_DC_LNA_REG_TRIM_MASK                                 CO_BITS(20, 23)
#define LNA_CAL_RX_DC_LNA_REG_TRIM_SHIFT                                20
#define LNA_CAL_RX_DC_LNA_IBIAS_TRIM_MASK                               CO_BITS(16, 19)
#define LNA_CAL_RX_DC_LNA_IBIAS_TRIM_SHIFT                              16
#define LNA_CAL_RX_DC_LNA_VCAL_EN                                       BIT15
#define LNA_CAL_RX_DC_LNA_CGS_TRIM_MASK                                 CO_BITS(11, 14)
#define LNA_CAL_RX_DC_LNA_CGS_TRIM_SHIFT                                11
#define LNA_CAL_RX_DC_RXBALUN_CTRIM_MASK                                CO_BITS(8, 10)
#define LNA_CAL_RX_DC_RXBALUN_CTRIM_SHIFT                               8
#define LNA_CAL_RX_DC_RXLO_VCM_MASK                                     CO_BITS(4, 6)
#define LNA_CAL_RX_DC_RXLO_VCM_SHIFT                                    4
#define LNA_CAL_RX_DC_RX_OS_I_EN                                        BIT2
#define LNA_CAL_RX_DC_RX_OS_Q_EN                                        BIT1
#define LNA_CAL_RX_DC_RX_OS_MODE                                        BIT0

/* RFIP_ADDR_RX_FILTER_PKDET */
#define RX_FILTER_PKDET_RX_PKDET_OVLD_DD_MASK                           CO_BITS(30, 31)
#define RX_FILTER_PKDET_RX_PKDET_OVLD_DD_SHIFT                          30
#define RX_FILTER_PKDET_RXFLT2_VCM_CTRL_MASK                            CO_BITS(28, 29)
#define RX_FILTER_PKDET_RXFLT2_VCM_CTRL_SHIFT                           28
#define RX_FILTER_PKDET_RXFLT2_BW_MODE                                  BIT25
#define RX_FILTER_PKDET_RXFLT1_BW_MODE                                  BIT24
#define RX_FILTER_PKDET_RXFLT1_AUXI_EN                                  BIT13
#define RX_FILTER_PKDET_RXFLT1_AUXQ_EN                                  BIT12
#define RX_FILTER_PKDET_RXPKDET_VREF0_MASK                              CO_BITS(4, 6)
#define RX_FILTER_PKDET_RXPKDET_VREF0_SHIFT                             4
#define RX_FILTER_PKDET_RXPKDET_VREF1_MASK                              CO_BITS(0, 2)
#define RX_FILTER_PKDET_RXPKDET_VREF1_SHIFT                             0

/* RFIP_ADDR_SAR_ADC */
#define SAR_ADC_DEBUG_EN_CLK2DIG_SAR                                    BIT5
#define SAR_ADC_SARADC_VSUB_CTRL                                        BIT4
#define SAR_ADC_SARADC_LP_EN                                            BIT3
#define SAR_ADC_SARADC_CLK_TRIM_EN                                      BIT2
#define SAR_ADC_SARADC_LDO_SEL_MASK                                     CO_BITS(0, 1)
#define SAR_ADC_SARADC_LDO_SEL_SHIFT                                    0

/* RFIP_ADDR_LDO_CFG1 */
#define LDO_CFG1_LDO_VCO_LQB                                            BIT27
#define LDO_CFG1_LDO_VCO_TST                                            BIT26
#define LDO_CFG1_LDO_ANA_TST                                            BIT25
#define LDO_CFG1_LDO_CLK_TST                                            BIT24
#define LDO_CFG1_LDO_VCO_BYP                                            BIT23
#define LDO_CFG1_LDO_VCO_CAL_MASK                                       CO_BITS(20, 22)
#define LDO_CFG1_LDO_VCO_CAL_SHIFT                                      20
#define LDO_CFG1_LDO_ANA_BYP                                            BIT19
#define LDO_CFG1_LDO_ANA_CAL_MASK                                       CO_BITS(16, 18)
#define LDO_CFG1_LDO_ANA_CAL_SHIFT                                      16
#define LDO_CFG1_LDO_CLK_BYP                                            BIT15
#define LDO_CFG1_LDO_CLK_CAL_MASK                                       CO_BITS(12, 14)
#define LDO_CFG1_LDO_CLK_CAL_SHIFT                                      12
#define LDO_CFG1_LDO_AUXADC_VBIT_MASK                                   CO_BITS(6, 7)
#define LDO_CFG1_LDO_AUXADC_VBIT_SHIFT                                  6
#define LDO_CFG1_EN_CLK_MDLL_TEST                                       BIT3
#define LDO_CFG1_EN_CLK_PLLS_960                                        BIT2
#define LDO_CFG1_EN_CLK_TEST_960                                        BIT1
#define LDO_CFG1_EN_RFTEST                                              BIT0

/* RFIP_ADDR_AUXADC_TEMP */
#define AUXADC_TEMP_PU_AUXADC                                           BIT27
#define AUXADC_TEMP_PU_TEMP_XTAL                                        BIT26
#define AUXADC_TEMP_PU_TEMP_PA                                          BIT25
#define AUXADC_TEMP_EN_VDDTM_MEASURE                                    BIT24
#define AUXADC_TEMP_REG_RESERVED_IN_MASK                                CO_BITS(0, 23)
#define AUXADC_TEMP_REG_RESERVED_IN_SHIFT                               0

/* RFIP_ADDR_XTAL_FREQ_TUNING */
#define XTAL_FREQ_TUNING_XTAL_CFIX                                      BIT7
#define XTAL_FREQ_TUNING_XTAL_FREQ_TUNING_MASK                          CO_BITS(0, 6)
#define XTAL_FREQ_TUNING_XTAL_FREQ_TUNING_SHIFT                         0

/* RFIP_ADDR_MDLL_CFG2 */
#define MDLL_CFG2_RFPLL_LOCK_NUM_MASK                                   CO_BITS(12, 19)
#define MDLL_CFG2_RFPLL_LOCK_NUM_SHIFT                                  12
#define MDLL_CFG2_MDLL_RBIT                                             BIT8
#define MDLL_CFG2_MDLL_CBIT_MASK                                        CO_BITS(5, 7)
#define MDLL_CFG2_MDLL_CBIT_SHIFT                                       5
#define MDLL_CFG2_MDLL_CKX2_EN                                          BIT4
#define MDLL_CFG2_MDLL_DIV_NUM_MASK                                     CO_BITS(0, 3)
#define MDLL_CFG2_MDLL_DIV_NUM_SHIFT                                    0

/* RFIP_ADDR_RX_RO_RESULT */
#define RX_RO_RESULT_RX_OS_I                                            BIT28
#define RX_RO_RESULT_RX_OS_Q                                            BIT24
#define RX_RO_RESULT_RX_OS_I_MASK                                       CO_BITS(16, 23)
#define RX_RO_RESULT_RX_OS_I_SHIFT                                      16
#define RX_RO_RESULT_RX_OS_Q_MASK                                       CO_BITS(8, 15)
#define RX_RO_RESULT_RX_OS_Q_SHIFT                                      8
#define RX_RO_RESULT_DBG_DIV_EN                                         BIT7
#define RX_RO_RESULT_XTAL_EN_OSCDET                                     BIT6
#define RX_RO_RESULT_EN_CLK_XTAL_PLLS                                   BIT4
#define RX_RO_RESULT_REG_PACAP_BIT_RX_MASK                              CO_BITS(0, 3)
#define RX_RO_RESULT_REG_PACAP_BIT_RX_SHIFT                             0

/* RFIP_ADDR_RO_STATE_RESULT */
#define RO_STATE_RESULT_RFPLL_STATE_OUT_MASK                            CO_BITS(22, 24)
#define RO_STATE_RESULT_RFPLL_STATE_OUT_SHIFT                           22
#define RO_STATE_RESULT_RX_STATE_MASK                                   CO_BITS(19, 21)
#define RO_STATE_RESULT_RX_STATE_SHIFT                                  19
#define RO_STATE_RESULT_TX_STATE_MASK                                   CO_BITS(16, 18)
#define RO_STATE_RESULT_TX_STATE_SHIFT                                  16
#define RO_STATE_RESULT_RF_VERSION_MASK                                 CO_BITS(12, 15)
#define RO_STATE_RESULT_RF_VERSION_SHIFT                                12
#define RO_STATE_RESULT_REG_RESERVED_OUT_MASK                           CO_BITS(8, 11)
#define RO_STATE_RESULT_REG_RESERVED_OUT_SHIFT                          8
#define RO_STATE_RESULT_RFPLL_VCTRL_OVLD_DD                             BIT7
#define RO_STATE_RESULT_OPPLCH                                          OUT BIT6
#define RO_STATE_RESULT_TX_PA_SAT                                       BIT5
#define RO_STATE_RESULT_RFPLL_END_CAL_DD                                BIT4
#define RO_STATE_RESULT_XTAL_OSCDET_OUT_DD                              BIT3
#define RO_STATE_RESULT_MDLL_UNLCK_DET_DD                               BIT2
#define RO_STATE_RESULT_PLLS_LOCK_LV_DD                                 BIT1
#define RO_STATE_RESULT_RFPLL_LOCK_DET_LV_DD                            BIT0

/* RFIP_ADDR_FORCE_STATE_ENABLE */
#define FORCE_STATE_ENABLE_PU_LDO_VCO                                   BIT6
#define FORCE_STATE_ENABLE_PSLVERR_EN                                   BIT5
#define FORCE_STATE_ENABLE_EN_BGCR_CAL_REG                              BIT4
#define FORCE_STATE_ENABLE_EN_BGCR_PLL_REG                              BIT3
#define FORCE_STATE_ENABLE_RFPLL_FORCESTATE                             BIT2
#define FORCE_STATE_ENABLE_FORCESTATE                                   BIT1
#define FORCE_STATE_ENABLE_PORT_FORCE_EN                                BIT0

/* RFIP_ADDR_PLLS_CFG1 */
#define PLLS_CFG1_PLLS_INT_FRAC_DR                                      BIT31
#define PLLS_CFG1_PLLS_INT_REG_MASK                                     CO_BITS(21, 30)
#define PLLS_CFG1_PLLS_INT_REG_SHIFT                                    21
#define PLLS_CFG1_PLLS_FRAC_REG_MASK                                    CO_BITS(0, 20)
#define PLLS_CFG1_PLLS_FRAC_REG_SHIFT                                   0

/* RFIP_ADDR_RFPLL_CFG2 */
#define RFPLL_CFG2_RFPLL_INT_FRAC_DR                                    BIT31
#define RFPLL_CFG2_RFPLL_INT_REG_MASK                                   CO_BITS(21, 29)
#define RFPLL_CFG2_RFPLL_INT_REG_SHIFT                                  21
#define RFPLL_CFG2_RFPLL_FRAC_REG_MASK                                  CO_BITS(0, 20)
#define RFPLL_CFG2_RFPLL_FRAC_REG_SHIFT                                 0

/* RFIP_ADDR_PORT_FORCE_VALUE1 */
#define PORT_FORCE_VALUE1_PU_XTAL_VALUE                                 BIT31
#define PORT_FORCE_VALUE1_XTAL_EN_OSC_VALUE                             BIT30
#define PORT_FORCE_VALUE1_XTAL_READY_VALUE                              BIT29
#define PORT_FORCE_VALUE1_EN_CLK_XTAL_DIG_VALUE                         BIT28
#define PORT_FORCE_VALUE1_EN_CLK_XTAL_PLLCPU_VALUE                      BIT27
#define PORT_FORCE_VALUE1_EN_CLK_XTAL_PLLAUD_VALUE                      BIT26
#define PORT_FORCE_VALUE1_PU_PLLS_VALUE                                 BIT25
#define PORT_FORCE_VALUE1_EN_PLLS_DIG_VALUE                             BIT24
#define PORT_FORCE_VALUE1_PLLS_CLK_DIG_SEL_VALUE_MASK                   CO_BITS(22, 23)
#define PORT_FORCE_VALUE1_PLLS_CLK_DIG_SEL_VALUE_SHIFT                  22
#define PORT_FORCE_VALUE1_PU_RFPLL_VALUE                                BIT21
#define PORT_FORCE_VALUE1_RFPLL_CAL_EN_VALUE                            BIT20
#define PORT_FORCE_VALUE1_PU_LDO_CLK_VALUE                              BIT19
#define PORT_FORCE_VALUE1_PU_LDO_ANA_VALUE                              BIT18
#define PORT_FORCE_VALUE1_PU_BG_VALUE                                   BIT17
#define PORT_FORCE_VALUE1_BLE_RXBW_MODE_VALUE                           BIT15
#define PORT_FORCE_VALUE1_WIFI_BN_MODE_VALUE                            BIT14
#define PORT_FORCE_VALUE1_BT_MODE_VALUE                                 BIT13
#define PORT_FORCE_VALUE1_LDO_ANA_LQB_VALUE                             BIT12
#define PORT_FORCE_VALUE1_LDO_CLK_LQB_VALUE                             BIT11
#define PORT_FORCE_VALUE1_LDO_EFUSE_EN_VALUE                            BIT10
#define PORT_FORCE_VALUE1_LDO_EFUSE2_EN_VALUE                           BIT9
#define PORT_FORCE_VALUE1_PLLS_REF_SEL_FORCE_VALUE                      BIT8
#define PORT_FORCE_VALUE1_RF_RESERVED_I_VALUE_MASK                      CO_BITS(4, 7)
#define PORT_FORCE_VALUE1_RF_RESERVED_I_VALUE_SHIFT                     4
#define PORT_FORCE_VALUE1_RXRF_ENABLE_VALUE                             BIT3
#define PORT_FORCE_VALUE1_RXIF_ENABLE_VALUE                             BIT2
#define PORT_FORCE_VALUE1_TXRF_ENABLE_VALUE                             BIT1
#define PORT_FORCE_VALUE1_TXIF_ENABLE_VALUE                             BIT0

/* RFIP_ADDR_PORT_FORCE_VALUE2 */
#define PORT_FORCE_VALUE2_RX_FRAMEDETECT_VALUE                          BIT31
#define PORT_FORCE_VALUE2_TX_PA_POWER_EN_VALUE                          BIT30
#define PORT_FORCE_VALUE2_RFTEST_EN_VALUE                               BIT29
#define PORT_FORCE_VALUE2_BG_VBIT_VALUE_MASK                            CO_BITS(24, 26)
#define PORT_FORCE_VALUE2_BG_VBIT_VALUE_SHIFT                           24
#define PORT_FORCE_VALUE2_RXFILTER_OS_FORCE_EN                          BIT21
#define PORT_FORCE_VALUE2_RX_DCOFFSETI_VALUE_MASK                       CO_BITS(12, 20)
#define PORT_FORCE_VALUE2_RX_DCOFFSETI_VALUE_SHIFT                      12
#define PORT_FORCE_VALUE2_RX_DCOFFSETQ_VALUE_MASK                       CO_BITS(0, 8)
#define PORT_FORCE_VALUE2_RX_DCOFFSETQ_VALUE_SHIFT                      0

/* RFIP_ADDR_TX_DATA */
#define TX_DATA_TX_DATA_I_VALUE_MASK                                    CO_BITS(12, 23)
#define TX_DATA_TX_DATA_I_VALUE_SHIFT                                   12
#define TX_DATA_TX_DATA_Q_VALUE_MASK                                    CO_BITS(0, 11)
#define TX_DATA_TX_DATA_Q_VALUE_SHIFT                                   0

/* RFIP_ADDR_FORCE_RX_GAIN_INDEX */
#define FORCE_RX_GAIN_INDEX_RX_GAIN_INDEX_VALUE_MASK                    CO_BITS(24, 31)
#define FORCE_RX_GAIN_INDEX_RX_GAIN_INDEX_VALUE_SHIFT                   24
#define FORCE_RX_GAIN_INDEX_RX_GAIN_INDEX_FORCE_EN                      BIT23

/* RFIP_ADDR_FORCE_RX_GAIN_VALUE */
#define FORCE_RX_GAIN_VALUE_RX_GAIN_FORCE_EN                            BIT21
#define FORCE_RX_GAIN_VALUE_RX_GAIN_LNA_BYP_FORCE_VALUE                 BIT20
#define FORCE_RX_GAIN_VALUE_RX_GAIN_LNA_P_FORCE_VALUE_MASK              CO_BITS(16, 19)
#define FORCE_RX_GAIN_VALUE_RX_GAIN_LNA_P_FORCE_VALUE_SHIFT             16
#define FORCE_RX_GAIN_VALUE_RX_GAIN_LNA_N_FORCE_VALUE_MASK              CO_BITS(12, 15)
#define FORCE_RX_GAIN_VALUE_RX_GAIN_LNA_N_FORCE_VALUE_SHIFT             12
#define FORCE_RX_GAIN_VALUE_RX_GAIN_PGA_FORCE_VALUE_MASK                CO_BITS(8, 11)
#define FORCE_RX_GAIN_VALUE_RX_GAIN_PGA_FORCE_VALUE_SHIFT               8
#define FORCE_RX_GAIN_VALUE_RX_GAIN_LPF1_FORCE_VALUE_MASK               CO_BITS(4, 7)
#define FORCE_RX_GAIN_VALUE_RX_GAIN_LPF1_FORCE_VALUE_SHIFT              4
#define FORCE_RX_GAIN_VALUE_RX_GAIN_LPF2_FORCE_VALUE_MASK               CO_BITS(0, 2)
#define FORCE_RX_GAIN_VALUE_RX_GAIN_LPF2_FORCE_VALUE_SHIFT              0

/* RFIP_ADDR_RX_LNA_GAIN_TABLE */
#define RX_LNA_GAIN_TABLE_RX_GAIN_LNA_3_MASK                            CO_BITS(24, 31)
#define RX_LNA_GAIN_TABLE_RX_GAIN_LNA_3_SHIFT                           24
#define RX_LNA_GAIN_TABLE_RX_GAIN_LNA_2_MASK                            CO_BITS(16, 23)
#define RX_LNA_GAIN_TABLE_RX_GAIN_LNA_2_SHIFT                           16
#define RX_LNA_GAIN_TABLE_RX_GAIN_LNA_1_MASK                            CO_BITS(8, 15)
#define RX_LNA_GAIN_TABLE_RX_GAIN_LNA_1_SHIFT                           8
#define RX_LNA_GAIN_TABLE_RX_GAIN_LNA_0_MASK                            CO_BITS(0, 7)
#define RX_LNA_GAIN_TABLE_RX_GAIN_LNA_0_SHIFT                           0

/* RFIP_ADDR_RX_PGA_GAIN_TABLE */
#define RX_PGA_GAIN_TABLE_RX_GAIN_PGA_3_MASK                            CO_BITS(28, 31)
#define RX_PGA_GAIN_TABLE_RX_GAIN_PGA_3_SHIFT                           28
#define RX_PGA_GAIN_TABLE_RX_GAIN_PGA_2_MASK                            CO_BITS(24, 27)
#define RX_PGA_GAIN_TABLE_RX_GAIN_PGA_2_SHIFT                           24
#define RX_PGA_GAIN_TABLE_RX_GAIN_PGA_1_MASK                            CO_BITS(20, 23)
#define RX_PGA_GAIN_TABLE_RX_GAIN_PGA_1_SHIFT                           20
#define RX_PGA_GAIN_TABLE_RX_GAIN_PGA_0_MASK                            CO_BITS(16, 19)
#define RX_PGA_GAIN_TABLE_RX_GAIN_PGA_0_SHIFT                           16
#define RX_PGA_GAIN_TABLE_RX_GAIN_LPF_9_MASK                            CO_BITS(8, 15)
#define RX_PGA_GAIN_TABLE_RX_GAIN_LPF_9_SHIFT                           8
#define RX_PGA_GAIN_TABLE_RX_GAIN_LPF_8_MASK                            CO_BITS(0, 7)
#define RX_PGA_GAIN_TABLE_RX_GAIN_LPF_8_SHIFT                           0

/* RFIP_ADDR_RX_LPF_GAIN_TABLE1 */
#define RX_LPF_GAIN_TABLE1_RX_GAIN_LPF_7_MASK                           CO_BITS(24, 31)
#define RX_LPF_GAIN_TABLE1_RX_GAIN_LPF_7_SHIFT                          24
#define RX_LPF_GAIN_TABLE1_RX_GAIN_LPF_6_MASK                           CO_BITS(16, 23)
#define RX_LPF_GAIN_TABLE1_RX_GAIN_LPF_6_SHIFT                          16
#define RX_LPF_GAIN_TABLE1_RX_GAIN_LPF_5_MASK                           CO_BITS(8, 15)
#define RX_LPF_GAIN_TABLE1_RX_GAIN_LPF_5_SHIFT                          8
#define RX_LPF_GAIN_TABLE1_RX_GAIN_LPF_4_MASK                           CO_BITS(0, 7)
#define RX_LPF_GAIN_TABLE1_RX_GAIN_LPF_4_SHIFT                          0

/* RFIP_ADDR_RX_LPF_GAIN_TABLE2 */
#define RX_LPF_GAIN_TABLE2_RX_GAIN_LPF_3_MASK                           CO_BITS(24, 31)
#define RX_LPF_GAIN_TABLE2_RX_GAIN_LPF_3_SHIFT                          24
#define RX_LPF_GAIN_TABLE2_RX_GAIN_LPF_2_MASK                           CO_BITS(16, 23)
#define RX_LPF_GAIN_TABLE2_RX_GAIN_LPF_2_SHIFT                          16
#define RX_LPF_GAIN_TABLE2_RX_GAIN_LPF_1_MASK                           CO_BITS(8, 15)
#define RX_LPF_GAIN_TABLE2_RX_GAIN_LPF_1_SHIFT                          8
#define RX_LPF_GAIN_TABLE2_RX_GAIN_LPF_0_MASK                           CO_BITS(0, 7)
#define RX_LPF_GAIN_TABLE2_RX_GAIN_LPF_0_SHIFT                          0

/* RFIP_ADDR_FORCE_TX_GAIN_INDEX */
#define FORCE_TX_GAIN_INDEX_TX_GAIN_INDEX_FORCE_EN                      BIT31
#define FORCE_TX_GAIN_INDEX_TX_GAIN_INDEX_VALUE_MASK                    CO_BITS(24, 30)
#define FORCE_TX_GAIN_INDEX_TX_GAIN_INDEX_VALUE_SHIFT                   24
#define FORCE_TX_GAIN_INDEX_RX_GAIN_INDEX_MASK                          CO_BITS(12, 19)
#define FORCE_TX_GAIN_INDEX_RX_GAIN_INDEX_SHIFT                         12
#define FORCE_TX_GAIN_INDEX_TX_RF_GAIN_O_MASK                           CO_BITS(0, 11)
#define FORCE_TX_GAIN_INDEX_TX_RF_GAIN_O_SHIFT                          0

/* RFIP_ADDR_FORCE_TX_GAIN_VALUE */
#define FORCE_TX_GAIN_VALUE_REG_TXDAC_RANGE_MASK                        CO_BITS(24, 25)
#define FORCE_TX_GAIN_VALUE_REG_TXDAC_RANGE_SHIFT                       24
#define FORCE_TX_GAIN_VALUE_PA_GAIN_FORCE_VALUE_MASK                    CO_BITS(22, 23)
#define FORCE_TX_GAIN_VALUE_PA_GAIN_FORCE_VALUE_SHIFT                   22
#define FORCE_TX_GAIN_VALUE_PAD_GAIN_FORCE_VALUE_MASK                   CO_BITS(20, 21)
#define FORCE_TX_GAIN_VALUE_PAD_GAIN_FORCE_VALUE_SHIFT                  20
#define FORCE_TX_GAIN_VALUE_MIX1_GAIN_FORCE_VALUE_MASK                  CO_BITS(18, 19)
#define FORCE_TX_GAIN_VALUE_MIX1_GAIN_FORCE_VALUE_SHIFT                 18
#define FORCE_TX_GAIN_VALUE_MIX2_GAIN_FORCE_VALUE_MASK                  CO_BITS(15, 17)
#define FORCE_TX_GAIN_VALUE_MIX2_GAIN_FORCE_VALUE_SHIFT                 15
#define FORCE_TX_GAIN_VALUE_FILTER_GAIN_FORCE_VALUE_MASK                CO_BITS(12, 14)
#define FORCE_TX_GAIN_VALUE_FILTER_GAIN_FORCE_VALUE_SHIFT               12
#define FORCE_TX_GAIN_VALUE_TX_RF_GAIN_FORCE_EN                         BIT10
#define FORCE_TX_GAIN_VALUE_TX_DIG_GAIN_FORCE_EN                        BIT9
#define FORCE_TX_GAIN_VALUE_TX_DIG_GAIN_FORCE_VALUE_MASK                CO_BITS(0, 8)
#define FORCE_TX_GAIN_VALUE_TX_DIG_GAIN_FORCE_VALUE_SHIFT               0

/* RFIP_ADDR_RO_TX_DIG_GAIN_VALUE */
#define RO_TX_DIG_GAIN_VALUE_SCALE_FACTOR_MASK                          CO_BITS(21, 31)
#define RO_TX_DIG_GAIN_VALUE_SCALE_FACTOR_SHIFT                         21
#define RO_TX_DIG_GAIN_VALUE_TX_DIG_GAIN_DB_MASK                        CO_BITS(0, 8)
#define RO_TX_DIG_GAIN_VALUE_TX_DIG_GAIN_DB_SHIFT                       0

/* RFIP_ADDR_FORCE_TX_COMP */
#define FORCE_TX_COMP_TX_I_COMP_FORCE_EN                                BIT25
#define FORCE_TX_COMP_TX_Q_COMP_FORCE_EN                                BIT24
#define FORCE_TX_COMP_TX_I_COMP_FORCE_VALUE_MASK                        CO_BITS(12, 23)
#define FORCE_TX_COMP_TX_I_COMP_FORCE_VALUE_SHIFT                       12
#define FORCE_TX_COMP_TX_Q_COMP_FORCE_VALUE_MASK                        CO_BITS(0, 11)
#define FORCE_TX_COMP_TX_Q_COMP_FORCE_VALUE_SHIFT                       0

/* RFIP_ADDR_RFPLL_FSM_STATE_OFF_LDOON */
#define RFPLL_FSM_STATE_OFF_LDOON_RFIP_RFPLL_PS_OFF_MASK                CO_BITS(16, 23)
#define RFPLL_FSM_STATE_OFF_LDOON_RFIP_RFPLL_PS_OFF_SHIFT               16
#define RFPLL_FSM_STATE_OFF_LDOON_RFIP_RFPLL_PS_LDOON_MASK              CO_BITS(0, 7)
#define RFPLL_FSM_STATE_OFF_LDOON_RFIP_RFPLL_PS_LDOON_SHIFT             0

/* RFIP_ADDR_RFPLL_FSM_STATE_VCOON_MDLL */
#define RFPLL_FSM_STATE_VCOON_MDLL_RFIP_RFPLL_PS_VCOON_MASK             CO_BITS(16, 23)
#define RFPLL_FSM_STATE_VCOON_MDLL_RFIP_RFPLL_PS_VCOON_SHIFT            16
#define RFPLL_FSM_STATE_VCOON_MDLL_RFIP_RFPLL_PS_MDLL_MASK              CO_BITS(0, 7)
#define RFPLL_FSM_STATE_VCOON_MDLL_RFIP_RFPLL_PS_MDLL_SHIFT             0

/* RFIP_ADDR_RFPLL_FSM_STATE_VCOCAL_ACTIVE */
#define RFPLL_FSM_STATE_VCOCAL_ACTIVE_RFIP_RFPLL_PS_VCOCAL_MASK         CO_BITS(16, 23)
#define RFPLL_FSM_STATE_VCOCAL_ACTIVE_RFIP_RFPLL_PS_VCOCAL_SHIFT        16
#define RFPLL_FSM_STATE_VCOCAL_ACTIVE_RFIP_RFPLL_PS_ACTIVE_MASK         CO_BITS(0, 7)
#define RFPLL_FSM_STATE_VCOCAL_ACTIVE_RFIP_RFPLL_PS_ACTIVE_SHIFT        0

/* RFIP_ADDR_RX_FSM_STATE_OFF_ANAON */
#define RX_FSM_STATE_OFF_ANAON_OFF_PU_RXPKDET1                          BIT29
#define RX_FSM_STATE_OFF_ANAON_OFF_PU_RXPKDET0                          BIT28
#define RX_FSM_STATE_OFF_ANAON_OFF_EN_CLK_PLLS_80_SAR                   BIT27
#define RX_FSM_STATE_OFF_ANAON_OFF_PU_SARADC                            BIT26
#define RX_FSM_STATE_OFF_ANAON_OFF_PU_RXFLT2                            BIT25
#define RX_FSM_STATE_OFF_ANAON_OFF_PU_RXFLT1                            BIT24
#define RX_FSM_STATE_OFF_ANAON_OFF_PU_PGA                               BIT23
#define RX_FSM_STATE_OFF_ANAON_OFF_PU_OSCAL                             BIT22
#define RX_FSM_STATE_OFF_ANAON_OFF_VCO_RXBUF_EN                         BIT21
#define RX_FSM_STATE_OFF_ANAON_OFF_PU_RXLO                              BIT20
#define RX_FSM_STATE_OFF_ANAON_OFF_PU_TIA                               BIT19
#define RX_FSM_STATE_OFF_ANAON_OFF_PU_RMIX                              BIT18
#define RX_FSM_STATE_OFF_ANAON_OFF_PU_LNA                               BIT17
#define RX_FSM_STATE_OFF_ANAON_OFF_EN_BGCR_RX                           BIT16
#define RX_FSM_STATE_OFF_ANAON_ANAON_PU_RXPKDET1                        BIT13
#define RX_FSM_STATE_OFF_ANAON_ANAON_PU_RXPKDET0                        BIT12
#define RX_FSM_STATE_OFF_ANAON_ANAON_EN_CLK_PLLS_80_SAR                 BIT11
#define RX_FSM_STATE_OFF_ANAON_ANAON_PU_SARADC                          BIT10
#define RX_FSM_STATE_OFF_ANAON_ANAON_PU_RXFLT2                          BIT9
#define RX_FSM_STATE_OFF_ANAON_ANAON_PU_RXFLT1                          BIT8
#define RX_FSM_STATE_OFF_ANAON_ANAON_PU_PGA                             BIT7
#define RX_FSM_STATE_OFF_ANAON_ANAON_PU_OSCAL                           BIT6
#define RX_FSM_STATE_OFF_ANAON_ANAON_VCO_RXBUF_EN                       BIT5
#define RX_FSM_STATE_OFF_ANAON_ANAON_PU_RXLO                            BIT4
#define RX_FSM_STATE_OFF_ANAON_ANAON_PU_TIA                             BIT3
#define RX_FSM_STATE_OFF_ANAON_ANAON_PU_RMIX                            BIT2
#define RX_FSM_STATE_OFF_ANAON_ANAON_PU_LNA                             BIT1
#define RX_FSM_STATE_OFF_ANAON_ANAON_EN_BGCR_RX                         BIT0

/* RFIP_ADDR_RX_FSM_STATE_IDLE_ACTIVE */
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_PU_RXPKDET1                       BIT29
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_PU_RXPKDET0                       BIT28
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_EN_CLK_PLLS_80_SAR                BIT27
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_PU_SARADC                         BIT26
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_PU_RXFLT2                         BIT25
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_PU_RXFLT1                         BIT24
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_PU_PGA                            BIT23
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_PU_OSCAL                          BIT22
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_VCO_RXBUF_EN                      BIT21
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_PU_RXLO                           BIT20
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_PU_TIA                            BIT19
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_PU_RMIX                           BIT18
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_PU_LNA                            BIT17
#define RX_FSM_STATE_IDLE_ACTIVE_IDLE_EN_BGCR_RX                        BIT16
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_PU_RXPKDET1                     BIT13
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_PU_RXPKDET0                     BIT12
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_EN_CLK_PLLS_80_SAR              BIT11
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_PU_SARADC                       BIT10
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_PU_RXFLT2                       BIT9
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_PU_RXFLT1                       BIT8
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_PU_PGA                          BIT7
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_PU_OSCAL                        BIT6
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_VCO_RXBUF_EN                    BIT5
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_PU_RXLO                         BIT4
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_PU_TIA                          BIT3
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_PU_RMIX                         BIT2
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_PU_LNA                          BIT1
#define RX_FSM_STATE_IDLE_ACTIVE_ACTIVE_EN_BGCR_RX                      BIT0

/* RFIP_ADDR_TX_FSM_STATE_OFF_ANAON */
#define TX_FSM_STATE_OFF_ANAON_OFF_EN_CLK_PLLS_TXINTFC                  BIT26
#define TX_FSM_STATE_OFF_ANAON_OFF_EN_BGCR_TX                           BIT25
#define TX_FSM_STATE_OFF_ANAON_OFF_EN_PA_OPP                            BIT24
#define TX_FSM_STATE_OFF_ANAON_OFF_PU_TXDAC                             BIT23
#define TX_FSM_STATE_OFF_ANAON_OFF_PU_TXFILTER                          BIT22
#define TX_FSM_STATE_OFF_ANAON_OFF_PU_TXMIXER_LO                        BIT21
#define TX_FSM_STATE_OFF_ANAON_OFF_PU_TXMIXER                           BIT20
#define TX_FSM_STATE_OFF_ANAON_OFF_PU_PAD                               BIT19
#define TX_FSM_STATE_OFF_ANAON_OFF_PU_PA                                BIT18
#define TX_FSM_STATE_OFF_ANAON_OFF_VCO_TXBUF_EN                         BIT17
#define TX_FSM_STATE_OFF_ANAON_OFF_EN_TX                                BIT16
#define TX_FSM_STATE_OFF_ANAON_ANAON_EN_CLK_PLLS_TXINTFC                BIT10
#define TX_FSM_STATE_OFF_ANAON_ANAON_EN_BGCR_TX                         BIT9
#define TX_FSM_STATE_OFF_ANAON_ANAON_EN_PA_OPP                          BIT8
#define TX_FSM_STATE_OFF_ANAON_ANAON_PU_TXDAC                           BIT7
#define TX_FSM_STATE_OFF_ANAON_ANAON_PU_TXFILTER                        BIT6
#define TX_FSM_STATE_OFF_ANAON_ANAON_PU_TXMIXER_LO                      BIT5
#define TX_FSM_STATE_OFF_ANAON_ANAON_PU_TXMIXER                         BIT4
#define TX_FSM_STATE_OFF_ANAON_ANAON_PU_PAD                             BIT3
#define TX_FSM_STATE_OFF_ANAON_ANAON_PU_PA                              BIT2
#define TX_FSM_STATE_OFF_ANAON_ANAON_VCO_TXBUF_EN                       BIT1
#define TX_FSM_STATE_OFF_ANAON_ANAON_EN_TX                              BIT0

/* RFIP_ADDR_TX_FSM_STATE_ACTIVE_RFOFF */
#define TX_FSM_STATE_ACTIVE_RFOFF_ACTIVE_EN_CLK_PLLS_TXINTFC            BIT26
#define TX_FSM_STATE_ACTIVE_RFOFF_ACTIVE_EN_BGCR_TX                     BIT25
#define TX_FSM_STATE_ACTIVE_RFOFF_ACTIVE_EN_PA_OPP                      BIT24
#define TX_FSM_STATE_ACTIVE_RFOFF_ACTIVE_PU_TXDAC                       BIT23
#define TX_FSM_STATE_ACTIVE_RFOFF_ACTIVE_PU_TXFILTER                    BIT22
#define TX_FSM_STATE_ACTIVE_RFOFF_ACTIVE_PU_TXMIXER_LO                  BIT21
#define TX_FSM_STATE_ACTIVE_RFOFF_ACTIVE_PU_TXMIXER                     BIT20
#define TX_FSM_STATE_ACTIVE_RFOFF_ACTIVE_PU_PAD                         BIT19
#define TX_FSM_STATE_ACTIVE_RFOFF_ACTIVE_PU_PA                          BIT18
#define TX_FSM_STATE_ACTIVE_RFOFF_ACTIVE_VCO_TXBUF_EN                   BIT17
#define TX_FSM_STATE_ACTIVE_RFOFF_ACTIVE_EN_TX                          BIT16
#define TX_FSM_STATE_ACTIVE_RFOFF_ANAON_EN_CLK_PLLS_TXINTFC             BIT10
#define TX_FSM_STATE_ACTIVE_RFOFF_ANAON_EN_BGCR_TX                      BIT9
#define TX_FSM_STATE_ACTIVE_RFOFF_ANAON_EN_PA_OPP                       BIT8
#define TX_FSM_STATE_ACTIVE_RFOFF_ANAON_PU_TXDAC                        BIT7
#define TX_FSM_STATE_ACTIVE_RFOFF_ANAON_PU_TXFILTER                     BIT6
#define TX_FSM_STATE_ACTIVE_RFOFF_ANAON_PU_TXMIXER_LO                   BIT5
#define TX_FSM_STATE_ACTIVE_RFOFF_ANAON_PU_TXMIXER                      BIT4
#define TX_FSM_STATE_ACTIVE_RFOFF_ANAON_PU_PAD                          BIT3
#define TX_FSM_STATE_ACTIVE_RFOFF_ANAON_PU_PA                           BIT2
#define TX_FSM_STATE_ACTIVE_RFOFF_ANAON_VCO_TXBUF_EN                    BIT1
#define TX_FSM_STATE_ACTIVE_RFOFF_ANAON_EN_TX                           BIT0

/* RFIP_ADDR_TX_FSM_STATE_DRON */
#define TX_FSM_STATE_DRON_DRON_EN_CLK_PLLS_TXINTFC                      BIT10
#define TX_FSM_STATE_DRON_DRON_EN_BGCR_TX                               BIT9
#define TX_FSM_STATE_DRON_DRON_EN_PA_OPP                                BIT8
#define TX_FSM_STATE_DRON_DRON_PU_TXDAC                                 BIT7
#define TX_FSM_STATE_DRON_DRON_PU_TXFILTER                              BIT6
#define TX_FSM_STATE_DRON_DRON_PU_TXMIXER_LO                            BIT5
#define TX_FSM_STATE_DRON_DRON_PU_TXMIXER                               BIT4
#define TX_FSM_STATE_DRON_DRON_PU_PAD                                   BIT3
#define TX_FSM_STATE_DRON_DRON_PU_PA                                    BIT2
#define TX_FSM_STATE_DRON_DRON_VCO_TXBUF_EN                             BIT1
#define TX_FSM_STATE_DRON_DRON_EN_TX                                    BIT0

/* RFIP_ADDR_RO_RX_GAIN_VALUE */
#define RO_RX_GAIN_VALUE_RX_GAIN_LNA_BYP                                BIT20
#define RO_RX_GAIN_VALUE_RX_GAIN_LNA_P_MASK                             CO_BITS(16, 19)
#define RO_RX_GAIN_VALUE_RX_GAIN_LNA_P_SHIFT                            16
#define RO_RX_GAIN_VALUE_RX_GAIN_LNA_N_MASK                             CO_BITS(12, 15)
#define RO_RX_GAIN_VALUE_RX_GAIN_LNA_N_SHIFT                            12
#define RO_RX_GAIN_VALUE_RX_GAIN_PGA_MASK                               CO_BITS(8, 11)
#define RO_RX_GAIN_VALUE_RX_GAIN_PGA_SHIFT                              8
#define RO_RX_GAIN_VALUE_RX_GAIN_LPF1_MASK                              CO_BITS(3, 6)
#define RO_RX_GAIN_VALUE_RX_GAIN_LPF1_SHIFT                             3
#define RO_RX_GAIN_VALUE_RX_GAIN_LPF2_MASK                              CO_BITS(0, 2)
#define RO_RX_GAIN_VALUE_RX_GAIN_LPF2_SHIFT                             0

/* RFIP_ADDR_VCO_CFG2 */
#define VCO_CFG2_VCO_FREQ_SHIFT_DF_MASK                                 CO_BITS(19, 20)
#define VCO_CFG2_VCO_FREQ_SHIFT_DF_SHIFT                                19
#define VCO_CFG2_VCO_FREQ_SHIFT_BTRX_MASK                               CO_BITS(17, 18)
#define VCO_CFG2_VCO_FREQ_SHIFT_BTRX_SHIFT                              17
#define VCO_CFG2_REG_RFPLL_DR                                           BIT16
#define VCO_CFG2_PU_DIV_VALUE                                           BIT15
#define VCO_CFG2_PU_RFPLL_REGDIV2_VALUE                                 BIT14
#define VCO_CFG2_PU_RFPLL_REGPRESC_VALUE                                BIT13
#define VCO_CFG2_PU_RFPLL_LOOP_VALUE                                    BIT12
#define VCO_CFG2_EN_RFPLL_LOOP_VREG_VALUE                               BIT11
#define VCO_CFG2_RFPLL_DIS_LF_REG                                       BIT10
#define VCO_CFG2_VCO_BAND_FORCE_EN                                      BIT9
#define VCO_CFG2_VCO_BAND_FORCE_VALUE_MASK                              CO_BITS(0, 8)
#define VCO_CFG2_VCO_BAND_FORCE_VALUE_SHIFT                             0

/* RFIP_ADDR_RFPLL_DIG_CFG1 */
#define RFPLL_DIG_CFG1_RFPLL_INT_SDMODE                                 BIT30
#define RFPLL_DIG_CFG1_RFPLL_FRAC_LSB_SET                               BIT29
#define RFPLL_DIG_CFG1_RFPLL_SDM_S1_X2P1_DIS                            BIT28
#define RFPLL_DIG_CFG1_RFPLL_SDM_S1_ACCU_SET                            BIT27
#define RFPLL_DIG_CFG1_RFPLL_DIGCKRST_DR                                BIT26
#define RFPLL_DIG_CFG1_XRST_SD_REG                                      BIT25
#define RFPLL_DIG_CFG1_XRST_SYNCH_REG                                   BIT24
#define RFPLL_DIG_CFG1_XRST_CLK_CTRL_REG                                BIT23
#define RFPLL_DIG_CFG1_RFPLL_MASHFIL_CKEN_REG                           BIT22
#define RFPLL_DIG_CFG1_RFPLL_SDCLK_EN_REG                               BIT21
#define RFPLL_DIG_CFG1_RFPLL_XPD_PIPELINE_LB_REG                        BIT20
#define RFPLL_DIG_CFG1_RFPLL_SD_FILT                                    BIT18
#define RFPLL_DIG_CFG1_RFPLL_SD_MOD_MASK                                CO_BITS(16, 17)
#define RFPLL_DIG_CFG1_RFPLL_SD_MOD_SHIFT                               16
#define RFPLL_DIG_CFG1_RFPLL_NDIVIDER_REG_EN                            BIT15
#define RFPLL_DIG_CFG1_RFPLL_NDIVIDER_REG_MASK                          CO_BITS(0, 14)
#define RFPLL_DIG_CFG1_RFPLL_NDIVIDER_REG_SHIFT                         0

/* RFIP_ADDR_RFPLL_DIG_CFG2 */
#define RFPLL_DIG_CFG2_RFPLL_SEL_FREQPLLIN_MASK                         CO_BITS(28, 31)
#define RFPLL_DIG_CFG2_RFPLL_SEL_FREQPLLIN_SHIFT                        28
#define RFPLL_DIG_CFG2_RFPLL_SEL_FCOMPMES_MASK                          CO_BITS(24, 27)
#define RFPLL_DIG_CFG2_RFPLL_SEL_FCOMPMES_SHIFT                         24
#define RFPLL_DIG_CFG2_RFPLL_ORDER_FWP_MASK                             CO_BITS(17, 23)
#define RFPLL_DIG_CFG2_RFPLL_ORDER_FWP_SHIFT                            17
#define RFPLL_DIG_CFG2_RFPLL_FREQ_CAL_ACCURACY_MASK                     CO_BITS(12, 15)
#define RFPLL_DIG_CFG2_RFPLL_FREQ_CAL_ACCURACY_SHIFT                    12
#define RFPLL_DIG_CFG2_RFPLL_FREQ_NUM_PERIOD_SET_MASK                   CO_BITS(9, 11)
#define RFPLL_DIG_CFG2_RFPLL_FREQ_NUM_PERIOD_SET_SHIFT                  9
#define RFPLL_DIG_CFG2_RFPLL_WAIT_FINE_F_MASK                           CO_BITS(6, 8)
#define RFPLL_DIG_CFG2_RFPLL_WAIT_FINE_F_SHIFT                          6
#define RFPLL_DIG_CFG2_RFPLL_AMP_EN                                     BIT5
#define RFPLL_DIG_CFG2_RFPLL_CO_AMP_BYPASS                              BIT4
#define RFPLL_DIG_CFG2_RFPLL_FINE_AMP_BYPASS                            BIT3
#define RFPLL_DIG_CFG2_RFPLL_CO_FREQ_BYPASS                             BIT2

/* RFIP_ADDR_RFPLL_VCOCAL_CFG1 */
#define RFPLL_VCOCAL_CFG1_RFPLL_CALFREQ_EXT_SEL                         BIT28
#define RFPLL_VCOCAL_CFG1_RFPLL_CALFREQ_EXT_MASK                        CO_BITS(16, 24)
#define RFPLL_VCOCAL_CFG1_RFPLL_CALFREQ_EXT_SHIFT                       16
#define RFPLL_VCOCAL_CFG1_RFPLL_CALFREQ_OUT_MASK                        CO_BITS(0, 8)
#define RFPLL_VCOCAL_CFG1_RFPLL_CALFREQ_OUT_SHIFT                       0

/* RFIP_ADDR_PLLS_CFG2 */
#define PLLS_CFG2_PLLS_INT_SDMODE                                       BIT9
#define PLLS_CFG2_PLLS_XRST                                             BIT8
#define PLLS_CFG2_PLLS_FRAC_LSB_SET                                     BIT7
#define PLLS_CFG2_PLLS_SDM_S1_X2P1_DIS                                  BIT6
#define PLLS_CFG2_PLLS_SDM_S1_ACCU_SET                                  BIT5
#define PLLS_CFG2_PLLS_SD_MOD_MASK                                      CO_BITS(1, 2)
#define PLLS_CFG2_PLLS_SD_MOD_SHIFT                                     1
#define PLLS_CFG2_PLLS_SD_FILT                                          BIT0

/* RFIP_ADDR_RO_FILTER_CCW */
#define RO_FILTER_CCW_RXFLT1_CCW_MASK                                   CO_BITS(12, 17)
#define RO_FILTER_CCW_RXFLT1_CCW_SHIFT                                  12
#define RO_FILTER_CCW_RXFLT2_CCW_MASK                                   CO_BITS(6, 11)
#define RO_FILTER_CCW_RXFLT2_CCW_SHIFT                                  6
#define RO_FILTER_CCW_REG_TXFLT_CCW_MASK                                CO_BITS(0, 5)
#define RO_FILTER_CCW_REG_TXFLT_CCW_SHIFT                               0

/* RFIP_ADDR_PA_BIAS_11N_CFG */
#define PA_BIAS_11N_CFG_XVR_BIAS_CFG_FORCE                              BIT20
#define PA_BIAS_11N_CFG_11N_PADBIAS_MODE_MASK                           CO_BITS(18, 19)
#define PA_BIAS_11N_CFG_11N_PADBIAS_MODE_SHIFT                          18
#define PA_BIAS_11N_CFG_11N_PABIAS_MODE_MASK                            CO_BITS(16, 17)
#define PA_BIAS_11N_CFG_11N_PABIAS_MODE_SHIFT                           16
#define PA_BIAS_11N_CFG_11N_MXIBIAS_BIT_MASK                            CO_BITS(14, 15)
#define PA_BIAS_11N_CFG_11N_MXIBIAS_BIT_SHIFT                           14
#define PA_BIAS_11N_CFG_11N_PADIB_BIT_MASK                              CO_BITS(11, 13)
#define PA_BIAS_11N_CFG_11N_PADIB_BIT_SHIFT                             11
#define PA_BIAS_11N_CFG_11N_PADCAS_BIT_MASK                             CO_BITS(7, 10)
#define PA_BIAS_11N_CFG_11N_PADCAS_BIT_SHIFT                            7
#define PA_BIAS_11N_CFG_11N_PAIB_BIT_MASK                               CO_BITS(4, 6)
#define PA_BIAS_11N_CFG_11N_PAIB_BIT_SHIFT                              4
#define PA_BIAS_11N_CFG_11N_PACAS_BIT_MASK                              CO_BITS(0, 3)
#define PA_BIAS_11N_CFG_11N_PACAS_BIT_SHIFT                             0

/* RFIP_ADDR_PA_BIAS_11B_CFG */
#define PA_BIAS_11B_CFG_11B_PADBIAS_MODE_MASK                           CO_BITS(18, 19)
#define PA_BIAS_11B_CFG_11B_PADBIAS_MODE_SHIFT                          18
#define PA_BIAS_11B_CFG_11B_PABIAS_MODE_MASK                            CO_BITS(16, 17)
#define PA_BIAS_11B_CFG_11B_PABIAS_MODE_SHIFT                           16
#define PA_BIAS_11B_CFG_11B_MXIBIAS_BIT_MASK                            CO_BITS(14, 15)
#define PA_BIAS_11B_CFG_11B_MXIBIAS_BIT_SHIFT                           14
#define PA_BIAS_11B_CFG_11B_PADIB_BIT_MASK                              CO_BITS(11, 13)
#define PA_BIAS_11B_CFG_11B_PADIB_BIT_SHIFT                             11
#define PA_BIAS_11B_CFG_11B_PADCAS_BIT_MASK                             CO_BITS(7, 10)
#define PA_BIAS_11B_CFG_11B_PADCAS_BIT_SHIFT                            7
#define PA_BIAS_11B_CFG_11B_PAIB_BIT_MASK                               CO_BITS(4, 6)
#define PA_BIAS_11B_CFG_11B_PAIB_BIT_SHIFT                              4
#define PA_BIAS_11B_CFG_11B_PACAS_BIT_MASK                              CO_BITS(0, 3)
#define PA_BIAS_11B_CFG_11B_PACAS_BIT_SHIFT                             0

/* RFIP_ADDR_PA_BIAS_BT_CFG */
#define PA_BIAS_BT_CFG_BT_PADBIAS_MODE_MASK                             CO_BITS(18, 19)
#define PA_BIAS_BT_CFG_BT_PADBIAS_MODE_SHIFT                            18
#define PA_BIAS_BT_CFG_BT_PABIAS_MODE_MASK                              CO_BITS(16, 17)
#define PA_BIAS_BT_CFG_BT_PABIAS_MODE_SHIFT                             16
#define PA_BIAS_BT_CFG_BT_MXIBIAS_BIT_MASK                              CO_BITS(14, 15)
#define PA_BIAS_BT_CFG_BT_MXIBIAS_BIT_SHIFT                             14
#define PA_BIAS_BT_CFG_BT_PADIB_BIT_MASK                                CO_BITS(11, 13)
#define PA_BIAS_BT_CFG_BT_PADIB_BIT_SHIFT                               11
#define PA_BIAS_BT_CFG_BT_PADCAS_BIT_MASK                               CO_BITS(7, 10)
#define PA_BIAS_BT_CFG_BT_PADCAS_BIT_SHIFT                              7
#define PA_BIAS_BT_CFG_BT_PAIB_BIT_MASK                                 CO_BITS(4, 6)
#define PA_BIAS_BT_CFG_BT_PAIB_BIT_SHIFT                                4
#define PA_BIAS_BT_CFG_BT_PACAS_BIT_MASK                                CO_BITS(0, 3)
#define PA_BIAS_BT_CFG_BT_PACAS_BIT_SHIFT                               0

/* RFIP_ADDR_TX_CALCOMP_WIFI */
#define TX_CALCOMP_WIFI_TXCAL_CLK_SEL_MASK                              CO_BITS(22, 23)
#define TX_CALCOMP_WIFI_TXCAL_CLK_SEL_SHIFT                             22
#define TX_CALCOMP_WIFI_TX_CALCOMP_N_MASK                               CO_BITS(8, 15)
#define TX_CALCOMP_WIFI_TX_CALCOMP_N_SHIFT                              8
#define TX_CALCOMP_WIFI_TX_CALCOMP_B_MASK                               CO_BITS(0, 7)
#define TX_CALCOMP_WIFI_TX_CALCOMP_B_SHIFT                              0

/* RFIP_ADDR_TX_CALCOMP_BT */
#define TX_CALCOMP_BT_TX_CALCOMP_BLE_MASK                               CO_BITS(0, 7)
#define TX_CALCOMP_BT_TX_CALCOMP_BLE_SHIFT                              0

/* RFIP_ADDR_TX_UPCONVERTER */
#define TX_UPCONVERTER_CLIP_FACTOR_CFG_MASK                             CO_BITS(20, 31)
#define TX_UPCONVERTER_CLIP_FACTOR_CFG_SHIFT                            20
#define TX_UPCONVERTER_UP_CONVERTER_SEL_X4_CFG2                         BIT8
#define TX_UPCONVERTER_UP_CONVERTER_SEL_X4_CFG                          BIT7
#define TX_UPCONVERTER_RX2TX_TEST_EN_CFG                                BIT6
#define TX_UPCONVERTER_TX_RF_IF_SWRST_CFG                               BIT4
#define TX_UPCONVERTER_TX_UPCONVERT_BYPASS_N                            BIT3

/* RFIP_ADDR_TX_DC_CFG1 */
#define TX_DC_CFG1_TX_DR_ON_DELAY_MASK                                  CO_BITS(16, 23)
#define TX_DC_CFG1_TX_DR_ON_DELAY_SHIFT                                 16
#define TX_DC_CFG1_TXDC_CAL_OUTI_DD                                     BIT15
#define TX_DC_CFG1_TX_DC_OFFSET_I_CFG_MASK                              CO_BITS(8, 14)
#define TX_DC_CFG1_TX_DC_OFFSET_I_CFG_SHIFT                             8
#define TX_DC_CFG1_TXDC_CAL_OUTQ_DD                                     BIT7
#define TX_DC_CFG1_TX_DC_OFFSET_Q_CFG_MASK                              CO_BITS(0, 6)
#define TX_DC_CFG1_TX_DC_OFFSET_Q_CFG_SHIFT                             0

/* RFIP_ADDR_DELAY_TX_RX_STATE */
#define DELAY_TX_RX_STATE_RX_ANA_ON_DELAY_MASK                          CO_BITS(24, 31)
#define DELAY_TX_RX_STATE_RX_ANA_ON_DELAY_SHIFT                         24
#define DELAY_TX_RX_STATE_RX_IDLE_DELAY_MASK                            CO_BITS(16, 23)
#define DELAY_TX_RX_STATE_RX_IDLE_DELAY_SHIFT                           16
#define DELAY_TX_RX_STATE_TX_ANA_ON_DELAY_MASK                          CO_BITS(8, 15)
#define DELAY_TX_RX_STATE_TX_ANA_ON_DELAY_SHIFT                         8
#define DELAY_TX_RX_STATE_TX_RFOFF_DELAY_MASK                           CO_BITS(0, 7)
#define DELAY_TX_RX_STATE_TX_RFOFF_DELAY_SHIFT                          0

/* RFIP_ADDR_DELAY_RFPLL_STATE */
#define DELAY_RFPLL_STATE_RFPLL_VCOCAL_DELAY_MASK                       CO_BITS(24, 31)
#define DELAY_RFPLL_STATE_RFPLL_VCOCAL_DELAY_SHIFT                      24
#define DELAY_RFPLL_STATE_RFPLL_MDLLLOCK_DELAY_MASK                     CO_BITS(16, 23)
#define DELAY_RFPLL_STATE_RFPLL_MDLLLOCK_DELAY_SHIFT                    16
#define DELAY_RFPLL_STATE_RFPLL_VCOON_DELAY_MASK                        CO_BITS(8, 15)
#define DELAY_RFPLL_STATE_RFPLL_VCOON_DELAY_SHIFT                       8
#define DELAY_RFPLL_STATE_RFPLL_LDOON_DELAY_MASK                        CO_BITS(0, 7)
#define DELAY_RFPLL_STATE_RFPLL_LDOON_DELAY_SHIFT                       0

/* RFIP_ADDR_PLLS_CFG3 */
#define PLLS_CFG3_PLLS_LCK_NUM_MASK                                     CO_BITS(24, 31)
#define PLLS_CFG3_PLLS_LCK_NUM_SHIFT                                    24
#define PLLS_CFG3_PU_PLLS_OFFDLY_CFG_MASK                               CO_BITS(8, 12)
#define PLLS_CFG3_PU_PLLS_OFFDLY_CFG_SHIFT                              8
#define PLLS_CFG3_RXINVADCCLK                                           BIT3

/* RFIP_ADDR_TX_UPCONVERTER_COEF0 */
#define TX_UPCONVERTER_COEF0_IN11N_CBW20_TX_COEFF2_MASK                 CO_BITS(20, 29)
#define TX_UPCONVERTER_COEF0_IN11N_CBW20_TX_COEFF2_SHIFT                20
#define TX_UPCONVERTER_COEF0_IN11N_CBW20_TX_COEFF1_MASK                 CO_BITS(10, 19)
#define TX_UPCONVERTER_COEF0_IN11N_CBW20_TX_COEFF1_SHIFT                10
#define TX_UPCONVERTER_COEF0_IN11N_CBW20_TX_COEFF0_MASK                 CO_BITS(0, 9)
#define TX_UPCONVERTER_COEF0_IN11N_CBW20_TX_COEFF0_SHIFT                0

/* RFIP_ADDR_TX_UPCONVERTER_COEF1 */
#define TX_UPCONVERTER_COEF1_IN11N_CBW20_TX_COEFF5_MASK                 CO_BITS(20, 29)
#define TX_UPCONVERTER_COEF1_IN11N_CBW20_TX_COEFF5_SHIFT                20
#define TX_UPCONVERTER_COEF1_IN11N_CBW20_TX_COEFF4_MASK                 CO_BITS(10, 19)
#define TX_UPCONVERTER_COEF1_IN11N_CBW20_TX_COEFF4_SHIFT                10
#define TX_UPCONVERTER_COEF1_IN11N_CBW20_TX_COEFF3_MASK                 CO_BITS(0, 9)
#define TX_UPCONVERTER_COEF1_IN11N_CBW20_TX_COEFF3_SHIFT                0

/* RFIP_ADDR_TX_UPCONVERTER_COEF2 */
#define TX_UPCONVERTER_COEF2_IN11N_CBW20_TX_COEFF8_MASK                 CO_BITS(20, 29)
#define TX_UPCONVERTER_COEF2_IN11N_CBW20_TX_COEFF8_SHIFT                20
#define TX_UPCONVERTER_COEF2_IN11N_CBW20_TX_COEFF7_MASK                 CO_BITS(10, 19)
#define TX_UPCONVERTER_COEF2_IN11N_CBW20_TX_COEFF7_SHIFT                10
#define TX_UPCONVERTER_COEF2_IN11N_CBW20_TX_COEFF6_MASK                 CO_BITS(0, 9)
#define TX_UPCONVERTER_COEF2_IN11N_CBW20_TX_COEFF6_SHIFT                0

/* RFIP_ADDR_TX_UPCONVERTER_COEF3 */
#define TX_UPCONVERTER_COEF3_IN11N_CBW20_TX_COEFF10_MASK                CO_BITS(10, 19)
#define TX_UPCONVERTER_COEF3_IN11N_CBW20_TX_COEFF10_SHIFT               10
#define TX_UPCONVERTER_COEF3_IN11N_CBW20_TX_COEFF9_MASK                 CO_BITS(0, 9)
#define TX_UPCONVERTER_COEF3_IN11N_CBW20_TX_COEFF9_SHIFT                0

/* RFIP_ADDR_TX_UPCONVERTER_COEF4 */
#define TX_UPCONVERTER_COEF4_IN11B_CBW20_TX_COEFF2_MASK                 CO_BITS(20, 29)
#define TX_UPCONVERTER_COEF4_IN11B_CBW20_TX_COEFF2_SHIFT                20
#define TX_UPCONVERTER_COEF4_IN11B_CBW20_TX_COEFF1_MASK                 CO_BITS(10, 19)
#define TX_UPCONVERTER_COEF4_IN11B_CBW20_TX_COEFF1_SHIFT                10
#define TX_UPCONVERTER_COEF4_IN11B_CBW20_TX_COEFF0_MASK                 CO_BITS(0, 9)
#define TX_UPCONVERTER_COEF4_IN11B_CBW20_TX_COEFF0_SHIFT                0

/* RFIP_ADDR_TX_UPCONVERTER_COEF5 */
#define TX_UPCONVERTER_COEF5_IN11B_CBW20_TX_COEFF5_MASK                 CO_BITS(20, 29)
#define TX_UPCONVERTER_COEF5_IN11B_CBW20_TX_COEFF5_SHIFT                20
#define TX_UPCONVERTER_COEF5_IN11B_CBW20_TX_COEFF4_MASK                 CO_BITS(10, 19)
#define TX_UPCONVERTER_COEF5_IN11B_CBW20_TX_COEFF4_SHIFT                10
#define TX_UPCONVERTER_COEF5_IN11B_CBW20_TX_COEFF3_MASK                 CO_BITS(0, 9)
#define TX_UPCONVERTER_COEF5_IN11B_CBW20_TX_COEFF3_SHIFT                0

/* RFIP_ADDR_TX_UPCONVERTER_COEF6 */
#define TX_UPCONVERTER_COEF6_IN11B_CBW20_TX_COEFF8_MASK                 CO_BITS(20, 29)
#define TX_UPCONVERTER_COEF6_IN11B_CBW20_TX_COEFF8_SHIFT                20
#define TX_UPCONVERTER_COEF6_IN11B_CBW20_TX_COEFF7_MASK                 CO_BITS(10, 19)
#define TX_UPCONVERTER_COEF6_IN11B_CBW20_TX_COEFF7_SHIFT                10
#define TX_UPCONVERTER_COEF6_IN11B_CBW20_TX_COEFF6_MASK                 CO_BITS(0, 9)
#define TX_UPCONVERTER_COEF6_IN11B_CBW20_TX_COEFF6_SHIFT                0

/* RFIP_ADDR_TX_UPCONVERTER_COEF7 */
#define TX_UPCONVERTER_COEF7_IN11B_CBW20_TX_COEFF10_MASK                CO_BITS(10, 19)
#define TX_UPCONVERTER_COEF7_IN11B_CBW20_TX_COEFF10_SHIFT               10
#define TX_UPCONVERTER_COEF7_IN11B_CBW20_TX_COEFF9_MASK                 CO_BITS(0, 9)
#define TX_UPCONVERTER_COEF7_IN11B_CBW20_TX_COEFF9_SHIFT                0

/* RFIP_ADDR_RO_RFPLL_DEBUG_STATUS1 */
#define RO_RFPLL_DEBUG_STATUS1_RFPLL_RF_COUNTER_DONE_OUT_MASK           CO_BITS(18, 31)
#define RO_RFPLL_DEBUG_STATUS1_RFPLL_RF_COUNTER_DONE_OUT_SHIFT          18
#define RO_RFPLL_DEBUG_STATUS1_RFPLL_FREQ_CALIBRE_MEM_M_OUT_MASK        CO_BITS(9, 17)
#define RO_RFPLL_DEBUG_STATUS1_RFPLL_FREQ_CALIBRE_MEM_M_OUT_SHIFT       9
#define RO_RFPLL_DEBUG_STATUS1_RFPLL_FREQ_CALIBRE_MEM_P_OUT_MASK        CO_BITS(0, 8)
#define RO_RFPLL_DEBUG_STATUS1_RFPLL_FREQ_CALIBRE_MEM_P_OUT_SHIFT       0

/* RFIP_ADDR_RO_RFPLL_DEBUG_STATUS2 */
#define RO_RFPLL_DEBUG_STATUS2_RFPLL_MSB_RF_COUNTER_OUT                 BIT28
#define RO_RFPLL_DEBUG_STATUS2_RFPLL_DIFF_MEM_M_OUT_MASK                CO_BITS(14, 27)
#define RO_RFPLL_DEBUG_STATUS2_RFPLL_DIFF_MEM_M_OUT_SHIFT               14
#define RO_RFPLL_DEBUG_STATUS2_RFPLL_DIFF_MEM_P_OUT_MASK                CO_BITS(0, 13)
#define RO_RFPLL_DEBUG_STATUS2_RFPLL_DIFF_MEM_P_OUT_SHIFT               0

/* RFIP_ADDR_RO_TX_RF_DEBUG_STATUS */
#define RO_TX_RF_DEBUG_STATUS_IN_TXDC_COMP_FACTOR_I_MASK                CO_BITS(20, 31)
#define RO_TX_RF_DEBUG_STATUS_IN_TXDC_COMP_FACTOR_I_SHIFT               20
#define RO_TX_RF_DEBUG_STATUS_IN_TXDC_COMP_FACTOR_Q_MASK                CO_BITS(8, 19)
#define RO_TX_RF_DEBUG_STATUS_IN_TXDC_COMP_FACTOR_Q_SHIFT               8
#define RO_TX_RF_DEBUG_STATUS_TX_AFIFO_ERR                              BIT1

/* RFIP_ADDR_AUXADC_CFG */
#define AUXADC_CFG_AUXADC_ON                                            BIT31
#define AUXADC_CFG_AUXADC_IN_CHSEL_MASK                                 CO_BITS(29, 30)
#define AUXADC_CFG_AUXADC_IN_CHSEL_SHIFT                                29
#define AUXADC_CFG_AUXADC_N_SAMP_MASK                                   CO_BITS(23, 28)
#define AUXADC_CFG_AUXADC_N_SAMP_SHIFT                                  23
#define AUXADC_CFG_AUXADC_SINGLE_SHOT                                   BIT22
#define AUXADC_CFG_AUXADC_CKEN                                          BIT21
#define AUXADC_CFG_AUXADC_CKDIV_MASK                                    CO_BITS(11, 16)
#define AUXADC_CFG_AUXADC_CKDIV_SHIFT                                   11
#define AUXADC_CFG_AUXADC_DATA_P_MASK                                   CO_BITS(1, 10)
#define AUXADC_CFG_AUXADC_DATA_P_SHIFT                                  1
#define AUXADC_CFG_AUXADC_DATA_VALID_FLAG                               BIT0

/* RFIP_ADDR_RC_CAL_CFG */
#define RC_CAL_CFG_RCCAL_REQ                                            BIT28
#define RC_CAL_CFG_RCCAL_NUM_MASK                                       CO_BITS(26, 27)
#define RC_CAL_CFG_RCCAL_NUM_SHIFT                                      26
#define RC_CAL_CFG_RCCAL_CLK_CFG_3_MASK                                 CO_BITS(16, 21)
#define RC_CAL_CFG_RCCAL_CLK_CFG_3_SHIFT                                16
#define RC_CAL_CFG_RCCAL_CLK_CFG_2_MASK                                 CO_BITS(8, 13)
#define RC_CAL_CFG_RCCAL_CLK_CFG_2_SHIFT                                8
#define RC_CAL_CFG_RCCAL_CLK_CFG_1_MASK                                 CO_BITS(0, 5)
#define RC_CAL_CFG_RCCAL_CLK_CFG_1_SHIFT                                0

/* RFIP_ADDR_RC_CAL_RESULT */
#define RC_CAL_RESULT_RCCAL_FINISH_D3                                   BIT31
#define RC_CAL_RESULT_RXFLT_CCW_CFG_MASK                                CO_BITS(24, 27)
#define RC_CAL_RESULT_RXFLT_CCW_CFG_SHIFT                               24
#define RC_CAL_RESULT_TXFLT_CCW_CFG_MASK                                CO_BITS(20, 23)
#define RC_CAL_RESULT_TXFLT_CCW_CFG_SHIFT                               20
#define RC_CAL_RESULT_RCCAL_ASSIGN_TX_MASK                              CO_BITS(16, 19)
#define RC_CAL_RESULT_RCCAL_ASSIGN_TX_SHIFT                             16
#define RC_CAL_RESULT_RCCAL_ASSIGN_RX_MASK                              CO_BITS(12, 15)
#define RC_CAL_RESULT_RCCAL_ASSIGN_RX_SHIFT                             12
#define RC_CAL_RESULT_RCCAL_RESULT_3_MASK                               CO_BITS(8, 11)
#define RC_CAL_RESULT_RCCAL_RESULT_3_SHIFT                              8
#define RC_CAL_RESULT_RCCAL_RESULT_2_MASK                               CO_BITS(4, 7)
#define RC_CAL_RESULT_RCCAL_RESULT_2_SHIFT                              4
#define RC_CAL_RESULT_RCCAL_RESULT_1_MASK                               CO_BITS(0, 3)
#define RC_CAL_RESULT_RCCAL_RESULT_1_SHIFT                              0

/* RFIP_ADDR_FILTER_CFG */
#define FILTER_CFG_RXFLT_BW_2M_CFG                                      BIT5
#define FILTER_CFG_RXFLT_BW_2M_CFG2                                     BIT4
#define FILTER_CFG_RXFLT_BW_3M_CFG                                      BIT3
#define FILTER_CFG_RXFLT_BW_3M_CFG2                                     BIT2
#define FILTER_CFG_REG_TXFLT_11BMODE_11B                                BIT1
#define FILTER_CFG_REG_TXFLT_11BMODE_BLE                                BIT0

/* RFIP_ADDR_LOFT_COMP_REG0 */
#define LOFT_COMP_REG0_LOFT_COMP_FORCE_EN                               BIT16
#define LOFT_COMP_REG0_LOFT_COMP_I_CFG0_MASK                            CO_BITS(8, 15)
#define LOFT_COMP_REG0_LOFT_COMP_I_CFG0_SHIFT                           8
#define LOFT_COMP_REG0_LOFT_COMP_Q_CFG0_MASK                            CO_BITS(0, 7)
#define LOFT_COMP_REG0_LOFT_COMP_Q_CFG0_SHIFT                           0

/* RFIP_ADDR_LOFT_COMP_REG1 */
#define LOFT_COMP_REG1_LOFT_COMP_I_CFG1_MASK                            CO_BITS(8, 15)
#define LOFT_COMP_REG1_LOFT_COMP_I_CFG1_SHIFT                           8
#define LOFT_COMP_REG1_LOFT_COMP_Q_CFG1_MASK                            CO_BITS(0, 7)
#define LOFT_COMP_REG1_LOFT_COMP_Q_CFG1_SHIFT                           0

/* RFIP_ADDR_LOFT_COMP_REG2 */
#define LOFT_COMP_REG2_LOFT_COMP_I_CFG2_MASK                            CO_BITS(8, 15)
#define LOFT_COMP_REG2_LOFT_COMP_I_CFG2_SHIFT                           8
#define LOFT_COMP_REG2_LOFT_COMP_Q_CFG2_MASK                            CO_BITS(0, 7)
#define LOFT_COMP_REG2_LOFT_COMP_Q_CFG2_SHIFT                           0

/* RFIP_ADDR_LOFT_COMP_REG3 */
#define LOFT_COMP_REG3_LOFT_COMP_I_CFG3_MASK                            CO_BITS(8, 15)
#define LOFT_COMP_REG3_LOFT_COMP_I_CFG3_SHIFT                           8
#define LOFT_COMP_REG3_LOFT_COMP_Q_CFG3_MASK                            CO_BITS(0, 7)
#define LOFT_COMP_REG3_LOFT_COMP_Q_CFG3_SHIFT                           0

/* RFIP_ADDR_LOFT_COMP_REG4 */
#define LOFT_COMP_REG4_LOFT_COMP_I_CFG4_MASK                            CO_BITS(8, 15)
#define LOFT_COMP_REG4_LOFT_COMP_I_CFG4_SHIFT                           8
#define LOFT_COMP_REG4_LOFT_COMP_Q_CFG4_MASK                            CO_BITS(0, 7)
#define LOFT_COMP_REG4_LOFT_COMP_Q_CFG4_SHIFT                           0

/* RFIP_ADDR_LOFT_COMP_REG5 */
#define LOFT_COMP_REG5_LOFT_COMP_I_CFG5_MASK                            CO_BITS(8, 15)
#define LOFT_COMP_REG5_LOFT_COMP_I_CFG5_SHIFT                           8
#define LOFT_COMP_REG5_LOFT_COMP_Q_CFG5_MASK                            CO_BITS(0, 7)
#define LOFT_COMP_REG5_LOFT_COMP_Q_CFG5_SHIFT                           0

/* RFIP_ADDR_TX_RF_GAIN_CFG0 */
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_0_CFG0_MASK                          CO_BITS(30, 31)
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_0_CFG0_SHIFT                         30
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_1_CFG0_MASK                          CO_BITS(28, 29)
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_1_CFG0_SHIFT                         28
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_2_CFG0_MASK                          CO_BITS(26, 27)
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_2_CFG0_SHIFT                         26
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_3_CFG0_MASK                          CO_BITS(23, 25)
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_3_CFG0_SHIFT                         23
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_4_CFG0_MASK                          CO_BITS(20, 22)
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_4_CFG0_SHIFT                         20
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_0_CFG1_MASK                          CO_BITS(14, 15)
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_0_CFG1_SHIFT                         14
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_1_CFG1_MASK                          CO_BITS(12, 13)
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_1_CFG1_SHIFT                         12
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_2_CFG1_MASK                          CO_BITS(10, 11)
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_2_CFG1_SHIFT                         10
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_3_CFG1_MASK                          CO_BITS(7, 9)
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_3_CFG1_SHIFT                         7
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_4_CFG1_MASK                          CO_BITS(4, 6)
#define TX_RF_GAIN_CFG0_TX_RF_GAIN_4_CFG1_SHIFT                         4

/* RFIP_ADDR_TX_RF_GAIN_CFG1 */
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_0_CFG2_MASK                          CO_BITS(30, 31)
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_0_CFG2_SHIFT                         30
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_1_CFG2_MASK                          CO_BITS(28, 29)
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_1_CFG2_SHIFT                         28
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_2_CFG2_MASK                          CO_BITS(26, 27)
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_2_CFG2_SHIFT                         26
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_3_CFG2_MASK                          CO_BITS(23, 25)
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_3_CFG2_SHIFT                         23
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_4_CFG2_MASK                          CO_BITS(20, 22)
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_4_CFG2_SHIFT                         20
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_0_CFG3_MASK                          CO_BITS(14, 15)
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_0_CFG3_SHIFT                         14
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_1_CFG3_MASK                          CO_BITS(12, 13)
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_1_CFG3_SHIFT                         12
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_2_CFG3_MASK                          CO_BITS(10, 11)
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_2_CFG3_SHIFT                         10
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_3_CFG3_MASK                          CO_BITS(7, 9)
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_3_CFG3_SHIFT                         7
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_4_CFG3_MASK                          CO_BITS(4, 6)
#define TX_RF_GAIN_CFG1_TX_RF_GAIN_4_CFG3_SHIFT                         4

/* RFIP_ADDR_TX_RF_GAIN_CFG2 */
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_0_CFG4_MASK                          CO_BITS(30, 31)
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_0_CFG4_SHIFT                         30
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_1_CFG4_MASK                          CO_BITS(28, 29)
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_1_CFG4_SHIFT                         28
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_2_CFG4_MASK                          CO_BITS(26, 27)
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_2_CFG4_SHIFT                         26
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_3_CFG4_MASK                          CO_BITS(23, 25)
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_3_CFG4_SHIFT                         23
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_0_CFG5_MASK                          CO_BITS(14, 15)
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_0_CFG5_SHIFT                         14
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_1_CFG5_MASK                          CO_BITS(12, 13)
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_1_CFG5_SHIFT                         12
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_2_CFG5_MASK                          CO_BITS(10, 11)
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_2_CFG5_SHIFT                         10
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_3_CFG5_MASK                          CO_BITS(7, 9)
#define TX_RF_GAIN_CFG2_TX_RF_GAIN_3_CFG5_SHIFT                         7

/* RFIP_ADDR_TX_RF_GAIN_CFG3 */
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_0_CFG6_MASK                          CO_BITS(30, 31)
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_0_CFG6_SHIFT                         30
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_1_CFG6_MASK                          CO_BITS(28, 29)
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_1_CFG6_SHIFT                         28
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_2_CFG6_MASK                          CO_BITS(26, 27)
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_2_CFG6_SHIFT                         26
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_3_CFG6_MASK                          CO_BITS(23, 25)
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_3_CFG6_SHIFT                         23
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_0_CFG7_MASK                          CO_BITS(14, 15)
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_0_CFG7_SHIFT                         14
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_1_CFG7_MASK                          CO_BITS(12, 13)
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_1_CFG7_SHIFT                         12
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_2_CFG7_MASK                          CO_BITS(10, 11)
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_2_CFG7_SHIFT                         10
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_3_CFG7_MASK                          CO_BITS(7, 9)
#define TX_RF_GAIN_CFG3_TX_RF_GAIN_3_CFG7_SHIFT                         7

/* RFIP_ADDR_TX_RF_GAIN_CFG4 */
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_0_CFG8_MASK                          CO_BITS(30, 31)
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_0_CFG8_SHIFT                         30
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_1_CFG8_MASK                          CO_BITS(28, 29)
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_1_CFG8_SHIFT                         28
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_2_CFG8_MASK                          CO_BITS(26, 27)
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_2_CFG8_SHIFT                         26
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_3_CFG8_MASK                          CO_BITS(23, 25)
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_3_CFG8_SHIFT                         23
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_0_CFG9_MASK                          CO_BITS(14, 15)
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_0_CFG9_SHIFT                         14
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_1_CFG9_MASK                          CO_BITS(12, 13)
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_1_CFG9_SHIFT                         12
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_2_CFG9_MASK                          CO_BITS(10, 11)
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_2_CFG9_SHIFT                         10
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_3_CFG9_MASK                          CO_BITS(7, 9)
#define TX_RF_GAIN_CFG4_TX_RF_GAIN_3_CFG9_SHIFT                         7

/* RFIP_ADDR_TX_RF_GAIN_CFG5 */
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_0_CFG10_MASK                         CO_BITS(30, 31)
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_0_CFG10_SHIFT                        30
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_1_CFG10_MASK                         CO_BITS(28, 29)
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_1_CFG10_SHIFT                        28
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_2_CFG10_MASK                         CO_BITS(26, 27)
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_2_CFG10_SHIFT                        26
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_3_CFG10_MASK                         CO_BITS(23, 25)
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_3_CFG10_SHIFT                        23
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_0_CFG11_MASK                         CO_BITS(14, 15)
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_0_CFG11_SHIFT                        14
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_1_CFG11_MASK                         CO_BITS(12, 13)
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_1_CFG11_SHIFT                        12
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_2_CFG11_MASK                         CO_BITS(10, 11)
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_2_CFG11_SHIFT                        10
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_3_CFG11_MASK                         CO_BITS(7, 9)
#define TX_RF_GAIN_CFG5_TX_RF_GAIN_3_CFG11_SHIFT                        7

/* RFIP_ADDR_TX_RF_GAIN_CFG6 */
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_0_CFG12_MASK                         CO_BITS(30, 31)
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_0_CFG12_SHIFT                        30
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_1_CFG12_MASK                         CO_BITS(28, 29)
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_1_CFG12_SHIFT                        28
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_2_CFG12_MASK                         CO_BITS(26, 27)
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_2_CFG12_SHIFT                        26
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_3_CFG12_MASK                         CO_BITS(23, 25)
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_3_CFG12_SHIFT                        23
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_0_CFG13_MASK                         CO_BITS(14, 15)
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_0_CFG13_SHIFT                        14
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_1_CFG13_MASK                         CO_BITS(12, 13)
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_1_CFG13_SHIFT                        12
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_2_CFG13_MASK                         CO_BITS(10, 11)
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_2_CFG13_SHIFT                        10
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_3_CFG13_MASK                         CO_BITS(7, 9)
#define TX_RF_GAIN_CFG6_TX_RF_GAIN_3_CFG13_SHIFT                        7

/* RFIP_ADDR_TX_RF_GAIN_CFG7 */
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_0_CFG14_MASK                         CO_BITS(30, 31)
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_0_CFG14_SHIFT                        30
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_1_CFG14_MASK                         CO_BITS(28, 29)
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_1_CFG14_SHIFT                        28
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_2_CFG14_MASK                         CO_BITS(26, 27)
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_2_CFG14_SHIFT                        26
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_3_CFG14_MASK                         CO_BITS(23, 25)
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_3_CFG14_SHIFT                        23
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_0_CFG15_MASK                         CO_BITS(14, 15)
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_0_CFG15_SHIFT                        14
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_1_CFG15_MASK                         CO_BITS(12, 13)
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_1_CFG15_SHIFT                        12
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_2_CFG15_MASK                         CO_BITS(10, 11)
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_2_CFG15_SHIFT                        10
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_3_CFG15_MASK                         CO_BITS(7, 9)
#define TX_RF_GAIN_CFG7_TX_RF_GAIN_3_CFG15_SHIFT                        7

/* RFIP_ADDR_TX_RF_GAIN_CFG8 */
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_0_CFG16_MASK                         CO_BITS(30, 31)
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_0_CFG16_SHIFT                        30
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_1_CFG16_MASK                         CO_BITS(28, 29)
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_1_CFG16_SHIFT                        28
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_2_CFG16_MASK                         CO_BITS(26, 27)
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_2_CFG16_SHIFT                        26
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_3_CFG16_MASK                         CO_BITS(23, 25)
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_3_CFG16_SHIFT                        23
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_0_CFG17_MASK                         CO_BITS(14, 15)
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_0_CFG17_SHIFT                        14
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_1_CFG17_MASK                         CO_BITS(12, 13)
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_1_CFG17_SHIFT                        12
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_2_CFG17_MASK                         CO_BITS(10, 11)
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_2_CFG17_SHIFT                        10
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_3_CFG17_MASK                         CO_BITS(7, 9)
#define TX_RF_GAIN_CFG8_TX_RF_GAIN_3_CFG17_SHIFT                        7

/* RFIP_ADDR_TX_RF_GAIN_CFG9 */
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_0_CFG18_MASK                         CO_BITS(30, 31)
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_0_CFG18_SHIFT                        30
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_1_CFG18_MASK                         CO_BITS(28, 29)
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_1_CFG18_SHIFT                        28
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_2_CFG18_MASK                         CO_BITS(26, 27)
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_2_CFG18_SHIFT                        26
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_3_CFG18_MASK                         CO_BITS(23, 25)
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_3_CFG18_SHIFT                        23
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_0_CFG19_MASK                         CO_BITS(14, 15)
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_0_CFG19_SHIFT                        14
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_1_CFG19_MASK                         CO_BITS(12, 13)
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_1_CFG19_SHIFT                        12
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_2_CFG19_MASK                         CO_BITS(10, 11)
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_2_CFG19_SHIFT                        10
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_3_CFG19_MASK                         CO_BITS(7, 9)
#define TX_RF_GAIN_CFG9_TX_RF_GAIN_3_CFG19_SHIFT                        7

/* RFIP_ADDR_TX_RF_GAIN_CFG10 */
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_0_CFG20_MASK                        CO_BITS(30, 31)
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_0_CFG20_SHIFT                       30
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_1_CFG20_MASK                        CO_BITS(28, 29)
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_1_CFG20_SHIFT                       28
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_2_CFG20_MASK                        CO_BITS(26, 27)
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_2_CFG20_SHIFT                       26
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_3_CFG20_MASK                        CO_BITS(23, 25)
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_3_CFG20_SHIFT                       23
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_0_CFG21_MASK                        CO_BITS(14, 15)
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_0_CFG21_SHIFT                       14
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_1_CFG21_MASK                        CO_BITS(12, 13)
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_1_CFG21_SHIFT                       12
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_2_CFG21_MASK                        CO_BITS(10, 11)
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_2_CFG21_SHIFT                       10
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_3_CFG21_MASK                        CO_BITS(7, 9)
#define TX_RF_GAIN_CFG10_TX_RF_GAIN_3_CFG21_SHIFT                       7

/* RFIP_ADDR_TX_RF_GAIN_CFG11 */
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_0_CFG22_MASK                        CO_BITS(30, 31)
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_0_CFG22_SHIFT                       30
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_1_CFG22_MASK                        CO_BITS(28, 29)
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_1_CFG22_SHIFT                       28
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_2_CFG22_MASK                        CO_BITS(26, 27)
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_2_CFG22_SHIFT                       26
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_3_CFG22_MASK                        CO_BITS(23, 25)
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_3_CFG22_SHIFT                       23
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_0_CFG23_MASK                        CO_BITS(14, 15)
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_0_CFG23_SHIFT                       14
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_1_CFG23_MASK                        CO_BITS(12, 13)
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_1_CFG23_SHIFT                       12
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_2_CFG23_MASK                        CO_BITS(10, 11)
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_2_CFG23_SHIFT                       10
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_3_CFG23_MASK                        CO_BITS(7, 9)
#define TX_RF_GAIN_CFG11_TX_RF_GAIN_3_CFG23_SHIFT                       7

/* RFIP_ADDR_TX_RF_GAIN_CFG12 */
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_0_CFG24_MASK                        CO_BITS(30, 31)
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_0_CFG24_SHIFT                       30
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_1_CFG24_MASK                        CO_BITS(28, 29)
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_1_CFG24_SHIFT                       28
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_2_CFG24_MASK                        CO_BITS(26, 27)
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_2_CFG24_SHIFT                       26
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_3_CFG24_MASK                        CO_BITS(23, 25)
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_3_CFG24_SHIFT                       23
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_0_CFG25_MASK                        CO_BITS(14, 15)
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_0_CFG25_SHIFT                       14
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_1_CFG25_MASK                        CO_BITS(12, 13)
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_1_CFG25_SHIFT                       12
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_2_CFG25_MASK                        CO_BITS(10, 11)
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_2_CFG25_SHIFT                       10
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_3_CFG25_MASK                        CO_BITS(7, 9)
#define TX_RF_GAIN_CFG12_TX_RF_GAIN_3_CFG25_SHIFT                       7

/* RFIP_ADDR_TX_RF_GAIN_CFG13 */
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_0_CFG26_MASK                        CO_BITS(30, 31)
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_0_CFG26_SHIFT                       30
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_1_CFG26_MASK                        CO_BITS(28, 29)
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_1_CFG26_SHIFT                       28
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_2_CFG26_MASK                        CO_BITS(26, 27)
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_2_CFG26_SHIFT                       26
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_3_CFG26_MASK                        CO_BITS(23, 25)
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_3_CFG26_SHIFT                       23
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_0_CFG27_MASK                        CO_BITS(14, 15)
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_0_CFG27_SHIFT                       14
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_1_CFG27_MASK                        CO_BITS(12, 13)
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_1_CFG27_SHIFT                       12
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_2_CFG27_MASK                        CO_BITS(10, 11)
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_2_CFG27_SHIFT                       10
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_3_CFG27_MASK                        CO_BITS(7, 9)
#define TX_RF_GAIN_CFG13_TX_RF_GAIN_3_CFG27_SHIFT                       7

/* RFIP_ADDR_TX_DIG_GAIN_CFG0 */
#define TX_DIG_GAIN_CFG0_TX_DIG_GAIN_TEMP_0_MASK                        CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG0_TX_DIG_GAIN_TEMP_0_SHIFT                       24
#define TX_DIG_GAIN_CFG0_TX_DIG_GAIN_TEMP_1_MASK                        CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG0_TX_DIG_GAIN_TEMP_1_SHIFT                       8

/* RFIP_ADDR_TX_DIG_GAIN_CFG1 */
#define TX_DIG_GAIN_CFG1_TX_DIG_GAIN_TEMP_2_MASK                        CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG1_TX_DIG_GAIN_TEMP_2_SHIFT                       24
#define TX_DIG_GAIN_CFG1_TX_DIG_GAIN_TEMP_3_MASK                        CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG1_TX_DIG_GAIN_TEMP_3_SHIFT                       8

/* RFIP_ADDR_TX_DIG_GAIN_CFG2 */
#define TX_DIG_GAIN_CFG2_TX_DIG_GAIN_TEMP_4_MASK                        CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG2_TX_DIG_GAIN_TEMP_4_SHIFT                       24
#define TX_DIG_GAIN_CFG2_TX_DIG_GAIN_TEMP_5_MASK                        CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG2_TX_DIG_GAIN_TEMP_5_SHIFT                       8

/* RFIP_ADDR_TX_DIG_GAIN_CFG3 */
#define TX_DIG_GAIN_CFG3_TX_DIG_GAIN_TEMP_6_MASK                        CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG3_TX_DIG_GAIN_TEMP_6_SHIFT                       24
#define TX_DIG_GAIN_CFG3_TX_DIG_GAIN_TEMP_7_MASK                        CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG3_TX_DIG_GAIN_TEMP_7_SHIFT                       8

/* RFIP_ADDR_TX_DIG_GAIN_CFG4 */
#define TX_DIG_GAIN_CFG4_TX_DIG_GAIN_TEMP_8_MASK                        CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG4_TX_DIG_GAIN_TEMP_8_SHIFT                       24
#define TX_DIG_GAIN_CFG4_TX_DIG_GAIN_TEMP_9_MASK                        CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG4_TX_DIG_GAIN_TEMP_9_SHIFT                       8

/* RFIP_ADDR_TX_DIG_GAIN_CFG5 */
#define TX_DIG_GAIN_CFG5_TX_DIG_GAIN_TEMP_10_MASK                       CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG5_TX_DIG_GAIN_TEMP_10_SHIFT                      24
#define TX_DIG_GAIN_CFG5_TX_DIG_GAIN_TEMP_11_MASK                       CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG5_TX_DIG_GAIN_TEMP_11_SHIFT                      8

/* RFIP_ADDR_TX_DIG_GAIN_CFG6 */
#define TX_DIG_GAIN_CFG6_TX_DIG_GAIN_TEMP_12_MASK                       CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG6_TX_DIG_GAIN_TEMP_12_SHIFT                      24
#define TX_DIG_GAIN_CFG6_TX_DIG_GAIN_TEMP_13_MASK                       CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG6_TX_DIG_GAIN_TEMP_13_SHIFT                      8

/* RFIP_ADDR_TX_DIG_GAIN_CFG7 */
#define TX_DIG_GAIN_CFG7_TX_DIG_GAIN_TEMP_14_MASK                       CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG7_TX_DIG_GAIN_TEMP_14_SHIFT                      24
#define TX_DIG_GAIN_CFG7_TX_DIG_GAIN_TEMP_15_MASK                       CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG7_TX_DIG_GAIN_TEMP_15_SHIFT                      8

/* RFIP_ADDR_TX_DIG_GAIN_CFG8 */
#define TX_DIG_GAIN_CFG8_TX_DIG_GAIN_TEMP_16_MASK                       CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG8_TX_DIG_GAIN_TEMP_16_SHIFT                      24
#define TX_DIG_GAIN_CFG8_TX_DIG_GAIN_TEMP_17_MASK                       CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG8_TX_DIG_GAIN_TEMP_17_SHIFT                      8

/* RFIP_ADDR_TX_DIG_GAIN_CFG9 */
#define TX_DIG_GAIN_CFG9_TX_DIG_GAIN_TEMP_18_MASK                       CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG9_TX_DIG_GAIN_TEMP_18_SHIFT                      24
#define TX_DIG_GAIN_CFG9_TX_DIG_GAIN_TEMP_19_MASK                       CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG9_TX_DIG_GAIN_TEMP_19_SHIFT                      8

/* RFIP_ADDR_TX_DIG_GAIN_CFG10 */
#define TX_DIG_GAIN_CFG10_TX_DIG_GAIN_TEMP_20_MASK                      CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG10_TX_DIG_GAIN_TEMP_20_SHIFT                     24
#define TX_DIG_GAIN_CFG10_TX_DIG_GAIN_TEMP_21_MASK                      CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG10_TX_DIG_GAIN_TEMP_21_SHIFT                     8

/* RFIP_ADDR_TX_DIG_GAIN_CFG11 */
#define TX_DIG_GAIN_CFG11_TX_DIG_GAIN_TEMP_22_MASK                      CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG11_TX_DIG_GAIN_TEMP_22_SHIFT                     24
#define TX_DIG_GAIN_CFG11_TX_DIG_GAIN_TEMP_23_MASK                      CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG11_TX_DIG_GAIN_TEMP_23_SHIFT                     8

/* RFIP_ADDR_TX_DIG_GAIN_CFG12 */
#define TX_DIG_GAIN_CFG12_TX_DIG_GAIN_TEMP_24_MASK                      CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG12_TX_DIG_GAIN_TEMP_24_SHIFT                     24
#define TX_DIG_GAIN_CFG12_TX_DIG_GAIN_TEMP_25_MASK                      CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG12_TX_DIG_GAIN_TEMP_25_SHIFT                     8

/* RFIP_ADDR_TX_DIG_GAIN_CFG13 */
#define TX_DIG_GAIN_CFG13_TX_DIG_GAIN_TEMP_26_MASK                      CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG13_TX_DIG_GAIN_TEMP_26_SHIFT                     24
#define TX_DIG_GAIN_CFG13_TX_DIG_GAIN_TEMP_27_MASK                      CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG13_TX_DIG_GAIN_TEMP_27_SHIFT                     8

/* RFIP_ADDR_TX_DIG_GAIN_CFG14 */
#define TX_DIG_GAIN_CFG14_TX_DIG_GAIN_TEMP_28_MASK                      CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG14_TX_DIG_GAIN_TEMP_28_SHIFT                     24
#define TX_DIG_GAIN_CFG14_TX_DIG_GAIN_TEMP_29_MASK                      CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG14_TX_DIG_GAIN_TEMP_29_SHIFT                     8

/* RFIP_ADDR_TX_DIG_GAIN_CFG15 */
#define TX_DIG_GAIN_CFG15_TX_DIG_GAIN_TEMP_30_MASK                      CO_BITS(24, 31)
#define TX_DIG_GAIN_CFG15_TX_DIG_GAIN_TEMP_30_SHIFT                     24
#define TX_DIG_GAIN_CFG15_TX_DIG_GAIN_TEMP_31_MASK                      CO_BITS(8, 15)
#define TX_DIG_GAIN_CFG15_TX_DIG_GAIN_TEMP_31_SHIFT                     8

/* RFIP_ADDR_TX_DIG_GAIN_CFG16 */
#define TX_DIG_GAIN_CFG16_TX_DIG_GAIN_TEMP_32_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG16_TX_DIG_GAIN_TEMP_32_SHIFT                     26
#define TX_DIG_GAIN_CFG16_TX_DIG_GAIN_TEMP_33_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG16_TX_DIG_GAIN_TEMP_33_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG17 */
#define TX_DIG_GAIN_CFG17_TX_DIG_GAIN_TEMP_34_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG17_TX_DIG_GAIN_TEMP_34_SHIFT                     26
#define TX_DIG_GAIN_CFG17_TX_DIG_GAIN_TEMP_35_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG17_TX_DIG_GAIN_TEMP_35_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG18 */
#define TX_DIG_GAIN_CFG18_TX_DIG_GAIN_TEMP_36_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG18_TX_DIG_GAIN_TEMP_36_SHIFT                     26
#define TX_DIG_GAIN_CFG18_TX_DIG_GAIN_TEMP_37_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG18_TX_DIG_GAIN_TEMP_37_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG19 */
#define TX_DIG_GAIN_CFG19_TX_DIG_GAIN_TEMP_38_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG19_TX_DIG_GAIN_TEMP_38_SHIFT                     26
#define TX_DIG_GAIN_CFG19_TX_DIG_GAIN_TEMP_39_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG19_TX_DIG_GAIN_TEMP_39_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG20 */
#define TX_DIG_GAIN_CFG20_TX_DIG_GAIN_TEMP_40_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG20_TX_DIG_GAIN_TEMP_40_SHIFT                     26
#define TX_DIG_GAIN_CFG20_TX_DIG_GAIN_TEMP_41_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG20_TX_DIG_GAIN_TEMP_41_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG21 */
#define TX_DIG_GAIN_CFG21_TX_DIG_GAIN_TEMP_42_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG21_TX_DIG_GAIN_TEMP_42_SHIFT                     26
#define TX_DIG_GAIN_CFG21_TX_DIG_GAIN_TEMP_43_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG21_TX_DIG_GAIN_TEMP_43_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG22 */
#define TX_DIG_GAIN_CFG22_TX_DIG_GAIN_TEMP_44_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG22_TX_DIG_GAIN_TEMP_44_SHIFT                     26
#define TX_DIG_GAIN_CFG22_TX_DIG_GAIN_TEMP_45_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG22_TX_DIG_GAIN_TEMP_45_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG23 */
#define TX_DIG_GAIN_CFG23_TX_DIG_GAIN_TEMP_46_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG23_TX_DIG_GAIN_TEMP_46_SHIFT                     26
#define TX_DIG_GAIN_CFG23_TX_DIG_GAIN_TEMP_47_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG23_TX_DIG_GAIN_TEMP_47_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG24 */
#define TX_DIG_GAIN_CFG24_TX_DIG_GAIN_TEMP_48_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG24_TX_DIG_GAIN_TEMP_48_SHIFT                     26
#define TX_DIG_GAIN_CFG24_TX_DIG_GAIN_TEMP_49_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG24_TX_DIG_GAIN_TEMP_49_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG25 */
#define TX_DIG_GAIN_CFG25_TX_DIG_GAIN_TEMP_50_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG25_TX_DIG_GAIN_TEMP_50_SHIFT                     26
#define TX_DIG_GAIN_CFG25_TX_DIG_GAIN_TEMP_51_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG25_TX_DIG_GAIN_TEMP_51_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG26 */
#define TX_DIG_GAIN_CFG26_TX_DIG_GAIN_TEMP_52_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG26_TX_DIG_GAIN_TEMP_52_SHIFT                     26
#define TX_DIG_GAIN_CFG26_TX_DIG_GAIN_TEMP_53_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG26_TX_DIG_GAIN_TEMP_53_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG27 */
#define TX_DIG_GAIN_CFG27_TX_DIG_GAIN_TEMP_54_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG27_TX_DIG_GAIN_TEMP_54_SHIFT                     26
#define TX_DIG_GAIN_CFG27_TX_DIG_GAIN_TEMP_55_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG27_TX_DIG_GAIN_TEMP_55_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG28 */
#define TX_DIG_GAIN_CFG28_TX_DIG_GAIN_TEMP_56_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG28_TX_DIG_GAIN_TEMP_56_SHIFT                     26
#define TX_DIG_GAIN_CFG28_TX_DIG_GAIN_TEMP_57_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG28_TX_DIG_GAIN_TEMP_57_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG29 */
#define TX_DIG_GAIN_CFG29_TX_DIG_GAIN_TEMP_58_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG29_TX_DIG_GAIN_TEMP_58_SHIFT                     26
#define TX_DIG_GAIN_CFG29_TX_DIG_GAIN_TEMP_59_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG29_TX_DIG_GAIN_TEMP_59_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG30 */
#define TX_DIG_GAIN_CFG30_TX_DIG_GAIN_TEMP_60_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG30_TX_DIG_GAIN_TEMP_60_SHIFT                     26
#define TX_DIG_GAIN_CFG30_TX_DIG_GAIN_TEMP_61_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG30_TX_DIG_GAIN_TEMP_61_SHIFT                     10

/* RFIP_ADDR_TX_DIG_GAIN_CFG31 */
#define TX_DIG_GAIN_CFG31_TX_DIG_GAIN_TEMP_62_MASK                      CO_BITS(26, 31)
#define TX_DIG_GAIN_CFG31_TX_DIG_GAIN_TEMP_62_SHIFT                     26
#define TX_DIG_GAIN_CFG31_TX_DIG_GAIN_TEMP_63_MASK                      CO_BITS(10, 15)
#define TX_DIG_GAIN_CFG31_TX_DIG_GAIN_TEMP_63_SHIFT                     10

/* RFIP_ADDR_TX_CURRENT_CFG0 */
#define TX_CURRENT_CFG0_REG_CALI_BIT_CFG0_MASK                          CO_BITS(16, 24)
#define TX_CURRENT_CFG0_REG_CALI_BIT_CFG0_SHIFT                         16
#define TX_CURRENT_CFG0_REG_CALQ_BIT_CFG0_MASK                          CO_BITS(0, 8)
#define TX_CURRENT_CFG0_REG_CALQ_BIT_CFG0_SHIFT                         0

/* RFIP_ADDR_TX_CURRENT_CFG1 */
#define TX_CURRENT_CFG1_REG_CALI_BIT_CFG1_MASK                          CO_BITS(16, 24)
#define TX_CURRENT_CFG1_REG_CALI_BIT_CFG1_SHIFT                         16
#define TX_CURRENT_CFG1_REG_CALQ_BIT_CFG1_MASK                          CO_BITS(0, 8)
#define TX_CURRENT_CFG1_REG_CALQ_BIT_CFG1_SHIFT                         0

/* RFIP_ADDR_TX_CURRENT_CFG2 */
#define TX_CURRENT_CFG2_REG_CALI_BIT_CFG2_MASK                          CO_BITS(16, 24)
#define TX_CURRENT_CFG2_REG_CALI_BIT_CFG2_SHIFT                         16
#define TX_CURRENT_CFG2_REG_CALQ_BIT_CFG2_MASK                          CO_BITS(0, 8)
#define TX_CURRENT_CFG2_REG_CALQ_BIT_CFG2_SHIFT                         0

/* RFIP_ADDR_TX_CURRENT_CFG3 */
#define TX_CURRENT_CFG3_REG_CAL_BIT_FORCE                               BIT31
#define TX_CURRENT_CFG3_TXDC_CAL_OUTI_DD                                BIT28
#define TX_CURRENT_CFG3_REG_CALI_BIT_CFG3_MASK                          CO_BITS(16, 24)
#define TX_CURRENT_CFG3_REG_CALI_BIT_CFG3_SHIFT                         16
#define TX_CURRENT_CFG3_TXDC_CAL_OUTQ_DD                                BIT12
#define TX_CURRENT_CFG3_REG_CALQ_BIT_CFG3_MASK                          CO_BITS(0, 8)
#define TX_CURRENT_CFG3_REG_CALQ_BIT_CFG3_SHIFT                         0

#define RF_103_A_CUT                                                    0
#define RF_103_B_CUT                                                    1

#define read_rf_reg(addr)                                               REG_RF_RD(addr)

#define write_rf_reg(addr,value)                                        REG_RF_WR((addr), (value))

void hal_rf_get_thermal(uint32_t sensor_chs, uint32_t count, uint32_t *result);

#endif

#define hal_write_rf_reg                                                write_rf_reg
#define hal_read_rf_reg                                                 read_rf_reg
#define write_rf_fields(reg, fields, value)                             write_rf_reg(reg, \
                                                                        ((read_rf_reg(reg) & (~fields##_MASK)) \
                                                                        | ((value << (fields##_SHIFT)) & (fields##_MASK))))

#define read_rf_fields(reg, fields)                                     ((read_rf_reg(reg) & (fields##_MASK)) >> (fields##_SHIFT))

#define clear_rf_bits(reg, bits)                                        write_rf_reg(reg, \
                                                                        (read_rf_reg(reg) & (~(bits))))

#define set_rf_bits(reg, bits)                                          write_rf_reg(reg, \
                                                                        (read_rf_reg(reg) | (bits)))
enum
{
    RFTX_CTRL_BY_BB = 0,
    RFTX_CTRL_MANUAL = 1,
};

#define CRYSTAL_TUNE_BASE                       0x40
#define CRYSTAL_TUNE_TYPE_BASE                  0
#define CRYSTAL_TUNE_TYPE_ACCT                  1  // Acceleration
#define XTAL_FREQ_TUNING_DEFAULT                0x40
#define XTAL_CAP_VALUE_MIN                      0
#define XTAL_CAP_VALUE_MAX                      0x7F

enum
{
    RF_BW_20 = 1,
    RF_BW_40 = 2,
};

void hal_set_crystal_cap(uint32_t tune_type, int32_t tune_value);
int32_t hal_get_crystal_cap(void);
uint8_t hal_init_rf(void);
void hal_start_rf_pll(void);
void hal_rfi_init(void);
void hal_set_power_cal(int8_t ax20_pwr_cal_offset, int8_t cck_pwr_cal_offset);
void hal_set_ble_power_cal(int8_t pwr_cal_offset);
int8_t hal_get_ble_power_cal(void);
void hal_rf_set_freq(uint32_t freq);
uint32_t hal_get_paib_cal(void);

#endif /* __HAL_RF_H */
