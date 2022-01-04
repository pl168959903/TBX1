#include "I2cPdma.h"

volatile void (*s_I2C0HandlerFn)(uint32_t status);

#define I2C_STATUS_MASTER_START                 0x08
#define I2C_STATUS_MASTER_REPEAT_START          0x10
#define I2C_STATUS_MASTER_TRANSMIT_ADDRESS_ACK  0x18
#define I2C_STATUS_MASTER_TRANSMIT_ADDRESS_NACK 0x20
#define I2C_STATUS_MASTER_TRANSMIT_DATA_ACK     0x28
#define I2C_STATUS_MASTER_TRANSMIT_DATA_NACK    0x30
#define I2C_STATUS_MASTER_ARBITRATION_LOST      0x38
#define I2C_STATUS_MASTER_RECEIVE_ADDRESS_ACK   0x40
#define I2C_STATUS_MASTER_RECEIVE_ADDRESS_NACK  0x48
#define I2C_STATUS_MASTER_RECEIVE_DATA_ACK      0x50
#define I2C_STATUS_MASTER_RECEIVE_DATA_NACK     0x58
#define I2C_STATUS_BUS_RELEASED                 0xF8

volatile uint8_t pdmaI2cLock;
volatile uint8_t I2cRxAddr, I2cRxReg;


void I2C_txPdma(uint32_t status){
    switch (status)
    {
        case I2C_STATUS_MASTER_START : {
        }
        break;
        case I2C_STATUS_MASTER_REPEAT_START : {

        }
        break;
        case I2C_STATUS_MASTER_TRANSMIT_ADDRESS_ACK : {

        }
        break;
        case I2C_STATUS_MASTER_TRANSMIT_ADDRESS_NACK : {
        }
        break;
        case I2C_STATUS_MASTER_TRANSMIT_DATA_ACK : {
        }
        break;
        case I2C_STATUS_MASTER_TRANSMIT_DATA_NACK : {

        }
        break;
        case I2C_STATUS_MASTER_ARBITRATION_LOST : {

        }
        break;
        case I2C_STATUS_MASTER_RECEIVE_ADDRESS_ACK : {

        }
        break;
        case I2C_STATUS_MASTER_RECEIVE_ADDRESS_NACK : {

        }
        break;
        case I2C_STATUS_MASTER_RECEIVE_DATA_ACK : {

        }
        break;
        case I2C_STATUS_MASTER_RECEIVE_DATA_NACK : {

        }
        break;
        case I2C_STATUS_BUS_RELEASED : {

        }
        break;
    }
}

void I2C_rxPdma(uint32_t status){
    switch (status)
    {
        case I2C_STATUS_MASTER_START : {
            I2C_SET_DATA(I2C0, (I2cRxAddr << 1) | 0x00);
            I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
        }
        break;
        case I2C_STATUS_MASTER_REPEAT_START : {
            I2C_SET_DATA(I2C0, (I2cRxAddr << 1) | 0x01);
            I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
        }
        break;
        case I2C_STATUS_MASTER_TRANSMIT_ADDRESS_ACK : {
            I2C_SET_DATA(I2C0, I2cRxReg);
            I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
        }
        break;
        case I2C_STATUS_MASTER_TRANSMIT_ADDRESS_NACK : {

        }
        break;
        case I2C_STATUS_MASTER_TRANSMIT_DATA_ACK : {
            I2C_SET_CONTROL_REG(I2C0, I2C_CTL_STA_SI);
        }
        break;
        case I2C_STATUS_MASTER_TRANSMIT_DATA_NACK : {

        }
        break;
        case I2C_STATUS_MASTER_ARBITRATION_LOST : {
            I2C_STOP(I2C0);
            // I2C_START(I2C0);
        }
        break;
        case I2C_STATUS_MASTER_RECEIVE_ADDRESS_ACK : {
            I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI_AA);
            I2C0->CTL1 |= I2C_CTL1_RXPDMAEN_Msk;
        }
        break;
        case I2C_STATUS_MASTER_RECEIVE_ADDRESS_NACK : {

        }
        break;
        case I2C_STATUS_MASTER_RECEIVE_DATA_ACK : {
            if(pdmaI2cLock == 0)  I2C_SET_CONTROL_REG(I2C0, I2C_CTL_STO | I2C_CTL_SI);
        }
        break;
        case I2C_STATUS_MASTER_RECEIVE_DATA_NACK : {
            I2C0->CTL1 &= ~(I2C_CTL1_RXPDMAEN_Msk);
            I2C_SET_CONTROL_REG(I2C0, I2C_CTL_STO | I2C_CTL_SI);
        }
        break;
        case I2C_STATUS_BUS_RELEASED : {

        }
        break;
    }
}

