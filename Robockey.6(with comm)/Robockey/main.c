/*
 * Robockey.c
 *
 * Created: 11/9/2016 9:36:26 PM
 * Author : wudao
 */ 

#include <avr/io.h>
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_wii.h"
#include "m_usb.h"
#include "ADC.h"
#include "drive.h"
#include "location.h"
#include "states.h"

#define CHANNEL 1
#define T1_ADDRESS 0x38 //yellow
#define MY_ADDRESS 0x39 //red
#define T2_ADDRESS 0x40 //blue
#define BUFFER_SIZE 10
#define MWII_FREQUENCY 250// should be less than 256 // 625
#define INITIAL_POWER 0.5

#define COMM_TEST 0xA0			// wireless communication states
#define PLAY 0xA1			
#define GOAL_R 0xA2		
#define GOAL_B 0xA3		
#define PAUSE 0xA4			
#define HALFTIME 0xA6		
#define GAME_OVER 0xA7
#define ASSIST 0xA8
#define DEFENSE 0xA9


#define DEBUG_LOCALIZATION 0		// if you want to debug the robot's localization, set this to 1
#define DEBUG_PHOTO 0				// if you want to debug the PT ADC inputs, set this to 1		
#define DEBUG_GOAL 0

char buffer[BUFFER_SIZE];
char message;
static unsigned int data[12];
float* position;

static float puck_angle = 0; // for puck angle
float PT_ADC[5] = {0,0,0,0,0}; // for ADC debug

int RF_READ = 0;
volatile int receive = 0;

int LED_pin = 5;  // switches ON: Blue, D5, goal left
int goal_x = 115; // switches OFF: Red, D3, goal right
int goal_y = 0; // need tuning
int home_x = -70; //need tuning in here and init
int home_y = 0;

void debug_localization();
void debug_photo();
void debug_goal();

void report_error();
void initialize();
void update_puck();

void shoot();
void comm_test();

void init0(unsigned int frequency); // timer0 init for mwii reading
void interupt0(bool on);

