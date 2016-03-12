/*
 * TaskSystem.h
 *
 * Created: 26. 8. 2013 10:18:10
 *  Author: Jirka
 */ 


#ifndef TASKSYSTEM_H_
#define TASKSYSTEM_H_

typedef enum {	COMMAND_SYSTEM_TO_RF,
				COMMAND_SYSTEM_TO_ACCELEROMETER}		ENUM_TYPE_OF_TASK_COMMAND;
				
typedef enum {	DOG_MOVING_STATE_OFF=0,
				DOG_MOVING_STATE_READY,
				DOG_MOVING_STATE_RUN,
				DOG_MOVING_STATE_PREPARATION_STOP_SOUND
				}										ENUM_EVOLUTION_DOG_MOVING;
				
typedef enum {
				WITHOUT_SOUND_1=0,
				WITH_SOUND_1,
				WITHOUT_SOUND_2,
				WITH_SOUND_2,
				WITHOUT_SOUND_3
				}										ENUM_EVOLUTION_DOG_MOVING_SUBSYSTEM;
				
typedef struct{
				uint8_t							CounterRTC_UltraLight;
				uint8_t							CounterRTC_Accelerometer;
				uint8_t							CounterRTC_AccChangeStateSignal;						
				}										STRUC_RTC_COUNTER;
				
typedef struct{
				uint8_t							Counter;
				uint8_t							ActualState;
				uint8_t							OldState;
				}										STRUCT_ITEM_STATE_MACHINE;
				
typedef struct{
				STRUCT_ITEM_STATE_MACHINE		MachineChangeStateAccelerometer;
				STRUCT_ITEM_STATE_MACHINE		MachineInformationBeep;
				STRUCT_ITEM_STATE_MACHINE		MachineInformationHorn;
				STRUCT_ITEM_STATE_MACHINE		MachineEvolutionDogMoving;
				STRUCT_ITEM_STATE_MACHINE		MachineEvolutionDogMovingSubsystem;
				STRUCT_ITEM_STATE_MACHINE		MachineVibration;
				STRUCT_ITEM_STATE_MACHINE		MachineUltraLight;
				}										STRUC_STATE_MACHINE_PERIPHERY;

//Public functions
void CallBackRTC(uint32_t Time);
void System_init(TASKS_STATE* ActualStates);
void System_run(TASKS_STATE* ActualStates,QUEUES_TASKS* ActualQueues);


#endif /* TASKSYSTEM_H_ */