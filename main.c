/*
 * 04.timer_test.c
 *
 * Created: 2024-04-26 오후 3:23:30
 * Author : kccistc
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>   // printf scanf fgets 등이 정의 되어 있다. 

#include "def.h"
#include "extern.h"

void init_timer0();
void init_uart0(void);

// 1. for printf
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);


volatile uint32_t ms_count=0;   // ms count
                                // interrupt service routine 안에 쓰는 변수는
							    // 변수 type앞에 volatile이라고 기입 한다.
								// 이유는 최적화 방지를 위함이다. 
volatile uint32_t shift_timer=0; 
volatile uint32_t ultrasonic_trigger_timer=0;  // trigger를 동작시키는 주기 1000ms
// 256개의 pulse를 count(1ms)하면 이곳으로 자동적으로 진입 한다.
// 즉 256개의 pulse ==> 1ms
// ISR 루틴은 가능한 짧게 구현 한다. 
ISR(TIMER0_OVF_vect)
{
	TCNT0=6;  // 6~256 --> 1ms를 맞춰주기 위해서 TCNT0를 6으로 설정
	ms_count++;
	shift_timer++;
	ultrasonic_trigger_timer++;
}


int main(void)
{	
	init_timer0();
	init_uart0();
	init_uart1();
	stdout = &OUTPUT;   // 2. printf가 동작 되도록 stdout에 OUTPUT화일 포인터를 assign
	                    // fprintf(stdout,"test\n");  printf("test\n"); scanf("%s"buff);  ==> stdin
	                    // fgets(input,100,stdin); // stdin : keyboard
	//init_ultrasonic();
	//I2C_LCD_init();
	sei();    // 전역적으로 인터럽트 허용
	
printf("DHT11 Test!!!");
//====================================================
#define DHT_DDR DDRF
#define DHT_PORT PORTF
#define DHT_Pin PINF
#define DHT_NUM 7  // 0b 10000 0000
uint32_t us_count=0;
enum state_t {OK,TIMEOUT,VALUE_ERROR,TRANS_ERROR};
enum state_t state=OK;

	//초기값.
	DDRF |= (1<<DHT_NUM); // = 0b 1000 0000
	PORTF |=(1<<DHT_NUM);
	
    while (1) 
    {	
		int data[5]={0,}; // 5개 묶음 바구니
		for(int i=0;i<5;i++)
		{
			data[i]=0;
		}
		state = OK;
		
		//====== Step1 Request from Master ==========
		DDRF |= (1<<DHT_NUM); // = 0b 1000 0000   
		PORTF |=(1<<DHT_NUM);
		// do not send any instruction to the sensor in within one second in order to pass the unstable status
		// 통신이 끝난후에는 HIGH로 계속 유지되기 때문에 100ms?
		_delay_ms(100); // 1로 초기셋팅후 기다리고
		
		PORTF &= ~(1<<DHT_NUM);
		_delay_ms(20); // 0으로 18ms기다리고
		
		PORTF |= (1<<DHT_NUM); // pull up register가 high로 바꿔준다는데, 우린 없으니깐 직접 올림.
		DDRF &= ~(1<<DHT_NUM); //input 모드로 변경. 0
		_delay_us(1);
		// DTH가 전압을 LOW로 끌어 내렸을거다. -> 지금상태 LOW(ㄷㄷ)
		
		// 답장이 없는 경우.
		us_count=0; // us_count변수는 pulse의 시간폭을 측정하기전 0으로 초기화한다.
		// while문 : 출력되는 전압값이 변경되는지 check
		//DHT11센서에서, 요청을 받지못해 PORTF를 LOW로 전압을 낮추지 못한 상태를 감지함
		// 정상동작이라면, 전압이 LOW여야한다.
		while(((DHT_Pin & 1 <<DHT_NUM)>>DHT_NUM)) //아직도 HIGH야?(ㄷㄷ)
		{
			_delay_us(2);
			us_count +=2;
			if(us_count>50) // 50us 만큼 기다려줄게.
			{
				state=TIMEOUT;
				break;
			}
		}
		
		// 정상동작 시 지금 LOW
		//====== Step2. Response from slave =====
		if(state==OK)
		{
			us_count=0;
			while(!((DHT_Pin & 1 <<DHT_NUM)>>DHT_NUM))  // LOW동안 반복.
			{
				_delay_us(2);
				us_count +=2;
				if(us_count>100) // 80us 동안만 low기는 한데 여유두고 100...변경가능
				{
					state = TIMEOUT;
					break;
				}
			}
			//===정상동작... 지금 전압 HIGH
			if(state==OK)
			{
				us_count=0;
				while(((DHT_Pin & 1 <<DHT_NUM)>>DHT_NUM))  // HIGH동안 반복.
				{
					_delay_us(2);
					us_count +=2;
					if(us_count>100) // 80us 동안만 HIGH기는 한데 여유두고 100...변경가능
					{
						state = TIMEOUT;
						break;
					}
				}
			}
		}
		
		//정상동작 했으면 상태 OK & 전압은 LOW.
		
		// ===== Case 3.data transmit ========
		// 이제부터 40개 pulse 재야해.{습H, 습L, 온H, 온L, checksum} 88888 개씩 묶어야해.
		// '0' -> 50us동안 LOW, 26~28us동안 HIGH
		// '1' -> 50us동안 LOW, 70us 동안 HIGH
		if(state == OK)
		{
			int pulse[8]={0,}; // 1개값 8개펄스 담는 바구니.
			
			for(int i=0; i<5;i++)
			{
				for(int j=7;j>=0;j--)
				{
					// 앞부분 50us LOW 확인
					us_count=0;
					while(!((DHT_Pin & 1 <<DHT_NUM)>>DHT_NUM))  // LOW동안 반복.
					{
						_delay_us(2);
						us_count +=2;
						if(us_count>70) // 50us 동안만 HIGH기는 한데 여유두고 70
						{
							state = TIMEOUT;
							i=5;
							j=-1;
							break;	
						}
					}
					
					// 이시점 정상이면, 상태 OK & 전압 HIGH
					// 뒷부분 HIGH 확인 (0, 1 따라 값이 다름.)
					if(state == OK)
					{
						us_count = 0;
						//We read data bit || 26-28us means '0' || 70us means '1'
						_delay_us(35);							//Wait for more than 28us
						if(state == OK && ((DHT_Pin & 1 <<DHT_NUM)>>DHT_NUM))
						{
							/*불필요함 초기상태가 어차피 0이기 땜에*/
							//if(us_count<40)// '0'
							//{
							//pulse[j]=0;
							//}
							//else if
							if(us_count>40) // '1' 70us유지.
							{
								pulse[j]=1;
							}
						}
						us_count=0;
						while(((DHT_Pin & 1 <<DHT_NUM)>>DHT_NUM))  // High동안 반복.
						{
							_delay_us(2);
							us_count +=2;
							if(us_count>90) // HIGH가 l->26~28, H -> 70인데 이것보다 길게 90이상 유지되면 오류. 
							{
								state = TIMEOUT;
								i=5;
								j=-1;
								break;
							}
						}

					}
				}
				//printf("pulse : %d, %d, %d, %d %d %d %d %d\n",pulse[7],pulse[6],pulse[5],pulse[4],pulse[3],pulse[2],pulse[1],pulse[0]);
				// 펄스 8개 확인했으니, 값 확인. 이거 키면 이후 동작 문제생겨 (printf 의 delay)
				if(state == OK)
				{
					data[i]=(pulse[0]<<0)|(pulse[1]<<1)|(pulse[2]<<2)|(pulse[3]<<3)|(pulse[4]<<4)|(pulse[5]<<5)|(pulse[6]<<6)|(pulse[7]<<7);
				}
			}	
			
			//Case 3-2. check sum 확인.
			if(state == OK)
			{
				if(data[4] != data[0]+data[1]+data[2]+data[3])
				{
					state = VALUE_ERROR;
				}
			}
			_delay_us(60); // LOW 50us
			// case 3-3. HIGH 지속 되는가?
			//정상이면 상태 OK & 전압 HIGH
			us_count=0;
			while(((DHT_Pin & 1 <<DHT_NUM)>>DHT_NUM))  // LOW동안 반복.
			{
				_delay_us(2);
				us_count +=2;
				if(us_count>90) // 또다른 입력이 있으면 50us low로 쉬고 high 될텐데 이를 확인.
				{
					break;	
				}
			}
			if(us_count<90)//이 상황이 오류. 뒤에 추가 data 있네?
			{
				state =TRANS_ERROR;
			}
		}
		//Case 4. 값 출력.
		if(state==OK)
		{
			
			printf("temp : %d.%d C\n",data[2],data[3]);
			printf("humi : %d.%d %%\n",data[0],data[1]);
		}
		if(state != OK)
		{
			printf("error code %d\n",state);
			printf("data: %d %d %d %d %d\n",data[0],data[1],data[2],data[3],data[4]);
		}
		//==================================
		
		//dht11_main();
		_delay_ms(2000);
	}
}

//void dht11_main(void)
//{
	//char temp[2],humi[2];
	//
	//DHT_Read(temp,humi);
	//
	//printf("temp : %d.%d \n",temp[0],temp[1]);
	//printf("humi : %d.%d \n",humi[0],humi[1]);
//}

// timer0를 초기화 한다. 8bit timer 0번과 2번중 0번을 사용한다. 
void init_timer0()
{
// 분주비를 64로 설정 
// 1.분주비 계산
// 16000000HZ/64 ==> 250,000 HZ
// 2. T(주기) pulse 1개가 잡아 먹는 시간: 1/f = 1/250000 => 0.000004sec (4us) : 0.004ms
// 3. 8bit timer OV(OV flow) : 0.004ms x 256 == 0.001024sec ==> 1.024ms
//                             0.004ms x 250 == 0.001sec ==> 1ms
  TCNT0 = 6;   // TCNT : 0~256 ==> 정확히 1ms마다 TIMER0_OVF_vect로 진입한다.
  TCCR0 |= (1 << CS02) | (0 << CS01) | (0 << CS00);  // (1) 분주비: 64로 설정
  TIMSK |= 1 << TOIE0;    // (2) timer0 overflow INT허용
}

