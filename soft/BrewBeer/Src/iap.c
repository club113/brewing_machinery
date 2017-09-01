#include "iap.h"



//TCM300 vTCM300;
//Enocean vEnocean;
IAP vIAP;
unsigned char	g_sendPosi,g_sendCount,g_sendBuf[MAX_DATALENGTH], 		 
			g_receTimeOut,g_receflag;

unsigned char g_EnoceanDMA_Sendoverflag;
Gateway vGateway;
APP_INFORMATION App_info;


extern ID_def id;
extern recv_data_def data_from_stm300;
extern S_Wifi_Iap_Info wifi_iap_info;
extern recv_data_def data_from_wifi;

typedef  void (*pFunction)(void);
#define JUMP_TEST	(uint32_t)0x08023000
pFunction JumpToApplication;
uint32_t JumpAddress;


extern unsigned char iap_recv_buf[DOWMLOAD_PAGE_SIAE];


void water_light(unsigned char led_number)
{
	LED_1_OFF;
	LED_2_OFF;
	LED_3_OFF;
	LED_4_OFF;
	LED_5_OFF;
	switch(led_number)
		{
			case (0):
				LED_1_ON;
				break;

			case (1):
				LED_2_ON;
				break;

			case (2):
				LED_3_ON;
				break;

			case (3):
				LED_4_ON;
				break;

			case (4):
				LED_5_ON;
				break;
			case (5):
				LED_4_ON;
				break;

			case (6):
				LED_3_ON;
				break;

			case (7):
				LED_2_ON;
				break;


		}
	
}

/*void choose_app(unsigned char app_num)
{
	switch(app_num)
		{
			case(APP_DOWNLOAD):
				App_info.cur_app_offset = 0X08002000;
				write_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
				NVIC_SystemReset();				
				break;

			case(APP_1):
				App_info.app_point = APP_1;
				App_info.cur_app_offset = App_info.app_offset_table[APP_1];
				write_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
				NVIC_SystemReset();				
				break;

			case(APP_2):
				App_info.app_point = APP_2;
				App_info.cur_app_offset = App_info.app_offset_table[APP_2];
				write_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
				NVIC_SystemReset();	
				break;

			case(APP_3):
				App_info.app_point = APP_3;
				App_info.cur_app_offset = App_info.app_offset_table[APP_3];
				write_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
				NVIC_SystemReset();	
				break;

			case(APP_4):
				App_info.app_point = APP_4;
				App_info.cur_app_offset = App_info.app_offset_table[APP_4];
				write_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
				break;

			default:
				App_info.cur_app_offset = 0X08002000;
				write_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
				NVIC_SystemReset();
				
				break;
		}
}*/

void test_jump(void)
{
	HAL_Init();	
	disable_all_irq();
	SystemClock_Config();
	if (((*(__IO uint32_t*)app_3_addr) & 0x2FFE0000 ) == 0x20000000)
				   {
				      /* Jump to user application */
				      JumpAddress = *(__IO uint32_t*) (app_3_addr + 4);
				     JumpToApplication = (pFunction) JumpAddress;
				      /* Initialize user application's Stack Pointer */
				      __set_MSP(*(__IO uint32_t*) app_3_addr);
				      JumpToApplication();
				    } 
	
}

void jump_to_app(unsigned int app_addr)
{
	HAL_Init();
	SystemClock_Config();	
	//__disable_irq();
	//__disable_fault_irq();
	disable_all_irq();
	HAL_RCC_DeInit();
	if (((*(__IO uint32_t*)app_addr) & 0x2FFE0000 ) == 0x20000000)
				   {
				      /* Jump to user application */
				      JumpAddress = *(__IO uint32_t*) (app_addr + 4);
				     JumpToApplication = (pFunction) JumpAddress;
				      /* Initialize user application's Stack Pointer */
				      __set_MSP(*(__IO uint32_t*) app_addr);
				      JumpToApplication();
				    } 
	
}


void disable_all_irq(void)
{
	unsigned char loopx;
	for(loopx = 0;loopx< 60;loopx ++)
		{			
			HAL_NVIC_DisableIRQ((IRQn_Type)loopx);
			NVIC_ClearPendingIRQ((IRQn_Type)loopx);
		}
}




void ControllerAckGatewaySub(void)
{
        

	SetSendGatewayHeadCodeSub(0X00);                     ///8Bytes，数据长度为0
	SetSendGatewayCommandCodeSub(0XF0);                  ///Salve Command=0XF0
        SetSendGatewayIDCodeSub();
        SetSendGatewayEndCodeSub(0X00);
        EnoceanSend(0X1B);

        
}


