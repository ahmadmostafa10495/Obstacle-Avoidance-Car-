/*
 * static.c
 *
 * Created: 12/24/2019 10:02:20 PM
 *  Author: ahmad
 */ 

#include "DIO.h"
#include "motor.h"
#include "interrupt.h"

#include "ExternalInterrupt.h"
#include "Us.h"
#include "ICU.h"
#include "Timer.h"
#include "Char_lcd.h"
#include "softwareDelay.h"
#include <stdlib.h>
#include "car_sm.h"


void hi(void){
	DIO_Write(GPIOB, BIT7, HIGH);
}


int main(void)
{
	sei();
	/*Timer_cfg_s timer_info = {TIMER_CH2, TIMER_MODE, TIMER_INTERRUPT_MODE, TIMER_PRESCALER_NO, hi};
	Timer_Init(&timer_info);
	Timer_Start(TIMER_CH2, 255);*/
	/*//sei();
	ExternInt_Cfg_s extint_info ={EXTRN_INT_2, RISING_EDGE, hi};
	ExternInt_Init(&extint_info);
	ExternInt_Enable(EXTRN_INT_2);
	DIO_Cfg_s DIOINFO = {
		GPIOA,
		FULL_PORT,
		OUTPUT
	};
	DIO_init(&DIOINFO);

	DIO_Write(GPIOA, FULL_PORT, HIGH);
*/
	/*DIO_Cfg_s DIOINFO = {
		GPIOB,
		BIT7|BIT6|BIT5,
		OUTPUT
	};
	DIO_init(&DIOINFO);
	
	/*Icu_cfg_s icu_info = {ICU_CH2, ICU_TIMER_CH2};
	Icu_Init(&icu_info);
	uint32 ICU_TIME = 0;
	//Icu_ReadTime(ICU_CH2, ICU_RISE_TO_FALL, &ICU_TIME);
	Motor_Init(MOTOR_1);
	Motor_Init(MOTOR_2);
	Motor_Direction(MOTOR_1, MOTOR_FORWARD);
	Motor_Direction(MOTOR_2, MOTOR_FORWARD);
	Motor_Start(MOTOR_1, 20);
	Motor_Start(MOTOR_2, 20);
	Us_Init();
	
	uint16 u16_distance = 0;
	char lcd_buffer[16];
	LCD_init();*/
	Car_SM_Init();
    while(1)
    {
		Car_SM_Update();
        //TODO:: Please write your application code 
		/*if (u16_distance > 30)
		{
				Motor_SpeedUpdate(MOTOR_1, 80);
				Motor_SpeedUpdate(MOTOR_2, 80);
		}
		else if(u16_distance <10)
		{
							Motor_SpeedUpdate(MOTOR_1, 20);
							Motor_SpeedUpdate(MOTOR_2, 20);
		}
		LCD_goto_xy(0,0);
		Us_Trigger();
		Us_GetDistance(&u16_distance);
		//Icu_ReadTime(ICU_CH2, ICU_RISE_TO_FALL, &ICU_TIME);
		//ICU_TIME/=58.2;
		itoa(u16_distance,lcd_buffer,10);
		LCD_send_string(lcd_buffer);
		SwDelay_ms(1000);
		LCD_clear();*/
				
		
    }
	/*DIO_Cfg_s st_USDio;
	st_USDio.GPIO=TRIGGER_GPIO;
	st_USDio.pins=TRIGGER_BIT;
	st_USDio.dir=HIGH;
	DIO_init(&st_USDio);
	
	Icu_cfg_s ST_USIcu={ICU_CH2,ICU_TIMER_CH2};
	Icu_Init(&ST_USIcu);
	Timer_cfg_s mytimer;
	mytimer.Timer_CH_NO=TIMER_CH2;
	mytimer.Timer_Mode=TIMER_MODE;
	mytimer.Timer_Prescaler=TIMER_PRESCALER_NO;
	mytimer.Timer_Polling_Or_Interrupt=TIMER_INTERRUPT_MODE;
	mytimer.Timer_Cbk_ptr=hi;
	Timer_Init(&mytimer);
	Timer_Start(TIMER_CH2,100);

	volatile uint16 timerrrr=0;
	char lcd_buffer[16];
	LCD_init();
	uint8 i=0;
	volatile uint16 Distance=0;
	
	DIO_Cfg_s myutdio;
	myutdio.GPIO=TRIGGER_GPIO;
	myutdio.pins=TRIGGER_BIT;
	myutdio.dir=HIGH;
	DIO_init(&myutdio);
	Us_Init();
	
	
	uint16 u16_TimerValue = 0;
	while (1)
	{
		
		volatile uint16 u16_loopCounter ;
		DIO_Write(TRIGGER_GPIO,TRIGGER_BIT,HIGH);
		for (u16_loopCounter =0;u16_loopCounter <1000;u16_loopCounter ++);
		DIO_Write(TRIGGER_GPIO,TRIGGER_BIT,LOW);
		Icu_ReadTime(ICU_CH2, ICU_RISE_TO_FALL, & u16_TimerValue);
		Distance=u16_TimerValue/ 58.2;
		LCD_goto_xy(0,0);
		Us_Trigger();
		Us_GetDistance(& Distance);
		itoa(Distance,lcd_buffer,10);
		
		LCD_send_string(lcd_buffer);
		LCD_clear();
		
	}*/
}