/*
 * RF_Task.c
 *
 * Created: 3.1.2014 21:30:17
 *  Author: JR
 */ 
#include <asf.h>
//#include <platform.h>
#include "RF_Task.h"
#include "string.h"
//#include "Main.h"
#include "sx1276-Hal.h"
#include "sx1276.h"
#include "sx1276-LoRa.h"
#include "sx1276-Fchp.h"


char string[20];

#define RxBufferSize 20
uint16_t BufferSize = 5;

		
extern volatile bool Tx_running;
extern tSX1276LR SX1276LR;
const char paket[]="12345678901234567890123456789";

void RF_init(void)
{	
	SX1276Init();
	delay_ms(20);
	
	/* /Reruseni IRQ0 */
	PORTC.INTMASK |= PIN2_bm;
	PORTC.INTCTRL |= PORT_INTLVL_LO_gc;
	
	
}
// 
// /************************************************************************/
// 
void Send_data(uint8_t *data,uint8_t Length)
{
	uint8_t Temp=0;
	uint16_t Timeout=2600;	// u kzadeho oboju musi byt jinak ?!?!?
	
	// see errata note
	//SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
	SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );

	 SX1276LR.RegIrqFlagsMask = 
	 RFLR_IRQFLAGS_RXTIMEOUT |
	 RFLR_IRQFLAGS_RXDONE |
	 RFLR_IRQFLAGS_PAYLOADCRCERROR |
	 RFLR_IRQFLAGS_VALIDHEADER |
	 //RFLR_IRQFLAGS_TXDONE |
	 RFLR_IRQFLAGS_CADDONE |
	 RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL
	 //|RFLR_IRQFLAGS_CADDETECTED
	 ;
			 
	SX1276LR.RegHopPeriod = 0;
	
	SX1276Write(REG_LR_HOPPERIOD, SX1276LR.RegHopPeriod );	//0x1C
	SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR.RegIrqFlagsMask );

	// Initializes the payload size
	SX1276LR.RegPayloadLength = Length;
	SX1276Write(REG_LR_PAYLOADLENGTH, SX1276LR.RegPayloadLength );	//0x17
	
	SX1276LR.RegFifoTxBaseAddr = 0x00; // Full buffer used for Tx
	SX1276Write( REG_LR_FIFOTXBASEADDR, SX1276LR.RegFifoTxBaseAddr );

	SX1276LR.RegFifoAddrPtr = SX1276LR.RegFifoTxBaseAddr;
	SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR.RegFifoAddrPtr );
	
	// Write payload buffer to LORA modem
	SX1276WriteFifo(data,Length);
								
								///TX done						//CAD DONE							//Detected CAD	
	 SX1276LR.RegDioMapping1 = RFLR_DIOMAPPING1_DIO0_01;// | RFLR_DIOMAPPING1_DIO0_10 |RFLR_DIOMAPPING1_DIO1_10 ;//| RFLR_DIOMAPPING1_DIO1_00 | RFLR_DIOMAPPING1_DIO2_00 | RFLR_DIOMAPPING1_DIO3_01;
	 // PllLock              Mode Ready
	 SX1276LR.RegDioMapping2 = 0;//RFLR_DIOMAPPING2_DIO4_01 | RFLR_DIOMAPPING2_DIO5_00;
	SX1276WriteBuffer( REG_LR_DIOMAPPING1, &SX1276LR.RegDioMapping1, 2 );

	//CAD detect
	//SX1276LoRaSetOpMode(RFLR_OPMODE_CAD);
	
	
// 	do 
// 	{	
// 		Timeout--;
// 		SX1276Read(0x12,&Temp);
// 		delay_ms(1);
// 	
// 	} while (((Temp&0b1)==0)&&(Timeout>0));	//dokud neni najduta preamble ve vzduchu, nebo timeout - zadna se nenasla
// 	
// 	if ((Temp&0b1)==1)	//byla nalezena pre. ve vzduchu
// 	{	
// 		gpio_toggle_pin(LED2);				//...tak pockame
// 		delay_ms(400); 
// 			
// 	}
// 	gpio_toggle_pin(LED2);
	SX1276LoRaSetOpMode( RFLR_OPMODE_TRANSMITTER );
	
	
}

/**************************************************************************/
void RF_Task(TASKS_STATE *Status,QUEUES_TASKS *Queue)
{	
	static uint8_t Buffer[40];
	
	
	//pokud jsou data odeslany, posli dalsi balik
	if ((Queue->DataToRF.Packet_sent==0x63)&&(Queue->DataToRF.GPS_New_Data==0x63))
	{	
			
		Queue->DataToRF.Packet_sent=0;
		Queue->DataToRF.GPS_New_Data=0;
		Status->RFTask=TASK_OFF;
		
		Tx_running=1;
		//spi_enable(&SPIC);
		Send_data(Queue->DataToRF.TX_Buff,8);
	//	spi_disable(&SPIC);
	//	tc45_write_clock_source(&TCD5, TC45_CLKSEL_DIV8_gc);
			
	}		
			
	
}
/************************************************************************/

uint8_t Check_status(void)
{
	
	//SX1276Read(REG_LR_IRQFLAGSMASK,&Temp);
	//If TX Done is set...
// 	if ((Temp&RFLR_IRQFLAGS_TXDONE)==RFLR_IRQFLAGS_TXDONE)
// 	{	
// 	
	
		//spi_enable(&SPIC);
		Tx_running=0;
		//
		// Clear Irq
		SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE  );	//or 0x10??
		// optimize the power consumption by switching off the transmitter as soon as the packet has been sent
		SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY);
		SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP);
		//spi_disable(&SPIC);
		delay_ms(1);
		
		return 0x63;	//packet sent
	//}
	
	//return 0;
}

char* My_strcat(char* dest, const char* src)
{
	char* tmp = dest;
	while(*tmp) ++tmp ;
	while( (*tmp++ = *src++ ) != '\0') ;
	return (dest);
}