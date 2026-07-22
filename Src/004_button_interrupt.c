/*
 * 004_button_interrupt.c
 *
 *  Created on: Jun 23, 2026
 *      Author: sneha
 */

/*
 * 002_LED_BUTTON.c
 *
 *  Created on: Jun 22, 2026
 *      Author: sneha
 */
#include <stm32f407xx.h>
#include<stm32f407xx_gpio_driver.h>
#include<string.h>
void delay(void){
	for(uint32_t i=0;i<=500000;i++);
}
int main(void){
	//button configurations
	GPIO_Handle_t Gpiobtn;
	memset(&Gpiobtn,0,sizeof(Gpiobtn));
	Gpiobtn.pGPIOBaseAddr = GPIOD;
	Gpiobtn.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	Gpiobtn.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	Gpiobtn.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&Gpiobtn);

	//Led configurations
	GPIO_Handle_t GpioLed;
	memset(&GpioLed,0,sizeof(GpioLed));
	GpioLed.pGPIOBaseAddr = GPIOD;
	    GpioLed.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
		GpioLed.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_OUT;
		GpioLed.GPIOPinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_HIGH;
		GpioLed.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP ;
		GpioLed.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
		GPIO_PeriClockControl(GPIOD, ENABLE);
		GPIO_Init(&GpioLed);
	//IRQ CONFIGURATIONS
		GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5,NVIC_IRQ_PRIO15);
		GPIO_IRQConfig(IRQ_NO_EXTI9_5 ,ENABLE);

		while(1);
}
 void EXTI9_5_IRQHandler (void){
	 delay();
	 GPIO_IRQHandling(GPIO_PIN_NO_5);
	 GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12);
 }
