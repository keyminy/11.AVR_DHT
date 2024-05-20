/*
 * uart.c
 *
 * Created: 2024-04-30 오전 10:38:21
 *  Author: HARMAN-06
 */ 
// 1. 전송속도 : 9600bps : 총byte(글자)수 : 9600 / 10 = 960자  -> 약 1ms에 한글자씩
// (1글자 전송 하는데 소요 되는 시간 : 약 1ms)
// 2. 비동기 방식이다.(uart의 경우 clock신호에 의지하지 않고 별도의 부호비트(start / stop)로 data를 구분하는 방식.
// data 8bit /non parity
//even parity 1이 짝수개가 되도록 약속을 해서 데이터의 오류를 판단함. (odd 는 홀수개.)
// 3. RX(수신) : interrupt 활성화.
// 입력패턴 예 : led_all_on\n
#define F_CPU 16000000UL //16MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> //인터럽트 관련 함수 쓰기 위한 헤더.
#include "def.h"
#include "uart1.h"
#include "extern.h"
#include <string.h> //strncmp, strcpy, strcmp등이 들어있다.

// 1byte를 수신 할때마다 이곳으로 들어온다.
ISR(USART1_RX_vect)
{
	static int i=0;
	uint8_t data1;

	data1 = UDR1; //UART1의 하드웨어 레지스터로부터 1byte 읽어간다.
				 // data = UDR1를 취하면 UDR1의 내용이 빈다. (move)
	if(data1=='\r' || data1 =='\n')
	{
		rx1_buffer[i]='\0'; // 문장의 끝을 알리는 null 을 insert
		i=0; // 다음 string 저장하기 위해 index값을 0으로 만듦.
		rx1_ready_flag=1; // 완전한 문장이 들어오면 1로 set
	}
	else
	{
		rx1_buffer[i++] = data1;		
	}
				 
	
	
}

void init_uart1(void)
{
	// 표 9-9
	UBRR1H = 0x00;
	UBRR1L = 207; //9600bps 설정
	
	UCSR1A |= 1 << U2X1; //2배속 통신
	//UCSR1C |= 0x06;	// ASYNC(비동기)/data8bit/non parity
	
	// RXEN1 : UART1부터 수신이 가능하도록 설정.
	// TXEN1 : UART1부터 신이 가능하도록 설정
	//RXCIE1 : UART1로부터 한바이트가 들어오면(stopbit까지 수신 완료) rx interrupts를 발생시켜라.
	
	UCSR1B |= 1<< RXEN1	| 1<< TXEN1 | 1 << RXCIE1;
}

//UART0를 1byte 전송하는 함수
void UART1_transmit(uint8_t data)
{
	while(!(UCSR1A & 1 << UDRE1))  //데이터가 전송중이면 전송이 끝날때까지 기다린다.
		; //no operation	NOP
		
	UDR1 = data; // HW전송 register(UDR0)에 data를 쏴준다.
}

//led_all_on\0
//led_all_off\0
//extern void shift_left2right_keep_ledon(int *pjob);
//extern void shift_right2left_keep_ledon(int *pjob);
//void flower_on(int*pjob);
//void flower_off(int*pjob);

void bt_command_processing(void)
{
	
	if(rx1_ready_flag) // if(rx_ready_flag > =,) data 가 \n까지 들어 왔으면
	{
		rx1_ready_flag=0;
		printf("%s\n",rx1_buffer);
		if(strncmp(rx1_buffer,"led_all_on",strlen("led_all_on"))==0)
		{
			PORTA = 0xff;
		}
		else if(strncmp(rx1_buffer,"led_all_off",strlen("led_all_off"))==0)
		{
			PORTA = 0x00;
		}
		else if(strncmp(rx1_buffer,"shift_left2right_keep_ledon",strlen("shift_left2right_keep_ledon"))==0)
		{
			shift_left2right_keep_ledon();
		}
		else if(strncmp(rx1_buffer,"shift_right2left_keep_ledon",strlen("shift_right2left_keep_ledon"))==0)
		{
			shift_right2left_keep_ledon();
		}
		else if(strncmp(rx1_buffer,"flower_on",strlen("flower_on"))==0)
		{
			flower_on();
		}
		else if(strncmp(rx1_buffer,"flower_off",strlen("flower_off"))==0)
		{
			flower_off();
		}
	}
}