#include "crc.h"

const unsigned char cCRC8Table[256]={
0X00, 0X07, 0X0E, 0X09, 0X1C, 0X1B, 0X12, 0X15,
0X38, 0X3F, 0X36, 0X31, 0X24, 0X23, 0X2A, 0X2D,
0X70, 0X77, 0X7E, 0X79, 0X6C, 0X6B, 0X62, 0X65,
0X48, 0X4F, 0X46, 0X41, 0X54, 0X53, 0X5A, 0X5D,
0XE0, 0XE7, 0XEE, 0XE9, 0XFC, 0XFB, 0XF2, 0XF5,
0XD8, 0XDF, 0XD6, 0XD1, 0XC4, 0XC3, 0XCA, 0XCD,
0X90, 0X97, 0X9E, 0X99, 0X8C, 0X8B, 0X82, 0X85,
0XA8, 0XAF, 0XA6, 0XA1, 0XB4, 0XB3, 0XBA, 0XBD,
0XC7, 0XC0, 0XC9, 0XCE, 0XDB, 0XDC, 0XD5, 0XD2,
0XFF, 0XF8, 0XF1, 0XF6, 0XE3, 0XE4, 0XED, 0XEA,
0XB7, 0XB0, 0XB9, 0XBE, 0XAB, 0XAC, 0XA5, 0XA2,
0X8F, 0X88, 0X81, 0X86, 0X93, 0X94, 0X9D, 0X9A,
0X27, 0X20, 0X29, 0X2E, 0X3B, 0X3C, 0X35, 0X32,
0X1F, 0X18, 0X11, 0X16, 0X03, 0X04, 0X0D, 0X0A,
0X57, 0X50, 0X59, 0X5E, 0X4B, 0X4C, 0X45, 0X42,
0X6F, 0X68, 0X61, 0X66, 0X73, 0X74, 0X7D, 0X7A,
0X89, 0X8E, 0X87, 0X80, 0X95, 0X92, 0X9B, 0X9C,
0XB1, 0XB6, 0XBF, 0XB8, 0XAD, 0XAA, 0XA3, 0XA4,
0XF9, 0XFE, 0XF7, 0XF0, 0XE5, 0XE2, 0XEB, 0XEC,
0XC1, 0XC6, 0XCF, 0XC8, 0XDD, 0XDA, 0XD3, 0XD4,
0X69, 0X6E, 0X67, 0X60, 0X75, 0X72, 0X7B, 0X7C,
0X51, 0X56, 0X5F, 0X58, 0X4D, 0X4A, 0X43, 0X44,
0X19, 0X1E, 0X17, 0X10, 0X05, 0X02, 0X0B, 0X0C,
0X21, 0X26, 0X2F, 0X28, 0X3D, 0X3A, 0X33, 0X34,
0X4E, 0X49, 0X40, 0X47, 0X52, 0X55, 0X5C, 0X5B,
0X76, 0X71, 0X78, 0X7F, 0X6A, 0X6D, 0X64, 0X63,
0X3E, 0X39, 0X30, 0X37, 0X22, 0X25, 0X2C, 0X2B,
0X06, 0X01, 0X08, 0X0F, 0X1A, 0X1D, 0X14, 0X13,
0XAE, 0XA9, 0XA0, 0XA7, 0XB2, 0XB5, 0XBC, 0XBB,
0X96, 0X91, 0X98, 0X9F, 0X8A, 0X8D, 0X84, 0X83,
0XDE, 0XD9, 0XD0, 0XD7, 0XC2, 0XC5, 0XCC, 0XCB,
0XE6, 0XE1, 0XE8, 0XEF, 0XFA, 0XFD, 0XF4, 0XF3
};

