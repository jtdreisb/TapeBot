/*
I2C Library, also known as TWI (Two-Wire Interface)
Darron Baida May 2009

This library provides a set of functions to make it easier to communicate with 
I2C/TWI devices, and is intended to cover a large variety of devices. These 
functions are currently all implemented in the least-efficient fashion, using 
blocking while loops that wait for flags to be set. They should be sufficient 
for most simple applications, but if you need the best possible performance you
should consider writing I2C interrupt handlers or a non-blocking task loop 
instead. However, even if you choose to implement a faster alternative, these 
functions should provide some hints about how to write I2C code, though you 
should also refer to the datasheet for your ATmega chip. Please be aware that 
due to the blocking nature of this implementation, it is possible for these 
functions to get stuck waiting for a flag condition that may never occur, 
effectively halting your program. This can happen if the I2C device is not 
communicating properly.

To decide which function is appropriate for your I2C device, read the datasheet
for the device to see if it uses addressable registers. If not, then you can 
simply read and write to the device using:
i2cReadByteFromDevice(u08 address) and
i2cSendByteToDevice(u08 address, u08 data)

For both reading and writing functions, you must always pass in the device 
address with the least significant bit (LSB) set to 0. In other words, the 
address you pass must always be an even number. In read mode, if there is an 
error, 0 will be returned. If it is possible that your data byte could be 0, 
you must check the error register yourself or else comment out the error checking.
A better way is to use the i2cReadDataFromRegisters function which returns a 1 for success and 0 for error in addition to loading the desired array with the received data.
If your device uses a 1 byte addressable register scheme, use i2cSendByteToRegister(u08 address, u08 reg, u08 data), i2cReadByteFromRegister(u08 address, u08 reg),
i2cSendDataToRegisters(u08 address, u08 reg, u08 byteCount, u08 data[]), and i2cReadDataFromRegisters(u08 address, u08 reg, u08 byteCount, u08 *data)
This last function is special because the last parameter is an initialized array which your data will be loaded in to. See example code for details.
If your device uses 2-byte addressable registers, like an EEPROM or something, use the functions ending in "2". This allows a u16 register address to be sent.
*/

#include "globals.h"
#include <util/twi.h>

//Normally called only by the initialize() function in utility.c.
inline void i2cInit()
{
	//set value of TWPS bits to 1, yielding an I2C prescaler value of 4
	cbi(TWSR, TWPS1);
	sbi(TWSR, TWPS0);

	//set I2C clock to 100kHz
	TWBR = 18;
	//set I2C clock to 400kHz
	//TWBR = 3;
}

u08 i2cStart()
{
	//Send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	//Wait for TWINT Flag set. This indicates that the START condition has been transmitted.
	while (!(TWCR & (1<<TWINT)));
	//Check value of TWI Status Register. Mask prescaler bits. Return status.
	return (TWSR & 0xF8);
}

u08 i2cByteTransmit(u08 byteToSend)
{
	//Load the byte to be sent into the TWDR register.
	TWDR = byteToSend;
	//Clear TWINT bit in TWCR to start transmission of address.
	TWCR = (1<<TWINT) | (1<<TWEN);
	//Wait for TWINT Flag to be set. This indicates that the byte has been transmitted, and ACK/NACK has been received.
	while (!(TWCR & (1<<TWINT)));
	//Check value of TWI Status Register. Mask prescaler bits. Return status.
	return (TWSR & 0xF8);
}

u08 i2cByteReceive(u08 ack)
{
	//Data byte will be received and ACK will be returned.
	if (ack == 1)
		TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
	//Data byte will be received and NACK will be returned.
	else
		TWCR = (1<<TWINT) | (1<<TWEN);

	//Wait for TWINT Flag to be set.
	while (!(TWCR & (1<<TWINT)));
	//Check value of TWI Status Register. Mask prescaler bits. If status different from received data, ack returned, return 0.
	if (ack == 1 && ((TWSR & 0xF8) != TW_MR_DATA_ACK))
		return 0;
	//Check value of TWI Status Register. Mask prescaler bits. If status different from received data, nack returned, return 0.
	if (ack == 0 && ((TWSR & 0xF8) != TW_MR_DATA_NACK))
		return 0;
	return TWDR;
}

void i2cStop()
{
	//Transmit STOP condition.
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	//Stop has no status.
}

u08 i2cSendByteToDevice(u08 address, u08 data)
{
	u08 status = 0;
	//Send start and note the status.
	status = i2cStart();
	//If the status is not "start sent" or "restart sent" return 0 for error.
	if (status != TW_START && status != TW_REP_START)
		return 0;

	//If the status is not "slave address + write bit transmitted, ACK received" return 0 for error.
	status = i2cByteTransmit(address);
	if (status != TW_MT_SLA_ACK)
		return 0;

	//If the status is not "data transmitted, ACK received " return 0 for error.
	status = i2cByteTransmit(data);
	if (status != TW_MT_DATA_ACK)
		return 0;

	//Send Stop
	i2cStop();
	return 1;
}

