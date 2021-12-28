/******************************************************************************//**
 * @file     int_handler.c
 * @version  V1.01
 * @brief    M031 interrupt handler file
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2021 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

/*!<Includes */
#include "TBX1.h"
void CKFAIL_IRQHandler(void)
{
    volatile uint32_t u32Reg;

    /* Unlock protected registers */
    SYS_UnlockReg();

    u32Reg = CLK->CLKDSTS;

    if (u32Reg & CLK_CLKDSTS_HXTFIF_Msk)
    {
        /* HCLK is switched to HIRC automatically if HXT clock fail interrupt is happened */
        printf("HXT Clock is stopped! HCLK is switched to HIRC.\n");

        /* Disable HXT clock fail interrupt */
        CLK->CLKDCTL &= ~(CLK_CLKDCTL_HXTFDEN_Msk | CLK_CLKDCTL_HXTFIEN_Msk);

        /* Write 1 to clear HXT Clock fail interrupt flag */
        CLK->CLKDSTS = CLK_CLKDSTS_HXTFIF_Msk;
    }

    if (u32Reg & CLK_CLKDSTS_LXTFIF_Msk)
    {
        /* LXT clock fail interrupt is happened */
        printf("LXT Clock is stopped!\n");

        /* Disable HXT clock fail interrupt */
        CLK->CLKDCTL &= ~(CLK_CLKDCTL_LXTFIEN_Msk | CLK_CLKDCTL_LXTFDEN_Msk);

        /* Write 1 to clear LXT Clock fail interrupt flag */
        CLK->CLKDSTS = CLK_CLKDSTS_LXTFIF_Msk;
    }

    if (u32Reg & CLK_CLKDSTS_HXTFQIF_Msk)
    {
        /* HCLK should be switched to HIRC if HXT clock frequency monitor interrupt is happened */
        CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
        printf("HXT Frequency is abnormal! HCLK is switched to HIRC.\n");

        /* Disable HXT clock frequency monitor interrupt */
        CLK->CLKDCTL &= ~(CLK_CLKDCTL_HXTFQDEN_Msk | CLK_CLKDCTL_HXTFQIEN_Msk);

        /* Write 1 to clear HXT Clock frequency monitor interrupt */
        CLK->CLKDSTS = CLK_CLKDSTS_HXTFQIF_Msk;
    }

    /* Lock protected registers */
    SYS_LockReg();
}

void PWRWU_IRQHandler(void)
{
    /* Check system power down mode wake-up interrupt status flag */
    if (CLK->PWRCTL & CLK_PWRCTL_PDWKIF_Msk)
    {
        /* Clear system power down wake-up interrupt flag */
        CLK->PWRCTL |= CLK_PWRCTL_PDWKIF_Msk;

        printf("System wake-up from Power-down mode.\n");
    }
}
void ADC_IRQHandler(void)
{

}
/**
 * @brief       GPIO PA/PB/PG/PH IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The PA/PB/PG/PH default IRQ, declared in startup_M031Series.s.
 */
void GPABGH_IRQHandler( void ) {
    volatile uint32_t temp;
    
    if ( GPIO_GET_INT_FLAG( PA, BIT6 ) ) {
        GPIO_CLR_INT_FLAG( PA, BIT6 );
        if (PA6 == 0){
            printf( "PA.6 falling INT occurred.\n" );
        }
        else{
            printf( "PA.6 rising INT occurred.\n" );
        }
    }
    else if( GPIO_GET_INT_FLAG( PA, BIT7 ) ) {
        GPIO_CLR_INT_FLAG( PA, BIT7 );
        if (PA7 == 0){
            printf( "PA.7 falling INT occurred.\n" );
        }
        else{ 
            printf( "PA.7 rising INT occurred.\n" );
        }
    }
    else if( GPIO_GET_INT_FLAG( PA, BIT9 ) ) {
        GPIO_CLR_INT_FLAG( PA, BIT9 );
        if (PA9 == 0){
            Key_up();
            Key_capture_start();
            // printf( "PA.9 falling INT occurred.\n" );
        }
        else{
            Key_down();
            Key_capture_start();
            // printf( "PA.9 rising INT occurred.\n" );
        }
    }
    else if( GPIO_GET_INT_FLAG( PA, BIT10 ) ) {
        GPIO_CLR_INT_FLAG( PA, BIT10 );
        if (PA10 == 0){
            printf( "PA.10 falling INT occurred.\n" );
        }
        else{
            printf( "PA.10 rising INT occurred.\n" );
        }
    }
    else if( GPIO_GET_INT_FLAG( PA, BIT11 ) ) {
        GPIO_CLR_INT_FLAG( PA, BIT11 );
        if (PA11 == 0){
            printf( "PA.11 falling INT occurred.\n" );
        }
        else{
            printf( "PA.11 rising INT occurred.\n" );
        }
    }
    else {
        printf( "Un-expected interrupts.\n" );
        printf( "PA->INTSRC = 0x%04X.\n",PA->INTSRC);
        printf( "PB->INTSRC = 0x%04X.\n",PB->INTSRC);
        printf( "PC->INTSRC = 0x%04X.\n",PC->INTSRC);
        printf( "PD->INTSRC = 0x%04X.\n",PD->INTSRC);
        printf( "PE->INTSRC = 0x%04X.\n",PE->INTSRC);
        printf( "PF->INTSRC = 0x%04X.\n",PF->INTSRC);
        printf( "PG->INTSRC = 0x%04X.\n",PG->INTSRC);
        printf( "PH->INTSRC = 0x%04X.\n",PH->INTSRC);
    }
}

