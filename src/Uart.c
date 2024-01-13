#include "Uart.h"

void UART0_Transmit(uint8_t data) {
	/* Utilizam masca TDRE pentru a verifica */
	/* disponibilitatea buffer-ului de transmisie */
	while(!(UART0->S1 & UART_S1_TDRE_MASK)) {}
	data = (uint8_t)((data & 0x01) << 7) |
         (uint8_t)((data & 0x02) << 5) |
         (uint8_t)((data & 0x04) << 3) |
         (uint8_t)((data & 0x08) << 1) |
         (uint8_t)((data & 0x10) >> 1) |
         (uint8_t)((data & 0x20) >> 3) |
         (uint8_t)((data & 0x40) >> 5) |
         (uint8_t)((data & 0x80) >> 7);
		
   UART0->D = data;
}
uint8_t UART0_Receive(void)
{
	/* Punem in asteptare pana cand registrul de receptie nu este plin */
	while(!(UART0->S1 & UART0_S1_RDRF_MASK)){}
				return UART0->D;
	
}

void UART0_Initialize(uint32_t baud_rate) {
	
	uint16_t osr = 4;
	uint16_t sbr;
	uint8_t temp;
	
	SIM->SCGC4 = SIM->SCGC4 | SIM_SCGC4_UART0_MASK;

	/* Activarea semnalului de ceas pentru portul A */
	/* PTA1 - receptie UART0 */
	/* PTA2 - transmisie UART0 */
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	/* Dezactivare receptor si emitator pentru realizarea configuratiilor*/
	UART0->C2 &= ~((UART0_C2_RE_MASK) | (UART0_C2_TE_MASK)); 
	
	/* Setarea sursei de ceas pentru modulul UART la 48MHz */
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(01);
	
	/*Fiecare pin pune la dispozitie mai multe functionalitati 
	la care avem acces prin intermediul multiplexarii */
	
	PORTA->PCR[1] = ~PORT_PCR_MUX_MASK;
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); /* Configurare RX pentru UART0 */
	PORTA->PCR[2] = ~PORT_PCR_MUX_MASK;
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); /* TX */
	
	
	/* Setarea baud rate-ului si a ratei de supraesantionare */
	sbr = (uint16_t)((DEFAULT_SYSTEM_CLOCK)/(baud_rate * (osr))/4);
	UART0->BDH &= UART0_BDH_SBR_MASK;
	temp = UART0->BDH & ~(UART0_BDH_SBR(0x1F));
	UART0->BDH = temp | UART0_BDH_SBR(((sbr & 0x1F00)>> 8));
	UART0->BDL = (uint8_t)(sbr & UART_BDL_SBR_MASK);
	UART0->C4 |= UART0_C4_OSR(osr - 1);
			
	/* Setarea numarului de biti de date la 8 si fara bit de paritate */
	UART0->C1 = 0;
	
	/* Transmisie incepand cu MSB */
	UART0->S2 = UART0_S2_MSBF(1);
	
	/* Dezactivare intreruperi la transmisie */
	UART0->C2 |= UART0_C2_TIE(0);
	UART0->C2 |= UART0_C2_TCIE(0);
	
	/* Activare intreruperi la receptie */
	UART0->C2 |= UART0_C2_RIE(1);
	
	UART0->C2 |= ((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
	NVIC_EnableIRQ(UART0_IRQn);
	
}

void UART0_IRQHandler(void) {

		if(UART0->S1 & UART0_S1_RDRF_MASK) {
			sens = UART0->D;
		}
			UART0->S1 &= ~UART0_S1_RDRF_MASK;
}
