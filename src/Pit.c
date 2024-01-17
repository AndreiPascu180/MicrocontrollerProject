#include "Pit.h"
#include "Uart.h"
#define SEQ_RED_LED_PIN (18) /* PORT B */
#define SEQ_GREEN_LED_PIN (19) /* PORT B */
#define SEQ_BLUE_LED_PIN (1) /* PORT D */

uint8_t state = 0;
uint8_t sens = 0;

void PIT_Init(void) {
	/* Activarea semnalului de ceas pentru perifericul PIT */
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	/* Utilizarea semnalului de ceas pentru tabloul de timere */
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	/* Oprirea decrementarii valorilor numaratoarelor in modul debug */
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	/* Setarea valoarea numaratorului de pe canalul 0 la o perioada de 1 secunda */
	/*PIT->CHANNEL[0].LDVAL = 0x9FFFFF;*/
	
	/* Setarea valoarea numaratorului de pe canalul 0 la o perioada de 0.25 secunde */
	/*PIT->CHANNEL[0].LDVAL = 0x27FFFF;*/ 
	
	/* Setarea valoarea numaratorului de pe canalul 0 la o perioada de 0.5 secunde */
	/*PIT->CHANNEL[0].LDVAL = 0x4FFFFF;*/
	
	/* Setarea valoarea numaratorului de pe canalul 0 la o perioada de 0.2 secunde */
	/*PIT->CHANNEL[0].LDVAL = 0x1FFFFC;*/
	
	/* Setarea valoarea numaratorului de pe canalul 0 la o perioada de 0.1 secunde */
	PIT->CHANNEL[0].LDVAL = 0xFFFFE;
	
  /* Activarea intreruperilor pe canalul 0 */
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	/* Activarea timerului de pe canalul 0 */
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	/* Setarea valoarea numaratorului de pe canalul 1 la o perioada de 0.711 secunde */
	PIT->CHANNEL[1].LDVAL = 0x71C28E;
	
	/* Activara ?ntreruperilor pe canalul 1 */
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;
	/* Activarea timerului de pe canalul 1 */
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	/* Activarea intreruperii mascabile si setarea prioritatiis */
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn,5);
	NVIC_EnableIRQ(PIT_IRQn);
}

void PIT_IRQHandler(void) {
	
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		
		ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		
	}
	if(PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		state+=1;
		state=state%4;
		
		if(sens==0){
			switch (state) {
				case 0:
					/* Alb */
					GPIOB->PCOR |= (1<< SEQ_RED_LED_PIN);
					GPIOB->PCOR |= (1<< SEQ_GREEN_LED_PIN);
					GPIOD->PCOR |= (1<< SEQ_BLUE_LED_PIN);
					break;
				case 1:
					/* Magenta */
					GPIOB->PCOR |= (1<< SEQ_RED_LED_PIN);
					GPIOB->PSOR |= (1<< SEQ_GREEN_LED_PIN);
					GPIOD->PCOR |= (1<< SEQ_BLUE_LED_PIN);
					break;
				case 2:
					/* Galben */
					GPIOB->PCOR |= (1<< SEQ_RED_LED_PIN);
					GPIOB->PCOR |= (1<< SEQ_GREEN_LED_PIN);
					GPIOD->PSOR |= (1<< SEQ_BLUE_LED_PIN);
					break;
				case 3:
					/* Stins */
					GPIOB->PSOR |= (1 << SEQ_RED_LED_PIN);
					GPIOB->PSOR |= (1 << SEQ_GREEN_LED_PIN);
					GPIOD->PSOR |= (1 << SEQ_BLUE_LED_PIN);
					break;
			}
		}
		if(sens==1)
		{	
			switch (state) {
				case 3:
					/* Alb */
					GPIOB->PCOR |= (1<< SEQ_RED_LED_PIN);
					GPIOB->PCOR |= (1<< SEQ_GREEN_LED_PIN);
					GPIOD->PCOR |= (1<< SEQ_BLUE_LED_PIN);
					break;
				case 2:
          /* Magenta */
					GPIOB->PCOR |= (1<< SEQ_RED_LED_PIN);
					GPIOB->PSOR |= (1<< SEQ_GREEN_LED_PIN);
					GPIOD->PCOR |= (1<< SEQ_BLUE_LED_PIN);
          break;
        case 1:
          /* Galben */
					GPIOB->PCOR |= (1<< SEQ_RED_LED_PIN);
					GPIOB->PCOR |= (1<< SEQ_GREEN_LED_PIN);
					GPIOD->PSOR |= (1<< SEQ_BLUE_LED_PIN);
          break;
				case 0:
          /* Stins */
          GPIOB->PSOR |= (1 << SEQ_RED_LED_PIN);
					GPIOB->PSOR |= (1 << SEQ_GREEN_LED_PIN);
					GPIOD->PSOR |= (1 << SEQ_BLUE_LED_PIN);
          break;
        }
			}
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
	}
}
