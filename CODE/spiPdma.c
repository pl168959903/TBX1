#include "spiPdma.h"

volatile uint8_t pdmaSpiLock = 0;

uint8_t spi_cmd_readWrite( uint8_t cmd, uint8_t readData[], uint8_t writeData[], uint32_t length ) {
    static uint8_t writeNullData = 0x00;
    static uint8_t readNullData  = 0x00;
    DMA_DESC_T     cmdDesc = { 0 }, writeDataDesc = { 0 }, statusDesc = { 0 }, readDataDesc = { 0 };
    uint8_t        status;

    cmdDesc = ( DMA_DESC_T ){
        .ctl    = RF_DESC_CTL | RF_WD_ADDR_TYPE,  //
        .src    = ( uint32_t )&cmd,               //
        .dest   = ( uint32_t )&SPI0->TX,          //
        .offset = 0                               //
    };
    statusDesc = ( DMA_DESC_T ){
        .ctl    = RF_DESC_CTL | RF_RD_ADDR_TYPE,  //
        .src    = ( uint32_t )&SPI0->RX,          //
        .dest   = ( uint32_t )&status,            //
        .offset = 0                               //
    };
    if ( length > 0 ) {
        cmdDesc.ctl |= PDMA_TBINTDIS_DISABLE;
        cmdDesc.offset = ( uint32_t )&writeDataDesc - ( PDMA->SCATBA );

        statusDesc.ctl |= PDMA_TBINTDIS_DISABLE;
        statusDesc.offset = ( uint32_t )&readDataDesc - ( PDMA->SCATBA );

        if ( writeData != NULL ) {
            writeDataDesc = ( DMA_DESC_T ){
                .ctl    = ( ( length - 1UL ) << PDMA_DSCT_CTL_TXCNT_Pos ) | RF_DESC_CTL | RF_WD_ADDR_TYPE,  //
                .src    = ( uint32_t )&writeData[ 0 ],                                                      //
                .dest   = ( uint32_t )&SPI0->TX,                                                            //
                .offset = 0                                                                                 //
            };
        }
        else {
            writeDataDesc = ( DMA_DESC_T ){
                .ctl    = ( ( length - 1UL ) << PDMA_DSCT_CTL_TXCNT_Pos ) | RF_DESC_CTL | RF_NULL_WD_ADDR_TYPE,  //
                .src    = ( uint32_t )&writeNullData,                                                            //
                .dest   = ( uint32_t )&SPI0->TX,                                                                 //
                .offset = 0                                                                                      //
            };
        }

        if ( readData != NULL ) {
            readDataDesc = ( DMA_DESC_T ){
                .ctl    = ( ( length - 1UL ) << PDMA_DSCT_CTL_TXCNT_Pos ) | RF_DESC_CTL | RF_RD_ADDR_TYPE,  //
                .src    = ( uint32_t )&SPI0->RX,                                                            //
                .dest   = ( uint32_t )&readData[ 0 ],                                                       //
                .offset = 0                                                                                 //
            };
        }
        else {
            readDataDesc = ( DMA_DESC_T ){
                .ctl    = ( ( length - 1UL ) << PDMA_DSCT_CTL_TXCNT_Pos ) | RF_DESC_CTL | RF_NULL_RD_ADDR_TYPE,  //
                .src    = ( uint32_t )&SPI0->RX,                                                                 //
                .dest   = ( uint32_t )&readNullData,                                                             //
                .offset = 0                                                                                      //
            };
        }
    }

    PDMA->DSCT[ SPI_TX_PDMA_CH ].CTL = PDMA_OP_SCATTER;
    PDMA->DSCT[ SPI_RX_PDMA_CH ].CTL = PDMA_OP_SCATTER;

    PDMA->DSCT[ SPI_TX_PDMA_CH ].NEXT = ( uint32_t )&cmdDesc - ( PDMA->SCATBA );
    PDMA->DSCT[ SPI_RX_PDMA_CH ].NEXT = ( uint32_t )&statusDesc - ( PDMA->SCATBA );

    pdmaSpiLock = 1;
    PDMA->CHCTL |= SPI_PDMA_CH_EN_MSK;
    SPI0->PDMACTL |= ( SPI_PDMACTL_TXPDMAEN_Msk | SPI_PDMACTL_RXPDMAEN_Msk );

    while ( pdmaSpiLock == 1 ) { __NOP(); }

    PDMA->PAUSE = SPI_PDMA_CH_EN_MSK;
    SPI0->PDMACTL |= SPI_PDMACTL_PDMARST_Msk;
    return status;
}

// void debug_functions( void ) {
//     printf( "wireless debug.\n" );

//     uint8_t status;

//     Timer_delay( 1000 );

//     for ( uint8_t i = 0; i <= 0x1C; i++ ) {
//         uint8_t reg;
//         status = rf_cmd_readWrite( i, &reg, NULL, 1 );
//         printf( "status = 0x%02X, Reg 0x%02X =  0x%02X.\n", status, i, reg );
//     }
//     while ( 1 ) { __NOP(); }
// }
