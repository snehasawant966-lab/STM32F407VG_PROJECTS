/*
 * stm32f407xx_I2C_driver.h
 *
 *  Created on: Jul 7, 2026
 *      Author: sneha
 */

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include <stm32f407xx.h>


/*configuration structure for I2C peripheral
 */
typedef struct{
	 uint32_t I2C_SCLSpeed;
	 uint8_t I2C_DeviceAddress;
	 uint8_t I2C_ACKControl;
	 uint8_t I2C_FMDutyCycle;
}I2C_Config_t;

/*Handle structure for I2C
 */
typedef struct{
	I2C_RegDef_t *pI2CBaseAddr;
	I2C_Config_t I2CConfig;
	uint8_t *pTXBuffer;//To store the app. TX buffer address
	uint8_t *pRXBuffer;//To store the app. RX buffer address
	uint32_t TXLen; //To store TX Length
	uint32_t RXLen; //To store RX Length
	uint8_t TxRxState; //To store communication state
	uint8_t DevAddr; //To store Slave/Device address
	uint32_t RxSize; //To store RX size
	uint8_t Sr; // To store repeated start value
}I2C_Handle_t;

/*@I2C_SCLSpeed
 */

#define I2C_SCL_SPEED_SM  100000   //100KHz
#define I2C_SCL_SPEED_FM4K  400000   //400KHz
#define I2C_SCL_SPEED_FM2K  200000   //400KHz

/*@I2C_ACKCONTROL  --DISABLED BY DEFAULT
 */
#define I2C_ACK_ENABLE     1
#define I2C_ACK_DISABLE    0

/*@I2C_DUTYCYCLE
 */
#define I2C_FM_DUTY_2         0
#define I2C_FM_DUTY_16_9      1


// SPI RELATED FLAG DEFINITIONS

#define I2C_FLAG_TXE     (1<<I2C_SR1_TxE)
#define I2C_FLAG_RXNE    (1<<I2C_SR1_RxNE)
#define I2C_FLAG_SB      (1<<I2C_SR1_SB)
#define I2C_FLAG_ADDR    (1<<I2C_SR1_ADDR)
#define I2C_FLAG_BTF     (1<<I2C_SR1_BTF)
#define I2C_FLAG_STOPF   (1<<I2C_SR1_STOPF)
#define I2C_FLAG_BERR    (1<<I2C_SR1_BERR)
#define I2C_FLAG_ARLO    (1<<I2C_SR1_ARLO)
#define I2C_FLAG_AF      (1<<I2C_SR1_AF)
#define I2C_FLAG_OVR     (1<<I2C_SR1_OVR)
#define I2C_FLAG_TIMEOUT (1<<I2C_SR1_TIMEOUT)

#define I2C_DISABLE_SR   RESET
#define I2C_ENABLE_SR    SET

//I2C application events macros
#define I2C_EV_TX_CMPLT   0
#define I2C_EV_RX_CMPLT   1
#define I2C_EV_STOP       2
#define I2C_EV_DATA_REQ   8
#define I2C_EV_DATA_RCV   9

//I2C ERROR EVENT MACROS
#define I2C_ERROR_BERR    3
#define I2C_ERROR_ARLO    4
#define I2C_ERROR_AF      5
#define I2C_ERROR_OVR     6
#define I2C_ERROR_TIMEOUT 7




//Application states

#define I2C_Ready           0
#define I2C_BUSY_IN_RX      1
#define I2C_BUSY_IN_TX      2


//Peripheral clock setup
 void I2C_PeriClockControl(I2C_RegDef_t *pI2C_BaseAddr, uint8_t EnorDi);

 //INIT AND DEINIT
  void I2C_Init(I2C_Handle_t *pI2CHandle);//initialize the given registers
  void I2C_DeInit(I2C_RegDef_t *pI2C_BaseAddr);


 //Data Send and Receive
  void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTXBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
  void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);

  //Data Send and Receive IT
  uint8_t  I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxBuffer, uint32_t Len,uint8_t SlaveAddr,uint8_t Sr);
  uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);

//slave api
   void I2C_SlaveSendData(I2C_RegDef_t *pI2C_BaseAddr, uint8_t data);
   uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C_BaseAddr);
   //IRQ Configuration and ISR handling

  void I2C_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
  void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
  void I2C_EV_Handler(I2C_Handle_t *pI2CHandle);
  void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);
//other peripheral control APIs


  void I2C_PeripheralControl(I2C_RegDef_t *pI2C_BaseAddr, uint8_t EnorDi);
  uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2C_BaseAddr,uint32_t FlagName);
  //application callback
  void I2C_ApplicationEvent_Callback(I2C_Handle_t *pI2CHandle,uint8_t App_Ev);
  void I2C_GenerateStopCondition(I2C_RegDef_t *pI2C_BaseAddr);
//
  void I2C_Close_ReceiveData(I2C_Handle_t *pI2CHandle);
   void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);
   void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2C_BaseAddr, uint8_t EnorDi);

#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */
