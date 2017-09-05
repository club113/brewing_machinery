#ifndef __CRC_H__
#define __CRC_H__



unsigned char VerifyCRC8Sub(unsigned char *vData,unsigned char vLong);

void SetCRC8Sub(unsigned char *vData,unsigned char vLong);

unsigned char VerifyCRC16Sub(unsigned char *vData,unsigned char vLong);
void SetCRC16Sub(unsigned char *vData,unsigned char vLong);


#endif
