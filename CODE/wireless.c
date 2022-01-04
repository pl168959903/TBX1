#include "wireless.h"

uint8_t rf_tx_addr[ 5 ] = { 0x00, 0x20, 0x21, 0x12, 0x30 };
uint8_t rf_rx_addr[ 5 ] = { 0x01, 0x20, 0x21, 0x12, 0x30 };

uint8_t *rf_self_addr;
uint8_t *rf_connt_addr;

volatile uint8_t rf_int_flag = 0;

static __inline void RF24_CE_Enable( void ) {
    PC0 = 1;
}

static __inline void RF24_CE_Disable( void ) {
    PC0 = 0;
}

uint8_t RF24_CmdReadWrite( uint8_t cmd, uint8_t readData[], uint8_t writeData[], uint32_t length ) {
    return spi_cmd_readWrite( cmd, readData, writeData, length );
}

uint8_t RF24_RegRead( uint8_t reg, uint8_t readData[], uint32_t length ) {
    return RF24_CmdReadWrite( reg | L01_CMD_R_REGISTER, readData, NULL, length );
}

uint8_t RF24_RegOneRead( uint8_t reg ) {
    uint8_t result;
    RF24_RegRead( reg, &result, 1 );
    return result;
}

uint8_t RF24_RegWrite( uint8_t reg, uint8_t writeData[], uint32_t length ) {
    return RF24_CmdReadWrite( reg | L01_CMD_W_REGISTER, NULL, writeData, length );
}

uint8_t RF24_RegOneWrite( uint8_t reg, uint8_t writeData ) {
    return RF24_CmdReadWrite( reg | L01_CMD_W_REGISTER, NULL, ( uint8_t[] ){ writeData }, 1 );
}

uint8_t RF24_RegOrWrite( uint8_t reg, uint8_t writeData ) {
    uint8_t status, data;
    status = RF24_RegRead( reg, &data, 1 );
    data |= writeData;
    status = RF24_RegWrite( reg, &data, 1 );
    return status;
}

uint8_t RF24_RegAndWrite( uint8_t reg, uint8_t writeData ) {
    uint8_t status, data;
    status = RF24_RegRead( reg, &data, 1 );
    data &= writeData;
    status = RF24_RegWrite( reg, &data, 1 );
    return status;
}

void RF24_init( void ) {

    RF24_RegWrite( 0x00, ( uint8_t[] ){ 0x08 }, 1 );
    RF24_RegWrite( 0x01, ( uint8_t[] ){ 0x03 }, 1 );
    RF24_RegWrite( 0x02, ( uint8_t[] ){ 0x03 }, 1 );
    RF24_RegWrite( 0x03, ( uint8_t[] ){ 0x03 }, 1 );
    RF24_RegWrite( 0x04, ( uint8_t[] ){ 0xFF }, 1 );
    RF24_RegWrite( 0x05, ( uint8_t[] ){ 0x64 }, 1 );  // 2400 + 100(0x64) = 2.5GHz
    RF24_RegWrite( 0x06, ( uint8_t[] ){ 0x0E }, 1 );
    RF24_RegWrite( 0x07, ( uint8_t[] ){ 0x70 }, 1 );
    RF24_RegWrite( 0x1C, ( uint8_t[] ){ 0x03 }, 1 );
    RF24_RegWrite( 0x1D, ( uint8_t[] ){ 0x07 }, 1 );
    RF24_CmdReadWrite( L01_CMD_FLUSH_TX, NULL, NULL, 0 );
    RF24_CmdReadWrite( L01_CMD_FLUSH_RX, NULL, NULL, 0 );
    Timer_delay( 1000 );
}

void RF24_Tx_init( void ) {
    rf_self_addr  = rf_tx_addr;
    rf_connt_addr = rf_rx_addr;
    RF24_RegOrWrite( 0x00, 0x02 );
    RF24_RegWrite( 0x0A, rf_connt_addr, 5 );
    RF24_RegWrite( 0x0B, rf_self_addr, 5 );
    RF24_RegWrite( 0x10, rf_connt_addr, 5 );
    RF24_CE_Enable();
}

void RF24_Rx_init( void ) {
    rf_self_addr  = rf_rx_addr;
    rf_connt_addr = rf_tx_addr;
    RF24_RegOrWrite( 0x00, 0x03 );
    RF24_RegWrite( 0x0A, rf_connt_addr, 5 );
    RF24_RegWrite( 0x0B, rf_self_addr, 5 );
    RF24_CE_Enable();
}