void EINT0_IRQHandler(void)
{

    /* To check if PA.6 external interrupt occurred */
    if (GPIO_GET_INT_FLAG(PA, BIT6))
    {
        GPIO_CLR_INT_FLAG(PA, BIT6);
        printf("PA.6 EINT0 occurred.\n");
    }

}

void EINT1_IRQHandler(void)
{

    /* To check if PA.7 external interrupt occurred */
    if (GPIO_GET_INT_FLAG(PA, BIT7))
    {
        GPIO_CLR_INT_FLAG(PA, BIT7);
        printf("PA.7 EINT1 occurred.\n");
    }

}
void I2C0_IRQHandler(void)
{
    uint32_t u32Status;

    u32Status = I2C_GET_STATUS(I2C0);

    (void)u32Status; /*To remove complier warning. The code is unnecessary if u32Status is used.*/

    if (I2C_GET_TIMEOUT_FLAG(I2C0))
    {
        /* Clear I2C0 Timeout Flag */
        I2C_ClearTimeoutFlag(I2C0);
    }
    else
    {
        /*TODO: implement user function here*/
        //                if (s_pfnI2C0Handler != NULL)
        //                    s_pfnI2C0Handler(u32Status);
    }
}
void PDMA_IRQHandler(void)
{
    uint32_t u32Status = PDMA_GET_INT_STATUS(PDMA);

    if (u32Status & PDMA_INTSTS_TDIF_Msk)     /* done */
    {
        /* Transfer done */
    }
    else
        printf("Unexpected interrupt !!\n");
}
void RTC_IRQHandler(void)
{
    /* Get RTC Tick Interrupt Flag */
    if (RTC_GET_TICK_INT_FLAG() && (RTC->INTEN & RTC_INTEN_TICKIEN_Msk))
    {
        /* Clear RTC Tick Interrupt Flag */
        RTC_CLEAR_TICK_INT_FLAG();
    }

    /* Get RTC Alarm Interrupt Flag */
    if (RTC_GET_ALARM_INT_FLAG() && (RTC->INTEN & RTC_INTEN_ALMIEN_Msk))
    {
        /* Clear RTC Alarm Interrupt Flag */
        RTC_CLEAR_ALARM_INT_FLAG();
    }
}
void SPI0_IRQHandler(void)
{
    uint32_t u32IntFlag;

    u32IntFlag = SPI_GetIntFlag(SPI0, SPI_UNIT_INT_MASK | SPI_FIFO_RXOV_INT_MASK | SPI_FIFO_RXTO_INT_MASK);

    if (u32IntFlag)
    {
        printf("SPI INT Flag:0x%x\n", u32IntFlag);
    }
    else
    {
        printf("Unknown INT occurred!\n");
    }

}
void BOD_IRQHandler(void)
{
    /* Get BOD interrupt flag */
    if (SYS_GET_BOD_INT_FLAG())
    {
        /* Clear BOD interrupt flag */
        SYS_CLEAR_BOD_INT_FLAG();
    }
}

void IRCTRIM_IRQHandler(void)
{

    if (SYS->HIRCTRIMSTS & SYS_HIRCTRIMSTS_TFAILIF_Msk)   /* Get Trim Failure Interrupt */
    {
        /* Display HIRC trim status */
        printf("HIRC Trim Failure Interrupt\n");
        /* Clear Trim Failure Interrupt */
        SYS->HIRCTRIMSTS = SYS_HIRCTRIMSTS_TFAILIF_Msk;
    }

    if (SYS->HIRCTRIMSTS & SYS_HIRCTRIMSTS_CLKERIF_Msk)   /* Get LXT Clock Error Interrupt */
    {
        /* Display HIRC trim status */
        printf("LXT Clock Error Interrupt\n");
        /* Clear LXT Clock Error Interrupt */
        SYS->HIRCTRIMSTS = SYS_HIRCTRIMSTS_CLKERIF_Msk;
    }

}
void UART0_IRQHandler(void)
{

}

