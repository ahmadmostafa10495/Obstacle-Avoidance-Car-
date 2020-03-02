/*
 * motor.c
 *
 * Created: 12/23/2019 12:29:57 PM
 *  Author: ahmad
 */ 



/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "motor.h"
#include "DIO.h"
#include "PWM.h"

/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/


#define MOTOR_EN_1_GPIO		(GPIOD)
#define MOTOR_EN_1_BIT		(BIT4)

#define MOTOR_OUT_1A_GPIO	(GPIOD)
#define MOTOR_OUT_1B_GPIO	(GPIOD)
#define MOTOR_OUT_1A_BIT	(BIT2)
#define MOTOR_OUT_1B_BIT	(BIT3)


#define MOTOR_EN_2_GPIO		(GPIOD)
#define MOTOR_EN_2_BIT		(BIT5)

#define MOTOR_OUT_2A_GPIO	(GPIOD)
#define MOTOR_OUT_2B_GPIO	(GPIOD)
#define MOTOR_OUT_2A_BIT	(BIT6)
#define MOTOR_OUT_2B_BIT	(BIT7)

#define MAX_SPEED           (100)
#define MIN_SPEED           (0)

#define MOTOR_FREQ          (1000)



/******************************************************************************************
*                                                                                         *
*                               STATIC VARIABLES                                          *
*																						  *
*																						  *
*******************************************************************************************/



static uint8 Mot_state[2] = {MOTOR_STOP, MOTOR_STOP};


/******************************************************************************************
*                                                                                         *
*                                 IMPLEMENTATION                                          *
*																						  *
*																						  *
*******************************************************************************************/







/**
 * Func			: Motor_Init
 * Input		: motorNumber
 *	  	  		  #def
 *	  	  		  number of motor (MOTOR_1, MOTOR_2)
 * output		: none
 * In out		: none
 * Return		: ERROR_STATUS
 * Description	: Initialize (Enable, IN1, IN2) pins as digital outputs.
 */
 ERROR_STATUS 
 Motor_Init(uint8 Motor_Number)
 {
	DIO_Cfg_s DIO_info;
	Pwm_Cfg_s PWM_info;
	switch(Motor_Number)
	{
		case MOTOR_1:
		DIO_info.GPIO = MOTOR_EN_1_GPIO;
		DIO_info.pins = MOTOR_EN_1_BIT;
		DIO_info.dir = OUTPUT;
		DIO_init(&DIO_info);
		DIO_info.GPIO = MOTOR_OUT_1A_GPIO;
		DIO_info.pins = MOTOR_OUT_1A_BIT;
		DIO_info.dir = OUTPUT;
		DIO_init(&DIO_info);
		DIO_info.GPIO = MOTOR_OUT_1B_GPIO;
		DIO_info.pins = MOTOR_OUT_1B_BIT;
		DIO_info.dir = OUTPUT;
		DIO_init(&DIO_info);
		PWM_info.Channel = PWM_CH1A;
		PWM_info.Prescaler = PWM_PRESCALER_64;
		Pwm_Init(&PWM_info);
		break;
		case MOTOR_2:
		DIO_info.GPIO = MOTOR_EN_2_GPIO;
		DIO_info.pins = MOTOR_EN_2_BIT;
		DIO_info.dir = OUTPUT;
		DIO_init(&DIO_info);
		DIO_info.GPIO = MOTOR_OUT_2A_GPIO;
		DIO_info.pins = MOTOR_OUT_2A_BIT;
		DIO_info.dir = OUTPUT;
		DIO_init(&DIO_info);
		DIO_info.GPIO = MOTOR_OUT_2B_GPIO;
		DIO_info.pins = MOTOR_OUT_2B_BIT;
		DIO_info.dir = OUTPUT;
		DIO_init(&DIO_info);
		PWM_info.Channel = PWM_CH1B;
		PWM_info.Prescaler = PWM_PRESCALER_64;
		Pwm_Init(&PWM_info);
		break;
		default:
		return E_NOK;
	}	
	return E_OK; 
 }
 
 
 /**
 * Fun		 : Motor_Direction
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2

			   @param Motor_Direction
			   #define
			   Stop, Forward, Backword
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: Controls the motor direction from getting the motor number and the direction.
*/
ERROR_STATUS 
Motor_Direction(uint8 Motor_Number, uint8 Motor_Direction)
{
	switch (Motor_Number)
	{
		case MOTOR_1:
		switch (Motor_Direction)
		{
			case MOTOR_STOP:
			DIO_Write(MOTOR_OUT_1A_GPIO, MOTOR_OUT_1A_BIT, LOW);
			DIO_Write(MOTOR_OUT_1B_GPIO, MOTOR_OUT_1B_BIT, LOW);
			Mot_state[MOTOR_1 - 1] = MOTOR_STOP;
			break;
			case MOTOR_FORWARD:
			DIO_Write(MOTOR_OUT_1A_GPIO, MOTOR_OUT_1A_BIT, HIGH);
			DIO_Write(MOTOR_OUT_1B_GPIO, MOTOR_OUT_1B_BIT, LOW);
			Mot_state[MOTOR_1 - 1] = MOTOR_FORWARD;
			break;
			case MOTOR_BACKWARD:
			DIO_Write(MOTOR_OUT_1A_GPIO, MOTOR_OUT_1A_BIT, LOW);
			DIO_Write(MOTOR_OUT_1B_GPIO, MOTOR_OUT_1B_BIT, HIGH);
			Mot_state[MOTOR_1 - 1] = MOTOR_BACKWARD;
			break;
			default:
			return E_NOK;
		}
		break;
		case MOTOR_2:
		switch (Motor_Direction)
		{
			case MOTOR_STOP:
			DIO_Write(MOTOR_OUT_2A_GPIO, MOTOR_OUT_2A_BIT, LOW);
			DIO_Write(MOTOR_OUT_2B_GPIO, MOTOR_OUT_2B_BIT, LOW);
			Mot_state[MOTOR_2 - 1] = MOTOR_STOP;
			break;
			case MOTOR_FORWARD:
			DIO_Write(MOTOR_OUT_2A_GPIO, MOTOR_OUT_2A_BIT, HIGH);
			DIO_Write(MOTOR_OUT_2B_GPIO, MOTOR_OUT_2B_BIT, LOW);
			Mot_state[MOTOR_2 - 1] = MOTOR_FORWARD;
			break;
			case MOTOR_BACKWARD:
			DIO_Write(MOTOR_OUT_2A_GPIO, MOTOR_OUT_2A_BIT, LOW);
			DIO_Write(MOTOR_OUT_2B_GPIO, MOTOR_OUT_2B_BIT, HIGH);
			Mot_state[MOTOR_2 - 1] = MOTOR_BACKWARD;
			break;
			default:
			return E_NOK;
		}
		break;
		default:
		return E_NOK;
	}	
	return E_NOK;
}


