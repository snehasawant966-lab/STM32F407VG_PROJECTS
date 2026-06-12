/*
 * 001led_toggle.c
 *
 *  Created on: Mar 6, 2026
 *      Author: sneha
 */
#include "stm32f407xx.h"
void delay1(void)
{
	for(uint32_t i=0; i< 500000; i++);

}
void delay2(void)
{
	for(uint32_t i=0; i< 1000000; i++);

}
void delay3(void)
{
	for(uint32_t i=0; i< 1500000; i++);

}
void delay4(void)
{
	for(uint32_t i=0; i< 2000000; i++);

}
int main(void)
{    //For green LED
	GPIO_Handle_t GpioLedG;
	GpioLedG.pGPIOx = GPIOD;
	GpioLedG.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLedG.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLedG.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST ;
	GpioLedG.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLedG.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_PeriClockControl(GPIOD,ENABLE);
	GPIO_Init(&GpioLedG);
	 //For Orange LED
	GPIO_Handle_t GpioLedO;
		GpioLedO.pGPIOx = GPIOD;
		GpioLedO.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
		GpioLedO.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
		GpioLedO.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST ;
		GpioLedO.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
		GpioLedO.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
		GPIO_PeriClockControl(GPIOD,ENABLE);
		GPIO_Init(&GpioLedO);
		 //For Red LED
	GPIO_Handle_t GpioLedR;
		GpioLedR.pGPIOx = GPIOD;
		GpioLedR.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
		GpioLedR.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
		GpioLedR.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST ;
		GpioLedR.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
		GpioLedR.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
		GPIO_PeriClockControl(GPIOD,ENABLE);
		GPIO_Init(&GpioLedR);
		 //For Blue LED
	GPIO_Handle_t GpioLedB;
		GpioLedB.pGPIOx = GPIOD;
		GpioLedB.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
		GpioLedB.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
		GpioLedB.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST ;
		GpioLedB.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
		GpioLedB.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
		GPIO_PeriClockControl(GPIOD,ENABLE);
		GPIO_Init(&GpioLedB);
/*
 * 4 LEDs toggle simultaneously after the delay()
*/
	while(1)
	{
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12);
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_13);
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_14);
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_15);
		delay1();
	}

/*
 * 4 LEDs toggle one after one the delay()

	while(1)
	{
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12);
		delay1();
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12);
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_13);
		delay1();
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_13);
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_14);
		delay1();
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_14);
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_15);
		delay1();
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_15);
	}

*/

	return 0;
}
