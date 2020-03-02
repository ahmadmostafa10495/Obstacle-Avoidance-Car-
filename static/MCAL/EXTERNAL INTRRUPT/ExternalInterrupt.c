/*
 * ExternalInterrupt.c
 *
 * Created: 12/23/2019 9:40:56 PM
 *  Author: ahmad
 */ 



/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "ExternalInterrupt.h"
#include "registers.h"
#include "interrupt.h"


/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/


#define INT0_GICR						(0x40)
#define INT1_GICR						(0x80)
#define INT2_GICR						(0x20)

#define INT0_EDGE_LOW_LEVEL				(0x00)
#define INT0_EDGE_CHANGE				(0x01)
#define INT0_EDGE_FALLING				(0x02)
#define INT0_EDGE_RISING				(0x03)
#define INT0_EDGE_BITS					(0x03)

#define INT1_EDGE_LOW_LEVEL				(0x00)
#define INT1_EDGE_CHANGE				(0x04)
#define INT1_EDGE_FALLING				(0x08)
#define INT1_EDGE_RISING				(0x0c)
#define INT1_EDGE_BITS					(0x0c)

#define INT2_EDGE_FALLING				(0x00)
#define INT2_EDGE_RISING				(0x40)
#define INT2_EDGE_BITS                  (0x40)

#define NO_OF_EXT_INT                   (3)


/******************************************************************************************
*                                                                                         *
*                               STATIC VARIABLES                                          *
*																						  *
*																						  *
*******************************************************************************************/

void(*EXT_INT_CBK_PTR[NO_OF_EXT_INT])(void) = {NULL,NULL,NULL};

/******************************************************************************************
*                                                                                         *
*                                 IMPLEMENTATION                                          *
*																						  *
*																						  *
*******************************************************************************************/


/*
 * Function: ExternInt_Init 
 * Inputs:	ExternIntCfg -> pointer of type ExternInt_Cfg_s which points to structure that contain the initialized data.
 * Outputs: 
 * In Out: 
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Initiate the required external interrupt configuration as it specify
 *	event of interrupt.
 *	Enable the required external interrupt from GICR but to allow the interrupt global interrupt must be set
 */
ERROR_STATUS 
ExternInt_Init(ExternInt_Cfg_s *ExternIntCfg)
{
	if (NULL == ExternIntCfg)
	{
		return E_NOK;
	} 
	else
	{
		switch (ExternIntCfg->ExternInt_No)
		{
			case EXTRN_INT_0:
				if (NULL != ExternIntCfg->ExternInt_CBF_Ptr)
				{
					EXT_INT_CBK_PTR[EXTRN_INT_0] = ExternIntCfg->ExternInt_CBF_Ptr;
				} 
				else
				{
					
				}
				switch (ExternIntCfg->ExternInt_Event)
				{
					case LOW_LEVEL:
					MCUCR |= INT0_EDGE_LOW_LEVEL;
					break;
					case LOGICAL_CHANGE:
					MCUCR |= INT0_EDGE_CHANGE;
					break;
					case FALLING_EDGE:
					MCUCR |= INT0_EDGE_FALLING;
					break;
					case RISING_EDGE:
					MCUCR |= INT0_EDGE_RISING;
					break;
					default:
					return E_NOK;
				}
				break;
			case EXTRN_INT_1:
				if (NULL != ExternIntCfg->ExternInt_CBF_Ptr)
				{
					EXT_INT_CBK_PTR[EXTRN_INT_1] = ExternIntCfg->ExternInt_CBF_Ptr;
				}
				else
				{
					
				}
				switch (ExternIntCfg->ExternInt_Event)
				{
					case LOW_LEVEL:
					MCUCR |= INT1_EDGE_LOW_LEVEL;
					break;
					case LOGICAL_CHANGE:
					MCUCR |= INT1_EDGE_CHANGE;
					break;
					case FALLING_EDGE:
					MCUCR |= INT1_EDGE_FALLING;
					break;
					case RISING_EDGE:
					MCUCR |= INT1_EDGE_RISING;
					break;
					default:
					return E_NOK;
				}
				break;
			case EXTRN_INT_2:
				if (NULL != ExternIntCfg->ExternInt_CBF_Ptr)
				{
					EXT_INT_CBK_PTR[EXTRN_INT_2] = ExternIntCfg->ExternInt_CBF_Ptr;
				}
				else
				{
					
				}
				switch (ExternIntCfg->ExternInt_Event)
				{
					case FALLING_EDGE:
						MCUCSR |= INT2_EDGE_FALLING;
						break;
					case RISING_EDGE:
						MCUCSR |= INT2_EDGE_RISING;
						break;
					default:
						return E_NOK;
				}
				break;
			default:
				return E_NOK;
		}
	}
	return E_OK;
}

