
/*
 * ADC.c
 *
 * Created: 11/14/2016 10:16:30 PM
 *  Author: wudao
 */ 
  #include "m_general.h"
  #include "ADC.h"
 
  void initADC()
  {
  // voltage reference Vcc
  clear(ADMUX,REFS1);
  set(ADMUX,REFS0); 

  //ADC Prescaler : /128
  set(ADCSRA, ADPS2);
  set(ADCSRA, ADPS1);
  set(ADCSRA, ADPS0);

  m_disableJTAG(); // enable F4-F7 as GPIO
  }


  float ADC0() //F0
  {
   clear(ADCSRA, ADEN); // prepare for channel change
   set(DIDR0, ADC0D); // disable F0 digital input

   // channel selection F0
   clear(ADCSRB, MUX5);
   clear(ADMUX, MUX2);
   clear(ADMUX, MUX1); 
   clear(ADMUX, MUX0);                 
   
   //enable and begin ADC conversion
   set(ADCSRA, ADEN); 
   set(ADCSRA, ADSC); 
   while(!check(ADCSRA,ADIF)){}
   float adc = (float)ADC;
   return adc;
   set(ADCSRA,ADIF); // wait for flag, read ADC and then reset the flag
  }

  float ADC1() //F1
  {
   clear(ADCSRA, ADEN); // prepare for channel change
   set(DIDR0, ADC1D); // disable F1 digital input

   // channel selection F1
   clear(ADCSRB, MUX5);
   clear(ADMUX, MUX2);
   clear(ADMUX, MUX1);
   set(ADMUX, MUX0);
  
   //enable and begin ADC conversion
   set(ADCSRA, ADEN);
   set(ADCSRA, ADSC);
   while(!check(ADCSRA,ADIF)){}
   float adc = (float)ADC;
   return adc;
   set(ADCSRA,ADIF); // wait for flag, read ADC and then reset the flag
  }

  float ADC2() //F4
  {
	   clear(ADCSRA, ADEN); // prepare for channel change
	   set(DIDR0, ADC4D); // disable F4 digital input

	   // channel selection F4
	   clear(ADCSRB, MUX5);
	   set(ADMUX, MUX2);
	   clear(ADMUX, MUX1);
	   clear(ADMUX, MUX0);
	   
	   //enable and begin ADC conversion
	   set(ADCSRA, ADEN);
	   set(ADCSRA, ADSC);
	   while(!check(ADCSRA,ADIF)){}
	   float adc = (float)ADC;
	   return adc;
	   set(ADCSRA,ADIF); // wait for flag, read ADC and then reset the flag
  }

  float ADC3() //F5
  {
	  clear(ADCSRA, ADEN); // prepare for channel change
	  set(DIDR0, ADC5D); // disable F5 digital input

	  // channel selection F5
	  clear(ADCSRB, MUX5);
	  set(ADMUX, MUX2);
	  clear(ADMUX, MUX1);
	  set(ADMUX, MUX0);
	  
	  //enable and begin ADC conversion
	  set(ADCSRA, ADEN);
	  set(ADCSRA, ADSC);
	  while(!check(ADCSRA,ADIF)){}
	  float adc = (float)ADC;
	  return adc;
	  set(ADCSRA,ADIF); // wait for flag, read ADC and then reset the flag
  }

  float ADC4() //F6
  {
	  clear(ADCSRA, ADEN); // prepare for channel change
	  set(DIDR0, ADC6D); // disable F6 digital input

	  // channel selection F6
	  clear(ADCSRB, MUX5);
	  set(ADMUX, MUX2);
	  set(ADMUX, MUX1);
	  clear(ADMUX, MUX0);
	  
	  //enable and begin ADC conversion
	  set(ADCSRA, ADEN);
	  set(ADCSRA, ADSC);
	  while(!check(ADCSRA,ADIF)){}
	  float adc = (float)ADC;
	  return adc;
	  set(ADCSRA,ADIF); // wait for flag, read ADC and then reset the flag
  }

  float ADC5() //F7
  {
	  clear(ADCSRA, ADEN); // prepare for channel change
	  set(DIDR0, ADC7D); // disable F7 digital input

	  // channel selection F7
	  clear(ADCSRB, MUX5);
	  set(ADMUX, MUX2);
	  set(ADMUX, MUX1);
	  set(ADMUX, MUX0);
	  
	  //enable and begin ADC conversion
	  set(ADCSRA, ADEN);
	  set(ADCSRA, ADSC);
	  while(!check(ADCSRA,ADIF)){}
	  float adc = (float)ADC;
	  return adc;
	  set(ADCSRA,ADIF); // wait for flag, read ADC and then reset the flag
  }

  float ADC6() //D6
  {
	  clear(ADCSRA, ADEN); // prepare for channel change
	  set(DIDR2, ADC9D); // disable D6 digital input

	  // channel selection D6
	  set(ADCSRB, MUX5);
	  clear(ADMUX, MUX2);
	  clear(ADMUX, MUX1);
	  set(ADMUX, MUX0);
	  
	  //enable and begin ADC conversion
	  set(ADCSRA, ADEN);
	  set(ADCSRA, ADSC);
	  while(!check(ADCSRA,ADIF)){}
	  float adc = (float)ADC;
	  return adc;
	  set(ADCSRA,ADIF); // wait for flag, read ADC and then reset the flag
  }

  float ADC7() //D7
  {
	  clear(ADCSRA, ADEN); // prepare for channel change
	  set(DIDR2, ADC10D); // disable D7 digital input

	  // channel selection D7
	  set(ADCSRB, MUX5);
	  clear(ADMUX, MUX2);
	  set(ADMUX, MUX1);
	  clear(ADMUX, MUX0);
	  
	  //enable and begin ADC conversion
	  set(ADCSRA, ADEN);
	  set(ADCSRA, ADSC);
	  while(!check(ADCSRA,ADIF)){}
	  float adc = (float)ADC;
	  return adc;
	  set(ADCSRA,ADIF); // wait for flag, read ADC and then reset the flag
  }