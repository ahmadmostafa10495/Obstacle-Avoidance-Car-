/*
 * PWM.c
 *
 * Created: 12/23/2019 1:52:12 PM
 *  Author: ahmad
 */ 


/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "PWM.h"
#include "registers.h"
#include "DIO.h"


/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/

#define NO_OF_CH								(4)

#define T0_PWM_PHASE_AND_FREQ_CORRECT			(0x40)
#define T0_CLEAR_ON_OC							(0x20)

#define T1_PWM_PHASE_AND_FREQ_CORRECT			(0x0010)
#define T1A_CLEAR_ON_OC							(0x8000)
#define T1B_CLEAR_ON_OC							(0x2000)

#define T2_PWM_PHASE_AND_FREQ_CORRECT			(0x40)
#define T2_CLEAR_ON_OC							(0x20)

#define PWM0_GPIO						GPIOB
#define PWM0_BIT						BIT3

#define PWM1A_GPIO						GPIOD
#define PWM1A_BIT						BIT5

#define PWM1B_GPIO						GPIOD
#define PWM1B_BIT						BIT4

#define PWM2_GPIO						GPIOD
#define PWM2_BIT						BIT7

#define TCNT_MAX                        (255)

#define DOUBLE_EDGE_FREQ                (2)

#define ONE_HUNDRED_PERCENT             (100)   

#define PRESCALER_BITS                  (0x07)

#define SHIFT_ONE						(0x01)
#define SHIFT_TWO						(0x02)
#define SHIFT_THREE						(0x03)
#define SHIFT_FOUR						(0x04)
#define SHIFT_FIVE						(0x05)
#define SHIFT_SIX						(0x06)
#define SHIFT_SEVEN						(0x07)

#define UNUSED							(0x00)

#define VALUE_1							(1)
#define VALUE_8							(8)
#define VALUE_32						(32)
#define VALUE_64						(64)
#define VALUE_128						(128)
#define VALUE_256						(256)
#define VALUE_1024						(1024)




/******************************************************************************************
*                                                                                         *
*                               STATIC VARIABLES                                          *
*																						  *
*																						  *
*******************************************************************************************/

static uint8 gsau8_prescalerMask[NO_OF_CH] = {ZERO_INITIALIZATION, ZERO_INITIALIZATION, ZERO_INITIALIZATION, ZERO_INITIALIZATION};
static uint16 gsau16_prescalerValue[NO_OF_CH] = {ZERO_INITIALIZATION, ZERO_INITIALIZATION, ZERO_INITIALIZATION, ZERO_INITIALIZATION};
static uint8 gsau8_Pwm0PrescalerTableMask[]={SHIFT_ONE, SHIFT_TWO, UNUSED, SHIFT_THREE, UNUSED, SHIFT_FOUR, SHIFT_FIVE};
static uint8 gsau8_Pwm1PrescalerTableMask[]={SHIFT_ONE, SHIFT_TWO, UNUSED, SHIFT_THREE, UNUSED, SHIFT_FOUR, SHIFT_FIVE};
static uint8 gsau8_Pwm2PrescalerTableMask[]={SHIFT_ONE, SHIFT_TWO, SHIFT_THREE, SHIFT_FOUR, SHIFT_FIVE, SHIFT_SIX, SHIFT_SEVEN};
static uint16 gsau16_Pwm0PrescalerTableValue[]={VALUE_1, VALUE_8, UNUSED, VALUE_64, UNUSED, VALUE_256, VALUE_1024};
static uint16 gsau16_Pwm1PrescalerTableValue[]={VALUE_1, VALUE_8, UNUSED, VALUE_64, UNUSED, VALUE_256, VALUE_1024};
static uint16 gsau16_Pwm2PrescalerTableValue[]={VALUE_1, VALUE_8, VALUE_32, VALUE_64, VALUE_128, VALUE_256, VALUE_1024};

/******************************************************************************************
*                                                                                         *
*                                 IMPLEMENTATION                                          *
*																						  *
*																						  *
*******************************************************************************************/



