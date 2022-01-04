#ifndef __RF24_REG_H
#define __RF24_REG_H

//-------------------------------------------------
// NRF CMD
#define L01_CMD_R_REGISTER          0x00
#define L01_CMD_W_REGISTER          0x20
#define L01_CMD_R_RX_PAYLOAD        0x61
#define L01_CMD_W_TX_PAYLOAD        0xA0
#define L01_CMD_FLUSH_TX            0xE1
#define L01_CMD_FLUSH_RX            0xE2
#define L01_CMD_REUSE_TX_PL         0xE3
#define L01_CMD_R_RX_PL_WID         0x60
#define L01_CMD_W_ACK_PAYLOAD       0xA8
#define L01_CMD_W_TX_PAYLOAD_NOACK  0xB0
#define L01_CMD_NOP                 0xFF
//-------------------------------------------------
// NRF REG
#define L01_REG_CFG                         0x00
#define L01_REG_CFG_MASK_RX_DR_POS          ( 6 )
#define L01_REG_CFG_MASK_RX_DR_MSK          ( 0x1 << L01_REG_CFG_MASK_RX_DR_POS )
#define L01_REG_CFG_MASK_TX_DS_POS          ( 5 )
#define L01_REG_CFG_MASK_TX_DS_MSK          ( 0x1 << L01_REG_CFG_MASK_TX_DS_POS )
#define L01_REG_CFG_MASK_MAX_RT_POS         ( 4 )
#define L01_REG_CFG_MASK_MAX_RT_MSK         ( 0x1 << L01_REG_CFG_MASK_MAX_RT_POS )
#define L01_REG_CFG_EN_CRC_POS              ( 3 )
#define L01_REG_CFG_EN_CRC_MSK              ( 0x1 << L01_REG_CFG_EN_CRC_POS )
#define L01_REG_CFG_CRCO_POS                ( 2 )
#define L01_REG_CFG_CRCO_MSK                ( 0x1 << L01_REG_CFG_CRCO_POS )
#define L01_REG_CFG_PWR_UP_POS              ( 1 )
#define L01_REG_CFG_PWR_UP_MSK              ( 0x1 << L01_REG_CFG_PWR_UP_POS )
#define L01_REG_CFG_PRIM_RX_POS             ( 0 )
#define L01_REG_CFG_PRIM_RX_MSK             ( 0x1 << L01_REG_CFG_PRIM_RX_POS )

#define L01_REG_EN_AA                       0x01
#define L01_REG_EN_AA_P5_POS                ( 5 )
#define L01_REG_EN_AA_P5_MSK                ( 0x1 << L01_REG_EN_AA_P5_POS )
#define L01_REG_EN_AA_P4_POS                ( 4 )
#define L01_REG_EN_AA_P4_MSK                ( 0x1 << L01_REG_EN_AA_P4_POS )
#define L01_REG_EN_AA_P3_POS                ( 3 )
#define L01_REG_EN_AA_P3_MSK                ( 0x1 << L01_REG_EN_AA_P3_POS )
#define L01_REG_EN_AA_P2_POS                ( 2 )
#define L01_REG_EN_AA_P2_MSK                ( 0x1 << L01_REG_EN_AA_P2_POS )
#define L01_REG_EN_AA_P1_POS                ( 1 )
#define L01_REG_EN_AA_P1_MSK                ( 0x1 << L01_REG_EN_AA_P1_POS )
#define L01_REG_EN_AA_P0_POS                ( 0 )
#define L01_REG_EN_AA_P0_MSK                ( 0x1 << L01_REG_EN_AA_P0_POS )

#define L01_REG_EN_RXADDR                   0x02
#define L01_REG_EN_RXADDR_ERX_P5_POS        ( 5 )
#define L01_REG_EN_RXADDR_ERX_P5_MSK        ( 0x1 << L01_REG_EN_RXADDR_ERX_P5_POS )
#define L01_REG_EN_RXADDR_ERX_P4_POS        ( 4 )
#define L01_REG_EN_RXADDR_ERX_P4_MSK        ( 0x1 << L01_REG_EN_RXADDR_ERX_P4_POS )
#define L01_REG_EN_RXADDR_ERX_P3_POS        ( 3 )
#define L01_REG_EN_RXADDR_ERX_P3_MSK        ( 0x1 << L01_REG_EN_RXADDR_ERX_P3_POS )
#define L01_REG_EN_RXADDR_ERX_P2_POS        ( 2 )
#define L01_REG_EN_RXADDR_ERX_P2_MSK        ( 0x1 << L01_REG_EN_RXADDR_ERX_P2_POS )
#define L01_REG_EN_RXADDR_ERX_P1_POS        ( 1 )
#define L01_REG_EN_RXADDR_ERX_P1_MSK        ( 0x1 << L01_REG_EN_RXADDR_ERX_P1_POS )
#define L01_REG_EN_RXADDR_ERX_P0_POS        ( 0 )
#define L01_REG_EN_RXADDR_ERX_P0_MSK        ( 0x1 << L01_REG_EN_RXADDR_ERX_P0_POS )

