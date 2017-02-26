/*
 * CFile1.c
 *
 * Created: 11/19/2016 3:57:05 PM
 *  Author: wudao
 */ 

 #include "states.h"

 enum MAINSTATE current_mainstate;
 enum PUCKSTATE current_puckstate;

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

 void setPosition(float *pos)
 {
	 current_position = pos; 
 }

 float *getPosition()
 {
	 return current_position;
 }