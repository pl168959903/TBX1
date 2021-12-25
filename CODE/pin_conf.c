/****************************************************************************
 * @file     pin_conf.c
 * @version  V0.42
 * @Date     2021/12/25-05:11:30
 * @brief    NuMicro generated code file
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (C) 2016-2021 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

/********************
MCU:M032SG8AE(LQFP64)
Pin Configuration:
Pin1:ABAT_EN
Pin2:ABAT_ADC
Pin3:PRE_ADC4
Pin4:PRE_ADC3
Pin5:PRE_ADC2
Pin6:PRE_ADC1
Pin7:PRE_ADC0
Pin8:CHRG_IO
Pin9:STDBY_IO
Pin10:KEY0_IO
Pin11:V33_EN
Pin14:X32_IN
Pin15:X32_OUT
Pin20:NRF_INT
Pin21:ICM_INT
Pin25:ICM_SCL
Pin26:ICM_SDA
Pin27:CLKO
Pin29:UART0_TXD
Pin30:UART0_RXD
Pin33:ICE_DAT
Pin34:ICE_CLK
Pin40:NRF_CE
Pin41:NRF_SS
Pin42:NRF_CLK
Pin43:NRF_DI
Pin44:NRF_DO
Pin62:LED0
Pin63:LED1
Pin64:LED2
Module Configuration:
ABAT_EN(Pin:1)
LED0(Pin:62)
LED1(Pin:63)
LED2(Pin:64)
ABAT_ADC(Pin:2)
PRE_ADC0(Pin:7)
PRE_ADC1(Pin:6)
PRE_ADC2(Pin:5)
PRE_ADC3(Pin:4)
PRE_ADC4(Pin:3)
CHRG_IO(Pin:8)
KEY0_IO(Pin:10)
STDBY_IO(Pin:9)
V33_EN(Pin:11)
X32_IN(Pin:14)
X32_OUT(Pin:15)
ICM_INT(Pin:21)
NRF_INT(Pin:20)
ICM_SCL(Pin:25)
ICM_SDA(Pin:26)
CLKO(Pin:27)
UART0_RXD(Pin:30)
UART0_TXD(Pin:29)
ICE_CLK(Pin:34)
ICE_DAT(Pin:33)
NRF_CE(Pin:40)
NRF_CLK(Pin:42)
NRF_DI(Pin:43)
NRF_DO(Pin:44)
NRF_SS(Pin:41)
GPIO Configuration:
PA.0:UART0_RXD(Pin:30)
PA.1:UART0_TXD(Pin:29)
PA.3:CLKO(Pin:27)
PA.4:ICM_SDA(Pin:26)
PA.5:ICM_SCL(Pin:25)
PA.6:ICM_INT(Pin:21)
PA.7:NRF_INT(Pin:20)
PA.8:V33_EN(Pin:11)
PA.9:KEY0_IO(Pin:10)
PA.10:STDBY_IO(Pin:9)
PA.11:CHRG_IO(Pin:8)
PB.0:PRE_ADC0(Pin:7)
PB.1:PRE_ADC1(Pin:6)
PB.2:PRE_ADC2(Pin:5)
PB.3:PRE_ADC3(Pin:4)
PB.4:PRE_ADC4(Pin:3)
PB.5:ABAT_ADC(Pin:2)
PB.6:ABAT_EN(Pin:1)
PB.7:LED2(Pin:64)
PB.8:LED1(Pin:63)
PB.9:LED0(Pin:62)
PC.0:NRF_CE(Pin:40)
PD.0:NRF_DO(Pin:44)
PD.1:NRF_DI(Pin:43)
PD.2:NRF_CLK(Pin:42)
PD.3:NRF_SS(Pin:41)
PF.0:ICE_DAT(Pin:33)
PF.1:ICE_CLK(Pin:34)
PF.4:X32_OUT(Pin:15)
PF.5:X32_IN(Pin:14)
********************/

#include "TBX1.h"
#include "pin_conf.h"

/* User-defined pins */
#define ABAT_EN PB6
#define ABAT_ADC ADC0_CH5
#define PRE_ADC4 ADC0_CH4
#define PRE_ADC3 ADC0_CH3
#define PRE_ADC2 ADC0_CH2
#define PRE_ADC1 ADC0_CH1
#define PRE_ADC0 ADC0_CH0
#define CHRG_IO PA11
#define STDBY_IO PA10
#define KEY0_IO PA9
#define V33_EN PA8
#define NRF_INT INT1
#define ICM_INT INT0
#define ICM_SCL I2C0_SCL
#define ICM_SDA I2C0_SDA
#define NRF_CE PC0
#define NRF_SS SPI0_SS
#define NRF_CLK SPI0_CLK
#define NRF_DI SPI0_MISO
#define NRF_DO SPI0_MOSI
#define LED0 PB9
#define LED1 PB8
#define LED2 PB7