const unsigned char cCRC16HiTable[256]={
0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0, 0X80, 0X41, 0X01, 0XC0,
0X80, 0X41, 0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0, 0X80, 0X41,
0X00, 0XC1, 0X81, 0X40, 0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0,
0X80, 0X41, 0X01, 0XC0, 0X80, 0X41, 0X00, 0XC1, 0X81, 0X40,
0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0, 0X80, 0X41, 0X00, 0XC1,
0X81, 0X40, 0X01, 0XC0, 0X80, 0X41, 0X01, 0XC0, 0X80, 0X41,
0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0, 0X80, 0X41, 0X00, 0XC1,
0X81, 0X40, 0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0, 0X80, 0X41,
 0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0, 0X80, 0X41, 0X01, 0XC0,
 0X80, 0X41, 0X00, 0XC1, 0X81, 0X40, 0X00, 0XC1, 0X81, 0X40,
0X01, 0XC0, 0X80, 0X41, 0X01, 0XC0, 0X80, 0X41, 0X00, 0XC1,
0X81, 0X40, 0X01, 0XC0, 0X80, 0X41, 0X00, 0XC1, 0X81, 0X40,
0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0, 0X80, 0X41, 0X01, 0XC0,
0X80, 0X41, 0X00, 0XC1, 0X81, 0X40, 0X00, 0XC1, 0X81, 0X40,
0X01, 0XC0, 0X80, 0X41, 0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0,
0X80, 0X41, 0X01, 0XC0, 0X80, 0X41, 0X00, 0XC1, 0X81, 0X40,
0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0, 0X80, 0X41, 0X01, 0XC0,
0X80, 0X41, 0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0, 0X80, 0X41,
0X00, 0XC1, 0X81, 0X40, 0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0,
0X80, 0X41, 0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0, 0X80, 0X41,
0X01, 0XC0, 0X80, 0X41, 0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0,
0X80, 0X41, 0X00, 0XC1, 0X81, 0X40, 0X00, 0XC1, 0X81, 0X40,
0X01, 0XC0, 0X80, 0X41, 0X01, 0XC0, 0X80, 0X41, 0X00, 0XC1,
0X81, 0X40, 0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0, 0X80, 0X41,
0X00, 0XC1, 0X81, 0X40, 0X01, 0XC0, 0X80, 0X41, 0X01, 0XC0,
0X80, 0X41, 0X00, 0XC1, 0X81, 0X40};

const unsigned char cCRC16LoTable[256]={0X00, 0XC0, 0XC1, 0X01, 0XC3, 0X03, 0X02, 0XC2, 0XC6, 0X06,
0X07, 0XC7, 0X05, 0XC5, 0XC4, 0X04, 0XCC, 0X0C, 0X0D, 0XCD,
0X0F, 0XCF, 0XCE, 0X0E, 0X0A, 0XCA, 0XCB, 0X0B, 0XC9, 0X09,
0X08, 0XC8, 0XD8, 0X18, 0X19, 0XD9, 0X1B, 0XDB, 0XDA, 0X1A,
0X1E, 0XDE, 0XDF, 0X1F, 0XDD, 0X1D, 0X1C, 0XDC, 0X14, 0XD4,
0XD5, 0X15, 0XD7, 0X17, 0X16, 0XD6, 0XD2, 0X12, 0X13, 0XD3,
0X11, 0XD1, 0XD0, 0X10, 0XF0, 0X30, 0X31, 0XF1, 0X33, 0XF3,
0XF2, 0X32, 0X36, 0XF6, 0XF7, 0X37, 0XF5, 0X35, 0X34, 0XF4,
0X3C, 0XFC, 0XFD, 0X3D, 0XFF, 0X3F, 0X3E, 0XFE, 0XFA, 0X3A,
0X3B, 0XFB, 0X39, 0XF9, 0XF8, 0X38, 0X28, 0XE8, 0XE9, 0X29,
0XEB, 0X2B, 0X2A, 0XEA, 0XEE, 0X2E, 0X2F, 0XEF, 0X2D, 0XED,
0XEC, 0X2C, 0XE4, 0X24, 0X25, 0XE5, 0X27, 0XE7, 0XE6, 0X26,
0X22, 0XE2, 0XE3, 0X23, 0XE1, 0X21, 0X20, 0XE0, 0XA0, 0X60,
0X61, 0XA1, 0X63, 0XA3, 0XA2, 0X62, 0X66, 0XA6, 0XA7, 0X67,
0XA5, 0X65, 0X64, 0XA4, 0X6C, 0XAC, 0XAD, 0X6D, 0XAF, 0X6F,
0X6E, 0XAE, 0XAA, 0X6A, 0X6B, 0XAB, 0X69, 0XA9, 0XA8, 0X68,
0X78, 0XB8, 0XB9, 0X79, 0XBB, 0X7B, 0X7A, 0XBA, 0XBE, 0X7E,
0X7F, 0XBF, 0X7D, 0XBD, 0XBC, 0X7C, 0XB4, 0X74, 0X75, 0XB5,
0X77, 0XB7, 0XB6, 0X76, 0X72, 0XB2, 0XB3, 0X73, 0XB1, 0X71,
0X70, 0XB0, 0X50, 0X90, 0X91, 0X51, 0X93, 0X53, 0X52, 0X92,
0X96, 0X56, 0X57, 0X97, 0X55, 0X95, 0X94, 0X54, 0X9C, 0X5C,
0X5D, 0X9D, 0X5F, 0X9F, 0X9E, 0X5E, 0X5A, 0X9A, 0X9B, 0X5B,
0X99, 0X59, 0X58, 0X98, 0X88, 0X48, 0X49, 0X89, 0X4B, 0X8B,
0X8A, 0X4A, 0X4E, 0X8E, 0X8F, 0X4F, 0X8D, 0X4D, 0X4C, 0X8C,
0X44, 0X84, 0X85, 0X45, 0X87, 0X47, 0X46, 0X86, 0X82, 0X42,
0X43, 0X83, 0X41, 0X81, 0X80, 0X40 };

