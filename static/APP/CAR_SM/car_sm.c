
/*
 * car_sm.c
 *
 * Created: 12/25/2019 3:12:12 PM
 *  Author: ahmad
 */ 




/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "car_sm.h"
#include "Steering.h"
#include "Us.h"
#include "std_types.h"
#include "common_macros.h"
#include "Char_lcd.h"
#include "softwareDelay.h"
#include <stdlib.h>

/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/


#define SM_STOP          (0)
#define SM_FORWARD80     (1)
#define SM_FORWARD30     (2)
#define SM_BACKWARD      (3)
#define SM_RIGHT         (4)

#define SPEED_ZERO       (0)
#define SPEED_FIFTY      (50)
#define SPEED_THIRTY     (30)
#if DEBUGTEST
#define SPEED_EIGHTY     (50)
#else
#define SPEED_EIGHTY     (80)
#endif  

#define DISTANCE_FIFTY   (50)
#define DISTANCE_THIRTY  (30)
 




/******************************************************************************************
*                                                                                         *
*                               STATIC VARIABLES                                          *
*																						  *
*																						  *
*******************************************************************************************/


static uint8 gsu8_CAR_state = SM_STOP;



/******************************************************************************************
*                                                                                         *
*                                 IMPLEMENTATION                                          *
*																						  *
*																						  *
*******************************************************************************************/



/*
 * Fun----------: ERROR_STATUS Car_SM_Init(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: Initiate the car state machine with state"forward at speed 80%",
 * -------------- And initiate steering and ultrasonic functions
*/
ERROR_STATUS 
Car_SM_Init(void)
{
	Steering_Init();
	Us_Init();
	#if DEBUGTEST
	LCD_init();
	#endif
	return E_OK;
}







/*
 * Fun----------: ERROR_STATUS Car_SM_Update(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: changes car state according to the ultrasonic input
 * -------------- And represents the motors output for each state
*/
ERROR_STATUS 
Car_SM_Update(void)
{
	uint16 u16_distance = ZERO_INITIALIZATION;
	#if DEBUGTEST
	char lcd_buffer[16];
	LCD_goto_xy(ZERO_INITIALIZATION, ZERO_INITIALIZATION);
	#endif
	Us_Trigger();
	Us_GetDistance(&u16_distance);
	#if DEBUGTEST
	itoa(u16_distance,lcd_buffer,10);
	LCD_send_string(lcd_buffer);
	#endif
	
	
	
	
	if (u16_distance > DISTANCE_FIFTY)
	{
		Steering_SteerCar(CAR_FORWARD, SPEED_EIGHTY);
		gsu8_CAR_state = SM_FORWARD80;
		#if DEBUGTEST
		LCD_goto_xy(ZERO_INITIALIZATION, ONE_INITIALIZATION);
		LCD_send_string("FORWARD80");
		#endif
	} 
	else if(u16_distance > DISTANCE_THIRTY)
	{
		if(gsu8_CAR_state == SM_BACKWARD || gsu8_CAR_state == SM_STOP)
		{
			Steering_SteerCar(CAR_RIGHT, SPEED_THIRTY);
			#if DEBUGTEST
			LCD_goto_xy(ZERO_INITIALIZATION, ONE_INITIALIZATION);
			LCD_send_string("RIGHT");
			#endif
			SwDelay_ms(1000);
			gsu8_CAR_state = SM_RIGHT;
		}
		else
		{
			Steering_SteerCar(CAR_FORWARD, SPEED_THIRTY);
			gsu8_CAR_state = SM_FORWARD30;
			#if DEBUGTEST
			LCD_goto_xy(ZERO_INITIALIZATION, ONE_INITIALIZATION);
			LCD_send_string("FORWARD30");
			#endif
		}
	}
	else if (u16_distance == DISTANCE_THIRTY)
	{
		if (gsu8_CAR_state == SM_STOP)
		{
			Steering_SteerCar(CAR_RIGHT, SPEED_THIRTY);
			#if DEBUGTEST
			LCD_goto_xy(ZERO_INITIALIZATION, ONE_INITIALIZATION);
			LCD_send_string("RIGHT");
			#endif
			SwDelay_ms(1000);
			gsu8_CAR_state = SM_RIGHT;
		} 
		else
		{
			Steering_SteerCar(CAR_FORWARD, SPEED_ZERO);
			#if DEBUGTEST
			LCD_goto_xy(ZERO_INITIALIZATION, ONE_INITIALIZATION);
			LCD_send_string("STOP");
			#endif
			//SwDelay_ms(100);
			gsu8_CAR_state = SM_STOP;
		}
	} 
	else if(u16_distance < DISTANCE_THIRTY)
	{
		if(gsu8_CAR_state == SM_STOP)
		{
			Steering_SteerCar(CAR_BACKWARD, SPEED_THIRTY);
			#if DEBUGTEST
			LCD_goto_xy(ZERO_INITIALIZATION, ONE_INITIALIZATION);
			LCD_send_string("BACKWARD");
			#endif
			//SwDelay_ms(1000);
			gsu8_CAR_state = SM_BACKWARD;
		}
		else
		{
			Steering_SteerCar(CAR_FORWARD, SPEED_ZERO);
			#if DEBUGTEST
			LCD_goto_xy(ZERO_INITIALIZATION, ONE_INITIALIZATION);
			LCD_send_string("STOP");
			#endif
			//SwDelay_ms(100);
			gsu8_CAR_state = SM_STOP;
		}
	}
	//SwDelay_ms(500);
	LCD_clear();
	return E_OK;
}
































			/*if (gu16_distance>50)
			en_carState=speed80;
			else if (gu16_distance>30)
			{
				if(en_carState==backward | en_carState==stop )
				en_carState=turning;
				else
				en_carState=speed30;
			}
			else if(gu16_distance==30)
			{
				if (en_carState==stop)
				en_carState=turning;
				else if (en_carState==turning)
				en_carState=turning;
				else
				en_carState=stop;
			}
			else if(gu16_distance<30)
			{
				if(en_carState==stop)
				en_carState = backward;
				else if (en_carState == backward)
				en_carState=backward;
				else
				en_carState=stop;
				
			}*/