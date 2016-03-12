/*
 * Main.h
 *
 * Created: 26.11.2013 20:06:12
 *  Author: JR
 */ 


#ifndef MAIN_H_
#define MAIN_H_

/************************************************************************/
void Usart_init(void);
void Before_Sleep(void);
void After_Sleep(void);

#define		ON_STATE	0
#define		OFF_STATE	1

/************************************************************************/

#define USART_SERIAL_EXAMPLE               &USARTD0
#define USART_SERIAL_EXAMPLE_BAUDRATE      9600
#define USART_SERIAL_CHAR_LENGTH           USART_CHSIZE_8BIT_gc
#define USART_SERIAL_PARITY                USART_PMODE_DISABLED_gc
#define USART_SERIAL_STOP_BIT              false
#define USART_RX_Vect                      USARTD0_RXC_vect

/************************************************************************/
#define ADDR_STATE_DEVICE		0x00					// Address state device

/************************************************************************/

typedef enum {STATE_OFF,STATE_ON} 		ENUM_STATE_DEVICE;
/************************************************************************/

typedef enum {	
TASK_NO_DEFINITION,
	TASK_OFF,
	TASK_ON,
	TASK_RDY_TO_SLEEP,
	TASK_RDY_TO_WAKE,														
	
}				ENUM_STATE_TASK;

/************************************************************************/

typedef struct{
	char Time[9];	
	//char Data_valid;
	char Latitude[12];
	char Longtitude[13];
	char Sat_used[3];
	char HDOP[5];
	char Altitude[7];
	char id_psa;
	char Konec_pole;
	
}GP_GGA;	//sizeof=51
	
/************************************************************************/
	
typedef struct{
	
	char Nmea_data[150];
	//pokud je nalezena vìta
	uint8_t GL_flag_gps;
	char Nmea_crc[3];
	uint8_t My_crc;
	GP_GGA Sentence;
}GPS;


/************************************/
typedef struct{
	bool RF_run;
	uint8_t Packet_sent;
	uint8_t GPS_New_Data;
	uint8_t TX_Buff[6];
	
}Semtech;

/************************************/
typedef struct
{
	GPS	DataToGPS;
	Semtech DataToRF;
	
}QUEUES_TASKS;

/************************************************************************/

typedef struct
{
	ENUM_STATE_TASK	XmegaState;
	ENUM_STATE_TASK GPSTask;
	ENUM_STATE_TASK RFTask;
		
}TASKS_STATE;	



#endif /* MAIN_H_ */