/*********************************************************************************/
/* Function: Error_State Pwm_Init(Pwm_Cfg_s_t *Pwm_Cfg);                         */
/* Type: public                                                                  */
/* Input parameters: Pwm_Cfg Structure (channel number, Prescaler)               */
/* Return type : void                                                            */
/*                                                                               */
/* Description: initialize the PWM configuration                                 */
/*********************************************************************************/
ERROR_STATUS 
Pwm_Init(Pwm_Cfg_s *Pwm_Cfg)
{
	DIO_Cfg_s DIO_info;
	if (NULL == Pwm_Cfg)
	{
		return E_NOK;
	}
	else if (Pwm_Cfg->Prescaler < PWM_PRESCALER_NO)
	{
		return E_NOK;
	}
	else if (Pwm_Cfg->Prescaler > PWM_PRESCALER_1024)
	{
		return E_NOK;
	}
	else if ((gsau8_Pwm0PrescalerTableMask[Pwm_Cfg->Prescaler] == UNUSED) && (Pwm_Cfg->Channel != PWM_CH2))
	{
		return E_NOK;
	}
	else
	{
		switch(Pwm_Cfg->Channel)
		{
			case PWM_CH0:
				TCCR0 = ZERO_INITIALIZATION;
				gsau8_prescalerMask[PWM_CH0] = gsau8_Pwm0PrescalerTableMask[Pwm_Cfg->Prescaler];
				gsau16_prescalerValue[PWM_CH0] = gsau16_Pwm0PrescalerTableValue[Pwm_Cfg->Prescaler];
				TCCR0 |= T0_PWM_PHASE_AND_FREQ_CORRECT | T0_CLEAR_ON_OC;
				DIO_info.GPIO = PWM0_GPIO;
				DIO_info.pins = PWM0_BIT;
				DIO_info.dir = OUTPUT;
				DIO_init(&DIO_info);
				break;
			case PWM_CH1A:
				gsau8_prescalerMask[PWM_CH1A] = gsau8_Pwm1PrescalerTableMask[Pwm_Cfg->Prescaler];
				gsau16_prescalerValue[PWM_CH1A] = gsau16_Pwm1PrescalerTableValue[Pwm_Cfg->Prescaler];
				TCCR1 |= T1_PWM_PHASE_AND_FREQ_CORRECT | T1A_CLEAR_ON_OC;
				DIO_info.GPIO = PWM1A_GPIO;
				DIO_info.pins = PWM1A_BIT;
				DIO_info.dir = OUTPUT;
				DIO_init(&DIO_info);
				break;
			case PWM_CH1B:
				gsau8_prescalerMask[PWM_CH1B] = gsau8_Pwm1PrescalerTableMask[Pwm_Cfg->Prescaler];
				gsau16_prescalerValue[PWM_CH1B] = gsau16_Pwm1PrescalerTableValue[Pwm_Cfg->Prescaler];
				TCCR1 |= T1_PWM_PHASE_AND_FREQ_CORRECT | T1B_CLEAR_ON_OC;
				DIO_info.GPIO = PWM1B_GPIO;
				DIO_info.pins = PWM1B_BIT;
				DIO_info.dir = OUTPUT;
				DIO_init(&DIO_info);
				break;
			case PWM_CH2:
				TCCR2 = ZERO_INITIALIZATION;
				gsau8_prescalerMask[PWM_CH2] = gsau8_Pwm2PrescalerTableMask[Pwm_Cfg->Prescaler];
				gsau16_Pwm2PrescalerTableValue[PWM_CH2] = gsau16_Pwm2PrescalerTableValue[Pwm_Cfg->Prescaler];
				TCCR2 |= T2_PWM_PHASE_AND_FREQ_CORRECT | T2_CLEAR_ON_OC;
				DIO_info.GPIO = PWM2_GPIO;
				DIO_info.pins = PWM2_BIT;
				DIO_info.dir = OUTPUT;
				DIO_init(&DIO_info);
				break;
		}
	}
	return E_OK;
}


