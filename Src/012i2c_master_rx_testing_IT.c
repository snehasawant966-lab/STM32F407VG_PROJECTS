/*
 * 010i2c_master_tx_testing.c
 *
 *  Created on: Jul 10, 2026
 *      Author: sneha
 */


#include <stm32f407xx.h>
#include<string.h>
#include<stdio.h>
//flag variable
uint8_t rxcmplt = RESET;
#define  MY_Addr 0x61
#define SLAVE_ADDR 0x68
void delay(void){
	for(uint32_t i=0;i<=500000;i++);
}

I2C_Handle_t I2C1Handle;

//SOME DATA

uint8_t rcv_buff[32];
/*
 * PB6 - SCL
 * PB9 - SDA
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
	I2CPins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	GPIO_Init(&I2CPins);
}

void I2C1_Inits(void){

	I2C1Handle.pI2CBaseAddr = I2C1;
	I2C_PeriClockControl(I2C1,ENABLE);
	I2C1Handle.I2CConfig.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2CConfig.I2C_DeviceAddress = MY_Addr;
	I2C1Handle.I2CConfig.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2CConfig.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
	I2C1Handle.TxRxState = I2C_Ready;

	I2C_Init(&I2C1Handle);
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
	uint8_t CommandCode;
	uint8_t Len;

	GPIO_ButtonInit();
	I2C1_GpioInits();
	I2C1_Inits();

	//I2C IRQ CONFIGURATIONS
	 I2C_IRQConfig(IRQ_NO_I2C1_EV,ENABLE);
	 I2C_IRQConfig(IRQ_NO_I2C1_ER,ENABLE);
	 /*NO NEED HERE
	 I2C_IRQPriorityConfig(IRQ_NO_I2C1_EV, NVIC_IRQ_PRIO15);
	 I2C_IRQPriorityConfig(IRQ_NO_I2C1_ER, NVIC_IRQ_PRIO12);
*/

	I2C_PeriClockControl(I2C1,ENABLE);//PE=1;
	//ACK BIT MADE 1 AFTER PE =1
	I2C1Handle.pI2CBaseAddr->CR1 |= (1<<I2C_CR1_ACK);

	while(1){
			//wait for button press
			 while(!GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0))
			 {

			 }
			 //to avoid button de-bouncing related issues 200ms of delay
			 delay();

			CommandCode=0x51;

			//Data write: Master sending command to slave 0x51 command means tell me lenghth of data
			while( I2C_MasterSendDataIT(&I2C1Handle,&CommandCode,1,SLAVE_ADDR,I2C_ENABLE_SR) != I2C_Ready);

			//Data read:  Master reading response from slave
			while( I2C_MasterReceiveDataIT(&I2C1Handle,&Len,1,SLAVE_ADDR,I2C_ENABLE_SR) != I2C_Ready);

			CommandCode=0x52;
			//Data write: Master sending command to slave 0x52 command means send data
			while( I2C_MasterSendDataIT(&I2C1Handle,&CommandCode,1,SLAVE_ADDR,I2C_ENABLE_SR) != I2C_Ready);

			//Data read:  Master reading response from slave which is actual data
			while( I2C_MasterReceiveDataIT(&I2C1Handle,rcv_buff,Len,SLAVE_ADDR,I2C_DISABLE_SR)!= I2C_Ready);
		    rxcmplt = RESET;
			while(rxcmplt != SET);
		    rcv_buff[Len+1] ='\0';
		    printf("Data : %s",rcv_buff);
		    rxcmplt = RESET;

	}
}
void I2C1_EV_IRQHandler(void){
	I2C_EV_Handler(&I2C1Handle);
}

void I2C1_ER_IRQHandler(void){
	I2C_ER_IRQHandling(&I2C1Handle);
}

void I2C_ApplicationEvent_Callback(I2C_Handle_t *pI2CHandle,uint8_t App_Ev){
	if(App_Ev == I2C_EV_TX_CMPLT){
		printf("TX is completed");
	}else if(App_Ev == I2C_EV_RX_CMPLT){
		printf("RX is completed");
		rxcmplt = SET;
	}else if(App_Ev == I2C_ERROR_AF){
		printf("ERR : ACK FAILURE");
		//in master ack failure happens when slave fails to send ack for the byte
		//sent from master.
		I2C_CloseSendData(pI2CHandle);
		I2C_GenerateStopCondition(I2C1);
		//hang in infinite loop
		while(1);
	}
}
