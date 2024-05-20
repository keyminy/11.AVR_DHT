/*
 * 04.TIMER_TEST.c
 *
 * Created: 2024-04-26 오후 3:23:34
 * Author : HARMAN-27
 */ 

#include "def.h"
#include "extern.h"
#include "DHT.h"


//stidio.h에 file구조체가 들어있다
// 1. for printf
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit,NULL, _FDEV_SETUP_WRITE);

char scm[50];
int job = 5;

int main(void)
{
	init_timer0();
	init_uart0();
	init_uart1();
	stdout = &OUTPUT; // 2. printf가 동작되도록 stdout에 OUTPUT파일 포인터를 assign한다.
	// fprintf(stdout,"test"); == printf("test\n");
	// scanf("%s",buff); --> stdin
	// fgets(input,100,stdin); //여기서 stdin이 가리키는 것은 키보드입니다.
	
	sei(); // 전역적으로 인터럽트를 허용하겠다
	
	printf("DHT11 Test!!!\n");
    while (1) 
    {
		// 온습도센서 체크 로직
		dht11_main();
		_delay_ms(2000);
    }
}

void dht11_main(void){
	char temp[2],humi[2];
	
	DHT_Read(temp,humi);
	
	printf("temp : %d.%d\n",temp[0],temp[1]); // 온도의 정수부분, 소수부분
	printf("humi : %d.%d\n",humi[0],humi[1]);  // 습도의 정수부분, 소수부분
	
}
