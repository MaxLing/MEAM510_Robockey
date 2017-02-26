/*
 * motor.h
 *
 * Created: 11/17/2016 8:33:54 PM
 *  Author: wudao
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

void start_left(unsigned int resolution, float duty_cycle);
void stop_left();

void start_right(unsigned int resolution, float duty_cycle);
void stop_right();



#endif /* MOTOR_H_ */