#ifndef __IAP_H__
#define __IAP_H__

#include "Include.h"

#define  cMaxRAM  200
#define  MAX_DATALENGTH  150

#define DEVICE_SOFT_VER  (0X0101)

#define app_base 		0X11
#define app_base_addr 0X8000000

#define app_1 		0X22
#define app_1_addr 	0X801C000

#define app_2  		0X33
#define app_2_addr   	0X8028000

#define app_3 		0X44
#define app_3_addr 	0X8034000

#define no_jump  		0X88
#define try_jump_flag	0X89




///////////////////////////////////////////////////////////////////////需要根据情况更改
#define THIS_APP  app_base
#define THIS_APP_ADDR  app_base_addr


#define NEXT_APP		app_1
#define WRITE_TO_ADDR app_1_addr
///////////////////////////////////////////////////////////////////////


#define NVIC_VectTab_RAM             ((unsigned int)0x20000000)
#define NVIC_VectTab_FLASH           ((unsigned int)0x08000000)
#define IS_NVIC_VECTTAB(VECTTAB) (((VECTTAB) == NVIC_VectTab_RAM) || \
                                  ((VECTTAB) == NVIC_VectTab_FLASH))

#define IS_NVIC_OFFSET(OFFSET)  ((OFFSET) < 0x0007FFFF)


#define cManufacturerID_Hi  0X68
//#define cManufacturerID_Lo  0X80

#define cSendDataToGatewayTime  	3
#define cMainCommand        			0X0A// 临时调试用

#define  ACKCMD   					0xf0   //0xf0
#define  CALLIDCMD  				0x00  // 0x00

#define   SET_REPEAT      			0x25 //0x25
#define  ONLINE_CLOSE 			0xfe
#define  ONLINE_OPEN		 		0xfd
#define  ONLINE_RESET              	 	0xf3
#define  ONLINE_READSOFTVER 		0Xf2		 
#define  ONLINE_APP_CHOOSE         	0xEB //  A ,B , 0XFF   






#define DOWMLOAD_PAGE_SIAE 0X800


#define cManufacturerID_Hi  0X68
#define cManufacturerID_Lo  0X8A

#define cSendDataToGatewayTime  3
#define cMainCommand        0X0A// 临时调试用

#define  ACKCMD   				0xf0   //0xf0
#define  CALLIDCMD  			0x00  // 0x00
#define  LEARN_REDCODE     	0x17  //0x10
#define  SET_AIRCONDITION   	0x11  // 0x11
#define  CHECK_SWID			0x13// 0x13
#define  LEARN_SWID			0x32 //0x32
#define  CLEAR_SWID			0x33 //0x33
#define  SET_REPEAT      		0x25 //0x25


#define  IAP_DATA 	0XF0
#define  IAP_BEGIN    	0xEF
#define  IAP_END		0xEE
#define  IAP_OK 		0xED
#define  IAP_FAIL      	0xEC

//#define  FLASH_PAGE_SIZE 0x400
 //#define  FLASH_PAGE_SIZE 0x800 



//#define   PAGE_MAX_FRAME   8 //  每页所存数据帧当前STM32F103R8 为64K flash   每页为1K 
#define   PAGE_MAX_FRAME  16 //  
#define  FRAME_MAX_NUMBER  128  //每一帧最大数据 
#define  BIGDATAACK_OK  0
#define  BIGDATAACK_NOSUPPORTED  2
#define  BIGDATAACK_WARMPARAM   3


#define  FLASH_OFFSET     ((unsigned int)0X0000)
#define  FLASH_OFFSETA     ((unsigned int)0X19000)
#define  FLASH_OFFSETB     ((unsigned int)0X19000)
 //#define FLASH_StartAddr  	((unsigned int)0x08005000)
 
 #define FLASH_StartAddr  	((unsigned int)0x0800A000)
 
 //#define EndAddr  		  	((unsigned int)0x0800f000) // 针对参数存储，单独留存一页
#define EndAddr  		  	((unsigned int)0x0803C000)
 
 //这是一页参数信息，用户可以在这里存储系统参数
#define   FLASH_MESSAGE_ADDR    EndAddr+FLASH_PAGE_SIZE// 

// 发射机需要存储内容 包括如下：
// 
#define    FLASH_START_IAP_BUFFA_ADDR      FLASH_StartAddr	   
#define    FLASH_START_IAP_BUFFB_ADDR      (FLASH_StartAddr +FLASH_OFFSETA)
/*
#define 	FLASH_APP_1_ADDR			((unsigned int)0x08010000)
#define 	FLASH_APP_2_ADDR			((unsigned int)0x08020000)
#define 	FLASH_APP_3_ADDR			((unsigned int)0x08030000)
#define 	FLASH_APP_BASE_ADDR		((unsigned int)0x08000000)


#define 	FLASH_APP_1_ADDR			((unsigned int)0x0801C000)
#define 	FLASH_APP_2_ADDR			((unsigned int)0x08028000)
#define 	FLASH_APP_3_ADDR			((unsigned int)0x08034000)
#define 	FLASH_APP_BASE_ADDR		((unsigned int)0x08000000)
*/
//extern  unsigned char g_flash_buffer[MAXFLASHPAGESIZE];


