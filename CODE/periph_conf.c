#include "TBX1.h"
void CLK_Init(void)
{

    /* Configure clock detector functions, which are able to monitor the operations of HXT and LXT */
    CLK->CLKDCTL = (CLK_CLKDCTL_HXTFDEN_Msk | CLK_CLKDCTL_HXTFQDEN_Msk | CLK_CLKDCTL_LXTFDEN_Msk);

    /* Configure the upper bound and lower bound of HXT frequency range detetor */
    CLK->CDUPB  = CLK_HXT_FREQUENCY_RANGE_UPPER_BOUND;
    CLK->CDLOWB = CLK_HXT_FREQUENCY_RANGE_LOWER_BOUND;

    /* Unlock protected registers before we configure power-down wakeup settings. */
    SYS_UnlockReg();

    /* NVIC enable clock power down wake-up interrupt */
    NVIC_EnableIRQ(PWRWU_IRQn);

    /* Enable the power down wake-up interrupt in clock controller. */
    CLK->PWRCTL |= (CLK_PWRCTL_PDWKIEN_Msk);

    /* Lock protected registers after we configure power-down wakeup settings. */
    SYS_LockReg();

    /* NVIC enable clock fail interrupt */
    NVIC_EnableIRQ(CKFAIL_IRQn);

    /* Enable the selected clock detector interrupt source in clock controller. */
    CLK->CLKDCTL |= (CLK_CLKDCTL_HXTFIEN_Msk | CLK_CLKDCTL_HXTFQIEN_Msk | CLK_CLKDCTL_LXTFIEN_Msk);

}
void CRC_Init()
{
    CRC_Open(CRC_32, 0, CRC_SEED, CRC_WDATA_8);
}
void ADC_Init(void)
{
    ADC_POWER_ON(ADC);

    ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_CONTINUOUS,

             BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT29
            );

    ADC_SetExtendSampleTime(ADC, 0, 255);

    ADC_ENABLE_PDMA(ADC);

}
void FMC_Init()
{

    /* Enable FMC ISP function */
    FMC_Open();

    /* APROM update enable */
    FMC_ENABLE_AP_UPDATE();

    /* LDROM update enable */
    FMC_ENABLE_LD_UPDATE();

    /* SPROM update enable */
    FMC_ENABLE_SP_UPDATE();

    /* Config update enable */
    FMC_ENABLE_CFG_UPDATE();

}
/*--------------------------------------------------------------------------------------------*/
/* GPIO                                                                                     */
/*--------------------------------------------------------------------------------------------*/
void GPIO_Init()
{
    volatile const uint32_t u32RegDefault = 0;

    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCTL_DBCLKSRC_LIRC, GPIO_DBCTL_DBCLKSEL_2048);

    //GPIO_PA mode
    PA->MODE =

        (0x0UL << GPIO_MODE_MODE6_Pos) |

        (0x0UL << GPIO_MODE_MODE7_Pos) |

        (0x1UL << GPIO_MODE_MODE8_Pos) |

        (0x0UL << GPIO_MODE_MODE9_Pos) |

        (0x3UL << GPIO_MODE_MODE10_Pos) |

        (0x3UL << GPIO_MODE_MODE11_Pos) |

        u32RegDefault;

    //GPIO_PA disable digital input path
    PA->DINOFF =

        (0 << GPIO_DINOFF_DINOFF6_Pos) |

        (0 << GPIO_DINOFF_DINOFF7_Pos) |

        (0 << GPIO_DINOFF_DINOFF8_Pos) |

        (0 << GPIO_DINOFF_DINOFF9_Pos) |

        (0 << GPIO_DINOFF_DINOFF10_Pos) |

        (0 << GPIO_DINOFF_DINOFF11_Pos) |

        u32RegDefault;

    //GPIO_PA out data
    PA->DOUT =

        (1 << GPIO_DOUT_DOUT6_Pos) |

        (1 << GPIO_DOUT_DOUT7_Pos) |

        (0 << GPIO_DOUT_DOUT8_Pos) |

        (1 << GPIO_DOUT_DOUT9_Pos) |

        (1 << GPIO_DOUT_DOUT10_Pos) |

        (1 << GPIO_DOUT_DOUT11_Pos) |

        u32RegDefault;

    //GPIO_PA DOUT mask
    PA->DATMSK =

        (0 << GPIO_DATMSK_DATMSK6_Pos) |

        (0 << GPIO_DATMSK_DATMSK7_Pos) |

        (0 << GPIO_DATMSK_DATMSK8_Pos) |

        (0 << GPIO_DATMSK_DATMSK9_Pos) |

        (0 << GPIO_DATMSK_DATMSK10_Pos) |

        (0 << GPIO_DATMSK_DATMSK11_Pos) |

        u32RegDefault;

    //GPIO_PA enable de-bounce
    PA->DBEN =

        (0 << GPIO_DBEN_DBEN6_Pos) |

        (0 << GPIO_DBEN_DBEN7_Pos) |

        (0 << GPIO_DBEN_DBEN8_Pos) |

        (1 << GPIO_DBEN_DBEN9_Pos) |

        (1 << GPIO_DBEN_DBEN10_Pos) |

        (1 << GPIO_DBEN_DBEN11_Pos) |

        u32RegDefault;

    //GPIO_PA edge or level interrupt
    PA->INTTYPE =

        (GPIO_INTTYPE_EDGE << GPIO_INTTYPE_TYPE6_Pos) |

        (GPIO_INTTYPE_EDGE << GPIO_INTTYPE_TYPE7_Pos) |

        (GPIO_INTTYPE_EDGE << GPIO_INTTYPE_TYPE8_Pos) |

        (GPIO_INTTYPE_EDGE << GPIO_INTTYPE_TYPE9_Pos) |

        (GPIO_INTTYPE_EDGE << GPIO_INTTYPE_TYPE10_Pos) |

        (GPIO_INTTYPE_EDGE << GPIO_INTTYPE_TYPE11_Pos) |

        u32RegDefault;

    //GPIO_PA enable falling(edge) or low(level) interrupt
    PA->INTEN =

        (1 << GPIO_INTEN_FLIEN6_Pos) |

        (1 << GPIO_INTEN_FLIEN7_Pos) |

        (0 << GPIO_INTEN_FLIEN8_Pos) |

        (1 << GPIO_INTEN_FLIEN9_Pos) |

        (1 << GPIO_INTEN_FLIEN10_Pos) |

        (1 << GPIO_INTEN_FLIEN11_Pos) |

        u32RegDefault;

    //GPIO_PA enable rising(edge) or high(level) interrupt
    PA->INTEN |=

        (0 << GPIO_INTEN_RHIEN6_Pos) |

        (0 << GPIO_INTEN_RHIEN7_Pos) |

        (0 << GPIO_INTEN_RHIEN8_Pos) |

        (1 << GPIO_INTEN_RHIEN9_Pos) |

        (1 << GPIO_INTEN_RHIEN10_Pos) |

        (1 << GPIO_INTEN_RHIEN11_Pos) |

        u32RegDefault;

    NVIC_EnableIRQ(GPIO_PAPBPGPH_IRQn);

    NVIC_EnableIRQ(EINT024_IRQn);//PA.6

    NVIC_EnableIRQ(EINT135_IRQn);//PA.7

    //GPIO_PB mode
    PB->MODE =

        (0x2UL << GPIO_MODE_MODE6_Pos) |

        (0x2UL << GPIO_MODE_MODE7_Pos) |

        (0x2UL << GPIO_MODE_MODE8_Pos) |

        (0x2UL << GPIO_MODE_MODE9_Pos) |

        u32RegDefault;

    //GPIO_PB disable digital input path
    PB->DINOFF =

        (0 << GPIO_DINOFF_DINOFF6_Pos) |

        (0 << GPIO_DINOFF_DINOFF7_Pos) |

        (0 << GPIO_DINOFF_DINOFF8_Pos) |

        (0 << GPIO_DINOFF_DINOFF9_Pos) |

        u32RegDefault;

    //GPIO_PB out data
    PB->DOUT =

        (1 << GPIO_DOUT_DOUT6_Pos) |

        (1 << GPIO_DOUT_DOUT7_Pos) |

        (1 << GPIO_DOUT_DOUT8_Pos) |

        (1 << GPIO_DOUT_DOUT9_Pos) |

        u32RegDefault;

    //GPIO_PB DOUT mask
    PB->DATMSK =

        (0 << GPIO_DATMSK_DATMSK6_Pos) |

        (0 << GPIO_DATMSK_DATMSK7_Pos) |

        (0 << GPIO_DATMSK_DATMSK8_Pos) |

        (0 << GPIO_DATMSK_DATMSK9_Pos) |

        u32RegDefault;

    //GPIO_PB enable de-bounce
    PB->DBEN =

        (0 << GPIO_DBEN_DBEN6_Pos) |

        (0 << GPIO_DBEN_DBEN7_Pos) |

        (0 << GPIO_DBEN_DBEN8_Pos) |

        (0 << GPIO_DBEN_DBEN9_Pos) |

        u32RegDefault;

    //GPIO_PB edge or level interrupt
    PB->INTTYPE =

        (GPIO_INTTYPE_EDGE << GPIO_INTTYPE_TYPE6_Pos) |

        (GPIO_INTTYPE_EDGE << GPIO_INTTYPE_TYPE7_Pos) |

        (GPIO_INTTYPE_EDGE << GPIO_INTTYPE_TYPE8_Pos) |

        (GPIO_INTTYPE_EDGE << GPIO_INTTYPE_TYPE9_Pos) |

        u32RegDefault;

    //GPIO_PB enable falling(edge) or low(level) interrupt
    PB->INTEN =

        (0 << GPIO_INTEN_FLIEN6_Pos) |

        (0 << GPIO_INTEN_FLIEN7_Pos) |

        (0 << GPIO_INTEN_FLIEN8_Pos) |

        (0 << GPIO_INTEN_FLIEN9_Pos) |

        u32RegDefault;

    //GPIO_PB enable rising(edge) or high(level) interrupt
    PB->INTEN |=

        (0 << GPIO_INTEN_RHIEN6_Pos) |

        (0 << GPIO_INTEN_RHIEN7_Pos) |

        (0 << GPIO_INTEN_RHIEN8_Pos) |

        (0 << GPIO_INTEN_RHIEN9_Pos) |

        u32RegDefault;

    //GPIO_PC mode
    PC->MODE =

        (0x1UL << GPIO_MODE_MODE0_Pos) |

        u32RegDefault;

    //GPIO_PC disable digital input path
    PC->DINOFF =

        (0 << GPIO_DINOFF_DINOFF0_Pos) |

        u32RegDefault;

    //GPIO_PC out data
    PC->DOUT =

        (0 << GPIO_DOUT_DOUT0_Pos) |

        u32RegDefault;

    //GPIO_PC DOUT mask
    PC->DATMSK =

        (0 << GPIO_DATMSK_DATMSK0_Pos) |

        u32RegDefault;

    //GPIO_PC enable de-bounce
    PC->DBEN =

        (0 << GPIO_DBEN_DBEN0_Pos) |

        u32RegDefault;

    //GPIO_PC edge or level interrupt
    PC->INTTYPE =

        (GPIO_INTTYPE_EDGE << GPIO_INTTYPE_TYPE0_Pos) |

        u32RegDefault;

    //GPIO_PC enable falling(edge) or low(level) interrupt
    PC->INTEN =

        (0 << GPIO_INTEN_FLIEN0_Pos) |

        u32RegDefault;

    //GPIO_PC enable rising(edge) or high(level) interrupt
    PC->INTEN |=

        (0 << GPIO_INTEN_RHIEN0_Pos) |

        u32RegDefault;

}
void I2C0_Init()
{
    I2C_Open(I2C0, 400000);

    I2C_EnableTimeout(I2C0, I2C0_TIMEOUT_SEL);

    I2C0->CTL1 |= I2C_CTL1_TXPDMAEN_Msk | I2C_CTL1_RXPDMAEN_Msk;

    I2C_EnableInt(I2C0);
    NVIC_EnableIRQ(I2C0_IRQn);

}
/*************************************/
/**             PDMA                **/
/*************************************/