void GateWayID2DeviceIDSub(unsigned char *pGatewayID)
{
        vGateway.vDeviceORG=0Xa0;
        vGateway.vDevice_ID[3]=*pGatewayID;//0x01;//
        pGatewayID++;
        vGateway.vDevice_ID[2]=*pGatewayID;//0x9d;//
        pGatewayID++;
        vGateway.vDevice_ID[1]=*pGatewayID;//0x42;//
        pGatewayID++;
        vGateway.vDevice_ID[0]=*pGatewayID;//0x75;//
}
void SendTCM300ID2GatewaySub(void)
{
	SetSendGatewayHeadCodeSub(0X00);                     ///8Bytes，数据长度为0
	SetSendGatewayCommandCodeSub(ACKCMD);                  ///old Salve Command=0XA0
        SetSendGatewayIDCodeSub();
        SetSendGatewayEndCodeSub(0X00);                      ///8Bytes，数据长度为0
        EnoceanSend(0X1b);
}

//********************************************************************************************************/
void SetSendGatewayHeadCodeSub(unsigned char vNodeLong)
{
	g_sendBuf[0X00]=0X55;
        g_sendBuf[0X01]=0X00;
        g_sendBuf[0X02]=vNodeLong+0X0E;
        g_sendBuf[0X03]=0X06;
	g_sendBuf[0X04]=0X01;
	SetCRC8Sub(&g_sendBuf[0X01],0X04);
}

//********************************************************************************************************/
void SetSendGatewayCommandCodeSub(unsigned char  vSubCommand)
{
	g_sendBuf[0X06]=0XD1;                      ///RORG
	g_sendBuf[0X07]=cManufacturerID_Hi;
	g_sendBuf[0X08]=cManufacturerID_Lo+cMainCommand;  ///MainCommand;
	g_sendBuf[0X09]=vSubCommand;
}

//********************************************************************************************************/
void SetSendGatewayIDCodeSub(void)
{
        g_sendBuf[0X0A]=vGateway.vDeviceORG;
        g_sendBuf[0X0B]=vGateway.vDevice_ID[3];
        g_sendBuf[0X0C]=vGateway.vDevice_ID[2];
        g_sendBuf[0X0D]=vGateway.vDevice_ID[1];
        g_sendBuf[0X0E]=vGateway.vDevice_ID[0];
}

//********************************************************************************************************/
void SetSendGatewayEndCodeSub(unsigned char vNodeLong)
{
	unsigned char  vLong;

        g_sendBuf[vNodeLong+0X0F]=id.TCM300_ID[0];             ///TCM300 ID
        g_sendBuf[vNodeLong+0X10]=id.TCM300_ID[1];
        g_sendBuf[vNodeLong+0X11]=id.TCM300_ID[2];
        g_sendBuf[vNodeLong+0X12]=id.TCM300_ID[3];

        g_sendBuf[vNodeLong+0X13]=0X00;
        g_sendBuf[vNodeLong+0X14]=0X03;

        g_sendBuf[vNodeLong+0X15]=0XFF;
        g_sendBuf[vNodeLong+0X16]=0XFF;
        g_sendBuf[vNodeLong+0X17]=0XFF;
        g_sendBuf[vNodeLong+0X18]=0XFF;
        g_sendBuf[vNodeLong+0X19]=0XFF;

        vLong=vNodeLong+0X14;
        SetCRC8Sub(&g_sendBuf[0X06],vLong);
}

//********************************************************************************************************/
void SetSendGatewayDataCodeSub(unsigned char *Channel_Status, unsigned char vNodeLong)
{
	unsigned char  vLoop;

	 
	if(vNodeLong==0X00)
	{       return;                                           }

	for(vLoop=0X01;vLoop<=vNodeLong;vLoop++)
	{
		g_sendBuf[vLoop+0X09]=*Channel_Status;
		Channel_Status++;
        }
}


