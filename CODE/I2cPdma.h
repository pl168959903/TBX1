#ifndef __I2C_PDMA_H
#define __I2C_PDMA_H

#include "TBX1.h"


#define I2C_ADDRESS_LENGTH (1)
#define I2C_REG_LENGTH     (1)

#define I2C_TX_ADDR_TYPE          ( PDMA_SAR_INC | PDMA_DAR_FIX )
#define I2C_RX_ADDR_TYPE          ( PDMA_SAR_FIX | PDMA_DAR_INC )
#define I2C_CTL                   ( PDMA_OP_SCATTER | PDMA_WIDTH_8 | PDMA_REQ_SINGLE | PDMA_BURST_1 )
#define I2C_DESC_DATA_LENGTH( n ) ( ( n - 1UL ) << PDMA_DSCT_CTL_TXCNT_Pos )

#define I2C_DESC_TX_CTL ( I2C_CTL | I2C_TX_ADDR_TYPE )
#define I2C_DESC_RX_CTL ( I2C_CTL | I2C_RX_ADDR_TYPE )

#define I2C_DESC_ADDR_CTL ( I2C_DESC_DATA_LENGTH( I2C_ADDRESS_LENGTH ) | I2C_DESC_TX_CTL )
#define I2C_DESC_REG_CTL  ( I2C_DESC_DATA_LENGTH( I2C_REG_LENGTH ) | I2C_DESC_TX_CTL )

#define I2C_DESC_OFFSET( addr ) ( ( ( uint32_t )( addr ) ) - ( PDMA->SCATBA ) )
#define I2C_DESC_I2C_DATA_ADDR  ( ( uint32_t )( &( I2C0->DAT ) ) )

#define I2C_TX_PDMA_CH 1
#define I2C_RX_PDMA_CH 2
#define I2C_PDMA_TX_CH_EN_MSK ( 1UL << I2C_TX_PDMA_CH )
#define I2C_PDMA_RX_CH_EN_MSK ( 1UL << I2C_RX_PDMA_CH )



#endif  