void UART02_IRQHandler(void)
{

    UART0_IRQHandler();

}
void WDT_IRQHandler(void)
{

    /* Get WDT Time-out interrupt flag */
    if (WDT_GET_TIMEOUT_INT_FLAG())
    {
        /* Clear WDT time-out interrupt flag */
        WDT_CLEAR_TIMEOUT_INT_FLAG();

        /* To prevent system from reset by WDT, it is necessary to clear WDT counter
           by software before WDT counter reaches the reset delay time after time-out occurs. */

        WDT_RESET_COUNTER();
    }

    /* Get WDT time-out wakeup  flag */
    if (WDT_GET_TIMEOUT_WAKEUP_FLAG())
    {
        /* Clear WDT time-out wakeup  flag */
        WDT_CLEAR_TIMEOUT_WAKEUP_FLAG();
    }

    /* Get WWDT interrupt flag */
    if (WWDT_GET_INT_FLAG())
    {
        /* Clear WWDT interrupt flag */
        WWDT_CLEAR_INT_FLAG();

        /* To prevent system from reset by WWDT, it is necessary to reload WWDT counter
           by software before WWDT counter reaches zero. */

        WWDT_RELOAD_COUNTER();
    }

}

uint8_t volatile g_u8Suspend = 0;

void USBD_IRQHandler(void)
{
    uint32_t volatile u32IntSts = USBD_GET_INT_FLAG();
    uint32_t volatile u32State = USBD_GET_BUS_STATE();

    if (u32IntSts & USBD_INTSTS_FLDET)
    {
        /* Floating detect */
        USBD_CLR_INT_FLAG(USBD_INTSTS_FLDET);

        if (USBD_IS_ATTACHED())
        {
            /* USB Plug In */
            USBD_ENABLE_USB();
        }
        else
        {
            /* USB Un-plug */
            USBD_DISABLE_USB();
        }
    }

    if (u32IntSts & USBD_INTSTS_BUS)
    {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_BUS);

        if (u32State & USBD_STATE_USBRST)
        {
            /* Bus reset */
            USBD_ENABLE_USB();
            USBD_SwReset();
            g_u8Suspend = 0;
        }

        if (u32State & USBD_STATE_SUSPEND)
        {
            /* Enter power down to wait USB attached */
            g_u8Suspend = 1;

            /* Enable USB but disable PHY */
            USBD_DISABLE_PHY();
        }

        if (u32State & USBD_STATE_RESUME)
        {
            /* Enable USB and enable PHY */
            USBD_ENABLE_USB();
            g_u8Suspend = 0;
        }

#ifdef SUPPORT_LPM

        if (u32State & USBD_STATE_L1SUSPEND)
        {
            /*
               TODO: Implement LPM SUSPEND flag here.
                     Recommend implementing the power-saving function in main loop.
            */

        }

        if (u32State & USBD_STATE_L1RESUME)
        {
            /*
               TODO: Implement LPM RESUME flag here.
            */

        }

#endif
    }

    if (u32IntSts & USBD_INTSTS_NEVWKIF_Msk)
    {
        /*Clear no-event wake up interrupt */
        USBD_CLR_INT_FLAG(USBD_INTSTS_NEVWKIF_Msk);
        /*
           TODO: Implement the function that will be executed when device is woken by non-USB event.
        */

    }

    if (u32IntSts & USBD_INTSTS_USB)
    {
        /* USB event */
        if (u32IntSts & USBD_INTSTS_SETUP)
        {
            /* Setup packet */
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_SETUP);

            /* Clear the data IN/OUT ready flag of control end-points */
            USBD_STOP_TRANSACTION(EP0);
            USBD_STOP_TRANSACTION(EP1);

            USBD_ProcessSetupPacket();
        }

        /* EP events */
        if (u32IntSts & USBD_INTSTS_EP0)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP0);
            /* control IN */
            USBD_CtrlIn();
        }

        if (u32IntSts & USBD_INTSTS_EP1)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP1);
            /* control OUT */
            USBD_CtrlOut();
        }

        if (u32IntSts & USBD_INTSTS_EP2)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP2);
        }

        if (u32IntSts & USBD_INTSTS_EP3)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP3);
        }

        if (u32IntSts & USBD_INTSTS_EP4)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP4);
        }

        if (u32IntSts & USBD_INTSTS_EP5)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP5);
        }

        if (u32IntSts & USBD_INTSTS_EP6)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP6);
        }

        if (u32IntSts & USBD_INTSTS_EP7)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP7);
        }
    }
}

extern volatile uint8_t g;
void TMR0_IRQHandler(void)
{
    if (TIMER_GetIntFlag(TIMER0))
    {
        uint8_t timer_en = 0;
        timer_en |= KeyTimer_Handler(); 
        if(timer_en == 1) 
					Key_capture_start();

        timer_en |= Timer_delay_Handeler();

        timer_en |= KeyCaptureTimer_Handler();

        timer_en |= LED_timer_handler();

        if(timer_en == 0){
            TIMER_Stop(TIMER0);
            TIMER_ResetCounter(TIMER0);
        }
        /* Clear Timer0 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER0);
    }
}