int main(void)
{
    initialize();
	m_red(ON);
	m_green(ON);
	while (1) 
    {
    
	if(DEBUG_LOCALIZATION){				
		while(1){						// this loop will endlessly repeat the debug_localization routine			
			debug_localization();		
		}								
	}									
	
	if(DEBUG_PHOTO){			
		while(1){				// this loop will endlessly repeat the debug_photo routine	
			debug_photo();		
		}						
	}
    
	if(DEBUG_GOAL){
		while(1){				// this loop will endlessly repeat the debug_photo routine
			debug_goal();
		}
	}

	//left_on(0.3,1);
	//right_on(0.3,1);
	//goStraight(0.3,0);// test
    //search();
	
	//move(goal_x,goal_y);//qualification
	//curve(puck_angle); // find puck
	
	if(receive){
	    m_wii_read(data);
		location_update(data);
    	m_green(TOGGLE);
		receive = 0;
	}

	update_puck();


	if(RF_READ) {
		m_rf_read(buffer, BUFFER_SIZE);
		message = buffer[0];
		switch(message){
			default:
			setMainState(COMM);//should be ?
			break;
			case COMM_TEST:
			setMainState(COMM);
			break;
			case PLAY:
			setMainState(RETURN);
			//should be setMainState(SEARCH);
			break;
			case GOAL_R:
			setMainState(WAIT);
			break;
			case GOAL_B:
			setMainState(WAIT);
			break;
			case PAUSE:
			setMainState(WAIT);
			break;
			case HALFTIME:
			setMainState(WAIT); // how to deal with halftime change?
			break;
			case GAME_OVER:
			setMainState(WAIT);
			break;

			case ASSIST:
			setMainState(RUSH);
			break;

			case DEFENSE:
			setMainState(RETURN);
			break;
		}
		RF_READ = 0;
		m_red(TOGGLE);
	}
   
	switch(getMainState()){
		default:
		setMainState(WAIT); // should be wait?
		break;

		case WAIT:
		stop(); // in drive.c
		break;

		case RETURN:
		position = getPosition();
		if((fabs(position[0] - home_x) < 5) && (fabs(position[1] - home_y) < 5))
		{stop();setMainState(SEARCH);}
		else{move(home_x,home_y);}
		break;

		case SEARCH:
		if(getPuckState()==PUCK_VISIBLE)
		{setMainState(APPROACH_PUCK);}
		else{search();}
		break;

		case APPROACH_PUCK:
		if(getPuckState()==I_HAVE_PUCK){
			setMainState(APPROACH_GOAL);
			char buffer_T2[BUFFER_SIZE] = {DEFENSE,DEFENSE,DEFENSE,DEFENSE,DEFENSE,DEFENSE,DEFENSE,DEFENSE,DEFENSE,DEFENSE};
			m_rf_send(T2_ADDRESS, buffer_T2, BUFFER_SIZE); // for blue it's defense anyway
		}
		else if(getPuckState()==PUCK_NDEF){
			setMainState(SEARCH);
			char buffer_T2[BUFFER_SIZE] = {DEFENSE,DEFENSE,DEFENSE,DEFENSE,DEFENSE,DEFENSE,DEFENSE,DEFENSE,DEFENSE,DEFENSE};
			m_rf_send(T2_ADDRESS, buffer_T2, BUFFER_SIZE); // for blue it's defense anyway
		}
		else{curve(puck_angle);}
		set(PORTD,LED_pin);
		break;

		case APPROACH_GOAL:
		//if close enough
		//	setMainState(SHOOT);

		if( getPuckState()!= I_HAVE_PUCK ){
			setMainState(APPROACH_PUCK);
			char buffer_T1[BUFFER_SIZE] = {PLAY,PLAY,PLAY,PLAY,PLAY,PLAY,PLAY,PLAY,PLAY,PLAY};
			m_rf_send(T1_ADDRESS, buffer_T1, BUFFER_SIZE);
		}
		else{move(goal_x, goal_y);
		    char buffer_T1[BUFFER_SIZE] = {ASSIST,ASSIST,ASSIST,ASSIST,ASSIST,ASSIST,ASSIST,ASSIST,ASSIST,ASSIST};
		    m_rf_send(T1_ADDRESS, buffer_T1, BUFFER_SIZE);
			}
		clear(PORTD,LED_pin);
		break;

		case SHOOT:
		shoot();
		setMainState(SEARCH);
		break;

		case COMM:
		comm_test();
		setMainState(WAIT);
		break;

		case RUSH:
		move(goal_x, goal_y);
		break;
	}


    }
}


void debug_localization(){
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
		 location_update(data);
		 position = getPosition();
		 receive = 0;
		 m_green(TOGGLE);
	 }
}

void debug_photo(){
     update_puck();
	 m_green(TOGGLE);
	 m_usb_tx_string("PA: ");
	 m_usb_tx_int(puck_angle/3.1416*180);		
	 m_usb_tx_string("\t ADC0: ");
	 m_usb_tx_int(PT_ADC[0]);			
	 m_usb_tx_string("\t ADC1: ");
	 m_usb_tx_int(PT_ADC[1]);
	 m_usb_tx_string("\t ADC2: ");
	 m_usb_tx_int(PT_ADC[2]);
	 m_usb_tx_string("\t ADC3: ");
	 m_usb_tx_int(PT_ADC[3]);
	 m_usb_tx_string("\t ADC4: ");
	 m_usb_tx_int(PT_ADC[4]);
     m_usb_tx_string("\t state: ");
	 m_usb_tx_int(getPuckState());
	 m_usb_tx_string("\n");

}

void debug_goal(){
     m_green(TOGGLE);
	 m_usb_tx_string("GX: ");
	 m_usb_tx_int(goal_x);
	 m_usb_tx_string("\t GY: ");
	 m_usb_tx_int(goal_y);
	 m_usb_tx_string("\n");
}



void report_error() 
{
    m_red(ON);
	if(m_usb_isconnected()) {m_usb_tx_string("error!\n\r");}
}

