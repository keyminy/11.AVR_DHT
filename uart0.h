/*
 * uart0.h
 *
 * Created: 2024-04-30 오전 10:38:00
 *  Author: HARMAN-06
 */ 


#ifndef UART0_H_
#define UART0_H_
// led_all_on
volatile uint8_t rx_buffer[COMMAND_NUMBER][COMMAND_LENGTH]; //uart0로 부터 들어온 문자를 저장하는 버퍼.
volatile int rear=0;
volatile int front=0;

void init_uart0(void);
void UART0_transmit(uint8_t data);
void pc_command_processing(void);

#endif /* UART0_H_ */