uint8_t RF24_GetStatus( void ) {
    return RF24_CmdReadWrite( L01_CMD_NOP, NULL, NULL, 0 );
}

void RF24_int_handler( void ) {
    rf_int_flag = 1;
}

void RF_InterruptClear( void ) {
    uint8_t status = RF24_GetStatus();
    RF24_RegOneWrite( L01_REG_STATUS, status );
    rf_int_flag = 0;
}

uint8_t RF24_Pull_RxPayload( uint8_t payload[] ) {
    uint8_t dataLength;
    RF24_CmdReadWrite( L01_CMD_R_RX_PL_WID, &dataLength, NULL, 1 );
    RF24_CmdReadWrite( L01_CMD_R_RX_PAYLOAD, payload, NULL, dataLength );
    return dataLength;
}

uint32_t RF24_SendPackat( uint8_t payload[], uint8_t dataLength, uint32_t resend ) {
    uint8_t  status, fsm = 0, rd = 0;
    uint32_t ts = 0;
    RF24_CmdReadWrite( L01_CMD_FLUSH_TX, NULL, NULL, 0 );
    RF24_CmdReadWrite( L01_CMD_W_TX_PAYLOAD, NULL, payload, dataLength );

    while ( 1 ) {
        switch ( fsm ) {
        case 0: {
            if ( RF_INT_PIN == 0 ) fsm = 1;
        } break;
        case 1: {
            status = RF24_GetStatus();
            if ( status & L01_REG_STATUS_TX_DS_MSK ) {
                rd  = 0;
                fsm = 2;
            }
            else if ( status & L01_REG_STATUS_MAX_RT_MSK )
                fsm = 3;
        } break;
        case 2: {
            RF24_RegOneWrite( L01_REG_STATUS, status );
            fsm = 4;
        } break;
        case 3: {
            if ( ++ts < resend ) {
                rd  = 1;
                fsm = 2;
            }
            else
                fsm = 5;
        } break;
        case 4: {
            if ( RF_INT_PIN == 1 ) {
                if ( rd == 0 )
                    fsm = 6;
                else
                    fsm = 0;
            }
        } break;
        case 5: {
            RF24_CmdReadWrite( L01_CMD_FLUSH_TX, NULL, NULL, 0 );
            rd  = 0;
            fsm = 2;
        } break;
        case 6: {
            return ts;
        } break;
        }
    }
}

