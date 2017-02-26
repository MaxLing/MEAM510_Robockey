 /*
 * drive.c
 *
 * Created: 11/16/2016 8:34:16 PM
 *  Author: wudao
 */ 

#include "drive.h"
#include "location.h"
#include "m_general.h"
#include "states.h"
#include <math.h>

#define forwards 1
#define backwards 0
#define pi 3.1416
#define Kp_move 0.5   // need tuning
#define Ki_move 0
#define Kd_move 0
#define Max_delta_angle 1.5708 //90 degree
#define Kp_curve 0.6 // red 0.6
#define Ki_curve 0
#define Kd_curve 0
static float power; // = INITIAL_POWER in main.c

static float delta_angle_prev = 0; // for move
static float delta_angle = 0;
static float pk_angle = 0; //for curve
static float pk_angle_prev = 0;
static float* location;

#define left_resolution 1024 // may change
#define right_resolution 1024
static int L_on = 0;
static int R_on = 0;




void set_power(float dpower){
    power = dpower;
	}

void stop(){
	right_off();
	left_off();
}

void search(){
	location = getPosition();
	right_on(0.2, forwards);
	left_on(0.2, backwards);
}

void diff(float turn_target) { // optimize turning
	location = getPosition();
	delta_angle = turn_target - location[2];//current_angle

	// delta angle rollover
	while(fabs(delta_angle) > pi) {
		if(delta_angle > 0) {delta_angle = delta_angle - 2 * pi;}
		else {delta_angle = delta_angle + 2 * pi;}
	}
}

void turn(float turn_target) // add a min power?
{
    diff(turn_target);
	if(delta_angle > 0) { // !careful direction
		left_on(power, forwards);
		right_on(power, backwards);
		} else {
		left_on(power, backwards);
		right_on(power, forwards);
	    }
}

void goStraight(float velocity, int direction)
{   
    left_on(velocity,direction);
	right_on(velocity,direction);
}

void move(int x, int y){
    location = getPosition();
    float target_angle = atan2f(y - location[1], x - location[0]);

    //change target angle to positive 0-2pi
    //if(target_angle < 0) {target_angle = target_angle + 2 * pi;}
    diff(target_angle); //update delta angle

	float P_move;
	static float I_move;
	float D_move;
	//drive PID
	P_move = delta_angle * Kp_move; // Proportional
	I_move = I_move + delta_angle * Ki_move; // Integral
	D_move = (delta_angle - delta_angle_prev) * Kd_move; // Derivative
	delta_angle_prev = delta_angle;
	
	float PID_move = fabs(P_move+I_move+D_move);
	if(PID_move > 1) {PID_move = 1;}
    if(fabs(delta_angle) > Max_delta_angle) {
	    if(delta_angle > 0) 
		     {left_on(PID_move, forwards);
		      right_on(PID_move, backwards);} 
		else {left_on(PID_move, backwards);
		      right_on(PID_move, forwards);}
	    } 
	else {
	    float left_power;
	    float right_power;
	    if (delta_angle > 0)
	    {left_power = power + PID_move;
	    right_power = power - PID_move;}// !careful direction
	    else if (delta_angle < 0) {
		    left_power = power - PID_move;
		    right_power = power + PID_move;
	    }
		else {left_power = power + PID_move;
			right_power = power + PID_move;}

	    if(left_power > 1) {left_power = 1;}
	    if(left_power < 0) {left_power = 0;}
	    if(right_power > 1) {right_power = 1;}
	    if(right_power < 0) {right_power = 0;}

	    left_on(left_power, forwards);
	    right_on(right_power, forwards);
    }

    //if(fabs(location[0] - target_x) < 5 && fabs(location[1] - target_y) < 5) 
	//{stop();}
}

void curve(float turn_target) // angle = puck_angle
{
pk_angle = turn_target;

float P_curve = 0;
static float I_curve = 0;
float D_curve = 0;
//drive PID
P_curve = pk_angle * Kp_curve; // Proportional
I_curve = I_curve + pk_angle * Ki_curve; // Integral
D_curve = (pk_angle - pk_angle_prev) * Kd_curve; // Derivative
pk_angle_prev = pk_angle;

float PID_curve = fabs(P_curve + I_curve + D_curve);

float left_power;
float right_power;
if (pk_angle > 0)
{left_power = power + PID_curve;
 right_power = power - PID_curve;}// !careful direction
else if (pk_angle < 0) {
	left_power = power - PID_curve;
	right_power = power + PID_curve;
}
else {left_power = power + PID_curve;
right_power = power + PID_curve;}

if(left_power > 1) {left_power = 1;}
if(left_power < 0) {left_power = 0;}
if(right_power > 1) {right_power = 1;}
if(right_power < 0) {right_power = 0;}

left_on(left_power, forwards);
right_on(right_power, forwards);

}

void left_on(float dpower, int direction) // change to B0B1
{
	if(dpower > 1) {dpower = 1;}
	if (direction == forwards) // !careful direction
	{set(PORTB, 0);clear(PORTB, 1);} 
	else {clear(PORTB, 0);set(PORTB, 1);}
	
	OCR1A = left_resolution; // frequency
	OCR1B = (int)(left_resolution * dpower); //duty cycle B6 output

	if (!L_on)
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
		L_on = 1;
	}
	
}

void right_on(float dpower, int direction) // change to B2B3
{
	if(dpower > 1) {dpower = 1;}
	if (direction == forwards) // !careful direction
	{set(PORTB, 2);clear(PORTB, 3);}
	else {clear(PORTB, 2);set(PORTB, 3);}
	ICR3 = right_resolution; // frequency
	OCR3A = (int)(right_resolution * dpower); //duty cycle C6 output

	if (!R_on)
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
		R_on = 1;
	}
	
}

void left_off(){
    // turn off the clock
	//clear(TCCR1B, CS12);
	//clear(TCCR1B, CS11);
	//clear(TCCR1B, CS10);
	// set port B6 input
	clear(DDRB, 6);
	L_on = 0;
	}


void right_off(){
    // turn off the clock
	//clear(TCCR3B, CS32);
	//clear(TCCR3B, CS31);
	//clear(TCCR3B, CS30);
	// set port C6 input
	clear(DDRC, 6);
	R_on = 0;
	}