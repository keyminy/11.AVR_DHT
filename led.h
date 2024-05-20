/*
 * led.h
 *
 * Created: 2024-04-29 오전 10:58:14
 *  Author: HARMAN-27
 */ 

#ifndef LED_H_
#define LED_H_

void shift_left2right_keep_ledon(int* pjob);
void shift_right2left_keep_ledon(int* pjob);
void shift_left22right_keep_ledon();
void shift_right22left_keep_ledon();
void led_all_on_off();
void flower_on(int* pjob);
void flower_off(int* pjob);
void flower_on2();
void flower_off2();
void make_pwm_led_control(void);


#endif /* LED_H_ */