void UploadVersionToGatewaySub (void)					///上传版本号给网关
{
       //unsigned long temp;
       
	SetSendGatewayHeadCodeSub(0X00);                     ///8Bytes，数据长度为0
	SetSendGatewayCommandCodeSub(0XF1);                  ///Salve Command=0XF2

        //temp = DEVICE_SOFT_VER;
	g_sendBuf[0X0A]=DEVICE_SOFT_VER&0xff;	                  ///版本号20151217
	g_sendBuf[0X0B]=(DEVICE_SOFT_VER>>8)&0xff;	
	g_sendBuf[0X0C]=(DEVICE_SOFT_VER>>16)&0xff;	
	g_sendBuf[0X0D]=(DEVICE_SOFT_VER>>24)&0xff;	

	g_sendBuf[0X0E]=id.TCM300_ID[0];             ///TCM300 ID
	g_sendBuf[0X0F]=id.TCM300_ID[1];
	g_sendBuf[0X10]=id.TCM300_ID[2];
	g_sendBuf[0X11]=id.TCM300_ID[3];

	g_sendBuf[0X12]=0X00;
	g_sendBuf[0X13]=0X03;

	g_sendBuf[0X14]=0XFF;
	g_sendBuf[0X15]=0XFF;
	g_sendBuf[0X16]=0XFF;
	g_sendBuf[0X17]=0XFF;
	g_sendBuf[0X18]=0XFF;
	SetCRC8Sub(&g_sendBuf[0X06],0X13);

	EnoceanSend(0X1A);
}


void SetRepeatParameterSub(unsigned char vRepeatMode,unsigned char  vRepeatLevel)
{
//	unsigned char vLoop;

        g_sendBuf[0]=0X55;
	g_sendBuf[1]=0X00;
	g_sendBuf[2]=0X03;
	g_sendBuf[3]=0X00;
	g_sendBuf[4]=0X05;
	SetCRC8Sub(&g_sendBuf[1],0X04);

        g_sendBuf[6]=0X09;
	if(vRepeatMode==0X00)
	{
	        g_sendBuf[7]=0X00;
	        g_sendBuf[8]=0X00; 

	        SetCRC8Sub(&g_sendBuf[6],3);  
	}
	else if(vRepeatMode==0X01)
	{
		if(vRepeatLevel==0X01)
		{
			g_sendBuf[7]=0X01;
	                g_sendBuf[8]=0X01;

	                SetCRC8Sub(&g_sendBuf[6],3);
		}

		else if(vRepeatLevel==0X02)
		{
			g_sendBuf[7]=0X01;
	                g_sendBuf[8]=0X02;

	                SetCRC8Sub(&g_sendBuf[6],3);
		}

		else
		{
			return;
		}
	}

	else
	{
		return;
	}
        EnoceanSend(0X0A);
}
//********************************************************************************************************/
void Deal_Gateway_RadioSub(void)
{
	unsigned char  vMainCommand;
	unsigned char  vSubCommand;
//	unsigned char  vParameterData01;
//	unsigned char  vParameterData02;
//	unsigned char  vPoint,Epoint;
	unsigned char temp;
//	union   UNION_FLOAT_LONG_CHAR  fdata;
      

        vMainCommand=(data_from_stm300.data[0X07]&0X0F);
        vSubCommand=data_from_stm300.data[0X08];
        if(vMainCommand!=cMainCommand)
        {    
               return;                       
               }

        if(vSubCommand==CALLIDCMD)                                                     ///网关查询所有设备ID old0x40
        {      
		GateWayID2DeviceIDSub(&data_from_stm300.data[9]);
		SendTCM300ID2GatewaySub();	
		vGateway.vSendDataToGatewayCount=0X00;
		// vGateway.vRandomTime=RandomSub();
		vGateway.bSendTCM300IDToGateway=1;

		vGateway.bSendDataToGateway=0;
		vGateway.vSendDataToGatewayTime=0X00;
        }     
        else                                                                   //判断网关是不是给本设备发送 数据
        {
               
                if(id.TCM300_ID[0X00]!=data_from_stm300.data[9])
                      return;                                             
                if(id.TCM300_ID[0X01]!=data_from_stm300.data[10])
                    return;                                           
                if(id.TCM300_ID[0X02]!=data_from_stm300.data[11])
                     return;                                          
                if(id.TCM300_ID[0X03]!=data_from_stm300.data[12])
                    return;                                             
                
        	switch(vSubCommand)
        	{      	      
		
		case  SET_REPEAT:                                    ///修改Repeater模?              				 
			GateWayID2DeviceIDSub(&data_from_stm300.data[0X0F]);        	     	      
			ControllerAckGatewaySub();
			//   SetRepeatParameterSub(data_from_stm300.data[0X0D],data_from_stm300.data[0X0E]);

			break;
                    case  ONLINE_APP_CHOOSE://用来解决重启时 APP的选择
				temp = data_from_stm300.data[13];
				/*if(temp=='A')
				fdata.unchar[3]='A';
				else if(temp=='B')
				fdata.unchar[3]='B';
				else 
				fdata.unchar[3]=0xff;

				vIAP.BUFFflag= fdata.unchar[3];
				for(j=0;j<3;j++)
				fdata.unchar[j]=0;
				//write_flash(FLASH_MESSAGE_ADDR,(char*)&fdata,fdata.slong);*/
				
				switch(temp)
					{
						case (app_base):
								{
									App_info.tem_jump = try_jump_flag;
									App_info.try_to_jump = app_base_addr;				
									break;	
								}
						
						case (app_1):
								{
									App_info.tem_jump = try_jump_flag;
									App_info.try_to_jump = app_1_addr;	
									break;	
								}
						
						case (app_2):
								{
									App_info.tem_jump = try_jump_flag;
									App_info.try_to_jump = app_2_addr;	
									break;	
								}
						
						case (app_3):
								{
									App_info.tem_jump = try_jump_flag;
									App_info.try_to_jump = app_3_addr;	
									break;	
								}
						
					}
				write_app_info();
				NVIC_SystemReset();
				break;
					
        	     	case ACKCMD:                                                 //GateWay  应答信号
                             vGateway.bSendTCM300IDToGateway=0;
                             vGateway.bSendDataToGateway=0;
                             vGateway.vSendDataToGatewayTime=0X00;
        	     	     break;
						 
        	     	 case ONLINE_READSOFTVER:
        	     	            UploadVersionToGatewaySub();
        	     	            break;
								
        	     	 case ONLINE_RESET:
        	     	           ControllerAckGatewaySub();
        	     	           //   while(1);//让系统重启
        	     	            break;
							   
        	     	 case ONLINE_OPEN: 					 
			 		ControllerAckGatewaySub();			 		 
			 		break;
					
        	       case ONLINE_CLOSE:
			 		 ControllerAckGatewaySub();
			 		break;
					
			default:
        	     	     break;
        	}
        }
}


