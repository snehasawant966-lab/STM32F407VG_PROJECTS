/*
 * 010i2c_master_tx_testing.c
 *
 *  Created on: Jul 10, 2026
 *      Author: sneha
 */


#include <stm32f407xx.h>
#include<string.h>
#include<stdio.h>

#define SLAVE_ADDR 0x68

#define  MY_Addr SLAVE_ADDR

void delay(void){
	for(uint32_t i=0;i<=500000;i++);
}

I2C_Handle_t I2C1Handle;

//SOME DATA

uint8_t Tx_buff[32] = "STM32 Slave Mode Testing..";

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
	GPIO_ButtonInit();
	I2C1_GpioInits();
	I2C1_Inits();

	//I2C IRQ CONFIGURATIONS
	 I2C_IRQConfig(IRQ_NO_I2C1_EV,ENABLE);
	 I2C_IRQConfig(IRQ_NO_I2C1_ER,ENABLE);

	 I2C_SlaveEnableDisableCallbackEvents(I2C1,ENABLE);

	I2C_PeriClockControl(I2C1,ENABLE);//PE=1;
	//ACK BIT MADE 1 AFTER PE =1 //manage acking
	I2C1Handle.pI2CBaseAddr->CR1 |= (1<<I2C_CR1_ACK);

	while(1);


}
void I2C1_EV_IRQHandler(void){
	I2C_EV_Handler(&I2C1Handle);
}

void I2C1_ER_IRQHandler(void){
	I2C_ER_IRQHandling(&I2C1Handle);
}

void I2C_ApplicationEvent_Callback(I2C_Handle_t *pI2CHandle,uint8_t App_Ev){

	static uint8_t commandcode = 0;
	static uint8_t cnt = 0;

	if(App_Ev == I2C_EV_DATA_REQ){
	//master wants some data. slave has to send it
	if(commandcode== 0x51){

	//send the length info
		I2C_SlaveSendData(pI2CHandle->pI2CBaseAddr, strlen((char*)Tx_buff));

	} else if(commandcode== 0x52){

	//send the actual content of tx buffer
		I2C_SlaveSendData(pI2CHandle->pI2CBaseAddr, Tx_buff[cnt++]);

	}
}else if(App_Ev == I2C_EV_DATA_RCV){
	//data is waiting for the slave to read. slave has to read it
	commandcode = I2C_SlaveReceiveData(pI2CHandle->pI2CBaseAddr);
}else if(App_Ev == I2C_ERROR_AF){
    //this happens only during slave transmission
	//master has sent the nack so slave should understand that master doesnt need more data
	commandcode = 0xff;
	cnt =0;
}else if(App_Ev == I2C_EV_STOP){
  //this happend only during slave reception
 // master has ended the I2C communication with the slave
}

}
