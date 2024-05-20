/*
 * ultrasonic.c
 *
 * Created: 2024-05-08 오전 10:56:03
 *  Author: HARMAN-08
 */ 
#include "ultrasonic.h"
#include "extern.h"

volatile int ultrasonic_distance=0;
char scm[50];
// PE4 : 외부 INT4 초음파센서 상승,하강에지 둘 다 이곳으로 들어온다.
//결국은 상승에지에서 1번 / 하강에지에서 1번 2번 들어온다.
ISR(INT4_vect)
{
	//1. 상승 에지일 때
	if(ECHO_PIN & (1 << ECHO))
	{
		TCNT1 = 0;
	}
	else //2. 하강 에지일 때
	{
		//에코핀에 들어오는 펄스 길이를 us단위로 환산
		ultrasonic_distance = 1000000.0 * TCNT1 * 1024 / F_CPU;
		// 예) TCNT1에 10이 들어있다고 가정하자
		// 15.625KHZ의 1주기가 64us이다.
		// 0.000064 * 10 = 0.00064sec(640us)
		// 640 / 58 ==> 11cm
		// 1cm : 58us 소요
		sprintf(scm, "dis: %dcm", ultrasonic_distance/58);  //cm로 환산
		if(ultrasonic_distance/58<=2)
		{
			PORTA=0x01;
		}
		else if(ultrasonic_distance/58<=4)
		{
			PORTA=0x03;
		}
		else if(ultrasonic_distance/58<=6)
		{
			PORTA=0x07;
		}
		else if(ultrasonic_distance/58<=8)
		{
			PORTA=0x0f;
		}
		else if(ultrasonic_distance/58<=10)
		{
			PORTA=0x1f;
		}
		else
		{
			PORTA=0x00;
		}
		
	}
}

void init_ultrasonic()
{
	TRIG_DDR |= 1 << TRIG;     //OUTPUT mode로 설정
	ECHO_DDR &= ~(1 << ECHO);     //INPUT mode로 설정
	
	//p289 표12-6
	// 0 1 : 상승 하강에지 둘 다 INT를 띄우도록 설정
	EICRB |= 0 << ISC41 | 1 << ISC40;
	
	//16bit timer 1번을 설정을 해서 사용한다.
	//16bit timer: 65535 (0b 1111 1111 1111 1111)
	//16MHZ : clock
	//16MHZ를 1024분주 16000000/1024 ==> 15625HZ ==> 15.625KHZ
	//1주기(1개의 펄스길이) T=1/f 1/15625 => 0.000064sec ==> 64us
	TCCR1B |= 1 << CS12 | 1 << CS10;  //1024로 분주 설정
	
	EIMSK |= 1 << INT4;   //외부 INT4(ECHO) 사용
}

void ultrasonic_trigger()
{
	TRIG_PORT &= ~(1 << TRIG);  //low
	_delay_us(1);
	TRIG_PORT |= 1 << TRIG;  //high
	_delay_us(15);
	TRIG_PORT &= ~(1 << TRIG);  //low
}

void ultrasonic_distance_check()
{
	if(ultrasonic_trigger_timer >= 1000)  //1초
	{
		ultrasonic_trigger_timer=0;
		//printf("%s", scm);
		I2C_LCD_clear();
		I2C_LCD_write_string_XY(0,0,"Ultra Sonic");
		I2C_LCD_write_string_XY(1,0,scm);

		ultrasonic_trigger();
	}	
}