//***************************************************************************************************************/


//********************************************************************************************************/
void SetRandomTimeSub(void)                                           ///取一随即时间
{
  //unsigned char i;
	//i=10;
	
}

//********************************************************************************************************/
void TCM300ID2DeviceIDSub(void)                                       ///表示控制器定时上传通道状态
{   

		vGateway.vDeviceORG=0X50;
		vGateway.vDevice_ID[3]=id.TCM300_ID[0];
		vGateway.vDevice_ID[2]=id.TCM300_ID[1];
		vGateway.vDevice_ID[1]=id.TCM300_ID[2];
		vGateway.vDevice_ID[0]=id.TCM300_ID[3];
}


// 处理大数据  zxw 
// 2016-1-21
void DealBigdata(void)
 {
	unsigned char  vMainCommand;
	unsigned char  vSubCommand;
	unsigned int  RxLength,temp;

	vIAP.owner = OWNER_IAP_BUFFER;
	
	//	union   UNION_FLOAT_LONG_CHAR  fdata;
         
	RxLength =data_from_stm300.vBigRxMax - 1;
	
        vMainCommand=(data_from_stm300.data[0X08]&0X0F);
        vSubCommand=data_from_stm300.data[0X09];//本数据也算在数据区内
        
        if(vMainCommand!=cMainCommand)
          {    
               return;     
               }
        if((id.TCM300_ID[0X00]!=data_from_stm300.data[10+RxLength])&&(data_from_stm300.data[10+RxLength]!=0xff))
          {       return;                                       }
        if((id.TCM300_ID[0X01]!=data_from_stm300.data[11+RxLength])&&(data_from_stm300.data[11+RxLength]!=0xff))
           {       return;                                       }
        if((id.TCM300_ID[0X02]!=data_from_stm300.data[12+RxLength])&&(data_from_stm300.data[12+RxLength]!=0xff))
          {       return;                                        }
        if((id.TCM300_ID[0X03]!=data_from_stm300.data[13+RxLength])&&(data_from_stm300.data[13+RxLength]!=0xff))
           {       return;                                     }
                
        	switch(vSubCommand)
        	{        	     
        	     	 case IAP_BEGIN: 
				vIAP.Frame_All_Number=(unsigned short)(data_from_stm300.data[10]<<8)+data_from_stm300.data[11];  
				vIAP.Frame_count=1;
				break;
				
			case IAP_END:
				
				/*vIAP.Frame_count=1;
				if(vIAP.BUFFflag=='B')       	     				 
				fdata.unchar[3]=vIAP.BUFFflag='A';
				else 
				fdata.unchar[3]=vIAP.BUFFflag='B';
				for(j=0;j<3;j++)
				fdata.unchar[j]=0;
				//write_flash(FLASH_MESSAGE_ADDR,(char*)&fdata,fdata.slong);
				// 重启代码*/
				//test_jump();

				App_info.tem_jump = try_jump_flag;
				App_info.try_to_jump = WRITE_TO_ADDR;
				write_app_info();
				NVIC_SystemReset();
				
				break;
        	     	  			
			case 0xf0: //   暂时不支持
				temp = data_from_stm300.data[10];// 对应按键值的红外码
				vIAP.Frame_Number=(unsigned short)(temp<<8)+data_from_stm300.data[11];   
				RxLength =data_from_stm300.data[12];
				//for(i=0;i<RxLength;i++)
				//vIAP.Frame_buffer[i]=data_from_stm300.data[13+i]; 
				if(data_from_stm300.data[13+RxLength]!=0xff)// 不是广播模式,要回复正确与否，广播模式不回复
				{
					if(vIAP.Frame_Number == vIAP.Frame_count)
						{
							ControllerAckBigDataSub(0);
							vIAP.Frame_count++;
						}
					else 
						{
							ControllerAckBigDataSub(vIAP.Frame_count);
							return;
						}			
				}	
				//IAP_Flash_write(RxLength);
				write_iap_buff(RxLength);
				break;         	     	    
				default:
				break;
        	}
		 
}

 //应答大数据