void RF24_RegView( void ) {
    uint8_t CONFIG, EN_AA, EN_RXADDR, SETUP_AW, RF_CH, RF_SETUP, STATUS, OBSERVE_TX, RPD;
    uint8_t RX_ADDR_P0[ 5 ];
    uint8_t RX_ADDR_P1[ 5 ];
    uint8_t RX_ADDR_P2, RX_ADDR_P3, RX_ADDR_P4, RX_ADDR_P5;
    uint8_t TX_ADDR[ 5 ];
    uint8_t RX_PW_P0, RX_PW_P1, RX_PW_P2, RX_PW_P3, RX_PW_P4, RX_PW_P5, FIFO_STATUS, DYNPD, FEATURE;

    CONFIG     = RF24_RegOneRead( L01_REG_CFG );
    EN_AA      = RF24_RegOneRead( L01_REG_EN_AA );
    EN_RXADDR  = RF24_RegOneRead( L01_REG_EN_RXADDR );
    SETUP_AW   = RF24_RegOneRead( L01_REG_SETUP_AW );
    RF_CH      = RF24_RegOneRead( L01_REG_RF_CH );
    RF_SETUP   = RF24_RegOneRead( L01_REG_RF_SETUP );
    STATUS     = RF24_RegOneRead( L01_REG_STATUS );
    OBSERVE_TX = RF24_RegOneRead( L01_REG_OBSERVE_TX );
    RPD        = RF24_RegOneRead( L01_REG_CD );
    RF24_RegRead( L01_REG_RX_ADDR_P0, RX_ADDR_P0, 5 );
    RF24_RegRead( L01_REG_RX_ADDR_P1, RX_ADDR_P1, 5 );
    RX_ADDR_P2 = RF24_RegOneRead( L01_REG_RX_ADDR_P2 );
    RX_ADDR_P3 = RF24_RegOneRead( L01_REG_RX_ADDR_P3 );
    RX_ADDR_P4 = RF24_RegOneRead( L01_REG_RX_ADDR_P4 );
    RX_ADDR_P5 = RF24_RegOneRead( L01_REG_RX_ADDR_P5 );
    RF24_RegRead( L01_REG_TX_ADDR, TX_ADDR, 5 );
    RX_PW_P0    = RF24_RegOneRead( L01_REG_RX_PW_P0 );
    RX_PW_P1    = RF24_RegOneRead( L01_REG_RX_PW_P1 );
    RX_PW_P2    = RF24_RegOneRead( L01_REG_RX_PW_P2 );
    RX_PW_P3    = RF24_RegOneRead( L01_REG_RX_PW_P3 );
    RX_PW_P4    = RF24_RegOneRead( L01_REG_RX_PW_P4 );
    RX_PW_P5    = RF24_RegOneRead( L01_REG_RX_PW_P5 );
    FIFO_STATUS = RF24_RegOneRead( L01_REG_FIFO_STATUS );
    DYNPD       = RF24_RegOneRead( L01_REG_DYNPD );
    FEATURE     = RF24_RegOneRead( L01_REG_FEATURE );

    printf( "CONFIG : 0x%02X.\n", CONFIG );
    printf( "EN_AA = 0x%02X.\n", EN_AA );
    printf( "EN_RXADDR = 0x%02X.\n", EN_RXADDR );
    printf( "SETUP_AW = 0x%02X\n", SETUP_AW );
    printf( "RF_CH = 0x%02X.\n", RF_CH );
    printf( "RF_SETUP = 0x%02X.\n", RF_SETUP );
    printf( "STATUS = 0x%02X.\n", STATUS );
    printf( "OBSERVE_TX = 0x%02X.\n", OBSERVE_TX );
    printf( "RPD = 0x%02X.\n", RPD );
    printf( "RX_ADDR_P0 = 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X. \n", RX_ADDR_P0[ 0 ], RX_ADDR_P0[ 1 ], RX_ADDR_P0[ 2 ], RX_ADDR_P0[ 3 ], RX_ADDR_P0[ 4 ] );
    printf( "RX_ADDR_P1 = 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X. \n", RX_ADDR_P1[ 0 ], RX_ADDR_P1[ 1 ], RX_ADDR_P1[ 2 ], RX_ADDR_P1[ 3 ], RX_ADDR_P1[ 4 ] );
    printf( "RX_ADDR_P2 = 0x%02X.\n", RX_ADDR_P2 );
    printf( "RX_ADDR_P3 = 0x%02X.\n", RX_ADDR_P3 );
    printf( "RX_ADDR_P4 = 0x%02X.\n", RX_ADDR_P4 );
    printf( "RX_ADDR_P5 = 0x%02X.\n", RX_ADDR_P5 );
    printf( "TX_ADDR = 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X. \n", TX_ADDR[ 0 ], TX_ADDR[ 1 ], TX_ADDR[ 2 ], TX_ADDR[ 3 ], TX_ADDR[ 4 ] );
    printf( "RX_PW_P0 = 0x%02X.\n", RX_PW_P0 );
    printf( "RX_PW_P1 = 0x%02X.\n", RX_PW_P1 );
    printf( "RX_PW_P2 = 0x%02X.\n", RX_PW_P2 );
    printf( "RX_PW_P3 = 0x%02X.\n", RX_PW_P3 );
    printf( "RX_PW_P4 = 0x%02X.\n", RX_PW_P4 );
    printf( "RX_PW_P5 = 0x%02X.\n", RX_PW_P5 );
    printf( "FIFO_STATUS = 0x%02X.\n", FIFO_STATUS );
    printf( "DYNPD = 0x%02X.\n", DYNPD );
    printf( "FEATURE = 0x%02X.\n", FEATURE );
}

// static uint8_t testTxData32Byte[ 32 ] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
//                                           0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };

// uint8_t testRxData32Byte[ 32 ] = { 0 };

// #define RF_DEVICE_TX 1
// #define RF_DEVICE_RX 0

// #define RF_DEVICE RF_DEVICE_TX

// #if ( RF_DEVICE == RF_DEVICE_TX )

// // void debug_functions( void ) {
// //     printf( "RF debug.\n" );

// //     RF24_init();
// //     RF24_Tx_init();