void initialize() {
	m_clockdivide(0); //the system clock must have 16 MHz frequency to use mBUS
	m_red(OFF);
	m_green(OFF);

    //enable inputs for switches,  enable the internal pull-up resistor
	clear(DDRB, 4);
	set(PORTB, 4);
	clear(DDRB, 5);
	set(PORTB, 5);

    // use a switch on B4 for team
	if(!check(PINB,4)) 
	{LED_pin = 3;} 
    //use a switch on B5 for goal
    if(!check(PINB,5)) 
	{goal_x = -115;
	 home_x = 70;}
	
	// common cathode for LED input
	clear(DDRD, 4); 
	set(PORTD,4);
	//Enabling Positioning LED Pins for output
	set(DDRD, LED_pin);
	set(PORTD, LED_pin);

    // driver board PWM line B6,C6
	// enable direction line B0B1 for left motor, B2B3 for right motor
	set(DDRB, 0);
	set(DDRB, 1);
	set(DDRB, 2);
	set(DDRB, 3);

	//enable kicker ports
	set(DDRB, 7);

	set_power(INITIAL_POWER);

	//initialize mRF
	m_bus_init();
	m_rf_open(CHANNEL, MY_ADDRESS, BUFFER_SIZE);

	// initialize mWii
	m_wii_open();

	//start timer0
	init0(MWII_FREQUENCY);
	interupt0(1);

	// start ADC
	initADC();

	sei(); //enable interrupts
	
	if ( DEBUG_LOCALIZATION || DEBUG_PHOTO || DEBUG_GOAL){		// if you are debugging...
		m_usb_init(); 								// connect USB
		while(!m_usb_isconnected()){};				// wait for connection
	}
	
	//setMainState(SEARCH); // testing without mRF
	//setMainState(RETURN);
}

void update_puck()
{
	float PT_angles[5] = {-2.3562, -0.7854, 0, 0.7854, 2.3562}; // 0 LR 45 135 PT configuration
	int total = 0;
	int j = 0;

	PT_ADC[0] = ADC0();
	PT_ADC[1] = ADC1();
	PT_ADC[2] = ADC2();
	PT_ADC[3] = ADC3();
	PT_ADC[4] = ADC4();


	for(j = 0; j<5; j++){
		if(PT_ADC[j] < 100){ // noise bar need tuning
			PT_ADC[j] = 0;
		}
	}
	// we can choose not to filter noise for some important PT, now it's all clear

	puck_angle = 0;
	total = 0;

	for(j = 0; j<5; j++){
		puck_angle = puck_angle + PT_ADC[j]*PT_angles[j]; // maybe use less PT
		total = total + PT_ADC[j];
	}
	puck_angle = puck_angle/total;


	if(total > 100)// need tuning
	//{if ((PT_ADC[2] > 1000 ) && (PT_ADC[2] < 1010 ) && (PT_ADC[1] > 990) && (PT_ADC[3] > 990 ) )// red+tp
	{if ((PT_ADC[2] > 1000 ) && (PT_ADC[2] < 1010 ) && ((PT_ADC[1] > 975) || (PT_ADC[3] > 975 )) )//yellow+tp
	{setPuckState(I_HAVE_PUCK);}
	else {setPuckState(PUCK_VISIBLE);}
	}
	else {setPuckState(PUCK_NDEF);}
	
	
}

void shoot(){
	//kick the puck into the goal
	set(PORTB, 7);
	m_wait(100);
	clear(PORTB, 7);
}


void comm_test(){
     int i; // for flashing
     for(i=0;i<10;i++){
     clear(PORTD,LED_pin);
	 m_wait(5000);
	 set(PORTD,LED_pin);
	 m_wait(5000);} // flash for 10 times				
     }

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
		set(TIMSK0, OCIE0A); //enable timer interupt
		sei(); //turn on interupts
	}
	else
	{   clear(TIMSK0, OCIE0A);}
}


ISR(INT2_vect)
{
	RF_READ = 1;
}

ISR(TIMER0_COMPA_vect){
	if(receive) {}
	else
	{receive = 1;}
}

