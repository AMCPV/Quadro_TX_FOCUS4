/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define LED0			IOPORT_CREATE_PIN(PORTD, 7)


#define SX1276_CS_PIN          IOPORT_CREATE_PIN(PORTC, 4)// OUT
#define SX1276_RESET_PIN       IOPORT_CREATE_PIN(PORTC, 3) // OUT
#define SX1276_RxTx_PIN        IOPORT_CREATE_PIN(PORTD, 4)// OUT
#define SX1276_MOSI_PIN        IOPORT_CREATE_PIN(PORTC,7)
#define SX1276_MISO_PIN        IOPORT_CREATE_PIN(PORTC,6)
#define SX1276_SCK_PIN         IOPORT_CREATE_PIN(PORTC,5)
//NIRQ
#define SX1276_DIO0_PIN         IOPORT_CREATE_PIN(PORTC, 2)
#define SX1276_DIO1_PIN         IOPORT_CREATE_PIN(PORTA, 7)
#define SX1276_DIO2_PIN         IOPORT_CREATE_PIN(PORTA, 6)

/* LEDs*/
#define  LED_R					IOPORT_CREATE_PIN(PORTC,1)
#define  LED_G					IOPORT_CREATE_PIN(PORTC,0)
#define  LED_B					IOPORT_CREATE_PIN(PORTA,2)
#define  LED_W					IOPORT_CREATE_PIN(PORTD,7)
#define  BEEP					IOPORT_CREATE_PIN(PORTD,5)



/* ADC */
#define ADC1					IOPORT_CREATE_PIN(PORTA, 3)
#define ADC2					IOPORT_CREATE_PIN(PORTD, 1)
#define ADC3					IOPORT_CREATE_PIN(PORTD, 2)
#define ADC4					IOPORT_CREATE_PIN(PORTA, 4)

/* ADC Battery */
#define ADC_BATT				IOPORT_CREATE_PIN(PORTA, 0)
#define ADC_BATT_GND			IOPORT_CREATE_PIN(PORTA, 1)

#endif // CONF_BOARD_H
