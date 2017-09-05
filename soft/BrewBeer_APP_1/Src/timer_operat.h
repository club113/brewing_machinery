#ifndef __TIMER_OPERAT_H__
#define __TIMER_OPERAT_H__
#include "include.h"

#define SOFTTIMER	htim6

#define MAX_COUNTVALUE_NUMBER	16

//MAX_COUNTVALUE_NUMBER  �����Գ���16


typedef void (*P_TimerCounter_Deal_Function)(void*);

typedef struct 
{
	//unsigned char value_length;//��ʱ���ĵĳ���
	unsigned char repeatability;//��ʱ���Ƿ���ظ�
	unsigned char overflow_flag;//�����־λ

	//unsigned int init_value;//��ʱ��ֵ
	unsigned int target_value;//��ʱ��ֵֹ
	unsigned int count_value;//��ʱֵ

	osMessageQId signal_queue;//������ʱ�����֪ͨ�Ķ���
	P_TimerCounter_Deal_Function timer_counter_callback_function;
}S_Value_In_Timer,*P_S_Value_In_Timer;

typedef struct 
{
	unsigned short space;//16λ����������ָʾ��Щ�ռ䱻ռ����Щ�ռ����
	S_Value_In_Timer timer[MAX_COUNTVALUE_NUMBER];
	unsigned int timer_tick;
	unsigned int second_tick;
}S_Timer_Counter,*P_S_Timer_Counter;
extern S_Timer_Counter timer_counter;


unsigned char register_value_into_timer(unsigned int count_value,unsigned char mod);
unsigned char timer_free(unsigned char space);

void register_queue_into_timer(unsigned char space,osMessageQId queue);
void register_callback_function_into_timer(unsigned char space,P_TimerCounter_Deal_Function function);


void InitSoftTimer(void);
void INC_COUNTER_TIMER(void);







unsigned int get_cur_second_tick(void);
unsigned char is_timer_counter_overflow(unsigned char space);
void clear_timer_counter_overflow_flag(unsigned char space);

#endif

