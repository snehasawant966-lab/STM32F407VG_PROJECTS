/*
 * 008_spi_cmd_handling.c
 *
 *  Created on: Jul 1, 2026
 *      Author: sneha
 */

/*
 * 007spi_txonly_arduino.c
 *
 *  Created on: Jul 1, 2026
 *      Author: sneha
 */



#include <stm32f407xx.h>
#include<string.h>
#include<stdio.h>
//command codes

#define COMMAND_LED_CTRL       0X50
#define COMMAND_SENSOR_READ    0X51
#define COMMAND_LED_READ       0X52
#define COMMAND_PRINT          0X53
#define COMMAND_ID_READ        0X54


#define LED_ON        1
#define LED_OFF       0


//arduino analog pins
#define ANALOG_PIN0       0
#define ANALOG_PIN1       1
#define ANALOG_PIN2       2
#define ANALOG_PIN3       3
#define ANALOG_PIN4       4

#define LED_PIN           9


void delay(void){
	for(uint32_t i=0;i<=500000;i++);
}

void SPI2_GpioInits(void){
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOBaseAddr = GPIOB;
	GPIO_PeriClockControl(GPIOB, ENABLE);
	SPIPins.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIOPinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIOPinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_HIGH;
	//SCLK
	SPIPins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

    // MOSI
	SPIPins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
	SPIPins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GPIO_Init(&SPIPins);
}

void SPI2_Inits(void){

	SPI_Handle_t SPI2_Handle;
	SPI2_Handle.pSPI_BaseAddr = SPI2;
	SPI_PeriClockControl(SPI2,ENABLE);
	SPI2_Handle.SPI_Config.Bus_Config = SPI_BUSCONFIG_FULL_DUPLEX;
	SPI2_Handle.SPI_Config.Devide_Mode = SPI_DEVICE_MODE_MASTER;
	SPI2_Handle.SPI_Config.Sclk_SPEED = SPI_SCLK_SPEED_DIV8; //Generates serial clock of 8MHz
	SPI2_Handle.SPI_Config.DFF = SPI_DFF_8_bit;
	SPI2_Handle.SPI_Config.CPOL = CPOL_LOW;
	SPI2_Handle.SPI_Config.CPHA = CPHA_LOW;
	SPI2_Handle.SPI_Config.SSM = SSM_DI; //Hardware slave management enable for NSS pin

	SPI_Init(&SPI2_Handle);
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

uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if(	ackbyte == 0xF5){
		//ack
		return 1;
	}else {
		return 0;
	}
}
int main(void){
    uint8_t dummy_write = 0xff;
    uint8_t dummy_read;

    GPIO_ButtonInit();
	//This function is used to initialize the GPIO pins to behave as SPI2 pins
    SPI2_GpioInits();
    //This function is used to initialize the SPI2 peripheral parameters
	SPI2_Inits();

	//************************
	//This makes signal internally High and avoids MODF error
	//SPI_SSIConfig(SPI2,ENABLE);->only for SSM

	//**********************************
	/*
	 * Making the SSOE 1 does NSS output enable
	 * The NSS pin is automatically managed by the hardware
	 * i.e. when SPOE = 1, NSS will be pulled to low
	 * and NSS pin will be high when SPOE = 0
	 */
	 SPI_SSOEConfig(SPI2,ENABLE);

	 while(1){
			 //wait till button is pressed
			 while(!GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0));
			 //to avoid button de-bouncing related issues 200ms of delay
			 delay();
			//Enable the SPI2 peripheral
			SPI_Peripheralcontrol(SPI2,ENABLE);

			//1. CMD_LED_CTRL   <Pin no (1)>  <value(1)>

			uint8_t commandcode = COMMAND_LED_CTRL;
			uint8_t ackbyte;
			uint8_t args[2];
			SPI_Send_Data(SPI2,&commandcode,1);
			//do dummy read to clear of RXNY
			SPI_Receive_Data(SPI2,&dummy_read,1);
			//send some dummy bits(1 byte) to fetch the response from the slave.
			SPI_Send_Data(SPI2,&dummy_write,1);
			//read the ack byte received
			SPI_Receive_Data(SPI2,&ackbyte,1);

			if(SPI_VerifyResponse(ackbyte)){
				//send arguments
				args[0] = LED_PIN;
				args[1] = LED_ON;
				SPI_Send_Data(SPI2,args,2);

			}

            //lets confirm SPI is not busy
			while(SPI_GetFlagStatus(SPI2,SPI_BSY_FLAG));

			//Disable the SPI2 communication
			SPI_Peripheralcontrol(SPI2,DISABLE);

	 }
}





/*
 *             //First send length information
			uint8_t dataLen = strlen(user_data);
			SPI_Send_Data(SPI2,&dataLen,1);
			//to send data
			SPI_Send_Data(SPI2,(uint8_t*)user_data,strlen(user_data));
            //lets confirm SPI is not busy
			while(SPI_GetFlagStatus(SPI2,SPI_BSY_FLAG));

			//Disable the SPI2 communication
			SPI_Peripheralcontrol(SPI2,DISABLE);
 */
