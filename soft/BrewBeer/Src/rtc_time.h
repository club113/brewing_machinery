#ifndef __RTC_TIME_H__
#define __RTC_TIME_H__
#include "include.h"

///

#define TIME_1990	2208988802UL
#define BJ_TIME	28800

typedef struct 
{  
    unsigned char LiVnMode;  
    unsigned char Stratum;  
    unsigned char Poll;  
    unsigned char Precision;  
    long int RootDelay;  
    long int RootDispersion;  
    char RefID[4];  
    long int RefTimeInt;  
    long int RefTimeFraction;  
    long int OriTimeInt;  
    long int OriTimeFraction;  
    long int RecvTimeInt;  
    long int RecvTimeFraction;  
    long int TranTimeInt;  
    long int TranTimeFraction;  
}STNP_Header,*P_STNP_Header; 
extern STNP_Header stnp_tel;

#define OFFSET_SECOND   946684800 //1970 1 1 0 0 0   ==>  2000 1 1 0 0 0
#define SECOND_OF_DAY   86400   
typedef struct date_time
{
	unsigned char is_time_checked;
	unsigned short  iYear;
	unsigned short   iMon;
	unsigned short   iDay;

	unsigned short   iHour;
	unsigned short   iMin;
	unsigned short   iSec;
	unsigned short   iMsec;
} DATE_TIME;
extern DATE_TIME time_info;

void GetDateTimeFromSecond(unsigned long lSec, DATE_TIME *tTime);
unsigned long GetSecondTime(DATE_TIME *date_time);
void get_stnp_time(void);
#endif

