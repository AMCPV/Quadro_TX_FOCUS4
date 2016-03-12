/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "RF_Task.h"
//*******************************************************//

volatile  QUEUES_TASKS	DataForTask;
volatile TASKS_STATE	TaskState={TASK_ON,TASK_ON,TASK_ON};	//zapnu vsechny Tasky
	
volatile uint8_t Global_stav=TASK_ON;
volatile GL_Temp_data[35];

#define MY_ADC   ADCA
volatile  uint16_t ch0_result;
volatile uint16_t ch1_result;
volatile uint16_t ch2_result;
volatile uint16_t ch3_result;
volatile uint16_t ch4_result_batt;

volatile bool Tx_running;

static void adc_init(uint8_t mux);


ISR(PORTC_INT_vect)
{
	irqflags_t LocSREG;
	//struct spi_device	device;

	LocSREG=cpu_irq_save();
	
	DataForTask.DataToRF.Packet_sent=Check_status();
	ioport_toggle_pin(LED_G);
	
	PORTC.INTFLAGS|=0x04;
	cpu_irq_restore(LocSREG);

}


static void adc_handler(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{	
	#define POCET	30
	struct adc_channel_config adcch_conf;
	irqflags_t LocSREG;
	LocSREG=cpu_irq_save();
	sleep_disable();
	
	//ioport_toggle_pin(LED_W);
 	static uint16_t		Suma[5];
// 	static uint16_t		pole[POCET][4];
	static uint16_t	counter=0;
	static uint8_t mux=0;
	
//  	if (Tx_running!=0)
//  	{
		
	
// 		Suma[mux]+=result;
// 		pole[counter[mux]][mux]=result;
// 		counter[mux]++;
// 		if (counter[mux]==POCET) counter[mux]=0;
// 		Suma[mux]-=pole[counter[mux]][mux];
				
		Suma[mux]=result;
		ch0_result=Suma[0];
		ch1_result=(Suma[1]);///POCET);
		ch2_result=(Suma[2]);///POCET);
		ch3_result=(Suma[3]);///POCET);
		ch4_result_batt=(Suma[4]);///POCET);
						   
		counter++;
		if (counter>1)
		{	
			adc_disable(&MY_ADC);
			/* incerase pin mux*/
			mux+=1;
			if (mux>4)	mux=0;
			adc_init(mux);
			counter=0;
		}
		//adc_disable(&MY_ADC);
		
		//adc_enable(&MY_ADC);
		DataForTask.DataToRF.GPS_New_Data=0x63;
		
	//}
	adc_clear_interrupt_flag (&MY_ADC, ADC_CH0);
	adc_start_conversion(&MY_ADC, ADC_CH0);
	
	
	cpu_irq_restore(LocSREG);

	
}
static void adc_init(uint8_t mux)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, ADC_CH0, &adcch_conf);
	
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,	ADC_REF_BANDGAP); //ref 1 V
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_FREERUN, 1, 0);
	adc_set_clock_rate(&adc_conf, 1800000UL);
	adcch_enable_averaging(&adcch_conf, ADC_SAMPNUM_512X);
	adc_set_callback(&MY_ADC, &adc_handler);
	adc_write_configuration(&MY_ADC, &adc_conf);
	adcch_enable_interrupt(&adcch_conf);

	switch (mux)
	{
		case 0:
			adcch_set_input(&adcch_conf, ADCCH_POS_PIN3, ADCCH_NEG_NONE, 1);
			break;
		
		case 1:
			adcch_set_input(&adcch_conf, ADCCH_POS_PIN9, ADCCH_NEG_NONE, 1);
			break;
	
		case 2:
			adcch_set_input(&adcch_conf, ADCCH_POS_PIN10, ADCCH_NEG_NONE, 1);
			break;
		
		case 3:
			adcch_set_input(&adcch_conf, ADCCH_POS_PIN4, ADCCH_NEG_NONE, 1);
			break;
		
		case 4:
			adcch_set_input(&adcch_conf, ADCCH_POS_PIN0, ADCCH_NEG_NONE, 1);
			break;
		
		default:
			/* Your code here */
			break;
	}
	
	
	//adcch_set_input(&adcch_conf, ADCCH_POS_PIN9, ADCCH_NEG_NONE, 1);
	
	/* Enable averaging */
