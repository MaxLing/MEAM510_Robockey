/*
 * states.h
 *
 * Created: 11/19/2016 3:57:32 PM
 *  Author: wudao
 */ 


#ifndef STATES_H_
#define STATES_H_

enum MAINSTATE
{
	WAIT = 0, // pause is for COMM
	RETURN,
	SEARCH,
	APPROACH_PUCK,
	APPROACH_GOAL,
	SHOOT,
	COMM,
	RUSH
};

enum PUCKSTATE
{
	PUCK_NDEF = 0,
	PUCK_VISIBLE,
	I_HAVE_PUCK,
	TEAMMATE_HAS_PUCK,
	OPPONENT_HAS_PUCK,
};

void setMainState(int state);
enum MAINSTATE getMainState();
void setPuckState(int state);
enum PUCKSTATE getPuckState();

void setPosition(float *pos);
float *getPosition();



#endif /* STATES_H_ */