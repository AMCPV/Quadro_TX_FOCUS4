/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

void board_init(void)
{	
	/* LED output */
	ioport_configure_pin(LED_R, IOPORT_DIR_OUTPUT);
	ioport_configure_pin(LED_G, IOPORT_DIR_OUTPUT);
	ioport_configure_pin(LED_B, IOPORT_DIR_OUTPUT);
	ioport_configure_pin(LED_W, IOPORT_DIR_OUTPUT);
	ioport_configure_pin(BEEP, IOPORT_DIR_OUTPUT);
	
	ioport_set_pin_low(LED_R);
	ioport_set_pin_low(LED_G);
	ioport_set_pin_low(LED_B);	
	ioport_set_pin_low(LED_W);
	
	/* AD batt*/
	ioport_configure_pin(ADC_BATT, IOPORT_DIR_INPUT);
	ioport_configure_pin(ADC_BATT_GND, IOPORT_DIR_OUTPUT);
	ioport_set_pin_low(ADC_BATT_GND);
	
	
	 /*ADC*/
	ioport_configure_pin(ADC1, IOPORT_DIR_INPUT);
	ioport_configure_pin(ADC2, IOPORT_DIR_INPUT);
	ioport_configure_pin(ADC3, IOPORT_DIR_INPUT);
	ioport_configure_pin(ADC4, IOPORT_DIR_INPUT);
	
	ioport_disable_pin(ADC1);
	ioport_disable_pin(ADC2);
	ioport_disable_pin(ADC3);
	ioport_disable_pin(ADC4);
	ioport_disable_pin(ADC_BATT);
	
	
	
}
