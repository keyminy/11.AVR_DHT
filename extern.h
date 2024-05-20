/*
 * extern.h
 *
 * Created: 2024-04-29 오전 11:27:30
 *  Author: HARMAN-06
 */ 


#ifndef EXTERN_H_
#define EXTERN_H_

extern void shift_left2right_keep_ledon();
extern void shift_right2left_keep_ledon();
extern void init_button(void);
extern int get_button(int button_pin, int button_number);
extern void led_all_on_off(void);
extern void flower_on();
extern void flower_off();

extern void init_uart1(void);
extern void UART1_transmit(uint8_t data);
extern void bt_command_processing(void);

extern void init_uart0(void);
extern void UART0_transmit(uint8_t data);
extern void pc_command_processing(void);

extern void init_ultrasonic();
extern void ultrasonic_trigger();
extern void ultrasonic_distance_check();

extern void I2C_LCD_init(void);
extern void I2C_LCD_Test();

extern void I2C_LCD_clear(void);
extern void I2C_LCD_write_string_XY(uint8_t row, uint8_t col, char *string);
extern char scm[50];

extern void make_pwm_led_control(void);

extern volatile uint32_t ms_count;
extern volatile uint32_t shift_timer;
extern volatile	uint32_t ultrasonic_trigger_timer;



#endif /* EXTERN_H_ */