/*
 * comm.h
 *
 * Created: 11/19/2016 2:35:39 PM
 *  Author: wudao
 */ 


#ifndef COMM_H_
#define COMM_H_

#define packet_length 10

char buffer[packet_length];
char message;
void rf_comm(char* buffer);



#endif /* COMM_H_ */