//********************************************************************************************************/
unsigned char VerifyCRC8Sub(unsigned char *vData,unsigned char vLong)
{
	unsigned char  vCRC_Data;
	unsigned char  vTempData;
	unsigned char  vLoop;

	vCRC_Data=0X00;

	for(vLoop=0X00;vLoop<vLong;vLoop++)
	{

		vTempData=vCRC_Data^(*vData);
		vCRC_Data=cCRC8Table[vTempData];
		vData++;
	}

        if(vCRC_Data!=*vData)
        {       return 0X00;                             }
        else
        {       return 0XFF;                             }
}

//********************************************************************************************************/
void SetCRC8Sub(unsigned char *vData,unsigned char vLong)
{
	unsigned char  vCRC_Data;
	unsigned char  vTempData;
	unsigned char  vLoop;

	vCRC_Data=0X00;

	for(vLoop=0X00;vLoop<vLong;vLoop++)
	{

		vTempData=vCRC_Data^(*vData);
		vCRC_Data=cCRC8Table[vTempData];
		vData++;
	}

        *vData=vCRC_Data;
}

///****************************************************************************************/
unsigned char VerifyCRC16Sub(unsigned char *vData,unsigned char vLong)
{
	unsigned char  vCRCHi_Data;
	unsigned char  vCRCLo_Data;
	unsigned char  vIndex;
	unsigned char  vLoop;

        vCRCHi_Data=0XFF;
        vCRCLo_Data=0XFF;
	for(vLoop=0X00;vLoop<vLong;vLoop++)
	{

		vIndex=vCRCHi_Data^(*vData);
		vData++;

		vCRCHi_Data=vCRCLo_Data^cCRC16HiTable[vIndex];
		vCRCLo_Data=cCRC16LoTable[vIndex];

	}

        if(vCRCHi_Data!=*vData)
        {       return 0X00;                             }

        vData++;
        if(vCRCLo_Data!=*vData)
        {       return 0X00;                             }

        else
        {       return 0XFF;                             }
}

///****************************************************************************************/
void SetCRC16Sub(unsigned char *vData,unsigned char vLong)
{
	unsigned char  vCRCHi_Data;
	unsigned char  vCRCLo_Data;
	unsigned char  vIndex;
	unsigned char  vLoop;

        vCRCHi_Data=0XFF;
        vCRCLo_Data=0XFF;
	for(vLoop=0X00;vLoop<vLong;vLoop++)
	{

		vIndex=vCRCHi_Data^(*vData);
		vData++;

		vCRCHi_Data=vCRCLo_Data^cCRC16HiTable[vIndex];
		vCRCLo_Data=cCRC16LoTable[vIndex];
	}

        *vData=vCRCHi_Data;

        vData++;
        *vData=vCRCLo_Data;
}



//********************************************************************************************************/
/*                                                                                                      */
