/*
 * ultrasonic.h
 *
 * Created: 2024-05-08 오전 10:54:53
 *  Author: HARMAN-06
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#define F_CPU 16000000UL //16MHz
#include <avr/io.h> //PORTA DDRA 등의 symbol이 정의되어있음.
#include <util/delay.h> // _delay_ms 등의 함수가 들어있다.
#include <avr/interrupt.h>

#define TRIG_DDR DDRG
#define TRIG_PORT PORTG
#define TRIG 4

#define ECHO_DDR DDRE
#define ECHO_PIN PINE	//INT4
#define ECHO 4

void init_ultrasonic();
void ultrasonic_trigger();
void ultrasonic_distance_check();

#endif /* ULTRASONIC_H_ */