typedef struct dma_i2c_desc_t {
    uint32_t ctl;
    uint32_t src;
    uint32_t dest;
    uint32_t offset;
} DMA_I2C_DESC_T;

void I2C_TxData(uint8_t address, uint8_t reg, uint8_t data[], uint32_t data_length){
    DMA_I2C_DESC_T addr_desc, reg_desc, data_desc;

    uint8_t addr_w = (address << 1U | 0x00U);

    addr_desc = (DMA_I2C_DESC_T){
        .ctl = I2C_DESC_ADDR_CTL | PDMA_TBINTDIS_DISABLE,
        .src = (uint32_t)&addr_w,
        .dest = I2C_DESC_I2C_DATA_ADDR,
        .offset = I2C_DESC_OFFSET(&reg_desc)
    };

    reg_desc = (DMA_I2C_DESC_T){
        .ctl = I2C_DESC_REG_CTL | PDMA_TBINTDIS_DISABLE,
        .src = (uint32_t)&reg,
        .dest = I2C_DESC_I2C_DATA_ADDR,
        .offset = I2C_DESC_OFFSET(&data_desc)
    };

    data_desc = (DMA_I2C_DESC_T){
        .ctl = I2C_DESC_DATA_LENGTH( data_length ) | I2C_DESC_TX_CTL ,
        .src = (uint32_t)data,
        .dest = I2C_DESC_I2C_DATA_ADDR,
        .offset = 0
    };

    

    s_I2C0HandlerFn = I2C_txPdma;

    pdmaI2cLock = 1;

    PDMA->DSCT[I2C_TX_PDMA_CH].CTL = PDMA_OP_SCATTER;
    PDMA->DSCT[I2C_TX_PDMA_CH].NEXT = (uint32_t)&addr_desc;    
    PDMA->CHCTL |= I2C_PDMA_TX_CH_EN_MSK;

    I2C0->CTL1 = (I2C_CTL1_TXPDMAEN_Msk);
    
    I2C_START(I2C0);

    while(pdmaI2cLock == 1 || I2C0->STATUS1){__NOP();}

    PDMA->CHCTL &= ~I2C_PDMA_TX_CH_EN_MSK;
}

volatile uint8_t i2cRestart;

void I2C_RxData(uint8_t address, uint8_t reg, uint8_t data[], uint32_t data_length){
    DMA_I2C_DESC_T data_desc;
    uint8_t addr_w = (address << 1U | 0x00U);
    uint8_t addr_r = (address << 1U | 0x01U);

    data_desc = (DMA_I2C_DESC_T){
        .ctl = I2C_DESC_DATA_LENGTH( data_length ) | I2C_DESC_RX_CTL ,
        .src = I2C_DESC_I2C_DATA_ADDR,
        .dest = (uint32_t)data,
        .offset = 0
    };

    pdmaI2cLock = 1;
    PDMA->DSCT[I2C_RX_PDMA_CH].CTL = PDMA_OP_SCATTER;
    PDMA->DSCT[I2C_RX_PDMA_CH].NEXT = (uint32_t)&data_desc;
    PDMA->CHCTL |= I2C_PDMA_RX_CH_EN_MSK;

    s_I2C0HandlerFn = I2C_rxPdma;

    I2cRxAddr = address;
    I2cRxReg = reg;

    I2C_START(I2C0);

    while(pdmaI2cLock == 1 || I2C0->STATUS1){__NOP();}
}

void debug_functions( void ){
    printf("Start I2C PDMA Debug.\n");
    // I2C_WriteByteOneReg(I2C0, 0x68, 0x00, 0x00);

    uint8_t data[10] = {0};

    I2C_RxData(0x68, 0x03, data, 1);
    printf("data = 0x%02X.\n", data);
	
		while(1){__NOP();}

    I2C_TxData(0x68, 0x03, (uint8_t[]){0x02}, 1);

    I2C_RxData(0x68, 0x03, &data, 1);
    printf("data = 0x%02X.\n", data);

    
    

}