void ControllerAckBigDataSub(unsigned int x)
{ 
	unsigned char k,max;	
	
	max=2;
	g_sendBuf[0]=0X55;
	g_sendBuf[1]=0X00;
	g_sendBuf[2]=max+5;// datalength+4 +1byte comd
	g_sendBuf[3]=0X0A;// 10 bytes 
	g_sendBuf[4]=0X07; // data type	
	
	SetCRC8Sub(&g_sendBuf[1],4);

	g_sendBuf[6]=0X02;
	g_sendBuf[7]=0X0A;

	g_sendBuf[8]=0X06;
	g_sendBuf[9]=0X8A;

	if(x==0)
	g_sendBuf[10]=IAP_OK;
	else 
	g_sendBuf[10]=IAP_FAIL;	 
		 	
	g_sendBuf [11 ]=(x>>8)&0xff  ;//data[i];//vUart2Rx.vUart2RxData[8+k];
	g_sendBuf [12 ]=x&0xff  ;//data[i];//vUart2Rx.vUart2RxData[8+k];
	k=2;  
	g_sendBuf[11+k]=vGateway.vDevice_ID[3];// 目标ID0xff;//
	g_sendBuf[12+k]=vGateway.vDevice_ID[2];//0xff;//
	g_sendBuf[13+k]=vGateway.vDevice_ID[1];//0xff;//
	g_sendBuf[14+k]=vGateway.vDevice_ID[0];//0xff;//
	
	g_sendBuf[15+k]=id.TCM300_ID[0];// 发送ID0x01;//
	g_sendBuf[16+k]=id.TCM300_ID[1];//0x9d;//
	g_sendBuf[17+k]=id.TCM300_ID[2];//0xff;//0x2f;//
	g_sendBuf[18+k]=id.TCM300_ID[3];//0xff;//0xb5;//
	
	g_sendBuf[19+k]=0X29;
	g_sendBuf[20+k]=0X00;
	SetCRC8Sub(&g_sendBuf[6],15+max);	
       EnoceanSend(22+max);
       }

void EnoceanSend(unsigned int length)
{
	g_EnoceanDMA_Sendoverflag = 1;
	if (g_EnoceanDMA_Sendoverflag)
	        {
	          g_EnoceanDMA_Sendoverflag =0;            
	           //DMA_Config(length );        
	          HAL_UART_Transmit(&huart2,g_sendBuf,length,80);
	         }
}