//////////IAP.Page_buffer较大,设计可以作为公用缓存,一下宏指引当前使用缓存的地方
#define OWNER_ON_ONE			0X00
#define OWNER_IAP_BUFFER		0X01
#define OWNER_FLASH_READER 	0X02
#define OWNER_RAM_BUFFER		0X03
#define OWNER_WIFI_IAP_BUFFER	0X04


#define WIFI_IAP_OPERAT_START			0X01
#define WIFI_IAP_OPERAT_START_BACK		0X02
#define WIFI_IAP_OPERAT_BEGIN			0X03
#define WIFI_IAP_OPERAT_BEGIN_BACK		0X04

#define WIFI_IAP_OPERAT_END				0X05
#define WIFI_IAP_OPERAT_TRANSMISSION 	0X07


typedef struct
	{
	unsigned char owner;
	unsigned char  BUFFflag; // 1; NEW  0 : old
	unsigned int Write_base_addr;
	unsigned int   Frame_Number;// 数据包号 
	unsigned int   Frame_All_Number;
	unsigned int   Frame_count;// 下位机帧计数器
	unsigned char  Page_Number;//存储页号
	unsigned char  Current_Frame_Len;
	//unsigned char   Frame_buffer [128];// 每一帧的数据缓存
	unsigned char   Page_buffer[FLASH_PAGE_SIZE];//一页的数据缓存可作为除了供iap以外的缓存使用
	}IAP;

typedef struct {
	unsigned bSendDataToGateway:1;
	unsigned bSendTCM300IDToGateway:1;
	unsigned char  vSendDataToGatewayCount;
	unsigned char  vRandomTime;

	unsigned char  vSendDataToGatewayTime;
	unsigned char  vDeviceORG;
	unsigned char  vDevice_ID[4];
 } Gateway;

union UNION_FLOAT_LONG_CHAR
{
	float 	sfloat;
	long 	slong;
	unsigned char 	unchar[4];
};
void water_light(unsigned char led_number);
//void choose_app(unsigned char app_num);
void test_jump(void);
void disable_all_irq(void);

void jump_to_app(unsigned int app_addr);
    
//extern Enocean vEnocean;

extern void Deal_Gateway_RadioSub(void);
extern void UploadLEDToGatewaySub (void);
extern void UploadVersionToGatewaySub (void);
extern void UploadONOFFToGatewaySub (void);
extern void GateWayID2DeviceIDSub(unsigned char *pGatewayID);
extern void UploadDataToGatewaySub(void);
extern void SendTCM300ID2GatewaySub(void);
extern void TCM300ID2DeviceIDSub(void);
extern void SetSendGatewayHeadCodeSub(unsigned char vNodeLong);
extern void SetSendGatewayCommandCodeSub(unsigned char  vSubCommand);
extern void SetSendGatewayIDCodeSub(void);
extern void SetSendGatewayEndCodeSub(unsigned char vNodeLong);
extern void SetRepeatParameterSub(unsigned char vRepeatMode,unsigned char  vRepeatLevel);
extern void ControllerAckGatewaySub(void);
extern void EndTCM300TxDataSub(void);
extern void SetRandomTimeSub(void) ;
extern void DealBigdata(void);
extern void ControllerAckBigDataSub(unsigned int x);
extern unsigned char VerifyCRC8Sub(unsigned char *vData,unsigned char vLong);
extern void SetCRC8Sub(unsigned char *vData,unsigned char vLong);
extern void EnoceanSend(unsigned int length);
extern void IAP_Flash_write(unsigned char Len);

//void write_iap_buff(unsigned int length);
void reset_iap_buff(void);
void SystemClock_Config(void);
void write_iap_buff(unsigned int length);

void try_to_jump(unsigned char app);
void reset_iap_buff(void);
void read_app_info(void);
void write_app_info(void);
void choose_app(void);
void init_save_data(void);


void wifi_iap_request_page(unsigned int page);
void wifi_iap_operater(void);
void wifi_iap_cmd_operat(P_S_Wifi_Iap_Cmd cmd_parameter);
void wifi_iap_send_ready_code(void);
void wifi_iap_write_to_flash(void);
void wifi_iap_send_end_code(void);
void wifi_iap_copy_to_flash(void);




#endif

