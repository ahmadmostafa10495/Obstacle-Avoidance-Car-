/*
 * ICU.c
 *
 * Created: 12/24/2019 3:44:21 PM
 *  Author: ahmad
 */ 

/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "ICU.h"
#include "DIO.h"
#include "Timer.h"
#include "interrupt.h"
#include "ExternalInterrupt.h"
#include "std_types.h"


/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/

#define INT2_EDGE_FLAG_RISING							   (0)
#define INT2_EDGE_FLAG_FALLING							   (1)

#define ICU_CH_NO										   (3)
#define TIMER_CH_NO										   (3) 

#define TCNT2_MAX										   (255U)
#define CONVERSION_CONSTANT_FROM_SECOND_TO_MICROSECOND     (16U)
#define MAX_OVF_COUNT									   (220U)


/******************************************************************************************
*                                                                                         *
*                               STATIC VARIABLES                                          *
*																						  *
*																						  *
*******************************************************************************************/

static uint8 gsu8_INTEdgeFlag[ICU_CH_NO] = {LOW_LEVEL,LOW_LEVEL,FALLING_EDGE};
static uint8 gsu8_TimerCH[ICU_CH_NO] = {TIMER_CH0,TIMER_CH1,TIMER_CH2};
static uint8 gsu8_TimerMode[ICU_CH_NO] = {TIMER_MODE,TIMER_MODE,TIMER_MODE};
static uint8 gsu8_TimerPollingOrInterrupt[ICU_CH_NO] = {TIMER_INTERRUPT_MODE,TIMER_INTERRUPT_MODE,TIMER_INTERRUPT_MODE};
static uint8 gsu8_TimerPrescaler[ICU_CH_NO] = {TIMER_PRESCALER_NO,TIMER_PRESCALER_NO,TIMER_PRESCALER_NO};
volatile static uint16 gsu16_OVFCount[ICU_CH_NO] = {ZERO_INITIALIZATION, ZERO_INITIALIZATION, ZERO_INITIALIZATION};
volatile static uint32 gsu32_Time[TIMER_CH_NO] = {ZERO_INITIALIZATION, ZERO_INITIALIZATION, ZERO_INITIALIZATION};
volatile static uint8 gsu8_INTFinishedFlag[ICU_CH_NO] = {ZERO_INITIALIZATION, ZERO_INITIALIZATION, ZERO_INITIALIZATION};



/******************************************************************************************
*                                                                                         *
*                                 IMPLEMENTATION                                          *
*																						  *
*																						  *
*******************************************************************************************/


void EXT_INT0_CBK(void);
void EXT_INT1_CBK(void);
void EXT_INT2_CBK(void);
void TIMER0_CBK(void);
void TIMER1_CBK(void);
void TIMER2_CBK(void);
/**************************************************************************
 * Function 	: Icu_Init                                                *
 * Input 		: Icu_cfg : pointer to structure of type Icu_cfg_s        *
 * Return 		: value of type ERROR_STATUS							  *
 * 				  which is one of the following values:					  *
 * 				  - E_OK  : initialized successfully					  *
 *				  - E_NOK : not initialized successfully				  *
 * Description  : Initializes the ICU by initializing the timer			  *
 * 				  and enabling the global interrupt						  *
 **************************************************************************/
ERROR_STATUS 
Icu_Init(Icu_cfg_s * Icu_Cfg)
{
	if (NULL == Icu_Cfg)
	{
		return E_NOK;
	} 
	else
	{
		ExternInt_Cfg_s ExtInt_info;
		Timer_cfg_s Timer_info;
		switch(Icu_Cfg->ICU_Ch_No)
		{
			case ICU_CH0:
				ExtInt_info.ExternInt_No = EXTRN_INT_0;
				ExtInt_info.ExternInt_CBF_Ptr = EXT_INT0_CBK;
				ExtInt_info.ExternInt_Event = gsu8_INTEdgeFlag[ICU_CH0];
				ExternInt_Init(&ExtInt_info);
				break;
			case ICU_CH1:
				ExtInt_info.ExternInt_No = EXTRN_INT_1;
				ExtInt_info.ExternInt_CBF_Ptr = EXT_INT1_CBK;
				ExtInt_info.ExternInt_Event = gsu8_INTEdgeFlag[ICU_CH1];
				ExternInt_Init(&ExtInt_info);
				break;
			case ICU_CH2:
				ExtInt_info.ExternInt_No = EXTRN_INT_2;
				ExtInt_info.ExternInt_CBF_Ptr = EXT_INT2_CBK;
				ExtInt_info.ExternInt_Event = gsu8_INTEdgeFlag[ICU_CH2];
				ExternInt_Init(&ExtInt_info);
				break;
			default:
				return E_NOK;	
		}
		switch(Icu_Cfg->ICU_Ch_Timer)
		{
			case ICU_TIMER_CH0:
				gsu8_TimerMode[Icu_Cfg->ICU_Ch_No] = TIMER_CH0;
				Timer_info.Timer_CH_NO = gsu8_TimerCH[ICU_TIMER_CH0];
				Timer_info.Timer_Mode = gsu8_TimerMode[ICU_TIMER_CH0];
				Timer_info.Timer_Polling_Or_Interrupt = gsu8_TimerPollingOrInterrupt[ICU_TIMER_CH0];
				Timer_info.Timer_Prescaler = gsu8_TimerPrescaler[ICU_TIMER_CH0];
				Timer_info.Timer_Cbk_ptr = TIMER0_CBK;
				Timer_Init(&Timer_info);
				break;
			case ICU_TIMER_CH1:
				gsu8_TimerMode[Icu_Cfg->ICU_Ch_No] = TIMER_CH1;
				Timer_info.Timer_CH_NO = gsu8_TimerCH[ICU_TIMER_CH1];
				Timer_info.Timer_Mode = gsu8_TimerMode[ICU_TIMER_CH1];
				Timer_info.Timer_Polling_Or_Interrupt = gsu8_TimerPollingOrInterrupt[ICU_TIMER_CH1];
				Timer_info.Timer_Prescaler = gsu8_TimerPrescaler[ICU_TIMER_CH1];
				Timer_info.Timer_Cbk_ptr = TIMER1_CBK;
				Timer_Init(&Timer_info);
				break;
			case ICU_TIMER_CH2:
				gsu8_TimerMode[Icu_Cfg->ICU_Ch_No] = TIMER_CH2;
				Timer_info.Timer_CH_NO = /*gsu8_TimerCH[ICU_TIMER_CH2]*/TIMER_CH2;
				Timer_info.Timer_Mode = /*gsu8_TimerMode[ICU_TIMER_CH2]*/TIMER_MODE;
				Timer_info.Timer_Polling_Or_Interrupt = /*gsu8_TimerPollingOrInterrupt[ICU_TIMER_CH2]*/TIMER_INTERRUPT_MODE;
				Timer_info.Timer_Prescaler = /*gsu8_TimerPrescaler[ICU_TIMER_CH2]*/TIMER_PRESCALER_NO;
				Timer_info.Timer_Cbk_ptr = TIMER2_CBK;
				Timer_Init(&Timer_info);
				break;
			default:
				return E_NOK;	
		}
	}
	return E_OK;
}




