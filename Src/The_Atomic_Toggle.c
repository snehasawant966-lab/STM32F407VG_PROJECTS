/*
 * The_Atomic_Toggle.c
 *
 *  Created on: Jun 24, 2026
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

void delay(void){
	for(uint32_t i=0;i<=500000;i++);
}

int main(void){

	//Led configurations
	GPIO_Handle_t GpioLed1;
	GpioLed1.pGPIOBaseAddr = GPIOD;
	    GpioLed1.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
		GpioLed1.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_OUT;
		GpioLed1.GPIOPinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_HIGH;
		GpioLed1.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP ;
		GpioLed1.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
		GPIO_PeriClockControl(GPIOD, ENABLE);
		GPIO_Init(&GpioLed1);
		GPIO_Handle_t GpioLed2;
			GpioLed2.pGPIOBaseAddr = GPIOD;
			    GpioLed2.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
				GpioLed2.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_OUT;
				GpioLed2.GPIOPinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_HIGH;
				GpioLed2.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP ;
				GpioLed2.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
				GPIO_PeriClockControl(GPIOD, ENABLE);
				GPIO_Init(&GpioLed2);
				GPIO_Handle_t GpioLed3;
					GpioLed3.pGPIOBaseAddr = GPIOD;
					    GpioLed3.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
						GpioLed3.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_OUT;
						GpioLed3.GPIOPinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_HIGH;
						GpioLed3.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP ;
						GpioLed3.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
						GPIO_PeriClockControl(GPIOD, ENABLE);
						GPIO_Init(&GpioLed3);
						GPIO_Handle_t GpioLed4;
							GpioLed4.pGPIOBaseAddr = GPIOD;
							    GpioLed4.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
								GpioLed4.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_OUT;
								GpioLed4.GPIOPinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_HIGH;
								GpioLed4.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP ;
								GpioLed4.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
								GPIO_PeriClockControl(GPIOD, ENABLE);
								GPIO_Init(&GpioLed4);
		while(1){


				 GPIO_ToggleOutputPin_12(GPIOD,GPIO_PIN_NO_12);
				 delay();
				 GPIO_ToggleOutputPin_12(GPIOD,GPIO_PIN_NO_13);
				 delay();
				 GPIO_ToggleOutputPin_12(GPIOD,GPIO_PIN_NO_14);
				 delay();
				 GPIO_ToggleOutputPin_12(GPIOD,GPIO_PIN_NO_15);
				 delay();
		}

}