/*void IAP_Flash_write(unsigned char Len)
{
    unsigned char i,l;
    unsigned short k,m,n;
    n=vIAP.Frame_Number-1;
   // j=n/PAGE_MAX_FRAME ;
    k=n%PAGE_MAX_FRAME  ; // 上位机是从1开始的s
    l=PAGE_MAX_FRAME-1;
    m= k*FRAME_MAX_NUMBER;

	if (k==l)
		{
			for(i=0;i<Len;i++)
			vIAP.Page_buffer[m+i]=vIAP.Frame_buffer[i];

			//  IWDG_ReloadCounter();
			//if(vIAP.BUFFflag=='B')// 1 为A 块存储区， 0 为B块存储区     
			//write_flash(FLASH_START_IAP_BUFFA_ADDR+j*FLASH_PAGE_SIZE,  vIAP.Page_buffer ,FLASH_PAGE_SIZE);    
			//else         
			//write_flash(FLASH_START_IAP_BUFFB_ADDR+j*FLASH_PAGE_SIZE,  vIAP.Page_buffer ,FLASH_PAGE_SIZE);
		}
	else 
		{
			if(vIAP.Frame_Number<vIAP.Frame_All_Number)
				{
					for(i=0;i<Len;i++)
					vIAP.Page_buffer[m+i]=vIAP.Frame_buffer[i];
				}
			else 
				{
				//IWDG_ReloadCounter();
					if(Len<FRAME_MAX_NUMBER) //小于每包的最大数，意味着该包是最后 一包，所以接收完也要写入flash
						{   	 	   	  
							for(i=0;i<Len;i++)
							vIAP.Page_buffer[m+i]=vIAP.Frame_buffer[i];
							n=m+i;
							for(k=n;k<FLASH_PAGE_SIZE;k++)
							vIAP.Page_buffer[k]=0xff;
						}
					else 
						{
							for(i=0;i<Len;i++)
							vIAP.Page_buffer[m+i]=vIAP.Frame_buffer[i];
						}
					if(vIAP.BUFFflag=='B')// 1 为A 块存储区， 0 为B块存储区
						{
							//write_flash(FLASH_START_IAP_BUFFA_ADDR+j*FLASH_PAGE_SIZE,  vIAP.Page_buffer ,FLASH_PAGE_SIZE);     				
						}        	     
					else 
						{   		 		  	
							//write_flash(FLASH_START_IAP_BUFFB_ADDR+j*FLASH_PAGE_SIZE,  vIAP.Page_buffer ,FLASH_PAGE_SIZE);     			
						}

				}
		}
   
  //SCB->VTOR = NVIC_VectTab | (Offset & (unsigned int)0x1FFFFF80);
}*/

void reset_iap_buff(void)
{
	unsigned short loop;
	for(loop = 0;loop<FLASH_PAGE_SIZE;loop++)
		{
			vIAP.Page_buffer[loop] = 0XFF;
		}
}

void write_iap_buff(unsigned int length)
{
	unsigned int page_num;
	unsigned int packet_num;
//	unsigned int max_page_num;
	unsigned int loop;
	page_num = (vIAP.Frame_Number-1) /PAGE_MAX_FRAME;
	packet_num =  (vIAP.Frame_Number-1)%PAGE_MAX_FRAME;
	if((packet_num == 0XF)||(vIAP.Frame_Number == vIAP.Frame_All_Number))
		{
			for(loop = 0;loop<length;loop++)
				{
					vIAP.Page_buffer[packet_num*FRAME_MAX_NUMBER+loop] = data_from_stm300.data[13+loop]; 
				}			
				///////////////////recv a packet and it's time to write to flash
				write_flash((WRITE_TO_ADDR+page_num*FLASH_PAGE_SIZE), (char *)vIAP.Page_buffer, FLASH_PAGE_SIZE);
				reset_iap_buff();	
				vIAP.owner = OWNER_ON_ONE;
		}
	else
		{
			for(loop = 0;loop<length;loop++)
				{
					vIAP.Page_buffer[packet_num*FRAME_MAX_NUMBER+loop] = data_from_stm300.data[13+loop]; 
				}			
		}
}

void try_to_jump_to_next_app(void)
{
	try_to_jump(NEXT_APP);
}
	

void try_to_jump(unsigned char app)
{
	switch(app)
		{
			case app_base:
			App_info.try_to_jump = app_base_addr;
			App_info.app_offset_table[0] = 0;
			break;

			case app_1:
			App_info.try_to_jump = app_1_addr;	
			App_info.app_offset_table[1] = 0;
			break;

			case app_2:
			App_info.try_to_jump = app_2_addr;	
			App_info.app_offset_table[2] = 0;
			break;

			case app_3:
			App_info.try_to_jump = app_3_addr;
			App_info.app_offset_table[3] = 0;
			break;
			
			default :
			App_info.try_to_jump = app_base_addr;
			App_info.app_offset_table[0] = 0;
			break;
		}
	App_info.tem_jump = try_jump_flag;
	write_app_info();
	NVIC_SystemReset();
}

