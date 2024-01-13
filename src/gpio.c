#include "gpio.h"

#define RED_LED_PIN (5) /* PORT A */
#define YELLOW_LED_PIN (4) /* PORT A */
#define GREEN_LED_PIN (12) /* PORT A */

#define SEQ_RED_LED_PIN (18) /* PORT B */
#define SEQ_GREEN_LED_PIN (19) /* PORT B */
#define SEQ_BLUE_LED_PIN (1) /* PORT D */

void RGBLed_Init(void){
	
	/* Activarea semnalului de ceas pentru pinii folositi in cadrul led-ului RGB */
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	
	/* --- RED LED --- */
	
	/* Utilizare GPIO ca varianta de multiplexare */
	PORTA->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);
	
	/* Configurare pin pe post de output */
	GPIOA_PDDR |= (1<<RED_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 0 logic) */
	GPIOA_PCOR |= (1<<RED_LED_PIN);
	
	
	/* --- YELLOW LED --- */
	
	/* Utilizare GPIO ca varianta de multiplexare */
	PORTA->PCR[YELLOW_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[YELLOW_LED_PIN] |= PORT_PCR_MUX(1);
	
	/* Configurare pin pe post de output */
	GPIOA_PDDR |= (1<<YELLOW_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 0 logic) */
	GPIOA_PCOR |= (1<<YELLOW_LED_PIN);
	
	 
	/* --- GREEN LED --- */
	
	/* Utilizare GPIO ca varianta de multiplexare */
	PORTA->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
	/* Configurare pin pe post de output */
	GPIOA_PDDR |= (1<<GREEN_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 0 logic) */
	GPIOA_PCOR |= (1<<GREEN_LED_PIN);
	
	/* --- SEQ RED LED --- */
	
	/* Utilizare GPIO ca varianta de multiplexare */
	PORTB->PCR[SEQ_RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[SEQ_RED_LED_PIN] |= PORT_PCR_MUX(1);
	
	/* Configurare pin pe post de output */
	GPIOB_PDDR |= (1<<SEQ_RED_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 0 logic) */
	GPIOB_PSOR |= (1<<SEQ_RED_LED_PIN);
	
	/* --- SEQ GREEN LED --- */
	
	/* Utilizare GPIO ca varianta de multiplexare */
	PORTB->PCR[SEQ_GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[SEQ_GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
	/* Configurare pin pe post de output */
	GPIOB_PDDR |= (1<<SEQ_GREEN_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 0 logic) */
	GPIOB_PSOR |= (1<<SEQ_GREEN_LED_PIN);
	
	/* --- SEQ BLUE LED --- */
	
	/* Utilizare GPIO ca varianta de multiplexare */
	PORTD->PCR[SEQ_BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[SEQ_BLUE_LED_PIN] |= PORT_PCR_MUX(1);
	
	/* Configurare pin pe post de output */
	GPIOD_PDDR |= (1<<SEQ_BLUE_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 0 logic) */
	GPIOD_PSOR |= (1<<SEQ_BLUE_LED_PIN);
	
}
