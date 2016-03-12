
#include <asf.h>
#include <conf_board.h>
#include <string.h>
#include "main.h"
#include "TaskSystem.h"


/************************************************************************/
/* System - start ON                                                    */
/************************************************************************/
void SystemStartON(void)
{
	//edma_enable(EDMA_CHMODE_PER0123_gc);
	//EDMA_InitRF();
}

/************************************************************************/
/*  System initialization                                               */
/************************************************************************/
void System_init(TASKS_STATE* ActualStates)
{
	ENUM_STATE_DEVICE	StateDevice;
	
	
// 	StateDevice=(ENUM_STATE_DEVICE) nvm_eeprom_read_byte(ADDR_STATE_DEVICE);
// 	
// 	if (StateDevice>STATE_ON)
// 	{
// 		StateDevice=STATE_OFF;
// 		nvm_eeprom_write_byte(ADDR_STATE_DEVICE,StateDevice);
// 	}
// 		
// 	if (StateDevice==STATE_ON)
// 	{
// 		ActualStates->XmegaState=TASK_START_ON;
// 		SystemStartON();
// 	}
// 	else
// 	{
// 		ActualStates->XmegaState=TASK_OFF;
// 	}
}