void TBX1_pin_init_adc0(void)
{
    SYS->GPB_MFPL &= ~(SYS_GPB_MFPL_PB5MFP_Msk | SYS_GPB_MFPL_PB4MFP_Msk | SYS_GPB_MFPL_PB3MFP_Msk | SYS_GPB_MFPL_PB2MFP_Msk | SYS_GPB_MFPL_PB1MFP_Msk | SYS_GPB_MFPL_PB0MFP_Msk);
    SYS->GPB_MFPL |= (SYS_GPB_MFPL_PB5MFP_ADC0_CH5 | SYS_GPB_MFPL_PB4MFP_ADC0_CH4 | SYS_GPB_MFPL_PB3MFP_ADC0_CH3 | SYS_GPB_MFPL_PB2MFP_ADC0_CH2 | SYS_GPB_MFPL_PB1MFP_ADC0_CH1 |
                      SYS_GPB_MFPL_PB0MFP_ADC0_CH0);

    return;
}

void TBX1_pin_deinit_adc0(void)
{
    SYS->GPB_MFPL &= ~(SYS_GPB_MFPL_PB5MFP_Msk | SYS_GPB_MFPL_PB4MFP_Msk | SYS_GPB_MFPL_PB3MFP_Msk | SYS_GPB_MFPL_PB2MFP_Msk | SYS_GPB_MFPL_PB1MFP_Msk | SYS_GPB_MFPL_PB0MFP_Msk);

    return;
}

void TBX1_pin_init_clko(void)
{
    SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA3MFP_Msk);
    SYS->GPA_MFPL |= (SYS_GPA_MFPL_PA3MFP_CLKO);

    return;
}

void TBX1_pin_deinit_clko(void)
{
    SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA3MFP_Msk);

    return;
}

void TBX1_pin_init_i2c0(void)
{
    SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA5MFP_Msk | SYS_GPA_MFPL_PA4MFP_Msk);
    SYS->GPA_MFPL |= (SYS_GPA_MFPL_PA5MFP_I2C0_SCL | SYS_GPA_MFPL_PA4MFP_I2C0_SDA);

    return;
}

void TBX1_pin_deinit_i2c0(void)
{
    SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA5MFP_Msk | SYS_GPA_MFPL_PA4MFP_Msk);

    return;
}

void TBX1_pin_init_ice(void)
{
    SYS->GPF_MFPL &= ~(SYS_GPF_MFPL_PF1MFP_Msk | SYS_GPF_MFPL_PF0MFP_Msk);
    SYS->GPF_MFPL |= (SYS_GPF_MFPL_PF1MFP_ICE_CLK | SYS_GPF_MFPL_PF0MFP_ICE_DAT);

    return;
}

void TBX1_pin_deinit_ice(void)
{
    SYS->GPF_MFPL &= ~(SYS_GPF_MFPL_PF1MFP_Msk | SYS_GPF_MFPL_PF0MFP_Msk);

    return;
}

void TBX1_pin_init_int(void)
{
    SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA7MFP_Msk | SYS_GPA_MFPL_PA6MFP_Msk);
    SYS->GPA_MFPL |= (SYS_GPA_MFPL_PA7MFP_INT1 | SYS_GPA_MFPL_PA6MFP_INT0);

    return;
}

void TBX1_pin_deinit_int(void)
{
    SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA7MFP_Msk | SYS_GPA_MFPL_PA6MFP_Msk);

    return;
}

void TBX1_pin_init_pa(void)
{
    SYS->GPA_MFPH &= ~(SYS_GPA_MFPH_PA11MFP_Msk | SYS_GPA_MFPH_PA10MFP_Msk | SYS_GPA_MFPH_PA9MFP_Msk | SYS_GPA_MFPH_PA8MFP_Msk);
    SYS->GPA_MFPH |= (SYS_GPA_MFPH_PA11MFP_GPIO | SYS_GPA_MFPH_PA10MFP_GPIO | SYS_GPA_MFPH_PA9MFP_GPIO | SYS_GPA_MFPH_PA8MFP_GPIO);

    return;
}

void TBX1_pin_deinit_pa(void)
{
    SYS->GPA_MFPH &= ~(SYS_GPA_MFPH_PA11MFP_Msk | SYS_GPA_MFPH_PA10MFP_Msk | SYS_GPA_MFPH_PA9MFP_Msk | SYS_GPA_MFPH_PA8MFP_Msk);

    return;
}

void TBX1_pin_init_pb(void)
{
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB9MFP_Msk | SYS_GPB_MFPH_PB8MFP_Msk);
    SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB9MFP_GPIO | SYS_GPB_MFPH_PB8MFP_GPIO);
    SYS->GPB_MFPL &= ~(SYS_GPB_MFPL_PB7MFP_Msk | SYS_GPB_MFPL_PB6MFP_Msk);
    SYS->GPB_MFPL |= (SYS_GPB_MFPL_PB7MFP_GPIO | SYS_GPB_MFPL_PB6MFP_GPIO);

    return;
}

