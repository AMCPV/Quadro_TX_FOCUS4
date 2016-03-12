

// #include <stdint.h>
// #include <stdbool.h> 


#include "board.h"
#include "sx1276-Hal.h"
#include <asf.h>
#include <ioport.h>
//#include <conf_sx1276.h>


uint8_t SpiInOut(uint8_t Data);


struct spi_device spi_device_conf =  {.id = SX1276_CS_PIN};


void SX1276InitIo( void )
{	
	
	ioport_configure_pin(SX1276_MOSI_PIN, IOPORT_DIR_OUTPUT);
	ioport_configure_pin(SX1276_MISO_PIN, IOPORT_DIR_INPUT);
	ioport_configure_pin(SX1276_SCK_PIN, IOPORT_DIR_OUTPUT);
	ioport_configure_pin(SX1276_RxTx_PIN, IOPORT_DIR_OUTPUT);
		ioport_set_pin_high(SX1276_RxTx_PIN);
	
		
	// Initializing of GPS SPI - via USART
	//*************************************************************************/
	//ioport_set_pin_dir(SX1276_CS_PIN,   IOPORT_DIR_OUTPUT );
	ioport_configure_pin(SX1276_CS_PIN, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH );
	//*************************************************************************//
	//ioport_set_pin_dir(SX1276_RxTx_PIN,  IOPORT_DIR_OUTPUT );
	//ioport_configure_pin(SX1276_RxTx_PIN, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH );
	//ioport_set_pin_dir(SX1276_RESET_PIN, IOPORT_DIR_OUTPUT );
	ioport_configure_pin(SX1276_RESET_PIN, IOPORT_DIR_OUTPUT  | IOPORT_INIT_LOW);
	
	//NIRQ
	//ioport_set_pin_dir(SX1276_DI0_PIN,   IOPORT_DIR_INPUT  );
	ioport_configure_pin(SX1276_DIO0_PIN, IOPORT_DIR_INPUT);
	ioport_configure_pin(SX1276_DIO1_PIN, IOPORT_DIR_INPUT);
	ioport_configure_pin(SX1276_DIO2_PIN, IOPORT_DIR_INPUT);
	
	spi_master_init(&SPIC);
 	spi_master_setup_device(&SPIC, &spi_device_conf, SPI_MODE_0, 1000000, 0);	//Max 10 MHz
 	spi_enable(&SPIC);
}

void SX1276SetReset( uint8_t state )
{
    if( state == RADIO_RESET_ON )
    {
        // Set RESET pin to 1
		ioport_set_pin_level(SX1276_RESET_PIN, true);
		//ioport_set_pin_mode(SX1276_RESET_PIN, IOPORT_DIR_INPUT);
	}
    else
    {
		// Set RESET pin to 0
		ioport_set_pin_level(SX1276_RESET_PIN, false);
    }
}

void SX1276Write( uint8_t addr, uint8_t data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}

void SX1276Read( uint8_t addr, uint8_t *data )
{
    SX1276ReadBuffer( addr, data, 1 );
}


// void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
// {
// 	uint8_t i;
// 
// 	//NSS = 0;
// 	spi_select_device(&SPIC, &spi_device_conf);
// 
// 	spi_put(&SPIC, addr | 0x80);
// 	// 	for (uint8_t i=0; i < size; i++)
// 	// 	{
// 	spi_write_packet(&SPIC,buffer,size);
// 	// 		while(!(spi_is_rx_ready(SPI)));
// 	// 		temp[i] = spi_get(SPI);
// 	//	}
// 
// 	//NSS = 1;
// 	spi_deselect_device(&SPIC, &spi_device_conf);
// 
// }
// 
// void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
// {
// 	uint8_t i;
// 
// 	//NSS = 0;
// 	spi_select_device(&SPIC, &spi_device_conf);
// 
// 	spi_put(&SPIC, addr & 0x7F);
// 	
// 	spi_read_packet(&SPIC,buffer,size);
// 
// 	//NSS = 1;
// 	spi_deselect_device(&SPIC, &spi_device_conf);
// }