/*********************************************************************************/
/* Function: Error_State Pwm_Start(uint8_t channel,uint8_t Duty);                */
/* Type: public                                                                  */
/* Input parameters: Channel ID, Duty cycle(0:100)                               */
/* Return type : Error State                                                     */
/*                                                                               */
/* Description: starts the PWM on the dedicated channel with the required duty   */
/*				cycle and frequency                                              */
/*-------------------------------------------------------                        */
/*(IN timer 0) there are only 5 available Frequencies:                           */
/*-------------------------------------------------------                        */
/*Frequency (NO-Prescaler) --> 31,372HZ                                          */
/*Frequency (Prescaler 8) --> 3,921HZ                                            */
/*Frequency (Prescaler 64) --> 490HZ                                             */
/*Frequency (Prescaler 256) --> 122HZ                                            */
/*Frequency (Prescaler 1024) --> 31HZ                                            */
/*--------------------------------------------------------                       */
/*(IN timer 2) there are only 7 available Frequencies:                           */
/*-------------------------------------------------------                        */
/*Frequency (NO-Prescaler) --> 31,372HZ                                          */
/*Frequency (Prescaler 8) --> 3,921HZ                                            */
/*Frequency (Prescaler 32) --> 980HZ                                             */
/*Frequency (Prescaler 64) --> 490HZ                                             */
/*Frequency (Prescaler 128) --> 245HZ                                            */
/*Frequency (Prescaler 256) --> 122HZ                                            */
/*Frequency (Prescaler 1024) --> 31HZ                                            */
/*-------------------------------------------------------                        */
/*((all this calculations are for phase correct mode))                           */
/*all this calculation for F_CPU 16MHz                                           */
/*********************************************************************************/
ERROR_STATUS 
Pwm_Start(uint8 Channel,uint8 Duty,uint32 Frequncy)
{
	switch(Channel)
	{
		case PWM_CH0:
			OCR0 = TCNT_MAX * Duty / ONE_HUNDRED_PERCENT;
			TCCR0 |= gsau8_prescalerMask[PWM_CH0];
			break;
		case PWM_CH1A:
			ICR1 = (F_CPU)/(gsau16_prescalerValue[PWM_CH1A] * Frequncy * DOUBLE_EDGE_FREQ);
			OCR1A= (Duty * ((F_CPU)/(gsau16_prescalerValue[PWM_CH1A] * Frequncy * DOUBLE_EDGE_FREQ))) / ONE_HUNDRED_PERCENT;
			TCCR1 |= gsau8_prescalerMask[PWM_CH1A];
			break;		
		case PWM_CH1B:
			ICR1 = (F_CPU)/(gsau16_prescalerValue[PWM_CH1A] * Frequncy * DOUBLE_EDGE_FREQ);
			OCR1B= (Duty * ((F_CPU)/(gsau16_prescalerValue[PWM_CH1B] * Frequncy * DOUBLE_EDGE_FREQ))) / ONE_HUNDRED_PERCENT;
			TCCR1 |= gsau8_prescalerMask[PWM_CH1B];
			break;
		case PWM_CH2:
			OCR0 = TCNT_MAX * Duty / ONE_HUNDRED_PERCENT;
			TCCR2 |= gsau8_prescalerMask[PWM_CH2];
			break;
		default:
		return E_NOK;
	}
	return E_OK;
}


/*********************************************************************************/
/* Function: Error_State Pwm_Start(uint8_t channel,uint8_t Duty);                */
/* Type: public                                                                  */
/* Input parameters: Channel ID, Duty cycle(0:100)                               */
/* Return type : Error State                                                     */
/*                                                                               */
/* Description: updates the duty cycle and frequency of the dedicated channel    */
/*********************************************************************************/
ERROR_STATUS 
Pwm_Update(uint8 Channel,uint8 Duty,uint32 Frequncy)
{
	switch(Channel)
	{
		case PWM_CH0:
		OCR0 = TCNT_MAX * Duty / ONE_HUNDRED_PERCENT;
		break;
		case PWM_CH1A:
		ICR1 = (F_CPU)/(gsau16_prescalerValue[PWM_CH1A] * Frequncy * DOUBLE_EDGE_FREQ);
		OCR1A= (Duty * ((F_CPU)/(gsau16_prescalerValue[PWM_CH1A] * Frequncy * DOUBLE_EDGE_FREQ))) / ONE_HUNDRED_PERCENT;
		break;
		case PWM_CH1B:
		ICR1 = (F_CPU)/(gsau16_prescalerValue[PWM_CH1A] * Frequncy * DOUBLE_EDGE_FREQ);
		OCR1B= (Duty * ((F_CPU)/(gsau16_prescalerValue[PWM_CH1B] * Frequncy * DOUBLE_EDGE_FREQ))) / ONE_HUNDRED_PERCENT;
		break;
		case PWM_CH2:
		OCR0 = TCNT_MAX * Duty / ONE_HUNDRED_PERCENT;
		break;
		default:
		return E_NOK;
	}
	return E_OK;
}


/*********************************************************************************/
/* Function: Error_State Pwm_Stop(uint8_t channel);                              */
/* Type: public                                                                  */
/* Input parameters: channel ID                                                  */
/* Return type : Error state                                                     */
/*                                                                               */
/* Description: responsible of Stopping the PWM by clearing the prescaler		 */
/*				of the corresponding channel                                     */
/*********************************************************************************/
ERROR_STATUS 
Pwm_Stop(uint8 Channel)
{
	switch(Channel)
	{
		case PWM_CH0:
		OCR0 = ZERO_INITIALIZATION;
		TCCR0 &= ~ PRESCALER_BITS;
		break;
		case PWM_CH1A:
		ICR1 = ZERO_INITIALIZATION;
		OCR1A = ZERO_INITIALIZATION;
		TCCR1 &= ~ PRESCALER_BITS;
		break;
		case PWM_CH1B:
		ICR1 = ZERO_INITIALIZATION;
		OCR1B= ZERO_INITIALIZATION;
		TCCR1 &= ~ PRESCALER_BITS;
		break;
		case PWM_CH2:
		OCR0 = ZERO_INITIALIZATION;
		TCCR2 &= ~ PRESCALER_BITS;
		break;
		default:
		return E_NOK;
	}
	return E_OK;	
}