#define L01_REG_SETUP_AW                    0x03
#define L01_REG_SETUP_AW_POS                ( 0 )
#define L01_REG_SETUP_AW_MSK                ( 0x3 << L01_REG_SETUP_AW_POS )

#define L01_REG_SETUP_RETR                  0x04
#define L01_REG_SETUP_RETR_ARD_POS          ( 4 )
#define L01_REG_SETUP_RETR_ARD_MSK          ( 0xF << L01_REG_SETUP_RETR_ARD_POS )
#define L01_REG_SETUP_RETR_ARC_POS          ( 0 )
#define L01_REG_SETUP_RETR_ARC_MSK          ( 0xF << L01_REG_SETUP_RETR_ARC_POS )

#define L01_REG_RF_CH                       0x05
#define L01_REG_RF_CH_POS                   ( 0 )
#define L01_REG_RF_CH_MSK                   ( 0x7F << L01_REG_RF_CH_POS )

#define L01_REG_RF_SETUP                    0x06
#define L01_REG_RF_SETUP_CONT_WAVE_POS      ( 7 )
#define L01_REG_RF_SETUP_CONT_WAVE_MSK      ( 0x1 << L01_REG_RF_SETUP_CONT_WAVE_POS )
#define L01_REG_RF_SETUP_RF_DR_LOW_POS      ( 5 )
#define L01_REG_RF_SETUP_RF_DR_LOW_MSK      ( 0x1 << L01_REG_RF_SETUP_RF_DR_LOW_POS )
#define L01_REG_RF_SETUP_PLL_LOCK_POS       ( 4 )
#define L01_REG_RF_SETUP_PLL_LOCK_MSK       ( 0x1 << L01_REG_RF_SETUP_PLL_LOCK_POS )
#define L01_REG_RF_SETUP_RF_DR_HIGH_POS     ( 3 )
#define L01_REG_RF_SETUP_RF_DR_HIGH_MSK     ( 0x1 << L01_REG_RF_SETUP_RF_DR_HIGH_POS )
#define L01_REG_RF_SETUP_RF_PWR_POS         ( 1 )
#define L01_REG_RF_SETUP_RF_PWR_MSK         ( 0x3 << L01_REG_RF_SETUP_RF_PWR_POS )
#define L01_REG_RF_SETUP_LNA_HCURR_POS      ( 0 )
#define L01_REG_RF_SETUP_LNA_HCURR_MSK      ( 0x1 << L01_REG_RF_SETUP_LNA_HCURR_POS )

#define L01_REG_STATUS                      0x07
#define L01_REG_STATUS_RX_DR_POS            ( 6 )
#define L01_REG_STATUS_RX_DR_MSK            ( 0x1 << L01_REG_STATUS_RX_DR_POS )
#define L01_REG_STATUS_TX_DS_POS            ( 5 )
#define L01_REG_STATUS_TX_DS_MSK            ( 0x1 << L01_REG_STATUS_TX_DS_POS )
#define L01_REG_STATUS_MAX_RT_POS           ( 4 )
#define L01_REG_STATUS_MAX_RT_MSK           ( 0x1 << L01_REG_STATUS_MAX_RT_POS )
#define L01_REG_STATUS_RX_P_NO_POS          ( 1 )
#define L01_REG_STATUS_RX_P_NO_MSK          ( 0x7 << L01_REG_STATUS_RX_P_NO_POS )
#define L01_REG_STATUS_TX_FULL_POS          ( 0 )
#define L01_REG_STATUS_TX_FULL_MSK          ( 0x1 << L01_REG_STATUS_TX_FULL_POS )

#define L01_REG_OBSERVE_TX                  0x08
#define L01_REG_OBSERVE_TX_PLOS_CNT_POS     ( 4 )
#define L01_REG_OBSERVE_TX_PLOS_CNT_MSK     ( 0xF << L01_REG_OBSERVE_TX_PLOS_CNT_POS )
#define L01_REG_OBSERVE_TX_ARC_CNT_POS      ( 0 )
#define L01_REG_OBSERVE_TX_ARC_CNT_MSK      ( 0xF << L01_REG_OBSERVE_TX_ARC_CNT_POS )

#define L01_REG_CD                          0x09
#define L01_REG_CD_POS                      ( 0 )
#define L01_REG_CD_MSK                      ( 0x1 << L01_REG_CD_POS )

#define L01_REG_RX_ADDR_P0                  0x0A
#define L01_REG_RX_ADDR_P1                  0x0B
#define L01_REG_RX_ADDR_P2                  0x0C
#define L01_REG_RX_ADDR_P3                  0x0D
#define L01_REG_RX_ADDR_P4                  0x0E
#define L01_REG_RX_ADDR_P5                  0x0F

#define L01_REG_TX_ADDR                     0x10

