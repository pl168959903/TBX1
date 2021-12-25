/****************************************************************************
 * @file     periph_conf.h
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
********************/

/*--------------------------------------------------------------------------*/
/* CLK Configuration                                                        */
/*--------------------------------------------------------------------------*/

#define CLK_HXT_FREQUENCY_RANGE_UPPER_BOUND     (697)

#define CLK_HXT_FREQUENCY_RANGE_LOWER_BOUND     (669)

/*--------------------------------------------------------------------------*/
/* CRC Configuration                                                        */
/*--------------------------------------------------------------------------*/

#define CRC_SEED                0x33

/* --------------------------------------------------------------------
 *  FMC
 * ---------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* GPIO De-bounce Configuration                                                      */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* GPIO PA Configuration                                                      */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* GPIO PB Configuration                                                      */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* GPIO PC Configuration                                                      */
/*--------------------------------------------------------------------------*/

//I2C0

#define I2C0_TIMEOUT_SEL    (1)

#define PDMA_CH0_DATA_LENGTH            (7)

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH0_DATA_LENGTH_TB0        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH0_DATA_LENGTH_TB1        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH0_DATA_LENGTH_TB2        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH0_DATA_LENGTH_TB3        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH0_DATA_LENGTH_TB4        (1)

#endif

#define PDMA_CH1_DATA_LENGTH            (1)

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH1_DATA_LENGTH_TB0        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH1_DATA_LENGTH_TB1        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH1_DATA_LENGTH_TB2        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH1_DATA_LENGTH_TB3        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH1_DATA_LENGTH_TB4        (1)

#endif

#define PDMA_CH2_DATA_LENGTH            (1)

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH2_DATA_LENGTH_TB0        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH2_DATA_LENGTH_TB1        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH2_DATA_LENGTH_TB2        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH2_DATA_LENGTH_TB3        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH2_DATA_LENGTH_TB4        (1)

#endif

#define PDMA_CH3_DATA_LENGTH            (1)

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH3_DATA_LENGTH_TB0        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH3_DATA_LENGTH_TB1        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH3_DATA_LENGTH_TB2        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH3_DATA_LENGTH_TB3        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH3_DATA_LENGTH_TB4        (1)

#endif

#define PDMA_CH4_DATA_LENGTH            (1)

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH4_DATA_LENGTH_TB0        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH4_DATA_LENGTH_TB1        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH4_DATA_LENGTH_TB2        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH4_DATA_LENGTH_TB3        (1)

#endif

#if (PDMA_OP_BASIC==PDMA_OP_SCATTER)
    #define PDMA_CH4_DATA_LENGTH_TB4        (1)

#endif

/*--------------------------------------------------------------------------*/
/* RTC Configuration                                                        */
/*--------------------------------------------------------------------------*/
/* RTC Tick Interval Configuration */

/* Calendar Alarm Mask Year Settings */
#define RTC_ALARM_TEN_YEAR_MASK                 (0)
#define RTC_ALARM_ONE_YEAR_MASK                 (0)

/* Calendar Alarm Mask Month Settings */
#define RTC_ALARM_TEN_MONTH_MASK                (0)
#define RTC_ALARM_ONE_MONTH_MASK                (0)

/* Calendar Alarm Mask Date Settings */
#define RTC_ALARM_TEN_DATE_MASK                 (0)
#define RTC_ALARM_ONE_DATE_MASK                 (0)

/* Time Alarm Mask Hour Settings */
#define RTC_ALARM_TEN_HOUR_MASK                 (0)
#define RTC_ALARM_ONE_HOUR_MASK                 (0)

/* Time Alarm Mask Minute Settings */
#define RTC_ALARM_TEN_MINUTE_MASK               (0)
#define RTC_ALARM_ONE_MINUTE_MASK               (0)

/* Time Alarm Mask Second Settings */
#define RTC_ALARM_TEN_SECOND_MASK               (0)
#define RTC_ALARM_ONE_SECOND_MASK               (0)

/* Spcify the LXT frequency for calibration. The value should be set to the current frequency (Hz) * 10000.
 * For example: Value 327680001 means 32768.0001 Hz.
 */
#define RTC_LXT_FREQ                            (327680000)

/*--------------------------------------------------------------------------*/
/* SPII2S Configuration                                                     */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* UART0 Configuration                                                      */
/*--------------------------------------------------------------------------*/

#define UART0_MATCH_ADDRSS1     0xC0
#define UART0_MATCH_ADDRSS2     0xA2
#define UART0_UNMATCH_ADDRSS1   0xB1
#define UART0_UNMATCH_ADDRSS2   0xD3

#define UART0_BAUD                      921600

//USB

/* Define the vendor id and product id */
#define USBD_VID   (0x0416)

#define USBD_PID   (0x0123)

/* Define Descriptor information */
#define USBD_SELF_POWERED      (0)

#define USBD_REMOTE_WAKEUP     (1)

#define USBD_MAX_POWER   (50)

/* Define Control Endpoint maximum packet size */
#define EP0_MAX_PKT_SIZE    (64)

#define EP1_MAX_PKT_SIZE    EP0_MAX_PKT_SIZE

/*--------------------------------------------------------------------------*/
/* WDT Configuration                                                        */
/*--------------------------------------------------------------------------*/
/* Configure WDT Timeout Interval */
/* Configure WDT Reset Delay Time */
/* Enable or Disable WDT Reset Function */
#define WDT_RSTEN_CONFIG                        (0)
/* Configure WDT Wake-up Ability */
#define WDT_WKEN_CONFIG                         (1)

/*--------------------------------------------------------------------------*/
/* WWDT Configuration                                                       */
/*--------------------------------------------------------------------------*/
/* Configure WWDT Prescale value */
/* Configure WWDT Compare Data Value */
#define WWDT_CMPDAT_CONFIG                      (62)

/* Configure WWDT Interrupt */
#define WWDT_INTEN_CONFIG                       (1)

/*** (C) COPYRIGHT 2016-2021 Nuvoton Technology Corp. ***/
