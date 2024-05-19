﻿/*
 * button.h
 *
 * Created: 2024-04-25 오후 1:15:19
 *  Author: HARMAN-27
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h> // PORTA DDRA 등의 symbom이 정의 되어 있다.

#define BUTTON_DDR  DDRD
#define BUTTON_PIN  PIND // PORTD를 read하는 register, 전압5v를 읽으면 1, 0v를 읽으면 0

#define BUTTON1_PIN 4 // PORTD.4
#define BUTTON2_PIN 5 // PORTD.5
#define BUTTON3_PIN 6 // PORTD.6
#define BUTTON4_PIN 7 // PORTD.7

/* 버튼 번호 */
#define BUTTON1 0
#define BUTTON2 1 
#define BUTTON3 2 
#define BUTTON4 3 

#define BUTTON_PRESS 1 //버튼을 누르면 high active-high
#define BUTTON_RELEASE 0 //버튼을 떼면 LOW
#define BUTTON_NUMBER 4 // 버튼 갯수는 4개다

#endif //  BUTTON_H

// portA는 write모드
// portD는 button이 연결되었음