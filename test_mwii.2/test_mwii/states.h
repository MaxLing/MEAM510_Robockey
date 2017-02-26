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
	WAITING = 0,
	STOPPED,
	MOVING
};

enum PUCKSTATE
{
	PUCK_NDEF = 0,
	I_HAVE_PUCK,
	TEAMMATE_HAS_PUCK,
	OPPONENT_HAS_PUCK,
	PUCK_VISIBLE
};

enum MOVINGSTATE
{
	MOVING_NDEF = 0,
	LOOKING_FOR_PUCK,
	MOVING_TO_GOAL,
	MOVING_TO_PUCK,
	MOVING_TO_OPPONENT
};

enum DRIVESTATE
{
	DRIVE_NDEF = 0,
	GOING,
	TURNING,
	CURVING
};

enum MESSAGE
{
	NONE = 0,
	COMM_TEST,
	PLAY,
	GOAL_R,
	GOAL_B,
	PAUSE,
	HALFTIME,
	GAME_OVER
};

void setMainState(int state);
enum MAINSTATE getMainState();
void setPuckState(int state);
enum PUCKSTATE getPuckState();
void setMovingState(int state);
enum MOVINGSTATE getMovingState();
void setDriveState(int state);
enum DRIVESTATE getDriveState();
void setMessage(int state);
enum MESSAGE getMessage();

void setPosition(float *pos);
float *getPosition();



#endif /* STATES_H_ */