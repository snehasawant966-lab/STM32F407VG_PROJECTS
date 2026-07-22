/*
 * 005_spi_tx_testing.c
 *
 *  Created on: Jun 25, 2026
 *      Author: sneha
 */
/*
 * PB14 - SPI2_MISO
 * PB15 - SP2_MOSI
 * PB13 - SPI2_SCK
 * PB12 - SPI2_NSS
 * ALT FNCTION MODE :5
 */

#include <stm32f407xx.h>
#include<string.h>
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
	//SPIPins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
   // GPIO_Init(&SPIPins);
}

void SPI2_Inits(void){

	SPI_Handle_t SPI2_Handle;
	SPI2_Handle.pSPI_BaseAddr = SPI2;
	SPI_PeriClockControl(SPI2,ENABLE);
	SPI2_Handle.SPI_Config.Bus_Config = SPI_BUSCONFIG_FULL_DUPLEX;
	SPI2_Handle.SPI_Config.Devide_Mode = SPI_DEVICE_MODE_MASTER;
	SPI2_Handle.SPI_Config.Sclk_SPEED = SPI_SCLK_SPEED_DIV2; //Generates serial clock of 8MHz
	SPI2_Handle.SPI_Config.DFF = SPI_DFF_8_bit;
	SPI2_Handle.SPI_Config.CPOL = CPOL_LOW;
	SPI2_Handle.SPI_Config.CPHA = CPHA_LOW;
	SPI2_Handle.SPI_Config.SSM = SSM_EN; //Software slave management enable for NSS pin

	SPI_Init(&SPI2_Handle);
}

int main(void){
    char user_data[] = "Hello World"; //stored in STACK(Part of RAM) after the execution of main() starts
	//This function is used to initialize the GPIO pins to behave as SPI2 pins
    SPI2_GpioInits();
    //This function is used to initialize the SPI2 peripheral parameters
	SPI2_Inits();
	//This makes signal internally High and avoids MODF error
	SPI_SSIConfig(SPI2,ENABLE);
	//Enable the SPI2 peripheral
	SPI_Peripheralcontrol(SPI2,ENABLE);
	//to send data
	SPI_Send_Data(SPI2,(uint8_t*)user_data,strlen(user_data));
	//lets confirm SPI is not busy
	while(SPI_GetFlagStatus(SPI2,SPI_BSY_FLAG));
	SPI_Peripheralcontrol(SPI2,DISABLE);

	while(1);

	return 0;
}