u08 i2cSend2BytesToDevice(u08 address, u16 data)
{
	u08 status = 0;
	//Send start and note the status.
	status = i2cStart();
	//If the status is not "start sent" or "restart sent" return 0 for error.
	if (status != TW_START && status != TW_REP_START)
		return 0;

	//If the status is not "slave address + write bit transmitted, ACK received" return 0 for error.
	status = i2cByteTransmit(address);
	if (status != TW_MT_SLA_ACK)
		return 0;

	//If the status is not "data transmitted, ACK received " return 0 for error.
	status = i2cByteTransmit((u08)data);//Send upper 8 bits.
	if (status != TW_MT_DATA_ACK)
		return 0;
	
	//If the status is not "data transmitted, ACK received " return 0 for error.
	status = i2cByteTransmit((u08)(data>>8));//Send lower 8 bits.
	if (status != TW_MT_DATA_ACK)
		return 0;

	//Send Stop
	i2cStop();
	return 1;
}

u08 i2cReadByteFromDevice(u08 address)
{
	u08 status = 0;
	u08 data = 0;
	//Send start and note the status.
	status = i2cStart();
	//If the status is not "start sent" or "restart sent" return 0 for error.
	if (status != TW_START && status != TW_REP_START)
		return 0;

	//Read addresses are one above write addresses
	status = i2cByteTransmit(address+1);
	//If the status is not "slave address + read bit transmitted, ACK received" return 0 for error.
	if (status != TW_MR_SLA_ACK)
		return 0;

	//Return NACK because only one byte is being read.
	data = i2cByteReceive(0);

	//Send Stop
	i2cStop();
	return data;
}

u08 i2cSendByteToRegister(u08 address, u08 reg, u08 data)
{
	u08 status = 0;
	//Send start and note the status.
	status = i2cStart();
	//If the status is not "start sent" or "restart sent" return 0 for error.
	if (status != TW_START && status != TW_REP_START)
		return 0;

	status = i2cByteTransmit(address);
	//If the status is not "slave address + write bit transmitted, ACK received" return 0 for error.
	if (status != TW_MT_SLA_ACK)
		return 0;

	status = i2cByteTransmit(reg);
	//If the status is not "data transmitted, ACK received" return 0 for error.
	if (status != TW_MT_DATA_ACK)
		return 0;

	status = i2cByteTransmit(data);
	//If the status is not "data transmitted, ACK received" return 0 for error.
	if (status != TW_MT_DATA_ACK)
		return 0;

	//Send Stop
	i2cStop();
	return 1;
}

u08 i2cReadByteFromRegister(u08 address, u08 reg)
{
	u08 status = 0;
	u08 data = 0;
	//Send start and note the status.
	status = i2cStart();
	//If the status is not "start sent" or "restart sent" return 0 for error.
	if (status != TW_START && status != TW_REP_START)
		return status;

	//Send Device Address
	status = i2cByteTransmit(address);
	//If the status is not "slave address + write bit transmitted, ACK received" return 0 for error.
	if (status != TW_MT_SLA_ACK)
		return status;

	//Send register number to read
	status = i2cByteTransmit(reg);
	//If the status is not "data transmitted, ACK received" return 0 for error.
	if (status != TW_MT_DATA_ACK)
		return status;

	//Send repeat start
	status = i2cStart();
	//If the status is not "start sent" or "restart sent" return 0 for error.
	if (status != TW_START && status != TW_REP_START)
		return status;

	//Send Device Address + read bit
	status = i2cByteTransmit(address+1);
	//If the status is not "slave address + read bit transmitted, ACK received" return 0 for error.
	if (status != TW_MR_SLA_ACK)
		return status;

	//Receive and send NACK
	data = i2cByteReceive(0);

	//Send Stop
	i2cStop();
	return data;
}

u08 i2cSendDataToRegisters(u08 address, u08 reg, u08 byteCount, u08 data[])
{
	u08 status = 0;
	u08 i = 0;
	//Send start
	status = i2cStart();
	//If the status is not "start sent" or "restart sent" return 0 for error.
	if (status != TW_START && status != TW_REP_START)
		return 0;

	status = i2cByteTransmit(address);
	//If the status is not "slave address + write bit transmitted, ACK received" return 0 for error.
	if (status != TW_MT_SLA_ACK)
		return 0;

	status = i2cByteTransmit(reg);
	//If the status is not "data transmitted, ACK received" return 0 for error.
	if (status != TW_MT_DATA_ACK)
		return 0;

	for (i = 0; i < byteCount; i++)
	{
		status = i2cByteTransmit(data[i]);
		//If the status is not "data transmitted, ACK received" return 0 for error.
		if (status != TW_MT_DATA_ACK)
			return 0;
	}

	//Send Stop
	i2cStop();
	return 1;
}

