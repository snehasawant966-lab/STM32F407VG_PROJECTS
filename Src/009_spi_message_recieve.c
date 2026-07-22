/*
 * 009_spi_message_recieve.c
 *
 *  Created on: Jul 4, 2026
 *      Author: sneha
 */


#include <stm32f407xx.h>
#include<string.h>
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
SPI_Handle_t SPI2handle;

#define MAX_LEN 500
char RcvBuff[MAX_LEN];
volatile char ReadByte;
volatile uint8_t rcvStop =0;
volatile uint8_t dataAvaialble =0;

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
	SPIPins.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
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

	SPI_Handle_t SPI2handle;
	SPI2handle.pSPI_BaseAddr = SPI2;
	SPI_PeriClockControl(SPI2,ENABLE);
	SPI2handle.SPI_Config.Bus_Config = SPI_BUSCONFIG_FULL_DUPLEX;
	SPI2handle.SPI_Config.Devide_Mode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPI_Config.Sclk_SPEED = SPI_SCLK_SPEED_DIV32; //Generates serial clock of 2MHz
	SPI2handle.SPI_Config.DFF = SPI_DFF_8_bit;
	SPI2handle.SPI_Config.CPOL = CPOL_LOW;
	SPI2handle.SPI_Config.CPHA = CPHA_LOW;
	SPI2handle.SPI_Config.SSM = SSM_DI; //Hardware slave management enable for NSS pin

	SPI_Init(&SPI2handle);
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

void  Slave_GPIO_InterruptPinInit(void){
	GPIO_Handle_t SPIIntPins;
	SPIIntPins.pGPIOBaseAddr = GPIOD;
		GPIO_PeriClockControl(GPIOD, ENABLE);
		SPIIntPins.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
		SPIIntPins.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
		SPIIntPins.GPIOPinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_LOW;
		//SCLK
		SPIIntPins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
		GPIO_Init(&SPIIntPins);

		GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5, NVIC_IRQ_PRIO15);
		GPIO_IRQConfig(IRQ_NO_EXTI9_5, ENABLE);
}

int main(void){
    uint8_t dummy = 0xff;

    Slave_GPIO_InterruptPinInit();

    //This function is used to initialize the GPIO pins to behave as SPI2 pins
    SPI2_GpioInits();

    //This function is used to initialize the SPI2 peripheral parameters
	SPI2_Inits();
/*
	 * Making the SSOE 1 does NSS output enable
	 * The NSS pin is automatically managed by the hardware
	 * i.e. when SPOE = 1, NSS will be pulled to low
	 * and NSS pin will be high when SPOE = 0
	 */
	 SPI_SSOEConfig(SPI2,ENABLE);

	 SPI_IRQConfig(IRQ_NO_SPI2, ENABLE);

	 while(1){
		   rcvStop = 0;
		   while(!dataAvaialble);
		   	GPIO_IRQConfig(IRQ_NO_EXTI9_5, DISABLE);
			SPI_Peripheralcontrol(SPI2,ENABLE);
           while(!rcvStop){
				//Fetch the data from the SPI peripheral byte by byte in interrupt mode
				 while(SPI_Send_DataIT(&SPI2handle, &dummy,1));
				 while(SPI_Receive_DataIT(&SPI2handle, &ReadByte,1)==SPI_BUSY_IN_RX);
           }
           //confirm SPI is not busy
			while(SPI_GetFlagStatus(SPI2,SPI_BSY_FLAG));

			//Disable the SPI2 communication
			SPI_Peripheralcontrol(SPI2,DISABLE);
			//1. CMD_LED_CTRL   <Pin no (1)>  <value(1)>

			printf("rcvd data = %s \n", RcvBuff);
			dataAvaialble = 0;
			GPIO_IRQConfig(IRQ_NO_EXTI9_5, ENABLE);

	 }
}

void SPI2_IRQHandler(void){

	SPI_IRQHandling(&SPI2handle);

}
void SPI_ApplicationEvent_Callback(SPI_Handle_t *pSPIHandle,uint8_t AppEv)
{
	static uint32_t i = 0;
	if(AppEv == SPI_EVENT_RX_CMPLT)
	{
		RcvBuff[i++] = ReadByte;
		if(ReadByte == '\0' ||  (i==MAX_LEN)){
			rcvStop =1;
			RcvBuff[i-1] ='\0';
			i=0;
		}
	}
}
//slave data available interrupt handler
void EXTI9_5_IRQHandler (void){
	 GPIO_IRQHandling(GPIO_PIN_NO_6);
	 dataAvaialble=1;
}