uint16_t g_au16CH0DesArray[PDMA_CH0_DATA_LENGTH];
uint16_t *g_pu16CH0DesArray;

uint8_t g_au8CH1SrcArray[PDMA_CH1_DATA_LENGTH];
uint8_t *g_pu8CH1SrcArray;

uint8_t g_au8CH2DesArray[PDMA_CH2_DATA_LENGTH];
uint8_t *g_pu8CH2DesArray;

uint8_t g_au8CH3SrcArray[PDMA_CH3_DATA_LENGTH];
uint8_t *g_pu8CH3SrcArray;

uint8_t g_au8CH4DesArray[PDMA_CH4_DATA_LENGTH];
uint8_t *g_pu8CH4DesArray;

void PDMA_Init()
{

    g_pu16CH0DesArray = g_au16CH0DesArray;

    /* Open CH0 */
    PDMA_Open(PDMA, 1 << 0);

    /* Transfer count is PDMA_CH0_DATA_LENGTH, transfer width is PDMA_WIDTH_16 */
    PDMA_SetTransferCnt(PDMA, 0, PDMA_WIDTH_16, PDMA_CH0_DATA_LENGTH);

    /* Set source address is &ADC->ADPDMA, destination address is g_pu16CH0DesArray */
    PDMA_SetTransferAddr(PDMA, 0, (uint32_t)&ADC->ADPDMA, PDMA_SAR_FIX, (uint32_t)g_pu16CH0DesArray, PDMA_DAR_INC);

    /* Set transfer mode and burst size */
    PDMA_SetBurstType(PDMA, 0, PDMA_REQ_SINGLE, PDMA_BURST_1);

    /* Enable timeout function and set timeout counter */
    PDMA_SetTimeOut(PDMA, 0, 1, 65535);

    /* Request source is PDMA_ADC_RX */
    PDMA_SetTransferMode(PDMA, 0, PDMA_ADC_RX, 0, 0);

    g_pu8CH1SrcArray = g_au8CH1SrcArray;

    /* Open CH1 */
    PDMA_Open(PDMA, 1 << 1);

    /* Transfer count is PDMA_CH1_DATA_LENGTH, transfer width is PDMA_WIDTH_8 */
    PDMA_SetTransferCnt(PDMA, 1, PDMA_WIDTH_8, PDMA_CH1_DATA_LENGTH);

    /* Set source address is g_pu8CH1SrcArray, destination address is &I2C0->DAT */
    PDMA_SetTransferAddr(PDMA, 1, (uint32_t)g_pu8CH1SrcArray, PDMA_SAR_INC, (uint32_t)&I2C0->DAT, PDMA_DAR_FIX);

    /* Set transfer mode and burst size */
    PDMA_SetBurstType(PDMA, 1, PDMA_REQ_SINGLE, PDMA_BURST_1);

    /* Enable timeout function and set timeout counter */
    PDMA_SetTimeOut(PDMA, 1, 1, 65535);

    /* Request source is PDMA_I2C0_TX */
    PDMA_SetTransferMode(PDMA, 1, PDMA_I2C0_TX, 0, 0);

    g_pu8CH2DesArray = g_au8CH2DesArray;

    /* Open CH2 */
    PDMA_Open(PDMA, 1 << 2);

    /* Transfer count is PDMA_CH2_DATA_LENGTH, transfer width is PDMA_WIDTH_8 */
    PDMA_SetTransferCnt(PDMA, 2, PDMA_WIDTH_8, PDMA_CH2_DATA_LENGTH);

    /* Set source address is &I2C0->DAT, destination address is g_pu8CH2DesArray */
    PDMA_SetTransferAddr(PDMA, 2, (uint32_t)&I2C0->DAT, PDMA_SAR_FIX, (uint32_t)g_pu8CH2DesArray, PDMA_DAR_INC);

    /* Set transfer mode and burst size */
    PDMA_SetBurstType(PDMA, 2, PDMA_REQ_SINGLE, PDMA_BURST_1);

    /* Request source is PDMA_I2C0_RX */
    PDMA_SetTransferMode(PDMA, 2, PDMA_I2C0_RX, 0, 0);

    g_pu8CH3SrcArray = g_au8CH3SrcArray;

    /* Open CH3 */
    PDMA_Open(PDMA, 1 << 3);

    /* Transfer count is PDMA_CH3_DATA_LENGTH, transfer width is PDMA_WIDTH_8 */
    PDMA_SetTransferCnt(PDMA, 3, PDMA_WIDTH_8, PDMA_CH3_DATA_LENGTH);

    /* Set source address is g_pu8CH3SrcArray, destination address is &SPI0->TX */
    PDMA_SetTransferAddr(PDMA, 3, (uint32_t)g_pu8CH3SrcArray, PDMA_SAR_INC, (uint32_t)&SPI0->TX, PDMA_DAR_FIX);

    /* Set transfer mode and burst size */
    PDMA_SetBurstType(PDMA, 3, PDMA_REQ_SINGLE, PDMA_BURST_1);

    /* Request source is PDMA_SPI0_TX */
    PDMA_SetTransferMode(PDMA, 3, PDMA_SPI0_TX, 0, 0);

    g_pu8CH4DesArray = g_au8CH4DesArray;

    /* Open CH4 */
    PDMA_Open(PDMA, 1 << 4);

    /* Transfer count is PDMA_CH4_DATA_LENGTH, transfer width is PDMA_WIDTH_8 */
    PDMA_SetTransferCnt(PDMA, 4, PDMA_WIDTH_8, PDMA_CH4_DATA_LENGTH);

    /* Set source address is &SPI0->RX, destination address is g_pu8CH4DesArray */
    PDMA_SetTransferAddr(PDMA, 4, (uint32_t)&SPI0->RX, PDMA_SAR_FIX, (uint32_t)g_pu8CH4DesArray, PDMA_DAR_INC);

    /* Set transfer mode and burst size */
    PDMA_SetBurstType(PDMA, 4, PDMA_REQ_SINGLE, PDMA_BURST_1);

    /* Request source is PDMA_SPI0_RX */
    PDMA_SetTransferMode(PDMA, 4, PDMA_SPI0_RX, 0, 0);

    PDMA_EnableInt(PDMA, 0, PDMA_INT_TRANS_DONE);

    PDMA_EnableInt(PDMA, 0, PDMA_INT_TIMEOUT);

    PDMA_EnableInt(PDMA, 1, PDMA_INT_TRANS_DONE);

    PDMA_EnableInt(PDMA, 1, PDMA_INT_TIMEOUT);

    PDMA_EnableInt(PDMA, 2, PDMA_INT_TRANS_DONE);

    PDMA_EnableInt(PDMA, 3, PDMA_INT_TRANS_DONE);

    PDMA_EnableInt(PDMA, 4, PDMA_INT_TRANS_DONE);

    NVIC_EnableIRQ(PDMA_IRQn);

}
void RTC_Init(void)
{
    S_RTC_TIME_DATA_T sDateTimeInit, sDateTimeAlarm;

    /* Configure RTC initial date and time structure. */
    sDateTimeInit.u32Year       = 2022;
    sDateTimeInit.u32Month      = 1;
    sDateTimeInit.u32Day        = 1;
    sDateTimeInit.u32DayOfWeek  = RTC_SATURDAY;
    sDateTimeInit.u32Hour       = 0;
    sDateTimeInit.u32Minute     = 0;
    sDateTimeInit.u32Second     = 0;
    sDateTimeInit.u32TimeScale  = RTC_CLOCK_24;
    sDateTimeInit.u32AmPm       = 0;

    /* Initial RTC module. Setup RTC current date and time. */
    RTC_Open(&sDateTimeInit);

    /* Setup RTC tick period */
    RTC_SetTickPeriod(RTC_TICK_1_8_SEC);

    /* Configure RTC alarm date and time structure. */
    sDateTimeAlarm.u32Year       = RTC_YEAR2000;
    sDateTimeAlarm.u32Month      = 0;
    sDateTimeAlarm.u32Day        = 0;
    sDateTimeAlarm.u32Hour       = 0;
    sDateTimeAlarm.u32Minute     = 0;
    sDateTimeAlarm.u32Second     = 0;
    sDateTimeAlarm.u32TimeScale  = RTC_CLOCK_24;
    sDateTimeAlarm.u32AmPm       = 0;

    /* Setup RTC alarm date and time configurations */
    RTC_SetAlarmDateAndTime(&sDateTimeAlarm);

    /* Setup RTC alarm date mask. */
    RTC_SetAlarmDateMask(RTC_ALARM_TEN_YEAR_MASK,  RTC_ALARM_ONE_YEAR_MASK,
                         RTC_ALARM_TEN_MONTH_MASK, RTC_ALARM_ONE_MONTH_MASK,
                         RTC_ALARM_TEN_DATE_MASK,  RTC_ALARM_ONE_DATE_MASK);

    /* Setup RTC alarm time mask. */
    RTC_SetAlarmTimeMask(RTC_ALARM_TEN_HOUR_MASK,   RTC_ALARM_ONE_HOUR_MASK,
                         RTC_ALARM_TEN_MINUTE_MASK, RTC_ALARM_ONE_MINUTE_MASK,
                         RTC_ALARM_TEN_SECOND_MASK, RTC_ALARM_ONE_SECOND_MASK);

    /* Set Frequency Compensation Data */
    RTC_32KCalibration(RTC_LXT_FREQ);

    /* Clear RTC interrupt source and flag for safe */
    RTC_DisableInt(RTC_INTEN_TICKIEN_Msk | RTC_INTEN_ALMIEN_Msk);

    /* Enable RTC interrupt source */
    RTC_EnableInt(RTC_INTEN_TICKIEN_Msk | RTC_INTEN_ALMIEN_Msk);

    /* Enable RTC NVIC interrupt */
    NVIC_EnableIRQ(RTC_IRQn);

}
/*--------------------------------------------------------------------------------------------*/
/* SPII2S                                                                                     */
/*--------------------------------------------------------------------------------------------*/
void SPII2S_Init()
{

    /* This function make SPI module be ready to transfer */
    SPI_Open(SPI0, SPI_MASTER, SPI_MODE_3, 8, 10000000);

    /* Enable auto SS function */
    SPI0->SSCTL = (SPI0->SSCTL & (~SPI_SSCTL_AUTOSS_Msk)) | SPI_SSCTL_AUTOSS_Msk | SPI_SSCTL_SS_Msk;

    /* Select SS level */
    SPI0->SSCTL = (SPI0->SSCTL & (~SPI_SSCTL_SSACTPOL_Msk)) | SPI_SS_ACTIVE_HIGH;

    /* Set MSB first */
    SPI_SET_MSB_FIRST(SPI0);

    /* Set FIFO threshold */
    SPI_SetFIFO(SPI0, 7, 7);

    /* Enable interrupt type */
    SPI_EnableInt(SPI0, SPI_UNIT_INT_MASK | SPI_FIFO_RXOV_INT_MASK | SPI_FIFO_RXTO_INT_MASK);

    /* Enable interrupt function */
    NVIC_EnableIRQ(SPI0_IRQn);

}
void SYS_Init(void)
{

    /* Brown-Out Detector initialization */
    SYS_EnableBOD(SYS_BODCTL_BOD_RST_EN, SYS_BODCTL_BODVL_2_5V);

    /* Setup BOD de-glitch time */
    SYS->BODCTL = (SYS->BODCTL & ~SYS_BODCTL_BODDGSEL_Msk) | SYS_BODCTL_BODDGSEL_0HCLK;

    /* Setup BOD mode as Normal-Power Mode */
    SYS_CLEAR_BOD_LPM();

    /* Enable BOD NVIC interrupt */
    NVIC_EnableIRQ(BOD_IRQn);

    /* Low-Voltage Detector initialization */
    SYS_ENABLE_LVR();

    /* Setup LVR detector threshold voltage */
    SYS->BODCTL = (SYS->BODCTL & ~SYS_BODCTL_LVRVL_Msk) | (1 << SYS_BODCTL_LVRVL_Pos);

    /* Setup LVR de-glitch time */
    SYS->BODCTL = (SYS->BODCTL & ~SYS_BODCTL_LVRDGSEL_Msk) | SYS_BODCTL_LVRDGSEL_4096HCLK;

    SYS->HIRCTRIMCTL = ((SYS->HIRCTRIMCTL & ~(SYS_HIRCTRIMCTL_FREQSEL_Msk | SYS_HIRCTRIMCTL_LOOPSEL_Msk | SYS_HIRCTRIMCTL_RETRYCNT_Msk |
                                              SYS_HIRCTRIMCTL_REFCKSEL_Msk | SYS_HIRCTRIMCTL_BOUNDEN_Msk | SYS_HIRCTRIMCTL_BOUNDARY_Msk | SYS_HIRCTRIMCTL_CESTOPEN_Msk))
                        | (3 << SYS_HIRCTRIMCTL_RETRYCNT_Pos)
                        | (3 << SYS_HIRCTRIMCTL_LOOPSEL_Pos)
                        | (0 << SYS_HIRCTRIMCTL_REFCKSEL_Pos)
                        | (1 << SYS_HIRCTRIMCTL_BOUNDEN_Pos)
                        | (8 << SYS_HIRCTRIMCTL_BOUNDARY_Pos)
                        | (0 << SYS_HIRCTRIMCTL_CESTOPEN_Pos)
                        | (0x1 << SYS_HIRCTRIMCTL_FREQSEL_Pos));

    /* Enable HIRC Trim Interrupt */
    SYS->HIRCTRIMIEN = (2 | 4);

    /* NVIC Enable RC Trim Interrupt */
    NVIC_EnableIRQ(CKFAIL_IRQn);

}
void USBD_Init(void)
{

    USBD_Open(NULL, NULL, NULL);

    NVIC_EnableIRQ(USBD_IRQn);

}
void UART0_Init()
{

    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, UART0_BAUD);

    /* Set Data Format*/
    UART_SetLine_Config(UART0, 0, UART_WORD_LEN_8, UART_PARITY_NONE, UART_STOP_BIT_1);

    /* RX FIFO Interrupt Trigger Level */
    UART0->FIFO = (UART0->FIFO & ~ UART_FIFO_RFITL_Msk) | UART_FIFO_RFITL_1BYTE;

}
void WDT_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Setup WDT Configuration */
    WDT_Open(WDT_TIMEOUT_2POW20, WDT_RESET_DELAY_1026CLK, WDT_RSTEN_CONFIG, WDT_WKEN_CONFIG);

    /* Enable WDT Time-out Interrupt */
    WDT_EnableInt();

    /* NVIC Enable WDT Interrupt */
    NVIC_EnableIRQ(WDT_IRQn);

}
void WWDT_Init(void)
{
    /* Setup WWDT configurations.
       Note that WWDT CTL register can only be written once after the chip resets. */

    WWDT_Open(WWDT_PRESCALER_2048, WWDT_CMPDAT_CONFIG, WWDT_INTEN_CONFIG);

    /* NVIC Enable WWDT Interrupt */
    NVIC_EnableIRQ(WDT_IRQn);

}
void HDIV_Init()
{
    /* Set dividend and divisor */
    HDIV->DIVIDEND = 0;
    HDIV->DIVISOR = 1;
}

void Periph_Init(void)
{

    CLK_Init();

    SYS_Init();

    PDMA_Init();

    UART0_Init();

    CRC_Init();

    GPIO_Init();

    ADC_Init();

    I2C0_Init();

    SPII2S_Init() ;

    USBD_Init();

    WDT_Init();

    WWDT_Init();

    RTC_Init();

    FMC_Init();

    HDIV_Init();

}