void read_app_info(void)
{
	read_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
}

void write_app_info(void)
{
	write_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
}

void choose_app(void)
{
	if((App_info.tem_jump == try_jump_flag)||(App_info.tem_jump == 0XFF))
		{
			App_info.tem_jump = no_jump;
			if(App_info.try_to_jump == THIS_APP_ADDR)
				{
					App_info.jump_to_app = App_info.try_to_jump;
					App_info.try_to_jump = 0X01234567;
					write_app_info();
					return;
				}
			else
				{					
					write_app_info();
					/////jump to try_to_jump app
					jump_to_app(App_info.try_to_jump);
				}
		}
	else
		{
			if((App_info.try_to_jump == app_base_addr)||(App_info.try_to_jump == app_1_addr)||(App_info.try_to_jump == app_2_addr)||(App_info.try_to_jump == app_3_addr))
				{
					if(App_info.try_to_jump == THIS_APP_ADDR)   //跳转成功
						{
							App_info.jump_to_app = App_info.try_to_jump;
							App_info.try_to_jump = 0X01234567;
							write_app_info();
							return;
						}
					else		//跳转失败
						{
							App_info.try_to_jump = 0X01234567;
							write_app_info();
							jump_to_app(App_info.jump_to_app);
						}
				}
			if(App_info.jump_to_app == THIS_APP_ADDR)
				{
					if(App_info.jump_to_app == app_base_addr)
						{
							if(App_info.app_offset_table[0] != App_info.jump_to_app )
								{
									App_info.app_offset_table[0] = App_info.jump_to_app;
									write_app_info();
								}
						}
					if(App_info.jump_to_app == app_1_addr)
						{
							if(App_info.app_offset_table[1] != App_info.jump_to_app )
								{
									App_info.app_offset_table[1] = App_info.jump_to_app;
									write_app_info();
								}
						}
					if(App_info.jump_to_app == app_2_addr)
						{
							if(App_info.app_offset_table[2] != App_info.jump_to_app )
								{
									App_info.app_offset_table[2] = App_info.jump_to_app;
									write_app_info();
								}
						}
					if(App_info.jump_to_app == app_3_addr)
						{
							if(App_info.app_offset_table[3] != App_info.jump_to_app )
								{
									App_info.app_offset_table[3] = App_info.jump_to_app;
									write_app_info();
								}
						}
					
					return;
				}
			if((App_info.jump_to_app != app_base_addr)&&(App_info.jump_to_app != app_1_addr)&&(App_info.jump_to_app != app_2_addr)&&(App_info.jump_to_app != app_3_addr)) //修改跳转bug

				{	
					App_info.jump_to_app = THIS_APP_ADDR;
					write_app_info();
					return;
				}
			jump_to_app(App_info.jump_to_app);
		}
	return;
}



void init_save_data(void)
{
	reset_iap_buff();
	read_app_info();
	if(App_info.data_effect != 0X88)
		{	
			App_info.data_effect = 0X88;
			App_info.tem_jump= no_jump;
			App_info.jump_to_app = THIS_APP_ADDR;
			
			App_info.app_offset_table[0] = 0;
			App_info.app_offset_table[1] = 0;
			App_info.app_offset_table[2] = 0;
			App_info.app_offset_table[3] = 0;
			App_info.cur_app_offset= App_info.app_offset_table[App_info.app_point] ;
			write_app_info();
		}
	
}


void wifi_iap_upload_soft_ver(void)
{
	unsigned char soft_ver[2] = {0};
	soft_ver[1] = DEVICE_SOFT_VER & 0XFF;
	soft_ver[0] = (DEVICE_SOFT_VER >> 8) & 0XFF;
	upload_data_by_wifi(0XF3,soft_ver,2);
}


void wifi_iap_cmd_operat(P_S_Wifi_Iap_Cmd cmd_parameter)
{
	upload_data_by_wifi(0XF1,(unsigned char *)cmd_parameter,6);
}

void wifi_iap_request_page(unsigned int page)
{
	unsigned char page_num[2] = {0};
	page_num[1] = page&0X000000FF;
	page_num[0] = (page>>8)&0X000000FF;
	upload_data_by_wifi(0XF0,page_num,2);
}

