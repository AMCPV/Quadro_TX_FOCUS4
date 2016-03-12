

#include <asf.h>
#include "sx1276.h"
#include "sx1276-Hal.h"
#include "sx1276-LoRa.h"

/*!
 * SX1276 registers variable
 */
extern tSX1276LR SX1276LR;

void SX1276Init( void )
{
    // InitializeLoRa registers structure
    //SX1276LR = ( tSX1276LR* )SX1276Regs;
	
	//SX1276Read( REG_LR_VERSION, &SX1276LR.RegVersion );
	
    SX1276InitIo( );
	//SX1276Read( REG_LR_VERSION, &SX1276LR.RegVersion );
	
    SX1276Reset( );
	
	//SX1276Write( REG_LR_OPMODE, 0x09 );
	
	SX1276Read( REG_LR_VERSION, &SX1276LR.RegVersion );
	if ((SX1276LR.RegVersion==0x11)||(SX1276LR.RegVersion==0x12))
	{
		for (uint8_t i=0;i<4;i++)
		{
			ioport_toggle_pin(LED_G);
			delay_ms(75)	;
		}
		
		ioport_set_pin_level(LED_G,LOW);
		
		 for (uint8_t i=0; i<2;i++)
		 {	
			ioport_toggle_pin(LED_R);
			
			 tc45_write_clock_source(&TCD5, TC45_CLKSEL_DIV8_gc);
			 delay_ms(100);
			ioport_toggle_pin(LED_R);
			
			 tc45_write_clock_source(&TCD5,  TC45_CLKSEL_OFF_gc);
			 delay_ms(100);
		 }	
			ioport_toggle_pin(LED_G);
			 tc45_write_period(&TCD5, 800);
			 tc45_write_cc(&TCD5, TC45_CCB, 450);
			 tc45_write_clock_source(&TCD5, TC45_CLKSEL_DIV8_gc);
			 delay_ms(100);
			 tc45_write_clock_source(&TCD5,  TC45_CLKSEL_OFF_gc);
			ioport_toggle_pin(LED_G);
			
			//ioport_toggle_pin(LED_G);
			
	}
		
    SX1276SetLoRaOn();
	
    // Initialize LoRa modem
    SX1276LoRaInit( );


}

void SX1276Reset( void )
{
    
    SX1276SetReset( RADIO_RESET_OFF );
    
    // Wait 10ms
    delay_ms(10);
    SX1276SetReset( RADIO_RESET_ON );
    
    // Wait 10ms
    delay_ms(15);
}

void SX1276SetLoRaOn(void)
{
    SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
        
    SX1276LR.RegOpMode = ( SX1276LR.RegOpMode & RFLR_OPMODE_LONGRANGEMODE_MASK ) | RFLR_OPMODE_LONGRANGEMODE_ON;	
	SX1276LR.RegOpMode&=~0x8;//-0x08 - HF
	
    SX1276Write( REG_LR_OPMODE, SX1276LR.RegOpMode );
        
    SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
                                    // RxDone               RxTimeout                   FhssChangeChannel           CadDone
    SX1276LR.RegDioMapping1 = 
	RFLR_DIOMAPPING1_DIO0_00 |
	RFLR_DIOMAPPING1_DIO0_01|
	RFLR_DIOMAPPING1_DIO2_00 | 
	RFLR_DIOMAPPING1_DIO3_00;	//TX_Done

                                    // CadDetected          ModeReady
    SX1276LR.RegDioMapping2 = RFLR_DIOMAPPING2_DIO4_00 | RFLR_DIOMAPPING2_DIO5_00;
	
    SX1276WriteBuffer( REG_LR_DIOMAPPING1, &SX1276LR.RegDioMapping1, 2 );
   // SX1276ReadBuffer( REG_LR_OPMODE, SX1276Regs + 1, 0x70 - 1 );
   
}


void SX1276SetOpMode( uint8_t opMode )
{
     SX1276LoRaSetOpMode( opMode );
 }

uint8_t SX1276GetOpMode( void )
{
   
   return SX1276LoRaGetOpMode( );
  
}

double SX1276ReadRssi( void )
{
    return SX1276LoRaReadRssi( );
}

uint8_t SX1276ReadRxGain( void )
{
     return SX1276LoRaReadRxGain( );
    
}

uint8_t SX1276GetPacketRxGain( void )
{
    return SX1276LoRaGetPacketRxGain(  );
    
}

int8_t SX1276GetPacketSnr( void )
{
    return SX1276LoRaGetPacketSnr(  );
    
}

double SX1276GetPacketRssi( void )
{
   return SX1276LoRaGetPacketRssi( );
    
}

uint32_t SX1276GetPacketAfc( void )
{
   return 0;
}

void SX1276StartRx( void )
{
    SX1276LoRaSetRFState( RFLR_STATE_RX_INIT );
 
}

void SX1276GetRxPacket( void *buffer, uint16_t *size )
{
     SX1276LoRaGetRxPacket( buffer, size );
    
}

void SX1276SetTxPacket( const void *buffer, uint16_t size )
{
   SX1276LoRaSetTxPacket( buffer, size );
    
}

uint8_t SX1276GetRFState( void )
{
    return SX1276LoRaGetRFState( );
    
}

void SX1276SetRFState( uint8_t state )
{
   SX1276LoRaSetRFState( state );
    
}

uint32_t SX1276Process( void )
{
    return SX1276LoRaProcess( );
}

