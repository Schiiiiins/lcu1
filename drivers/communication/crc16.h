

#ifndef __CRC_16__
#define __CRC_16__

// definitions
#define CRC16_POLYNOM 0x1002;

// function prototypes
extern void hs485_crc16_init(unsigned short* crc16_register);
extern void hs485_crc16_shift(unsigned short* crc16_register, unsigned char w);

#endif /* __CRC_16__ */
