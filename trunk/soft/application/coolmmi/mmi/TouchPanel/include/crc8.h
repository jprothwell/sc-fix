#ifndef	__CRC8_H__
#define	__CRC8_H__

/*crc8Ð£Ñéº¯Êý*/
unsigned char crc8(unsigned char *data, unsigned int num_bytes);

int ChkCrc8(unsigned char checksum, char *InputBuf, int length);

void CalculateCrc8(unsigned char *CrcRes, char *InputBuf, int length);

#endif
