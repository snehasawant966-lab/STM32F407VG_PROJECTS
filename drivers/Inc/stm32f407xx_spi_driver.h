/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: Jun 25, 2026
 *      Author: sneha
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_

#include <stm32f407xx.h>

 typedef struct{
	 __vo uint32_t Devide_Mode;
	 __vo uint32_t Bus_Config;
	 __vo uint32_t Sclk_SPEED;
	 __vo uint32_t DFF;
	 __vo uint32_t CPOL;
	 __vo uint32_t CPHA;
	 __vo uint32_t SSM;
 }SPI_Config_t;

 typedef struct{
	SPI_RegDef_t *pSPI_BaseAddr;
	SPI_Config_t  SPI_Config;
	uint8_t  *pTXBuffer;
	uint8_t  *pRXBuffer;
	uint8_t   TXLen;
	uint8_t   RXLen;
	uint8_t   TXState;
	uint8_t   RXState;
 }SPI_Handle_t;

 //Macros of DFF

 #define SPI_DFF_16_bit    1
 #define SPI_DFF_8_bit     0

 ////Macros of SSM
 #define SSM_EN  1
 #define SSM_DI  0

 //Macros of DEVICE_MODE
 #define SPI_DEVICE_MODE_MASTER 1
 #define SPI_DEVICE_MODE_SLAVE  0

 //Macros of Bus_Config

#define SPI_BUSCONFIG_FULL_DUPLEX       1
#define SPI_BUSCONFIG_HALF_DUPLEX       2
#define SPI_BUSCONFIG_SIMPLEX_RXONLY    3

 //CLOCK SPEED MACROS
#define SPI_SCLK_SPEED_DIV2    0
#define SPI_SCLK_SPEED_DIV4    1
#define SPI_SCLK_SPEED_DIV8    2
#define SPI_SCLK_SPEED_DIV16   3
#define SPI_SCLK_SPEED_DIV32   4
#define SPI_SCLK_SPEED_DIV64   5
#define SPI_SCLK_SPEED_DIV128  6
#define SPI_SCLK_SPEED_DIV256  7

 //macros of CPHA
#define CPHA_HIGH    1
#define CPHA_LOW     0
 //macros CPOL

#define CPOL_HIGH    1
#define CPOL_LOW     0

// SPI RELATED FLAG DEFINITIONS

#define SPI_TXE_FLAG  (1<<SPI_SR_TXE)
#define SPI_RXNE_FLAG  (1<<SPI_SR_RXNE)
#define SPI_BSY_FLAG  (1<<SPI_SR_BSY)
#define SPI_CHSIDE_FLAG  (1<<SPI_SR_CHSIDE)
#define SPI_UDR_FLAG  (1<<SPI_SR_UDR)
#define SPI_CRCERR_FLAG  (1<<SPI_SR_CRCERR)
#define SPI_MODF_FLAG  (1<<SPI_SR_MODF)
#define SPI_OVR_FLAG  (1<<SPI_SR_OVR)
#define SPI_FRE_FLAG  (1<<SPI_SR_FRE)


//SPI application states
#define SPI_READY                0
#define SPI_BUSY_IN_RX           1
#define SPI_BUSY_IN_TX           2

//Posible SPI application ecvents
#define SPI_EVENT_TX_CMPLT    1
#define SPI_EVENT_RX_CMPLT    2
#define SPI_EVENT_OVR_ERR     3
#define SPI_EVENT_CRC_ERR     4




 //API Supported by SPI

 //Peripheral clock setup
 void SPI_PeriClockControl(SPI_RegDef_t *pSPI_BaseAddr, uint8_t EnorDi);

 //INIT AND DEINIT
  void SPI_Init(SPI_Handle_t *pSPIHandle);//initialize the given registers
  void SPI_DeInit(SPI_RegDef_t *pSPI_BaseAddr);


 //Data Send and Receive

  void SPI_Send_Data(SPI_RegDef_t *pSPI_BaseAddr, uint8_t *pTXBuffer, uint32_t Len);
  void SPI_Receive_Data(SPI_RegDef_t *pSPI_BaseAddr, uint8_t *pRXBuffer, uint32_t Len);

  uint8_t SPI_Send_DataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTXBuffer, uint32_t Len);
  uint8_t SPI_Receive_DataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRXBuffer, uint32_t Len);

 //IRQ Configuration and ISR handling

  void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
  void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
  void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

//other peripheral control APIs


  void SPI_Peripheralcontrol(SPI_RegDef_t *pSPI_BaseAddr, uint8_t EnorDi);
  void SPI_SSIConfig(SPI_RegDef_t *pSPI_BaseAddr, uint8_t EnorDi);
  void SPI_SSOEConfig(SPI_RegDef_t *pSPI_BaseAddr, uint8_t EnorDi);
  uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPI_BaseAddr,uint32_t FlagName);
  void SPI_ClearOVRFlag(SPI_RegDef_t *pSPI_BaseAddr);
  void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
  void SPI_CloseReception(SPI_Handle_t *pSPIHandle);
  //application callback
  void SPI_ApplicationEvent_Callback(SPI_Handle_t *pSPIHandle,uint8_t App_Ev);

#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