#define L01_REG_RX_PW_P0                    0x11
#define L01_REG_RX_PW_P0_POS                ( 0 )
#define L01_REG_RX_PW_P0_MSK                ( 0x3F << L01_REG_RX_PW_P0_POS )

#define L01_REG_RX_PW_P1                    0x12
#define L01_REG_RX_PW_P1_POS                ( 0 )
#define L01_REG_RX_PW_P1_MSK                ( 0x3F << L01_REG_RX_PW_P1_POS )

#define L01_REG_RX_PW_P2                    0x13
#define L01_REG_RX_PW_P2_POS                ( 0 )
#define L01_REG_RX_PW_P2_MSK                ( 0x3F << L01_REG_RX_PW_P2_POS )

#define L01_REG_RX_PW_P3                    0x14
#define L01_REG_RX_PW_P3_POS                ( 0 )
#define L01_REG_RX_PW_P3_MSK                ( 0x3F << L01_REG_RX_PW_P3_POS )

#define L01_REG_RX_PW_P4                    0x15
#define L01_REG_RX_PW_P4_POS                ( 0 )
#define L01_REG_RX_PW_P4_MSK                ( 0x3F << L01_REG_RX_PW_P4_POS )

#define L01_REG_RX_PW_P5                    0x16
#define L01_REG_RX_PW_P5_POS                ( 0 )
#define L01_REG_RX_PW_P5_MSK                ( 0x3F << L01_REG_RX_PW_P5_POS )

#define L01_REG_FIFO_STATUS                 0x17
#define L01_REG_FIFO_STATUS_TX_REUSE_POS    ( 6 )
#define L01_REG_FIFO_STATUS_TX_REUSE_MSK    ( 0x1 << L01_REG_FIFO_STATUS_TX_REUSE_POS )
#define L01_REG_FIFO_STATUS_TX_FULL_POS     ( 5 )
#define L01_REG_FIFO_STATUS_TX_FULL_MSK     ( 0x1 << L01_REG_FIFO_STATUS_TX_FULL_POS )
#define L01_REG_FIFO_STATUS_TX_EMPTY_POS    ( 4 )
#define L01_REG_FIFO_STATUS_TX_EMPTY_MSK    ( 0x1 << L01_REG_FIFO_STATUS_TX_EMPTY_POS )
#define L01_REG_FIFO_STATUS_RX_FULL_POS     ( 1 )
#define L01_REG_FIFO_STATUS_RX_FULL_MSK     ( 0x1 << L01_REG_FIFO_STATUS_RX_FULL_POS )
#define L01_REG_FIFO_STATUS_RX_EMPTY_POS    ( 0 )
#define L01_REG_FIFO_STATUS_RX_EMPTY_MSK    ( 0x1 << L01_REG_FIFO_STATUS_RX_EMPTY_POS )

#define L01_REG_DYNPD                       0x1C
#define L01_REG_DYNPD_DPL_P5_POS            ( 5 )               
#define L01_REG_DYNPD_DPL_P5_MSK            ( 0x1 << L01_REG_DYNPD_DPL_P5_POS )              
#define L01_REG_DYNPD_DPL_P4_POS            ( 4 )               
#define L01_REG_DYNPD_DPL_P4_MSK            ( 0x1 << L01_REG_DYNPD_DPL_P4_POS )              
#define L01_REG_DYNPD_DPL_P3_POS            ( 3 )               
#define L01_REG_DYNPD_DPL_P3_MSK            ( 0x1 << L01_REG_DYNPD_DPL_P3_POS )              
#define L01_REG_DYNPD_DPL_P2_POS            ( 2 )               
#define L01_REG_DYNPD_DPL_P2_MSK            ( 0x1 << L01_REG_DYNPD_DPL_P2_POS )              
#define L01_REG_DYNPD_DPL_P1_POS            ( 1 )               
#define L01_REG_DYNPD_DPL_P1_MSK            ( 0x1 << L01_REG_DYNPD_DPL_P1_POS )              
#define L01_REG_DYNPD_DPL_P0_POS            ( 0 )               
#define L01_REG_DYNPD_DPL_P0_MSK            ( 0x1 << L01_REG_DYNPD_DPL_P0_POS ) 

#define L01_REG_FEATURE                     0x1D
#define L01_REG_FEATURE_EN_DPL_POS          ( 2 )
#define L01_REG_FEATURE_EN_DPL_MSK          ( 0x1 << L01_REG_FEATURE_EN_DPL_POS )
#define L01_REG_FEATURE_EN_ACK_PAY_POS      ( 1 )
#define L01_REG_FEATURE_EN_ACK_PAY_MSK      ( 0x1 << L01_REG_FEATURE_EN_ACK_PAY_POS )
#define L01_REG_FEATURE_EN_DYN_ACK_POS      ( 0 )
#define L01_REG_FEATURE_EN_DYN_ACK_MSK      ( 0x1 << L01_REG_FEATURE_EN_DYN_ACK_POS )

//-------------------------------------------------

#endif /* __RF24_REG_H */
