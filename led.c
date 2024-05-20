
#include "button.h" // 버튼 초기설정 입력 후, 모든 PORTA 입력을 LED_PORT로 변환하여 사용하였습니다.
#include "led.h"
#include "extern.h"
#include "def.h"

void shift_left2right_keep_ledon();
void shift_right2left_keep_ledon();
void led_all_on_off(void);
void flower_on();
void flower_off();

void make_pwm_led_control(void);

//left to right
void shift_left2right_keep_ledon()
{
	for(int i=0;i<9;i++)
	{
		while(shift_timer <= 310){}; //300으로 딱 맞추기엔 값이 불안정해서 310으로 잡았습니다.
	
		shift_timer = 0;
		if(i >= 8)
		{
			PORTA=0x00;
			break;
		}
		else
		{
			PORTA |= 0b00000001 << i;
		}
	}
}

//right to left
void shift_right2left_keep_ledon()
{	
	for(int i=0;i<9;i++)
	{
		while(shift_timer <= 310){};
		
		shift_timer = 0;
		if(i == 8)
		{
			PORTA=0x00;
			break;
		}
		else
		{
			PORTA |= 0b10000000 >> i;
		}
	}
}

void led_all_on_off()
{
	static int led_toggle=0;
	
	if(ms_count >= 500) //갑자기 999에서 1001로 튈 수도 있기 때문에, ==가 아니라 >=를 사용.
	{
		ms_count=0;
		led_toggle = !led_toggle;
		if(led_toggle)
		{
			PORTA=0xff;
		}
		else
		{
			PORTA=0x00;
		}
	}
}

void flower_on()
{
	int flower_state=0;
	uint8_t bit_value=0b00010000;
	
	for(int i=0;i<6;i++)
	{
		while(ms_count <= 310){};
			
		ms_count=0;
		if(flower_state==0)
		{
			PORTA=0x00;
			flower_state++;
		}
		else if(flower_state>=1 & flower_state<5)
		{
			PORTA |= bit_value << (flower_state-1)|bit_value>>flower_state;
			flower_state++;
		}
		else
		{
			if(flower_state==5)
			{
				PORTA=0x00;
			}
		}
	}
	
}

void flower_off()
{
	int flower_state=0;
	uint8_t bit_value=0b01111111;
	
	
	for(int i=0;i<6;i++)
	{
		while(ms_count <= 310){};

		ms_count=0;
		
		if(flower_state==0)
		{
			PORTA=0xff;
			flower_state++;
		}
		else if(flower_state>0 & flower_state<5)
		{
			PORTA &= bit_value >> (flower_state-1) & bit_value << flower_state;
			flower_state++;
			
		}
		else
		{
			PORTA=0X00;
		}
	}
}

void make_pwm_led_control(void)
{
	int dim = 0; //led 밝기 조절 변수.
	int direction = 1;   //1 : 밝기 증가하는 모드   -1 : 밝기 감소하는 모드
	
	DDRA = 0xff; //data direction register
	PORTA = 0xff; //led all on
	
	//d  ->  b  b  ->  d
	while(1)
	{
		led_on_pwm(dim);
		dim += direction;
		if(dim == 255) direction=-1;
		if(dim == 0) direction = 1;
	}
}

void led_on_pwm(int dim)
{
	PORTA=0xff;
	for(int i=0;i<256;i++)
	{
		if(i>dim)
		{
			PORTA=0x00; //duty cycle
		}
		_delay_us(20);
	}
}