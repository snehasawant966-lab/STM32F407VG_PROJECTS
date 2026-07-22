/*
 * 007spi_txonly_arduino.c
 *
 *  Created on: Jul 1, 2026
 *      Author: sneha
 */



#include <stm32f407xx.h>
#include<stm32f407xx_spi_driver.h>

#include<string.h>

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
	//SPIPins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	//GPIO_Init(&SPIPins);

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
int main(void){
    char user_data[] = "Hello World"; //stored in STACK(Part of RAM) after the execution of main() starts

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
            //First send length information
			uint8_t dataLen = strlen(user_data);
			SPI_Send_Data(SPI2,&dataLen,1);
			//to send data
			SPI_Send_Data(SPI2,(uint8_t*)user_data,strlen(user_data));

            //lets confirm SPI is not busy
			while(SPI_GetFlagStatus(SPI2,SPI_BSY_FLAG));

			//Disable the SPI2 communication
			SPI_Peripheralcontrol(SPI2,DISABLE);
	 }
}