/*
 * Function: ExternInt_SetEvent
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2).
 * 	       InterruptEvent -> indicate required event for INT0 and INT1 there are 4 events to check (RISING_EDGE,FALLING_EDGE,LOW_LEVEL,LOGICAL_CHANGE).
							  But for Interrupt 2 there are only Two cases (Rising,Falling)	
 * Outputs: 
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: function is used to set event of required external interrupt. 
 * 				note: usually used to poll on the flag.
 */
ERROR_STATUS 
ExternInt_SetEvent(uint8 ExternInt_No,uint8 InterruptEvent)
{
	switch (ExternInt_No)
	{
		case EXTRN_INT_0:
			MCUCR &= ~INT0_EDGE_BITS;
			switch (InterruptEvent)
			{
				case LOW_LEVEL:
				MCUCR |= INT0_EDGE_LOW_LEVEL;
				break;
				case LOGICAL_CHANGE:
				MCUCR |= INT0_EDGE_CHANGE;
				break;
				case FALLING_EDGE:
				MCUCR |= INT0_EDGE_FALLING;
				break;
				case RISING_EDGE:
				MCUCR |= INT0_EDGE_RISING;
				break;
				default:
				return E_NOK;
			}
			break;
		case EXTRN_INT_1:
			MCUCR &= ~INT1_EDGE_BITS;
			switch (InterruptEvent)
			{
				case LOW_LEVEL:
				MCUCR |= INT1_EDGE_LOW_LEVEL;
				break;
				case LOGICAL_CHANGE:
				MCUCR |= INT1_EDGE_CHANGE;
				break;
				case FALLING_EDGE:
				MCUCR |= INT1_EDGE_FALLING;
				break;
				case RISING_EDGE:
				MCUCR |= INT1_EDGE_RISING;
				break;
				default:
				return E_NOK;
			}
			break;
		case EXTRN_INT_2:
			MCUCSR &= ~INT2_EDGE_BITS;
			switch (InterruptEvent)
			{
				case FALLING_EDGE:
				MCUCSR |= INT2_EDGE_FALLING;
				break;
				case RISING_EDGE:
				MCUCSR |= INT2_EDGE_RISING;
				break;
				default:
				return E_NOK;
			}
			break;
		default:
			return E_NOK;
	}
	return E_OK;	
}

/*
 * Function: ExternInt_GetStatus
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2).
 * Outputs: *status -> points to the value of selected interrupt flag.
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: function is used to get flag of required interrupt status. 
 * 				note: usually used to poll on the flag.
 */
ERROR_STATUS 
ExternInt_GetStatus(uint8 ExternInt_No,uint8 *Status)
{
	return E_OK;
}

/*
 * Function: ExternInt_Enable
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2).
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Enable the required external interrupt from GICR but to allow the interrupt global interrupt must be set.
 */
ERROR_STATUS 
ExternInt_Enable(uint8 ExternInt_No)
{
	sei();
	switch(ExternInt_No)
	{
		case EXTRN_INT_0:
		GICR |= INT0_GICR;
		break;
		case EXTRN_INT_1:
		GICR |= INT1_GICR;
		break;
		case EXTRN_INT_2:
		GICR |= INT2_GICR;
		break;
		default:
		return E_NOK;
	}
	return E_OK;	
}

/*
 * Function: ExternInt_Disable
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2)
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Disable the required external interrupt.
 */
ERROR_STATUS 
ExternInt_Disable(uint8 ExternInt_No)
{
	switch(ExternInt_No)
	{
		case EXTRN_INT_0:
			GICR &= ~ INT0_GICR;
			MCUCR &= ~INT0_EDGE_BITS;
			break;
		case EXTRN_INT_1:
			GICR &= ~ INT1_GICR;
			MCUCR &= ~INT1_EDGE_BITS;
			break;
		case EXTRN_INT_2:
			GICR &= ~ INT2_GICR;
			MCUCSR &= ~INT2_EDGE_BITS;
			break;
		default:
			return E_NOK;
	}
	return E_OK;
}

ISR(INT0_vect)
{
	if (NULL != EXT_INT_CBK_PTR[EXTRN_INT_0])
	{
		EXT_INT_CBK_PTR[EXTRN_INT_0]();
	} 
	else
	{
		
	}
}


ISR(INT1_vect)
{
	if (NULL != EXT_INT_CBK_PTR[EXTRN_INT_1])
	{
		EXT_INT_CBK_PTR[EXTRN_INT_1]();
	}
	else
	{
		
	}
}


ISR(INT2_vect)
{
	if (NULL != EXT_INT_CBK_PTR[EXTRN_INT_2])
	{
		EXT_INT_CBK_PTR[EXTRN_INT_2]();
	}
	else
	{
		
	}
}