/****************************************************************************
 * @file     main.c
 * @version  V1.01
 * $Revision: 1 $
 * $Date: 2019/05/02 8:05p $
 * @brief    TBX1 Sample
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include "TBX1.h"

#if defined(__GNUC__) && !defined(__ARMCC_VERSION) && defined(OS_USE_SEMIHOSTING)
    extern void initialise_monitor_handles(void);
#endif

extern void Periph_Init(void);
extern void Clock_Init(void);
extern void Pin_Init(void);

void System_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    Clock_Init();
    Pin_Init();
    Periph_Init();

    /* Lock protected registers */
    SYS_LockReg();
}

int32_t main(void)
{
    /* Init System, IP clock, multi-function I/O and Peripheral*/
    System_Init();

    printf("*** Init Done, User add operation code***\n");

    while (1);

}
