//#include <Windows.h>
//#include <conio.h>

#include <stdio.h>
#include <string.h>

//#include "TBX1.h"

#include "NuMicro.h"

#include "driver/inv_mems_hw_config.h"
#include "invn/common/invn_types.h"



int inv_serial_interface_write_hook(uint16_t reg, uint32_t length, uint8_t *data)
{
    uint32_t dataByte = I2C_WriteMultiBytesOneReg( I2C0, ACCEL_GYRO_CHIP_ADDR, reg, data, length );

    return ((dataByte > 0)?0:-1);
}

int inv_serial_interface_read_hook(uint16_t reg, uint32_t length, uint8_t *data)
{
	uint32_t dataByte = I2C_ReadMultiBytesOneReg( I2C0, ACCEL_GYRO_CHIP_ADDR, reg, data, length );

	return ((dataByte > 0)?0:-1);
}

/**
 *  @brief  Sleep function.
**/
void inv_sleep(unsigned long mSecs)
{
	uint32_t i;
    for(i=0; i<mSecs; i++){
        CLK_SysTickDelay(1000);
    }
}

void inv_sleep_100us(unsigned long nHowMany100MicroSecondsToSleep)
{
    uint32_t i;
    for(i=0; i<nHowMany100MicroSecondsToSleep; i++){
        CLK_SysTickDelay(100);
    }
}

/**
 *  @brief  get system's internal tick count.
 *          Used for time reference.
 *  @return current tick count.
**/
long inv_get_tick_count(void)
{
    unsigned long count;


    return (long)count;
}

