/*
 * comm.c
 *
 * Created: 11/19/2016 2:35:54 PM
 *  Author: wudao
 */ 

 #include <avr/io.h>
 #include "m_general.h"
 #include "m_rf.h"
 #include "states.h"

 char buffer[10] = {0,0,0,0,0,0,0,0,0,0};

 char message = 0;
 int i = 0;
 int run = 1;

 void rf_comm(char* buffer) {
	 while(run)
	 {
		 message = buffer[0];

		 switch(message){
		     case 0xA0: // Comm Test
			 for(i=0; i<10; i++){
				 set(PORTD,4); 	//Flash positioning LED 10 times, disable and enable input
				 clear(PORTD,4);}
			 setMessage(1);
			 break;

			 case 0xA1:	// PLAY
			 setMessage(2);
			 break;

			 case 0xA2: // GOAL R (STOP)
			 setMessage(3);
			 break;

			 case 0xA3: // GOAL B (STOP)
			 setMessage(4);
			 break;

			 case 0xA4: // PAUSE
			 setMessage(5);
			 break;

			 case 0xA6: // HALFTIME(STOP_)
			 setMessage(6);
			 break;

			 case 0xA7: // GAME OVER (STOP)
			 setMessage(7);
			 break;

			 default: setMessage(0);
		 }

		 run = 0;
	 }
 }
