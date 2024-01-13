#include "MKL25Z4.h"

extern uint8_t sens;

void UART0_Transmit(uint8_t data);
void UART0_Initialize(uint32_t baud_rate);
uint8_t UART0_Receive(void);
void UART0_IRQHandler(void);
