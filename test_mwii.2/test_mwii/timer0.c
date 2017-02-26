/*
 * timer0.c
 *
 * Created: 11/19/2016 6:55:04 PM
 *  Author: wudao
 */ 

 #include "m_general.h"
 #include "timer0.h"

 void init0(unsigned int frequency)
 {
	  OCR0A = frequency;

      //clock prescaler /1024
      set(TCCR0B, CS02);    
      clear(TCCR0B, CS01); 
      set(TCCR0B, CS00);    

      //timer mode: up to OCR0A
      clear(TCCR0B, WGM02);
      set(TCCR0A, WGM01);
      clear(TCCR0A, WGM00);
 }

 void interupt0(bool on)
 {    if(on)
 {
	 set(TIMSK0 ,OCIE0A); //enable timer interupt
	 sei(); //turn on interupts
 }
      else
 {   clear(TIMSK0, TOIE0);}
 }