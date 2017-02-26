/*
 * Location.c
 *
 * Created: 11/9/2016 9:58:19 PM
 *  Author: wudao
 */ 

#include "location.h"
#include "states.h"
#include "m_bus.h"
#include "m_general.h"
#include "m_wii.h"
#include <math.h>
#include <stdlib.h>

 #define pi 3.1416
 #define filter 0.02 // adjust maybe
 #define mrf_offset_x 505// 500
 #define mrf_offset_y 370 //375
 #define robot_offset_x 0
 #define robot_offset_y 0
 #define max_change 30// in cm
 
 static float position[2] = {0,0};
 static float orientation = 0;
 static float scale = 0; // use scale or max/min ration for 3 star calculation?
 static float location[3];
 static float prev_location[3]; // drop error location
 static char init_mwii = 0;
 static char init_location = 0;
 static int stars = 0; // static???
 
 void location_init()
 {
 if (m_wii_open()){init_mwii =1;}
 else {}//error
 }

 void location_update(unsigned int* data)
 {
 if (!init_mwii){location_init();}
 // if (m_wii_read(data)) 
 //{
 location_calculate(data);
 setPosition(location);
  //}
 //else {}//error
 }
 
 void countstar(unsigned int* data) // counting visible stars
 {
 static int lost[4] = {0,0,0,0};
 int i = 0;
 for (i=0;i<4;i++){
     if(data[i*3] ==1023 && data[i*3+1] ==1023)
	      {lost[i]=1;}
	 else {lost[i]=0;}
 stars = 4 - (lost[0]+lost[1]+lost[2]+lost[3]);
 }
 }

void location_calculate(unsigned int* data)
{
countstar(data);
switch(stars) 
{
	case 4:
	calculate4(data);
	break;

	case 3:
	calculate3(data);
	break;
	
	case 2:
	drop();
	break;
	
	case 1:
	drop();
	break;
	
	case 0:
	drop();
	break;
	
	default: 
	drop(); // caution
}

}

void calculate4(unsigned int* data)
{
    float dmax = 0;
    float d[4][4];
    int i = 0;
    int j = 0;
    int major_points[2];
    int rest_points[2];
    static int northstar;
    static int southstar;

	// find north and south stars
    for (i=0;i<4;i++){
	    for (j=0;j<4;j++){
		    float delta_x = abs((float)data[i*3] - (float)data[j*3]);
		    float delta_y = abs((float)data[i*3 + 1] - (float)data[j*3 + 1]);
		    d[i][j] = sqrtf(powf(delta_x, 2) + powf(delta_y, 2));
		    if (d[i][j] > dmax){
			    major_points[0] = i;
			    major_points[1] = j;
				dmax = d[i][j];
		    }
	    }
    }


	// find rest 2 stars
    i = 0;
	  for (j=0;j<4;j++){
		 if (major_points[0] != j && major_points[1] != j)
		 {  rest_points[i] = j;
		    i++;}
	  }

	// differ north and south stars 
    float dsum[2];
    dsum[0] = d[major_points[0]][rest_points[0]] + d[major_points[0]][rest_points[1]];
    dsum[1] = d[major_points[1]][rest_points[0]] + d[major_points[1]][rest_points[1]];
    if (dsum[0] < dsum[1]){
	    northstar = major_points[0];
        southstar = major_points[1];}
    else{northstar = major_points[1];
        southstar = major_points[0];}

    // scale
    scale = 29/(d[northstar][southstar]);

    // orientation
    float angle = atan2f(((float)data[3*northstar]-(float)data[3*southstar]), ((float)data[3*northstar+1]-(float)data[3*southstar+1])); 

    // position
    float center[2];
    center[0] = -((float)(data[(northstar) * 3] + data[(southstar) * 3])/2 - mrf_offset_x);
    center[1] = -((float)(data[(northstar) * 3 + 1] + data[(southstar) * 3 + 1])/2 - mrf_offset_y);
	
	float robot[2];
    robot[0] = cosf(angle) * center[0] - sinf(angle) * center[1] - robot_offset_x; //
    robot[1] = sinf(angle) * center[0] + cosf(angle) * center[1] - robot_offset_y;

    //add filters
    orientation = orientation * (filter) + (angle) * (1 - filter);
    position[0] = position[0] * (filter) + (robot[0]) * (1 - filter);
	position[1] = position[1] * (filter) + (robot[1]) * (1 - filter);
    
	// 180 offset for mwii
	orientation = orientation - pi;
	if (orientation < -pi) {orientation = orientation + 2*pi;}

	location[0] = (position[0])*(scale); //x
	location[1] = (position[1])*(scale); //y
	location[2] =  orientation;

	/*//save previous location
	prev_location[0] = location[0];
	prev_location[1] = location[1];
	prev_location[2] = location[2];

	if (!init_location )
	{
	location[0] = (position[0])*(scale); //x
	location[1] = (position[1])*(scale); //y
	location[2] =  orientation;
	init_location = 1;
	}

	else if ( (fabs(position[0] - prev_location[0]) < max_change) && (fabs(position[1] - prev_location[1]) < max_change) )
	{
    //return location
    location[0] = (position[0])*(scale); //x
    location[1] = (position[1])*(scale); //y
    location[2] =  orientation;
	}
	else {} // drop error location
	*/
}

void calculate3(unsigned int* data)
{
    drop(); // change later
}

void drop()
{
    // drop might be wrong!!!
	 location[0] = (position[0])*(scale);
	 location[1] = (position[1])*(scale);
	 location[2] = orientation;
}