//Function call format is: u08 result = i2cReadDataFromRegisters(address, register number, number of bytes to read, dataStorageArray);
//Notice that the last element is the location of the first byte where you want the read data stored.
u08 i2cReadDataFromRegisters(u08 address, u08 reg, u08 byteCount, u08 *data)
{
	u08 status = 0;
	u08 i = 0;
	//Send start and note the status.
	status = i2cStart();
	//If the status is not "start sent" or "restart sent" return 0 for error.
	if (status != TW_START && status != TW_REP_START)
		return 0;

	//Send device address
	status = i2cByteTransmit(address);
	//If the status is not "slave address + read bit transmitted, ACK received" return 0 for error.
	if (status != TW_MT_SLA_ACK)
		return 0;

	status = i2cByteTransmit(reg);
	//If the status is not "data transmitted, ACK received" return 0 for error.
	if (status != TW_MT_DATA_ACK)
		return 0;

	//Send repeat start
	status = i2cStart();
	//If the status is not "start sent" or "restart sent" return 0 for error.
	if (status != TW_START && status != TW_REP_START)
		return 0;

	//Send device address + read bit
	status = i2cByteTransmit(address+1);
	//If the status is not "slave address + read bit transmitted, ACK received" return 0 for error.
	if (status != TW_MR_SLA_ACK)
		return 0;

	for (i = 0; i < (byteCount-1); i++)
	{
		data[i] = i2cByteReceive(1);
	}
	data[i] = i2cByteReceive(0);

	//Send Stop
	i2cStop();
	return 1;
}

//For devices with 2-byte internal address registers
u08 i2cSendDataToRegisters2(u08 address, u16 reg, u08 byteCount, u08 data[])
{
	u08 status = 0;
	u08 i = 0;
	//Send start
	status = i2cStart();
	//If the status is not "start sent" or "restart sent" return 0 for error.
	if (status != TW_START && status != TW_REP_START)
		return 0;

	status = i2cByteTransmit(address);
	//If the status is not "slave address + write bit transmitted, ACK received" return 0 for error.
	if (status != TW_MT_SLA_ACK)
		return 0;

	//Transmit high byte of register
	status = i2cByteTransmit((u08)(reg>>8));
	//If the status is not "data transmitted, ACK received" return 0 for error.
	if (status != TW_MT_DATA_ACK)
		return 0;
		
	status = i2cByteTransmit(reg);
	//If the status is not "data transmitted, ACK received" return 0 for error.
	if (status != TW_MT_DATA_ACK)
		return 0;

	for (i = 0; i < byteCount; i++)
	{
		status = i2cByteTransmit(data[i]);
		//If the status is not "data transmitted, ACK received" return 0 for error.
		if (status != TW_MT_DATA_ACK)
			return 0;
	}

	//Send Stop
	i2cStop();
	return 1;
}

//For devices with 2-byte internal address registers
u08 i2cReadDataFromRegisters2(u08 address, u16 reg, u08 byteCount, u08* data)
{
	u08 status = 0;
	u08 i = 0;
	//Send start and note the status.
	status = i2cStart();
	//If the status is not "start sent" or "restart sent" return 0 for error.
	if (status != TW_START && status != TW_REP_START)
		return 0;

	//Send device address
	status = i2cByteTransmit(address);
	//If the status is not "slave address + read bit transmitted, ACK received" return 0 for error.
	if (status != TW_MT_SLA_ACK)
		return 0;

	status = i2cByteTransmit((u08)(reg>>8));
	//If the status is not "data transmitted, ACK received" return 0 for error.
	if (status != TW_MT_DATA_ACK)
		return 0;

	status = i2cByteTransmit(reg);
	//If the status is not "data transmitted, ACK received" return 0 for error.
	if (status != TW_MT_DATA_ACK)
		return 0;

	//Send repeat start
	status = i2cStart();
	//If the status is not "start sent" or "restart sent" return 0 for error.
	if (status != TW_START && status != TW_REP_START)
		return 0;

	//Send device address + read bit
	status = i2cByteTransmit(address+1);
	//If the status is not "slave address + read bit transmitted, ACK received" return 0 for error.
	if (status != TW_MR_SLA_ACK)
		return 0;

	//Receive all but the last byte using the mode that returns an ACK.
	for (i = 0; i < (byteCount-1); i++)
	{
		data[i] = i2cByteReceive(1);
	}
	//Receive the last byte using the mode that returns a NACK, to indicate that this is the last byte.
	data[i] = i2cByteReceive(0);

	//Send Stop
	i2cStop();
	return 1;
}