// 	adcch_read_configuration(&ADCA, ADC_CH0, &adcch_conf);
 	
// 	adcch_write_configuration(&ADCA, ADC_CH0, &adcch_conf);
	
	
	adcch_write_configuration(&MY_ADC, ADC_CH0, &adcch_conf);
	
	adc_enable(&MY_ADC);
	adc_clear_interrupt_flag (&MY_ADC, ADC_CH0);
	adc_start_conversion(&MY_ADC, ADC_CH0);
}



int main (void)
{	
	uint32_t temp=0;
	bool timer_run=0;
	sysclk_init();
	pmic_init();
	
	board_init();
	adc_init(0);
	
	cpu_irq_enable();
 	
	 /* Timer for Beep */
 	tc45_enable(&TCD5);
 	tc45_set_wgm(&TCD5, TC45_WG_SS);
 	tc45_write_period(&TCD5, 960);
 	tc45_write_cc(&TCD5, TC45_CCB, 300);
 	tc45_enable_cc_channels(&TCD5,TC45_CCBCOMP);
 
	 /* RF init */
	RF_init();
 	
	delay_ms(10) ;
// tc45_enable(&TCD5);
// tc45_set_wgm(&TCD5, TC45_WG_SS);
// tc45_write_period(&TCD5, 60);
// tc45_write_cc(&TCD5, TC45_CCB, 20);
// tc45_enable_cc_channels(&TCD5,TC45_CCBCOMP);
		 
	//TaskState.XmegaState=TASK_ON;
	DataForTask.DataToRF.Packet_sent=0x63;
	
	while (1)
	{
// 		adcch_set_input(&adcch_conf, ADCCH_POS_PIN0, ADCCH_NEG_NONE, 1);	 
// 		adc_start_conversion(&MY_ADC, ADC_CH0);
// 		adc_wait_for_interrupt_flag(&MY_ADC, ADC_CH0);
// 
// 		 result = adc_get_result(&MY_ADC, ADC_CH0);
	
	
	
 		cpu_irq_disable();
		DataForTask.DataToRF.TX_Buff[0]=(uint8_t)ch0_result;
		DataForTask.DataToRF.TX_Buff[1]=(uint8_t)(ch0_result>>8);
		
		DataForTask.DataToRF.TX_Buff[2]=(uint8_t)ch1_result;
		DataForTask.DataToRF.TX_Buff[3]=(uint8_t)(ch1_result>>8);
		
		DataForTask.DataToRF.TX_Buff[4]=(uint8_t)ch2_result;
		DataForTask.DataToRF.TX_Buff[5]=(uint8_t)(ch2_result>>8);
				
		DataForTask.DataToRF.TX_Buff[6]=(uint8_t)ch3_result;
		DataForTask.DataToRF.TX_Buff[7]=(uint8_t)(ch3_result>>8);
		cpu_irq_enable();
		
		RF_Task(&TaskState,&DataForTask);
		
// 		ioport_set_pin_high(BEEP);
// 		delay_ms(1)	;
// 		ioport_set_pin_low(BEEP);
// 		delay_ms(1)	;
// 		
		//delay_us(10)	;
  		if (ch4_result_batt<3500)
  		{
	  		temp++;
	  		
	  		if (temp>2000)
	  		{
		  		ioport_toggle_pin(LED_R);
		  		if(timer_run==0)
		  		{
			  		tc45_write_clock_source(&TCD5, TC45_CLKSEL_DIV8_gc);
			  		timer_run=1;
		  		}else
		  		{
			  		tc45_write_clock_source(&TCD5, TC45_CLKSEL_OFF_gc);
			  		timer_run=0;
			  		
		  		}
		  		temp=0;
	  		}else
			{
				ioport_set_pin_low(LED_R);
			  }
	  		
	  		
  		}
			
		
	
	
// 		ioport_toggle_pin(LED2);
// 
// 		delay_ms(50);
// 		ioport_toggle_pin(LED1);
	}
	/* Insert application code here, after the board has been initialized. */
}
