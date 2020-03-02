/*
 * Us.c
 *
 * Created: 12/23/2019 2:52:18 PM
 *  Author: ahmad
 */ 


/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "Us.h"
#include "ICU.h"
#include "DIO.h"


/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/

#define TRIGGER_GPIO								  (GPIOB)
#define TRIGGER_BIT									  (1)

#define ECHO_GPIO									  (GPIOB)
#define ECHO_BIT									  (2)

#define CONVERSION_CONSTANT_FROM_MICROSECOND_TO_CM    (58.2)


/******************************************************************************************
*                                                                                         *
*                               STATIC VARIABLES                                          *
*																						  *
*																						  *
*******************************************************************************************/



/******************************************************************************************
*                                                                                         *
*                                 IMPLEMENTATION                                          *
*																						  *
*																						  *
*******************************************************************************************/










/****************************************************************************************************
 * Function Name   : Us_Init.                                                                       *
 * Input Parameters : None.                                                                         *                                                                   *                                                                                                *
 * Output Parameters: None.                                                                         *
 * In_Out Parameters: None.                                                                         *
 * Return           : ERROR_STATUS.                                                                 *
 * Description      : This function initiate the UltraSonic by set the value of the Dio_Dirction of *
 *                    the pin trigger as an output then initiate the ICU to use it to calculate the *
 *                    On time of the ECHO signal.                                                   *
 *                                                                                                  *
 *                                                                                                  *
 ***************************************************************************************************/
ERROR_STATUS 
Us_Init(void)
{
	DIO_Cfg_s trigger_info = {TRIGGER_GPIO,TRIGGER_BIT,OUTPUT};
	if(DIO_init(&trigger_info) == E_NOK)
	{
		return E_NOK;
	}
	else
	{
		DIO_Cfg_s echo_info = {ECHO_GPIO,ECHO_BIT,INPUT};
		DIO_init(&echo_info);
		Icu_cfg_s ICU_info = {ICU_CH2, ICU_TIMER_CH2};
		Icu_Init(&ICU_info);
	}
	return E_OK;
}



/**************************************************************************************************
 * Function Name    : Us_Trigger.                                                                 *
 * Input Parameters : None.                                                                       *                                                                              *                                                                                                *
 * Output Parameters: None.                                                                       *
 * In_Out Parameters: None.                                                                       *
 * Return           : ERROR_STATUS                                                                *
 * Description      : This function send the  pulse, to trigger pin of the UltraSonic to enhance  *
 *                    the  ECUO signal                                                            *
 *************************************************************************************************/
ERROR_STATUS 
Us_Trigger(void)
{
	volatile int x;
	DIO_Write(TRIGGER_GPIO, TRIGGER_BIT, HIGH);
	for (x=0;x<10;x++);
	DIO_Write(TRIGGER_GPIO, TRIGGER_BIT, LOW);
	return E_OK;
}


/**************************************************************************************************
 * Function Name    : Us_GetDistance.                                                             *
 * Input Parameters : pointer to uint16 that will return the distance on it.                      *                                                                                              *                                                                                                *
 * Output Parameters: Distance.                                                                   *
 * In_Out Parameters: None.                                                                       *
 * Return           : ERROR_STATUS.                                                               *
 * Description      : This function get the time of high signal of ECUO pulse, then               *
 *                    use it to calculate the distance, then return the value in the              *
 *                    Distance parameter.                                                         *
 *************************************************************************************************/
ERROR_STATUS 
Us_GetDistance(uint16 *Distance)
{
	if (NULL == Distance)
	{
		return E_NOK;
	}
	else
	{
		static uint32 u32_signlwidth;
		Icu_ReadTime(ICU_CH2, ICU_RISE_TO_FALL, &u32_signlwidth);
		(*(Distance)) = u32_signlwidth / CONVERSION_CONSTANT_FROM_MICROSECOND_TO_CM;
	}
	return E_OK;
}