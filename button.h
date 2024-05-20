//최종
#if 1
/*
 * button.h
 *
 * Created: 2024-04-25 오후 1:15:28
 *  Author: HARMAN-06
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_
#define F_CPU 16000000UL //16MHz
#include <avr/io.h> //PORTA DDRA 등의 symbol이 정의되어있음.
#include <util/delay.h> // _delay_ms 등의 함수가 들어있다.

#define LED_DDR DDRA    //이렇게 하는 이유는 LED_DDR에서 바꿔주면 complier가 알아서 변경해준다.
#define LED_PORT PORTA   //
#define BUTTON_DDR DDRD
#define BUTTON_PIN PIND // PORTD를 read하는 register  전압 5v를 읽으면 1, 0v를 읽으면 0

#define BUTTON1PIN 4  //PORTD.4
#define BUTTON2PIN 5  //PORTD.5
#define BUTTON3PIN 6  //PORTD.6
#define BUTTON4PIN 7  //PORTD.7

#define BUTTON1 0  //PORTD.4의 가상 index
#define BUTTON2 1  //PORTD.5의 가상 index
#define BUTTON3 2  //PORTD.6의 가상 index
#define BUTTON4 3  //PORTD.7의 가상 index

#define BUTTON_PRESS 1 //버튼 누르면 high (active-high)
#define BUTTON_RELEASE 0 //버튼을 떼면 low
#define BUTTON_NUMBER 4 //버튼 갯수

#endif /* BUTTON_H_ */

#endif

#if 0
/*
 * button.h
 *
 * Created: 2024-04-25 오후 1:15:28
 *  Author: HARMAN-06
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_
#define F_CPU 16000000UL //16MHz
#include <avr/io.h> //PORTA DDRA 등의 symbol이 정의되어있음.
#include <util/delay.h> // _delay_ms 등의 함수가 들어있다.

#define LED_DDR DDRA    //이렇게 하는 이유는 LED_DDR에서 바꿔주면 complier가 알아서 변경해준다.
#define LED_PORT PORTA   //
#define BUTTON_DDR DDRD
#define BUTTON_PIN PIND // PORTD를 read하는 register  전압 5v를 읽으면 1, 0v를 읽으면 0

#define BUTTON1PIN 4  //PORTD.4
#define BUTTON2PIN 5  //PORTD.5
#define BUTTON3PIN 6  //PORTD.6
#define BUTTON4PIN 7  //PORTD.7

#define BUTTON1 0  //PORTD.4의 가상 index
#define BUTTON2 1  //PORTD.5의 가상 index
#define BUTTON3 2  //PORTD.6의 가상 index
#define BUTTON4 3  //PORTD.7의 가상 index

#define BUTTON_PRESS 1 //버튼 누르면 high (active-high)
#define BUTTON_RELEASE 0 //버튼을 떼면 low
#define BUTTON_NUMBER 4 //버튼 갯수

#endif /* BUTTON_H_ */

#endif