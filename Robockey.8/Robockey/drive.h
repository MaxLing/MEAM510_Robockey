/*
 * drive.h
 *
 * Created: 11/17/2016 9:34:41 PM
 *  Author: wudao
 */ 


#ifndef DRIVE_H_
#define DRIVE_H_

void set_power(float dpower);
void stop();
void search();
void diff(float turn_target);
void move(int x, int y);
void curve(float turn_target);//angle = puck angle
void turn(float turn_target); // necessary?
void goStraight(float velocity, int direction);//necessary
void left_on(float dpower, int direction);
void left_off();
void right_on(float dpower, int direction);
void right_off();



#endif /* DRIVE_H_ */