// //     LED_B = 1;
// //     while ( 1 ) {
// //         // Timer_delay(100);
// //         uint32_t resend = RF24_SendPackat(testTxData32Byte, 32, 10);
// //         if(resend >= 10){
// //             LED_B = 1;
// //             LED_R = 0;
// //         }
// //         else{
// //             LED_B = 0;
// //             LED_R = 1;
// //         }
// //         // printf("resend = %d.\n", resend);

// //     }
// // }

// #define TEST_RF_TIME 10
// counter  counts_ms;
// uint32_t lost_c, packet_c;

// uint8_t counts_ms_handler( void ) {
//     counts_ms.Trigger( &counts_ms );
//     return counts_ms.en;
// }

// void debug_functions( void ) {
//     printf( "RF debug.\n" );
//     counter_init( &counts_ms );

//     RF24_init();
//     RF24_Tx_init();

//     uint32_t timeCounter = 0;
//     LED_B                = 1;

//     while ( 1 ) {
//         printf( "====================================.\n" );
//         printf( "Start RF Test.\n" );
//         printf( "Test time = %d s.\n", TEST_RF_TIME );
//         lost_c = packet_c = 0;
//         timeCounter       = 0;
//         counts_ms.SetCounter( &counts_ms , 0);
//         counts_ms.Enable( &counts_ms );
//         TIMER_Start( TIMER0 ); 
//         while ( counts_ms.GetCounter( &counts_ms ) < TEST_RF_TIME * 1000 ) {
//             uint32_t resend = RF24_SendPackat( testTxData32Byte, 32, 1 );
//             packet_c++;
//             lost_c += resend;
//         }
//         counts_ms.Disable( &counts_ms );


//         printf("Packet = %d ( %d Packet/s ).\n", packet_c, packet_c/TEST_RF_TIME);
//         printf("Lost Packet = %d (%f%%).\n", lost_c, ((float)lost_c / packet_c) * 100.0);
//         printf("Complete Packet = %d (%f%%).\n", (packet_c - lost_c), ((float)(packet_c - lost_c) / packet_c) * 100.0);
//         printf("Transfer rate = %d byte/s .\n", ((packet_c - lost_c) * 32) / TEST_RF_TIME);
//         Timer_delay(2000);
//     }
// }
// #endif

// #if ( RF_DEVICE == RF_DEVICE_RX )

// // void debug_functions( void ) {
// //     printf( "RF debug.\n" );

// //     uint8_t data;

// //     RF24_init();
// //     RF24_Rx_init();

// //     uint8_t statusData, fifo;
// //     while ( 1 ) {
// //         if ( rf_int_flag == 1 ) {
// //             statusData = RF24_CmdReadWrite( L01_CMD_NOP, NULL, NULL, 0 );
// //             printf( "statusData = 0x%02X.\n", statusData );

// //             if ( statusData & L01_REG_STATUS_RX_DR_MSK ) {
// //                 uint8_t ch = ( statusData & L01_REG_STATUS_RX_P_NO_MSK ) >> L01_REG_STATUS_RX_P_NO_POS;
// //                 uint8_t dataLength;
// //                 RF24_CmdReadWrite( L01_CMD_R_RX_PL_WID, &dataLength, NULL, 1 );
// //                 RF24_CmdReadWrite( L01_CMD_R_RX_PAYLOAD, testRxData32Byte, NULL, dataLength );

// //                 printf( "CH = %d, Length = %d.\n", ch, dataLength );

// //                 for ( uint8_t i = 0; i < dataLength; i++ ) { printf( "0x%02X, ", testRxData32Byte[ i ] ); }
// //                 printf( "\n" );

// //                 RF24_RegWrite( L01_REG_STATUS, &statusData, 1 );
// //                 rf_int_flag = 0;
// //             }
// //         }
// //     }
// // }

// void debug_functions( void ) {
//     printf( "RF debug.\n" );
//     RF24_init();
//     RF24_Rx_init();
//     uint8_t statusData;
//     while ( 1 ) {
//         if ( RF_INT_PIN == 0 ) {
//             LED_B      = 0;
//             statusData = RF24_GetStatus();
//             RF24_CmdReadWrite( L01_CMD_FLUSH_RX, NULL, NULL, 0 );
//             RF24_RegOneWrite( L01_REG_STATUS, statusData );
//             while ( RF_INT_PIN == 0 ) { __NOP(); }
//         }
//         LED_B = 1;
//     }
// }

// #endif