void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{

// #if OPTIMIZE_O0
//    
//    spi_select_device(&SPIC, &spi_device_conf);
//    spi_put(&SPIC, addr | 0x80);
//    spi_write_packet(&SPIC,buffer,size);
//    spi_deselect_device(&SPIC, &spi_device_conf);
// 
// #elif  OPTIMIZE_OS

   spi_select_device(&SPIC, &spi_device_conf);
   spi_put(&SPIC, addr | 0x80);
   
   while (!spi_is_rx_ready(&SPIC));
   // 			spi_write_single(&SPIC,0x00);
   // 			while (!spi_is_tx_ready(&SPIC));
   // 			spi_read_single(&SPIC, &bCtsValue);			// Read command buffer; get CTS value
   spi_write_packet(&SPIC,buffer,size);
   spi_deselect_device(&SPIC, &spi_device_conf);
   
// #else
// 	#error Neni definovana optimalizace
// #endif


}

void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

// #if  OPTIMIZE_O0
// 	
// 	spi_select_device(&SPIC, &spi_device_conf);
//     spi_put(&SPIC, addr & 0x7F);
//     spi_read_packet(&SPIC,buffer,size);
//     spi_deselect_device(&SPIC, &spi_device_conf);
// 
// #elif OPTIMIZE_OS

	spi_select_device(&SPIC, &spi_device_conf);
    spi_put(&SPIC, addr & 0x7F);
	
	while (!spi_is_rx_ready(&SPIC));
// 			spi_write_single(&SPIC,0x00);
// 			while (!spi_is_tx_ready(&SPIC));
// 			spi_read_single(&SPIC, &bCtsValue);			// Read command buffer; get CTS value
    spi_read_packet(&SPIC,buffer,size);
    spi_deselect_device(&SPIC, &spi_device_conf);

// #else 
// 	#error Neni definovana optimalizace
// #endif
		
}

void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
///   	tc45_disable(&TCD5);
	    SX1276WriteBuffer( 0, buffer, size );
   //	tc45_enable(&TCD5);
	
}

void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{	
   //	tc45_disable(&TCD5);
	
    SX1276ReadBuffer( 0, buffer, size );
   //	tc45_enable(&TCD5);
	
}

uint8_t SpiInOut(uint8_t Data)
{	
	return usart_spi_transmit(&SPIC, Data);
}

uint8_t SX1276ReadDio0( void )
{
	return (uint8_t)ioport_get_pin_level(SX1276_DIO0_PIN);
}

uint8_t SX1276ReadDio1( void )
{
	// Unfortunately we have not the free pin
	//return (uint8_t)ioport_get_pin_level(Sx1276_DI1_PIN);
	return(0);
}

uint8_t SX1276ReadDio2( void )
{
    //return GPIO_ReadInputDataBit( DIO2_IOPORT, DIO2_PIN );
	return(0);
}

uint8_t SX1276ReadDio3( void )
{
    //return IoePinGet( RF_DIO3_PIN );
	return(0);
}

uint8_t SX1276ReadDio4( void )
{
   // return IoePinGet( RF_DIO4_PIN );
   return(0);
}

uint8_t SX1276ReadDio5( void )
{
    //return IoePinGet( RF_DIO5_PIN );
	return(0);
}


void SX1276WriteRxTx( uint8_t txEnable )
{
    if( txEnable != 0 )
    {	
		// High in TX
		//ioport_set_pin_level(Sx1276_RxTxSwitch_PIN, true);
		ioport_set_pin_high(SX1276_RxTx_PIN);
    }
    else
    {
		//ioport_set_pin_level(Sx1276_RxTxSwitch_PIN, false);
		ioport_set_pin_low(SX1276_RxTx_PIN);
    }
}