/***************************************************************************
 * Function		: Icu_ReadTime
 * Input		: 
 *				Icu_Channel --> ICU timer channel
 *									- ICU_TIMER_CH0
 *									- ICU_TIMER_CH1
 *									- ICU_TIMER_CH2
 *				Icu_EdgeToEdge -- > edges to calculate pusle time between:
 *									- ICU_RISE_TO_RISE
 *									- ICU_RISE_TO_FALE
 *									- ICU_FALE_TO_RISE					   *
 * Output 		: Icu_Time : pointer to uint32 variable to give the time   *
 * 				  from falling edge to rising edge						   *
 * Return 		: value of type ERROR_STATUS							   *
 * 				  which is one of the following values:					   *
 * 				  - E_OK : successful									   *
 *				  - E_NOK : not successful								   *
 * Description	: calculates the time between 2 edges				       *
 ***************************************************************************/
ERROR_STATUS 
Icu_ReadTime(uint8 Icu_Channel, uint8 Icu_EdgeToEdge, uint32 * Icu_Time)
{
	switch(Icu_Channel)
	{
		case ICU_CH2:
			switch(Icu_EdgeToEdge)
			{
				case ICU_RISE_TO_FALL:
					ExternInt_SetEvent(EXTRN_INT_2, RISING_EDGE);
					gsu8_INTEdgeFlag[EXTRN_INT_2] = RISING_EDGE;	
					ExternInt_Enable(EXTRN_INT_2);
					while(!gsu8_INTFinishedFlag[EXTRN_INT_2]);
					*Icu_Time = gsu32_Time[EXTRN_INT_2];
					gsu8_INTFinishedFlag[EXTRN_INT_2] = ZERO_INITIALIZATION;
					break;
			}
	}
	return E_OK;
}




void 
EXT_INT0_CBK(void)
{
	
}

void 
EXT_INT1_CBK(void)
{
	
}

void 
EXT_INT2_CBK(void)
{
	if (gsu8_INTEdgeFlag[EXTRN_INT_2] == RISING_EDGE)
	{
		gsu16_OVFCount[ICU_TIMER_CH2] = ZERO_INITIALIZATION;
		Timer_Start(gsu8_TimerCH[ICU_CH2],TCNT2_MAX);
		ExternInt_SetEvent(EXTRN_INT_2, FALLING_EDGE);
		gsu8_INTEdgeFlag[EXTRN_INT_2] = FALLING_EDGE;
		#if DEBUGTEST
		DIO_Write(GPIOB, BIT5, HIGH);
		#endif
	} 
	else if(gsu8_INTEdgeFlag[EXTRN_INT_2] == FALLING_EDGE)
	{
		Timer_Stop(gsu8_TimerCH[ICU_CH2]);
		uint16 u16_timerCount = ZERO_INITIALIZATION; 
		Timer_GetValue(gsu8_TimerCH[ICU_CH2], &u16_timerCount);
		if (gsu16_OVFCount[gsu8_TimerCH[ICU_CH2]] > MAX_OVF_COUNT)
		{
			gsu32_Time[gsu8_TimerCH[ICU_CH2]] = (MAX_OVF_COUNT * TCNT2_MAX) / CONVERSION_CONSTANT_FROM_SECOND_TO_MICROSECOND;

		}
		else
		{
			gsu32_Time[gsu8_TimerCH[ICU_CH2]] = (gsu16_OVFCount[gsu8_TimerCH[ICU_CH2]] * TCNT2_MAX + u16_timerCount) / CONVERSION_CONSTANT_FROM_SECOND_TO_MICROSECOND;

		}
		gsu8_INTFinishedFlag[ICU_CH2] = ONE_INITIALIZATION;
		#if DEBUGTEST
		DIO_Write(GPIOB, BIT6, HIGH);
		#endif
	}
	
				
}

void 
TIMER0_CBK(void)
{
	
}

void 
TIMER1_CBK(void)
{
	
}

void 
TIMER2_CBK(void)
{
	gsu16_OVFCount[ICU_TIMER_CH2]++;
	#if DEBUGTEST
	DIO_Write(GPIOB, BIT7, HIGH);
	#endif
	
}