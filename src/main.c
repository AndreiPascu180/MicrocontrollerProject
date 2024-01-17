#include "Adc.h"
#include "Uart.h"
#include "Pit.h"
#include "gpio.h"

int main(void) {
	UART0_Initialize(38400);
	PIT_Init();
  ADC0_Init();
	RGBLed_Init();
	
	for(;;){ 
	
	}
	
}