void TBX1_pin_deinit_pb(void)
{
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB9MFP_Msk | SYS_GPB_MFPH_PB8MFP_Msk);
    SYS->GPB_MFPL &= ~(SYS_GPB_MFPL_PB7MFP_Msk | SYS_GPB_MFPL_PB6MFP_Msk);

    return;
}

void TBX1_pin_init_pc(void)
{
    SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC0MFP_Msk);
    SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC0MFP_GPIO);

    return;
}

void TBX1_pin_deinit_pc(void)
{
    SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC0MFP_Msk);

    return;
}

void TBX1_pin_init_spi0(void)
{
    SYS->GPD_MFPL &= ~(SYS_GPD_MFPL_PD3MFP_Msk | SYS_GPD_MFPL_PD2MFP_Msk | SYS_GPD_MFPL_PD1MFP_Msk | SYS_GPD_MFPL_PD0MFP_Msk);
    SYS->GPD_MFPL |= (SYS_GPD_MFPL_PD3MFP_SPI0_SS | SYS_GPD_MFPL_PD2MFP_SPI0_CLK | SYS_GPD_MFPL_PD1MFP_SPI0_MISO | SYS_GPD_MFPL_PD0MFP_SPI0_MOSI);

    return;
}

void TBX1_pin_deinit_spi0(void)
{
    SYS->GPD_MFPL &= ~(SYS_GPD_MFPL_PD3MFP_Msk | SYS_GPD_MFPL_PD2MFP_Msk | SYS_GPD_MFPL_PD1MFP_Msk | SYS_GPD_MFPL_PD0MFP_Msk);

    return;
}

void TBX1_pin_init_uart0(void)
{
    SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA1MFP_Msk | SYS_GPA_MFPL_PA0MFP_Msk);
    SYS->GPA_MFPL |= (SYS_GPA_MFPL_PA1MFP_UART0_TXD | SYS_GPA_MFPL_PA0MFP_UART0_RXD);

    return;
}

void TBX1_pin_deinit_uart0(void)
{
    SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA1MFP_Msk | SYS_GPA_MFPL_PA0MFP_Msk);

    return;
}

void TBX1_pin_init_x32(void)
{
    SYS->GPF_MFPL &= ~(SYS_GPF_MFPL_PF5MFP_Msk | SYS_GPF_MFPL_PF4MFP_Msk);
    SYS->GPF_MFPL |= (SYS_GPF_MFPL_PF5MFP_X32_IN | SYS_GPF_MFPL_PF4MFP_X32_OUT);

    return;
}

void TBX1_pin_deinit_x32(void)
{
    SYS->GPF_MFPL &= ~(SYS_GPF_MFPL_PF5MFP_Msk | SYS_GPF_MFPL_PF4MFP_Msk);

    return;
}

void Pin_Init(void)
{
    //SYS->GPA_MFPH = 0x00000000UL;
    //SYS->GPA_MFPL = 0xFF99E077UL;
    //SYS->GPB_MFPH = 0x00000000UL;
    //SYS->GPB_MFPL = 0x00111111UL;
    //SYS->GPC_MFPH = 0x00000000UL;
    //SYS->GPC_MFPL = 0x00000000UL;
    //SYS->GPD_MFPH = 0x00000000UL;
    //SYS->GPD_MFPL = 0x00004444UL;
    //SYS->GPF_MFPH = 0x00000000UL;
    //SYS->GPF_MFPL = 0x00AA00EEUL;

    TBX1_pin_init_adc0();
    TBX1_pin_init_clko();
    TBX1_pin_init_i2c0();
    TBX1_pin_init_ice();
    TBX1_pin_init_int();
    TBX1_pin_init_pa();
    TBX1_pin_init_pb();
    TBX1_pin_init_pc();
    TBX1_pin_init_spi0();
    TBX1_pin_init_uart0();
    TBX1_pin_init_x32();

    return;
}

void Pin_Deinit(void)
{
    TBX1_pin_deinit_adc0();
    TBX1_pin_deinit_clko();
    TBX1_pin_deinit_i2c0();
    TBX1_pin_deinit_ice();
    TBX1_pin_deinit_int();
    TBX1_pin_deinit_pa();
    TBX1_pin_deinit_pb();
    TBX1_pin_deinit_pc();
    TBX1_pin_deinit_spi0();
    TBX1_pin_deinit_uart0();
    TBX1_pin_deinit_x32();

    return;
}

/*** (C) COPYRIGHT 2016-2021 Nuvoton Technology Corp. ***/
