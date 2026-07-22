/*
 * 002_LED_BUTTON.c
 *
 *  Created on: Jun 22, 2026
 *      Author: sneha
 */
#include <stm32f407xx.h>
#include<stm32f407xx_gpio_driver.h>
void delay(void){
	for(uint32_t i=0;i<=500000;i++);
}
int main(void){
	//button configurations
	GPIO_Handle_t Gpiobtn;
	Gpiobtn.pGPIOBaseAddr = GPIOB;
	Gpiobtn.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	Gpiobtn.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_IN;
	Gpiobtn.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GPIO_PeriClockControl(GPIOB, ENABLE);
	GPIO_Init(&Gpiobtn);
	//Led configurations
	GPIO_Handle_t GpioLed;
	GpioLed.pGPIOBaseAddr = GPIOA;
	    GpioLed.GPIOPinConfig.GPIO_PinNumber = 8;
		GpioLed.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_OUT;
		GpioLed.GPIOPinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_HIGH;
		GpioLed.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP ;
		GpioLed.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
		GPIO_PeriClockControl(GPIOA, ENABLE);
		GPIO_Init(&GpioLed);
		GpioLed.pGPIOBaseAddr = GPIOB;
		    GpioLed.GPIOPinConfig.GPIO_PinNumber = 8;
			GpioLed.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_OUT;
			GpioLed.GPIOPinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_HIGH;
			GpioLed.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP ;
			GpioLed.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
			GPIO_PeriClockControl(GPIOB, ENABLE);
			GPIO_Init(&GpioLed);
		while(1){
			if(GPIO_ReadFromInputPin(GPIOB,GPIO_PIN_NO_12) == BTN_PRESSED ){
				 delay();
				GPIO_ToggleOutputPin(GPIOA,GPIO_PIN_NO_8);
				GPIO_ToggleOutputPin(GPIOB,GPIO_PIN_NO_8);

			}
		}

		return 0;
}
