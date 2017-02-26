/*
 * motor.c
 *
 * Created: 11/17/2016 8:34:10 PM
 *  Author: wudao
 */ 

#include "m_general.h"
#include "motor.h"

volatile unsigned int left_resolution;
volatile unsigned int right_resolution;
static int left_on = 0;
static int right_on = 0;


void start_left(unsigned int resolution, float duty_cycle)
{
     left_resolution = resolution;

	 OCR1A = resolution; // frequency
	 OCR1B = (unsigned int)(((float)resolution) * duty_cycle); //duty cycle B6 output

	 if (!left_on)
	 {
     //prescaler: /8
     clear(TCCR1B, CS12);
     set(TCCR1B, CS11); 
     clear(TCCR1B, CS10);
	 //timer mode 15: UP to OCR1A, PWM mode
     set(TCCR1B, WGM13);
     set(TCCR1B, WGM12);
     set(TCCR1A, WGM11);
     set(TCCR1A, WGM10);
     //output options B6: clear at OCR1B, set at rollover
     set(TCCR1A, COM1B1);
     clear(TCCR1A, COM1B0);
     //set port B6 output
     set(DDRB, 6); 
     left_on = 1;
	 }
}

void stop_left()
{
     // turn off the clock
     clear(TCCR1B, CS12);
     clear(TCCR1B, CS11);
     clear(TCCR1B, CS10);
	 // set port B6 input
     clear(DDRB, 6);
     left_on = 0;
}


void start_right(unsigned int resolution, float duty_cycle)
{
	right_resolution = resolution;

	ICR3 = resolution; // frequency
	OCR3A = (unsigned int)(((float)resolution) * duty_cycle); //duty cycle C6 output

	if (!right_on)
	{
		//prescaler: /8
		clear(TCCR3B, CS32);
		set(TCCR3B, CS31);
		clear(TCCR3B, CS30);
		//timer mode 14: UP to ICR3, PWM mode
		set(TCCR3B, WGM33);
		set(TCCR3B, WGM32);
		set(TCCR3A, WGM31);
		clear(TCCR3A, WGM30);
		//output options C6: clear at OCR3A, set at rollover
		set(TCCR3A, COM3A1);
		clear(TCCR3A, COM3A0);
		//set port C6 output
		set(DDRC, 6);
		right_on = 1;
	}
}

void stop_right()
{
	// turn off the clock
	clear(TCCR3B, CS32);
	clear(TCCR3B, CS31);
	clear(TCCR3B, CS30);
	// set port C6 input
	clear(DDRC, 6);
	right_on = 0;
}