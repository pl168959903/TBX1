/****************************************************************************
 * @file     TBX1.h
 * @version  V1.00
 * @Date     2019/03/28-12:06:28
 * @brief    NuMicro generated code file
 *
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2016-2021 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#include "NuMicro.h"
#include "counter.h"
#include "periph_conf.h"
#include "sw_buf.h"
#include "key.h"
#include "user.h"
#include "spiPdma.h"
#include "RF24_Reg.h"
#include "axis.h"
#include <stdio.h>
#include <string.h>

#define KEY0 PA9
#define EN_V33 PA8

uint8_t Timer_delay_Handeler(void);
void Timer_delay( uint32_t ms );




    /*** (C) COPYRIGHT 2016-2019 Nuvoton Technology Corp. ***/
