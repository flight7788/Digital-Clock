#ifndef I2C_H
#define	I2C_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define _XTAL_FREQ 48000000

#define SDA		 PORTBbits.RB0
#define SDA_IO 	 TRISBbits.RB0
#define SCK		 PORTBbits.RB1
#define SCK_IO	 TRISBbits.RB1


void I2C_Init(void);
void I2C_is_idle(void);
void I2C_Start(void);
void I2C_RepStart(void);
void I2C_Stop(void);
void I2C_Write(uint8_t DATA);
uint8_t I2C_Read(bool GenerateAck);

void I2C_Mem_Transmit(uint8_t addr, uint8_t MemAddress, uint8_t data);
void I2C_Mem_Read(uint8_t addr, uint8_t MemAddress, uint8_t *data);
void I2C_Master_Transmit(uint8_t addr, uint8_t data);
void I2C_Master_Receive(uint8_t addr, uint8_t* data);


#endif	/* I2C_H */