/**
 * Fun		 : Motor_Start
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2
			   @param Mot_Speed
			   set the motor speed
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: Start the motor.
*/
ERROR_STATUS 
Motor_Start(uint8 Motor_Number, uint8 Mot_Speed)
{
	if (Mot_Speed > MAX_SPEED || Mot_Speed < MIN_SPEED)
	{
		return E_NOK;
	}
	else
	{
		switch(Motor_Number)
		{
			case MOTOR_1:
			Pwm_Start(PWM_CH1A, Mot_Speed, MOTOR_FREQ);
			break;
			case MOTOR_2:
			Pwm_Start(PWM_CH1B, Mot_Speed, MOTOR_FREQ);
			break;
			default:
			return E_NOK;			
		}
	}
	return E_OK;
}


/**
 * Fun		 : Motor_SpeedUpdate
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2
			   @param Mot_Speed
			   set the motor speed
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: Controls the motor speed from getting the motor number and the speed.
*/
ERROR_STATUS 
Motor_SpeedUpdate(uint8 Motor_Number, uint8 Speed)
{
	if (Speed > MAX_SPEED || Speed < MIN_SPEED)
	{
		return E_NOK;
	}
	else
	{
		switch(Motor_Number)
		{
			case MOTOR_1:
			Pwm_Update(PWM_CH1A, Speed, MOTOR_FREQ);
			break;
			case MOTOR_2:
			Pwm_Update(PWM_CH1B, Speed, MOTOR_FREQ);
			break;
			default:
			return E_NOK;
		}
	}
	return E_OK;	
}


/**
 * Fun		 : Motor_Stop
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: stop the motor.
*/
ERROR_STATUS 
Motor_Stop(uint8 Motor_Number)
{

	switch(Motor_Number)
	{
		case MOTOR_1:
		Pwm_Stop(PWM_CH1A);
		Motor_Direction(MOTOR_1, MOTOR_STOP);
		Mot_state[MOTOR_1 - 1] = MOTOR_STOP;
		break;
		case MOTOR_2:
		Pwm_Stop(PWM_CH1B);
		Motor_Direction(MOTOR_2, MOTOR_STOP);
		Mot_state[MOTOR_2 - 1] = MOTOR_STOP;
		break;
		default:
		return E_NOK;
	}
	return E_OK;	
}

 /**
 * Func			: Motor_GetStatus
 * Input		: Motor_Number
 *	  	  		 #def
 *	  	  		 number of motor (MOTOR_1, MOTOR_2)
 * output		: none
 * In out		: none
 * Return		: Initialization_STATUS
 * Description	: Returns status of the motor whether intialized or not
 */
 ERROR_STATUS 
 Motor_GetStatus(uint8 Motor_Number, uint8* Mot_status)
 {
	 if (NULL == Mot_status)
	 {
		 return E_NOK;
	 }
	 else
	 {
		 switch(Motor_Number)
		 {
			 case MOTOR_1:
			 *Mot_status = Mot_state[MOTOR_1 - 1];
			 break;
			 case MOTOR_2:
			 *Mot_status = Mot_state[MOTOR_2 - 1];
			 break;	
			 default:
			 return E_NOK;	 
		 } 
	 }
	 return E_OK;
 }