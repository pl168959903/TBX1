#ifndef __SPI_PDMA_H
#define __SPI_PDMA_H

#include "TBX1.h"

#define SPI_TX_PDMA_CH 3
#define SPI_RX_PDMA_CH 4
#define SPI_PDMA_CH_EN_MSK ((1UL << SPI_TX_PDMA_CH) | (1UL << SPI_RX_PDMA_CH))

#define RF_DESC_CTL          ( PDMA_WIDTH_8 | PDMA_REQ_SINGLE | PDMA_BURST_1 | PDMA_OP_SCATTER )
#define RF_WD_ADDR_TYPE      ( PDMA_SAR_INC | PDMA_DAR_FIX )
#define RF_NULL_WD_ADDR_TYPE ( PDMA_SAR_FIX | PDMA_DAR_FIX )
#define RF_RD_ADDR_TYPE      ( PDMA_SAR_FIX | PDMA_DAR_INC )
#define RF_NULL_RD_ADDR_TYPE ( PDMA_SAR_FIX | PDMA_DAR_FIX )

typedef struct dma_desc_t {
    uint32_t ctl;
    uint32_t src;
    uint32_t dest;
    uint32_t offset;
} DMA_DESC_T;

uint8_t spi_cmd_readWrite( uint8_t cmd, uint8_t readData[], uint8_t writeData[], uint32_t length );



#endif
