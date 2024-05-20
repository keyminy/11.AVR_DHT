/*
 * uart1.h
 *
 * Created: 2024-05-07 오후 3:39:55
 *  Author: HARMAN-06
 */ 


#ifndef UART1_H_
#define UART1_H_

// led_all_on
volatile uint8_t rx1_buffer[100]; //uart0로 부터 들어온 문자를 저장하는 버퍼.
volatile uint8_t rx1_ready_flag=0; //완전한 문장(\n)을 만나면 1로 set된다.

void init_uart1(void);
void UART1_transmit(uint8_t data);
void bt_command_processing(void);

#endif /* UART1_H_ */