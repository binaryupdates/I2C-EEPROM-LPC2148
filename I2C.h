#ifndef I2C_H
#define I2C_H

/* Settings */
#define	PERIFERAL_OPERATING_FREQUENCY_IN_HZ	14745600
#define	EEPROM_OPERATING_FREQUENCY_IN_HZ	50000 
#define EEPROM_DEVADDR 						0xA0
#define	EEPROM_24C512						1		 // EEPROM IC used 
#define	EEPROM_SIZE							0xFFFF	 // Size
#define	EEPROM_WAIT_TIME_OUT				5000	 // 1msec

//EEPROM_24C02 	size 0x0000FF
//EEPROM_24C04 	size 0x0001FF
//EEPROM_24C08 	size 0x0003FF
//EEPROM_24C16 	size 0x0007FF
//EEPROM_24C32 	size 0x000FFF
//EEPROM_24C64 	size 0x001FFF
//EEPROM_24C128 size 0x003FFF
//EEPROM_24C256 size 0x007FFF
//EEPROM_24C512 size 0x00FFFF


/* Function Definations */
extern	void 	I2C_Init (void);
extern	BOOL	I2C_WaitStatus (uint8 u8status);
extern	BOOL 	I2C_WriteToEEPROM (uint32 u32startAddr, uint8 *u8ptr2arr, uint32 u32len);
extern	BOOL 	I2C_ReadFromEEPROM (uint32 u32startAddr, uint8 *u8ptr2arr, uint32 u32len);

#endif //I2C_H