void wifi_iap_send_ready_code(void)
{
	S_Wifi_Iap_Cmd send_cmd_parameter = {0};
	send_cmd_parameter.cmd = WIFI_IAP_OPERAT_START_BACK;
	send_cmd_parameter.parameter_1 = THIS_APP >> 4;
	wifi_iap_cmd_operat(&send_cmd_parameter);
}

void wifi_iap_send_end_code(void)
{
	S_Wifi_Iap_Cmd send_cmd_parameter = {0};
	send_cmd_parameter.cmd = WIFI_IAP_OPERAT_END;
	send_cmd_parameter.parameter_1 = THIS_APP >> 4;
	wifi_iap_cmd_operat(&send_cmd_parameter);
}

void wifi_iap_operater(void)
{
	//unsigned char operat_type;
	
	if(data_from_wifi.data[9]==0XF1) //iap_cmd
		{
			switch(data_from_wifi.data[10])
				{
					case WIFI_IAP_OPERAT_START:
						wifi_iap_send_ready_code();
						break;
						
					case WIFI_IAP_OPERAT_BEGIN:
						wifi_iap_info.total_size = 0;
						wifi_iap_info.recved_page_sum = 0;
						wifi_iap_info.request_page = 0;
						wifi_iap_info.block = 0;
						wifi_iap_info.page = 0;
						
						wifi_iap_info.total_size |= data_from_wifi.data[11];
						wifi_iap_info.total_size <<= 8;
						wifi_iap_info.total_size |= data_from_wifi.data[12];
						
						wifi_iap_request_page(0);
						break;

					case WIFI_IAP_OPERAT_TRANSMISSION:
						break;

					case WIFI_IAP_OPERAT_END:
						break;
				}

		}

	if(data_from_wifi.data[9]==0XF0)//transfer
		{
			wifi_iap_write_to_flash();
		}
	
	if(data_from_wifi.data[9]==0XF3)//SOFT_VER
		{
			if(data_from_wifi.data[7]==1)
				{
					wifi_iap_upload_soft_ver();
				}
		}

}


void wifi_iap_write_to_flash(void)
{
	vIAP.owner = OWNER_WIFI_IAP_BUFFER;

	wifi_iap_info.recved_page_sum++;
	wifi_iap_info.recved_page =0;
	wifi_iap_info.recved_page |= data_from_wifi.data[10];
	wifi_iap_info.recved_page <<= 8;
	wifi_iap_info.recved_page |= data_from_wifi.data[11];
	wifi_iap_info.request_page = wifi_iap_info.recved_page+1;
	wifi_iap_info.cur_page_size = data_from_wifi.point - 13;

	wifi_iap_info.page = wifi_iap_info.recved_page % 16;
	wifi_iap_info.block = wifi_iap_info.recved_page /16;

	wifi_iap_copy_to_flash();
	
	if(wifi_iap_info.recved_page == wifi_iap_info.total_size- 1)
		{
			if(wifi_iap_info.page)
				{
					//wifi_iap_info.block ++;
					memcopy((char*)&data_from_wifi.data[12],(char*)&vIAP.Page_buffer[wifi_iap_info.page *128],(wifi_iap_info.cur_page_size));
					write_flash(WRITE_TO_ADDR+(wifi_iap_info.block * FLASH_PAGE_SIZE),(char*)vIAP.Page_buffer,FLASH_PAGE_SIZE);
					reset_iap_buff();
				}
			
			vIAP.owner = OWNER_ON_ONE;
			wifi_iap_send_end_code();
		}
	else
		{
			wifi_iap_request_page(wifi_iap_info.request_page);
		}
}

void wifi_iap_copy_to_flash(void)
{	
	if(wifi_iap_info.recved_page < wifi_iap_info.total_size -1)
		{
			if(wifi_iap_info.cur_page_size < 128)
				{
					wifi_iap_info.request_page = wifi_iap_info.recved_page;
					return;
				}
		}
	memcopy((char*)&data_from_wifi.data[12],(char*)&vIAP.Page_buffer[wifi_iap_info.page *128],(wifi_iap_info.cur_page_size));
	if(wifi_iap_info.page == 15)
		{
			write_flash(WRITE_TO_ADDR+(wifi_iap_info.block * FLASH_PAGE_SIZE),(char*)vIAP.Page_buffer,FLASH_PAGE_SIZE);			
			reset_iap_buff();	
		}
	else
		{
			//memcopy((char*)&data_from_wifi.data[12],(char*)&vIAP.Page_buffer[wifi_iap_info.page *128],(wifi_iap_info.cur_page_size));
		}
	
}



