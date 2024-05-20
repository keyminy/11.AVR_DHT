//최종
#if 1
/*
 * button.c
 *
 * Created: 2024-04-25 오후 1:30:35
 *  Author: HARMAN-06
 */ 
#include "button.h"
void init_button(void);
int get_button(int button_pin, int button_number);


unsigned char previous_button_status[BUTTON_NUMBER]=
{BUTTON_RELEASE,BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE};
// 76543210
// DDRD &= 0x0f;

void init_button(void)
{
	BUTTON_DDR &= ~( (1 << BUTTON1PIN) | (1 << BUTTON2PIN) | (1 << BUTTON3PIN) | (1 << BUTTON4PIN));
	
}

//버튼이 온전하게 눌렸다 떼어진 상태이면 1을 리턴.
int get_button(int button_pin, int button_number) // 예) BUTTON1인경우 물리적위치pin :4, (index)number: 0
{
	int current_state;
	
	current_state= BUTTON_PIN & (1 << button_pin); // 예) 1 << 4(BUTTON1)    00010000 //버튼 값 읽는다.
	if(current_state && previous_button_status[button_number] == BUTTON_RELEASE ) //버튼이 처음 눌려진 상태
	{
		_delay_ms(60); // 노이즈가 지나가길 기다린다.
		previous_button_status[button_number] = BUTTON_PRESS; //처음 눌려진 상태가 아니다.
		return 0;//아직 완전히 스위치를 눌렀다 뗀상태가 아닌 경우
	}
	else if(previous_button_status[button_number]==BUTTON_PRESS && current_state == BUTTON_RELEASE)
			//버튼이 이전에 눌린 상태였으며, 현재는 떼어진 상태.
	{
		previous_button_status[button_number] = BUTTON_RELEASE;
		_delay_ms(60); // 노이즈가 지나가길 기다린다.
		return 1;
	}
	return 0; 
			  //2)스위치가 open된 상태
}
#endif

#if 0
/*
 * button.c
 *
 * Created: 2024-04-25 오후 1:30:35
 *  Author: HARMAN-06
 */ 
#include "button.h"
void init_button(void);
int get_button(int button_pin, int button_number);

unsigned char previous_button_status[BUTTON_NUMBER]=
{BUTTON_RELEASE,BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE};
// 76543210
// DDRD &= 0x0f;
void init_button(void)
{
	BUTTON_DDR &= ~( (1 << BUTTON1PIN) | (1 << BUTTON2PIN) | (1 << BUTTON3PIN) | (1 << BUTTON4PIN));
	
}

//버튼이 온전하게 눌렸다 떼어진 상태이면 1을 리턴.
int get_button(int button_pin, int button_number) // 예) BUTTON1인경우 물리적위치pin :4, (index)number: 0
{
	int current_state;
	
	current_state= BUTTON_PIN & (1 << button_pin); // 예) 1 << 4(BUTTON1)    00010000 //버튼 값 읽는다.
	if(current_state && previous_button_status[button_number] == BUTTON_RELEASE ) //버튼이 처음 눌려진 상태
	{
		_delay_ms(60); // 노이즈가 지나가길 기다린다.
		previous_button_status[button_number] = BUTTON_PRESS; //처음 눌려진 상태가 아니다.
		return 0;//아직 완전히 스위치를 눌렀다 뗀상태가 아닌 경우
	}
	else if(previous_button_status[button_number]==BUTTON_PRESS && current_state == BUTTON_RELEASE)
			//버튼이 이전에 눌린 상태였으며, 현재는 떼어진 상태.
	{
		previous_button_status[button_number] = BUTTON_RELEASE;
		_delay_ms(60); // 노이즈가 지나가길 기다린다.
		return 1;
	}
	return 0; 
			  //2)스위치가 open된 상태
}
#endif