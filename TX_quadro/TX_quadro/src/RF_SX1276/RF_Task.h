/*
 * RF_Task.h
 *
 * Created: 3.1.2014 21:32:01
 *  Author: JR
 */ 


#ifndef RF_TASK_H_
#define RF_TASK_H_

#include "Main.h"


void Send_data(uint8_t *data,uint8_t Length);
uint8_t Check_status(void);
void RF_Task(TASKS_STATE *Status,QUEUES_TASKS *Queue);
void RF_init(void);
char* My_strcat(char* dest, const char* src);



#endif /* RF_TASK_H_ */