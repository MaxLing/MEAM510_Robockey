/*
 * puck.c
 *
 * Created: 11/13/2016 10:56:15 PM
 *  Author: wudao
 */ 

 #include "m_general.h"
 #include "ADC.h"
 #include "puck.h"
 #include "states.h"// need to set puck states 

 static float puck_angle = 0;

 #define noisebar 10

 void update_puck()
 {
	 float PT_angles[7] = {-2.3562, -0.7854, -0.2, 0, 0.2, 0.7854, 2.3562}; // 0 LR 45 135 PT configuration
	 float PT_ADC[8] = {0,0,0,0,0,0,0,0};
	 int total = 0;
	 int i = 0;

	 PT_ADC[0] = ADC0();
	 PT_ADC[1] = ADC1();
	 PT_ADC[2] = ADC2();
	 PT_ADC[3] = ADC3();
	 PT_ADC[4] = ADC4();
	 PT_ADC[5] = ADC5();
   	 PT_ADC[6] = ADC6();
	 PT_ADC[7] = ADC7();

	 for(i = 0; i<7; i++){
		 if(PT_ADC[i]<noisebar){
			 PT_ADC[i] = 0;
		 }
	 }
	 // we can choose not to filter noise for some important PT, now it's all clear

	 puck_angle = 0;
	 total = 0;


	 for(i = 0; i<7; i++){
		 puck_angle += PT_ADC[i]*PT_angles[i];
		 total += PT_ADC[i];
	 }
	 puck_angle = puck_angle/total;


	 if(total > 10)
	      {setPuckState(PUCK_VISIBLE);} 
	 else {setPuckState(PUCK_NDEF);} 
	 // need tuning

     if (PT_ADC[7] > 100)
	      {setPuckState(I_HAVE_PUCK);}//... tell teammate
	 else {setPuckState(PUCK_VISIBLE);}
	  // need tuning
 }

 float get_puck_angle() {
	 return puck_angle;
 }