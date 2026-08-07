/*
 * 010i2c_master_tx_testing.c
 *
 *  Created on: Jul 10, 2026
 *      Author: sneha
 */


#include <stm32f407xx.h>
#include<string.h>
#include<stdio.h>


#define  MY_Addr 0x61
#define SLAVE_ADDR 0x68
void delay(void){
	for(uint32_t i=0;i<=500000;i++);
}

I2C_Handle_t I2C1Handle;

//SOME DATA

uint8_t some_data[] = "we are testing I2C master TX\n";
/*
 * PB6 - SCL
 * PB7 - SDA
 */
void I2C1_GpioInits(void){
	GPIO_Handle_t I2CPins;
	I2CPins.pGPIOBaseAddr = GPIOB;
	GPIO_PeriClockControl(GPIOB, ENABLE);
	I2CPins.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIOPinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPins.GPIOPinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_HIGH;
	//SCLK
	I2CPins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);

    // SDA
	I2CPins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&I2CPins);
}

void I2C1_Inits(void){

	I2C1Handle.pI2CBaseAddr = I2C1;
	I2C_PeriClockControl(I2C1,ENABLE);
	I2C1Handle.I2CConfig.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2CConfig.I2C_DeviceAddress = MY_Addr;
	I2C1Handle.I2CConfig.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2CConfig.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
	I2C_Init(&I2C1Handle);
}
void LED_Init(void)
{
    GPIO_Handle_t LED;

    LED.pGPIOBaseAddr = GPIOD;

    GPIO_PeriClockControl(GPIOD, ENABLE);

    LED.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    LED.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    LED.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    LED.GPIOPinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_LOW;
    LED.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_Init(&LED);
}
void  GPIO_ButtonInit(void){
	//button configurations
	GPIO_Handle_t Gpiobtn;
	Gpiobtn.pGPIOBaseAddr = GPIOA;
	Gpiobtn.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	Gpiobtn.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_IN;
	Gpiobtn.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&Gpiobtn);
}
int main (void){

    LED_Init();
	GPIO_ButtonInit();
	I2C1_GpioInits();
	//RCC->APB1ENR |= (1 << 21);
	I2C1_Inits();

	I2C_PeriClockControl(I2C1,ENABLE);
	while(1){
	//wait for button press
	 while(!GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0)){
		 ;
	 }
				 //to avoid button de-bouncing related issues 200ms of delay
				 delay();
				 // Add this test
				    GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);

				    delay();

	//send data
	I2C_MasterSendData(&I2C1Handle,some_data, strlen((char*)some_data),SLAVE_ADDR,0);

	}
}
