/*
 * Robockey.c
 *
 * Created: 11/9/2016 9:36:26 PM
 * Author : wudao
 */ 

#include "m_general.h"
#include "m_wii.h"
#include "location.h"
#include "timer0.h"
#include "states.h"
#include "m_usb.h"

#define MWII_FREQUENCY 625

static unsigned int data[12];
float* position;
volatile int receive = 0;

void initialize();


int main(void)
{   
    initialize();
	m_green(ON);
    while (!m_usb_isconnected()){};
	m_red(ON);
	while(1)
	{      
	 m_usb_tx_string("X: ");
		   m_usb_tx_int(position[0]);
		   m_usb_tx_string("\t");
	 m_usb_tx_string("Y: ");
		   m_usb_tx_int(position[1]);
		   m_usb_tx_string("\t");
     m_usb_tx_string("A: ");
		   m_usb_tx_int(180/3.1416*position[2]);
		   m_usb_tx_string("\n");
		   if(receive){
		   m_wii_read(data);
		   receive = 0;
		   location_update(data);
		   position = getPosition();	
		   m_green(TOGGLE);	}
		
}
}

void initialize() {
	m_red(OFF);
	m_green(OFF);
	m_clockdivide(0); //the system clock must have 16 MHz frequency to use mBUS
	//start timer0
	
	m_usb_init();
	m_wii_open();
	init0(MWII_FREQUENCY);
	interupt0(1);
}

ISR(TIMER0_COMPA_vect){
    if(receive) {m_red(TOGGLE);}
    else 
	{receive = 1;
	}
	}

