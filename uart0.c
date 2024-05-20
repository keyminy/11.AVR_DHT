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
#include "uart0.h"
#include "extern.h"
#include <string.h> //strncmp, strcpy, strcmp등이 들어있다.

// 1byte를 수신 할때마다 이곳으로 들어온다.
// 입력패턴 예 : led_all_on\n
ISR(USART0_RX_vect)
{
	volatile static int i=0;
	volatile uint8_t data;

	data = UDR0; //UART0의 하드웨어 레지스터로부터 1byte 읽어간다.
				 // data = UDR0를 취하면 UDR0의 내용이 빈다. (move)
				 

	if(data=='\r' || data =='\n')
	{
		rx_buffer[rear][i]='\0'; // 문장의 끝을 알리는 null 을 insert
		i=0; // 다음 string 저장하기 위해 index값을 0으로 만듦.
		rear++;
		rear %= COMMAND_NUMBER;
		// queue full 조건 check하는 로직이 들어가야 한다.
	}
	else
	{
		rx_buffer[rear][i++] = data;		
	}
				 
	
	
}

void init_uart0(void)
{
	// 표 9-9
	UBRR0H = 0x00;
	UBRR0L = 207; //9600bps 설정
	
	UCSR0A |= 1 << U2X0; //2배속 통신
	UCSR0C |= 0x06;	// ASYNC(비동기)/data8bit/non parity
	
	// RXEN0 : UART0부터 수신이 가능하도록 설정.
	// TXEN0 : UART0부터 신이 가능하도록 설정
	//RXCIE0 : UART0로부터 한바이트가 들어오면(stopbit까지 수신 완료) rx interrupts를 발생시켜라.
	
	UCSR0B |= 1<< RXEN0	| 1<< TXEN0 | 1 << RXCIE0;
}

//UART0를 1byte 전송하는 함수
void UART0_transmit(uint8_t data)
{
	while(!(UCSR0A & 1 << UDRE0))  //데이터가 전송중이면 전송이 끝날때까지 기다린다.
		; //no operation	NOP
		
	UDR0 = data; // HW전송 register(UDR0)에 data를 쏴준다.
}

//led_all_on\0
//led_all_off\0
//extern void shift_left2right_keep_ledon(int *pjob);
//extern void shift_right2left_keep_ledon(int *pjob);
//void flower_on(int*pjob);
//void flower_off(int*pjob);

void pc_command_processing(void)
{
	if(front != rear) // 
	{
		printf("%s\n",rx_buffer[front]); // &rx_buffer[front][0]
		if(strncmp(rx_buffer[front],"led_all_on",strlen("led_all_on"))==0)
		{
			PORTA = 0xff;
		}
		else if(strncmp(rx_buffer[front],"led_all_off",strlen("led_all_off"))==0)
		{
			PORTA = 0x00;
		}
		else if(strncmp(rx_buffer[front],"shift_left2right_keep_ledon",strlen("shift_left2right_keep_ledon"))==0)
		{
			shift_left2right_keep_ledon();
		}
		else if(strncmp(rx_buffer[front],"shift_right2left_keep_ledon",strlen("shift_right2left_keep_ledon"))==0)
		{
			shift_right2left_keep_ledon();
		}
		else if(strncmp(rx_buffer[front],"flower_on",strlen("flower_on"))==0)
		{
			flower_on();
		}
		else if(strncmp(rx_buffer[front],"flower_off",strlen("flower_off"))==0)
		{
			flower_off();
		}
		front++;
		front %= COMMAND_NUMBER;
		//queue full check하는 로직이 들어가야 함.
	}
}