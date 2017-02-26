/*
 * CFile1.c
 *
 * Created: 11/19/2016 3:57:05 PM
 *  Author: wudao
 */ 

 #include "states.h"

 enum MAINSTATE current_mainstate;
 enum PUCKSTATE current_puckstate;
 enum MOVINGSTATE current_movingstate;
 enum DRIVESTATE current_drivestate;
 enum MESSAGE current_message;

 float *current_position;

 
 void setMainState(int state)
 {
	 current_mainstate = state;
 }

 enum MAINSTATE getMainState()
 {
	 return current_mainstate;
 }

 void setPuckState(int state)
 {
	 current_puckstate = state;
 }

 enum PUCKSTATE getPuckState()
 {
	 return current_puckstate;
 }

 void setMovingState(int state)
 {
	 current_movingstate = state;
 }

 enum MOVINGSTATE getMovingState()
 {
	 return current_movingstate;
 }

void setDriveState(int state)
{
	current_drivestate = state;
}

enum DRIVESTATE getDriveState()
{
	return current_drivestate;
}

 void setMessage(int state)
 {
	 current_message = state;
 }

 enum MESSAGE getMessage()
 {
	 return current_message;
 }

 void setPosition(float *pos)
 {
	 current_position = pos; 
 }

 float *getPosition()
 {
	 return current_position;
 }