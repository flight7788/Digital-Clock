#include "I2C.h"

void I2C_Init(void){
    SCK_IO = 1;		
    SDA_IO = 1;
    SSPSTATbits.SMP = 1;
    SSPSTATbits.CKE = 0;
    SSPCON1bits.SSPM = 0x08;
    SSPADD = 119; // 100KHz
    SSPCON1bits.SSPEN = 1;
}
//--------------------------------------------------------------------------------------------
void I2C_is_idle(void) {
    uint16_t TimeOut = 0;
    while((SSPCON2 & 0x1F) || (SSPSTAT & 0x04) ){
        if(TimeOut++>=100) {
            break;
        }
        __delay_ms(1);
    };
}
//--------------------------------------------------------------------------------------------
void I2C_Start(void) {
    I2C_is_idle();
    SSPCON2bits.SEN = 1;
}
//--------------------------------------------------------------------------------------------
void I2C_RepStart(void) {
    I2C_is_idle();
    SSPCON2bits.RSEN = 1;
}
//--------------------------------------------------------------------------------------------
void I2C_Stop(void) {
    I2C_is_idle();
    SSPCON2bits.PEN = 1;
}
//--------------------------------------------------------------------------------------------
void I2C_Write(uint8_t data) {
    I2C_is_idle();
    SSPBUF = data;
    while(SSPSTATbits.BF != 0);
    while(SSPCON2bits.ACKSTAT != 0);
} 
//--------------------------------------------------------------------------------------------
uint8_t I2C_Read(bool GenerateAck) {   
    uint8_t data =0;
    I2C_is_idle();
    SSPCON2bits.RCEN = 1;
    while(SSPSTATbits.BF != 1);
    data = SSPBUF;
    SSPCON2bits.ACKEN = GenerateAck;
    return data;
}
//--------------------------------------------------------------------------------------------
void I2C_Mem_Transmit(uint8_t addr, uint8_t MemAddress, uint8_t data){
    I2C_Start();
    I2C_Write(addr&0xFE);   // ADDRESS+WRITE
    I2C_Write(MemAddress);  // MemAddress
    I2C_Write(data);        // DATA
    I2C_Stop();
}
//--------------------------------------------------------------------------------------------
void I2C_Mem_Read(uint8_t addr,uint8_t MemAddress,uint8_t *data){
    I2C_Start();
    I2C_Write(addr&0xFE);   // ADDRESS+WRITE
    I2C_Write(MemAddress);  // MemAddress
    I2C_RepStart();
    I2C_Write(addr|0x01);   // ADDRESS+READ
    *data = I2C_Read(0);
    I2C_Stop();
}
//--------------------------------------------------------------------------------------------
void I2C_Master_Receive(uint8_t addr,uint8_t* data) {
    I2C_Start();
    I2C_Write(addr | 0x01);  // Address+Read
    *data = I2C_Read(0);     // Read DATA
    I2C_Stop();
}
//--------------------------------------------------------------------------------------------
void I2C_Master_Transmit(uint8_t addr,uint8_t data) {
    I2C_Start();
    I2C_Write(addr & 0xFE);   // Address+Write
    I2C_Write(data);          // Write DATA
    I2C_Stop();
}

