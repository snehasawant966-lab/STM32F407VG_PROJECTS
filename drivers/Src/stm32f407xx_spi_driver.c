/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Jun 25, 2026
 *      Author: sneha
 */

#include <stm32f407xx.h>
#include<stm32f407xx_spi_driver.h>
//private function are created using static keyword so that application layr should not call them
  static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
  static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
  static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);
 //Peripheral clock setup
 void SPI_PeriClockControl(SPI_RegDef_t *pSPI_BaseAddr, uint8_t EnorDi){

	 if (EnorDi == ENABLE){
	 if(pSPI_BaseAddr == SPI1){
		 SPI1_PCLK_EN();
	 }else if (pSPI_BaseAddr == SPI2){
		 SPI2_PCLK_EN();
	 }else if (pSPI_BaseAddr == SPI3){
		 SPI3_PCLK_EN();
	 }
 } else {
	 if(pSPI_BaseAddr == SPI1){
	 		 SPI1_PCLK_DI();
	 	 }else if (pSPI_BaseAddr == SPI2){
	 		 SPI2_PCLK_DI();
	 	 }else if (pSPI_BaseAddr == SPI3){
	 		 SPI3_PCLK_DI();
	 	 }
 }

 }
 //INIT AND DEINIT
  void SPI_Init(SPI_Handle_t *pSPIHandle){

  SPI_PeriClockControl(pSPIHandle->pSPI_BaseAddr,ENABLE);
  uint32_t tempreg = 0;
  //1. Configuring the mode
  tempreg |= pSPIHandle->SPI_Config.Devide_Mode << SPI_CR1_MSTR;

  // 2.configure the bus config
  if(pSPIHandle->SPI_Config.Bus_Config == SPI_BUSCONFIG_FULL_DUPLEX){
	//bidimode should be cleared

	  tempreg &= ~(1<<SPI_CR1_BIDIMODE);
  }else if(pSPIHandle->SPI_Config.Bus_Config == SPI_BUSCONFIG_HALF_DUPLEX){
		//bidimode should be set
	  tempreg |= (1<<SPI_CR1_BIDIMODE);
  }else if(pSPIHandle->SPI_Config.Bus_Config == SPI_BUSCONFIG_SIMPLEX_RXONLY){
		//bidimode should be cleared
	  tempreg &= ~(1<<SPI_CR1_BIDIMODE);
	  tempreg |= (1<< SPI_CR1_RXONLY);
  }

  //3. configure the spi serial clock speed (baud rate)

  tempreg |= pSPIHandle->SPI_Config.Sclk_SPEED << SPI_CR1_BR;

  //4. Configure the DFF
  tempreg |= pSPIHandle->SPI_Config.DFF << SPI_CR1_DFF ;

  //5. configure the CPOL
  tempreg |= pSPIHandle->SPI_Config.CPOL <<SPI_CR1_CPOL;

  //5. configure the CPHA
   tempreg |= pSPIHandle->SPI_Config.CPHA << SPI_CR1_CPHA;

   pSPIHandle->pSPI_BaseAddr->CR1 |= tempreg;
  }
  void SPI_DeInit(SPI_RegDef_t *pSPI_BaseAddr){
	  if (pSPI_BaseAddr == SPI1){
	  			   SPI1_REG_RESET();
	  		   }else if (pSPI_BaseAddr == SPI2){
	  			   SPI2_REG_RESET();
	  		   }else if (pSPI_BaseAddr == SPI3){
	  			   SPI3_REG_RESET();
	  		   }
  }

  uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPI_BaseAddr,uint32_t FlagName){
	  if (pSPI_BaseAddr->SR & FlagName){

		  return FLAG_SET;
	  }
	  return FLAG_RESET;
  }

 //Data Send and Receive
//This is blocking call or polling type API function
  void SPI_Send_Data(SPI_RegDef_t *pSPI_BaseAddr, uint8_t *pTXBuffer, uint32_t Len){
   while(Len > 0 ){
	   //1. wait until TXE is set
	   // while(!(pSPI_BaseAddr->SR & (1<<1)));
	   while(SPI_GetFlagStatus(pSPI_BaseAddr,SPI_TXE_FLAG) == FLAG_RESET);
	   //2.CHECK THE DFF BIT IN CR1
	   if(pSPI_BaseAddr->CR1 & (1<<SPI_CR1_DFF)){
		   //16 bit data format
		   //1. LOad the data in DR
		   pSPI_BaseAddr->DR = *((uint16_t*)pTXBuffer);
		   Len--;
		   Len--;
		   (uint16_t*)pTXBuffer++;
	   }else{
		   //8 bit DFF
		   //1. LOad the data in DR
				   pSPI_BaseAddr->DR = *pTXBuffer;
				   Len--;
				   pTXBuffer++;
	   }
   }
  }
void SPI_Receive_Data(SPI_RegDef_t *pSPI_BaseAddr, uint8_t *pRXBuffer, uint32_t Len){
 while(Len>0){
	//wait until RXNE is SET
	while(SPI_GetFlagStatus(pSPI_BaseAddr,SPI_SR_RXNE)== FLAG_RESET);
    //Check the DFF bit in CR1
	if((pSPI_BaseAddr->CR1 & (1 << SPI_CR1_DFF))){
		        *((uint16_t*)pRXBuffer) = pSPI_BaseAddr->DR;
				pRXBuffer+=2; //OR (uint16_t*)pRXBuffer++;
				Len-=2;
	}else{
				*pRXBuffer = pSPI_BaseAddr->DR;
				pRXBuffer++;
				Len--;
	}
 }
}


uint8_t SPI_Send_DataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTXBuffer, uint32_t Len){
   uint8_t state = pSPIHandle->TXState;
	if(state != SPI_BUSY_IN_TX){
	//Save the TX buffer address and Len information in some global variables
	pSPIHandle->pTXBuffer = pTXBuffer;
	pSPIHandle->TXLen = Len;
	//mark the SPI state as busy in transmission so that
	//no other code can take over same SPI peripheral until transmission is over
	pSPIHandle->TXState = SPI_BUSY_IN_TX;
	//3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
	pSPIHandle->pSPI_BaseAddr->CR2 |= (1<<SPI_CR2_TXEIE);
	}
	//4. Data transmission will be handled by the ISR code
     return state;
}
uint8_t SPI_Receive_DataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRXBuffer, uint32_t Len){
	  uint8_t state = pSPIHandle->RXState;
		if(state != SPI_BUSY_IN_RX){
		//Save the RX buffer address and Len information in some global variables
		pSPIHandle->pRXBuffer = pRXBuffer;
		pSPIHandle->RXLen = Len;
		//mark the SPI state as busy in Receiving so that
		//no other code can take over same SPI peripheral until receive action is over
		pSPIHandle->RXState = SPI_BUSY_IN_RX;
		//3. Enable the RXNEIE control bit to get interrupt whenever RXE flag is set in SR
		pSPIHandle->pSPI_BaseAddr->CR2 |= (1<<SPI_CR2_RXNEIE);
		}
		//4. Data transmission will be handled by the ISR code
	     return state;
}


 //IRQ Configuration and ISR handling

  void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi){

	  if(EnorDi == ENABLE){
	  	if (IRQNumber <= 31){
	  		*NVIC_ISER0 |= (1 << IRQNumber);
	  	}else if (IRQNumber > 31 && IRQNumber <= 64){

	  		*NVIC_ISER1 |= (1 << (IRQNumber%32));

	  	}else if(IRQNumber >=64 && IRQNumber < 96){
	  		*NVIC_ISER2 |= (1 << (IRQNumber%64));

	  	}
	  }else {
	  	if (IRQNumber <= 31){
	  		*NVIC_ICER0 |= (1 << IRQNumber);

	  		}else if (IRQNumber > 31 && IRQNumber <= 64){

	  			*NVIC_ICER1 |= (1 << (IRQNumber%32));

	  		}else if(IRQNumber >=64 && IRQNumber < 96){

	  			*NVIC_ICER2 |= (1 << (IRQNumber%64));
	  		}
      }
  }

  void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
		 uint8_t iprx = IRQNumber/4;
		 uint8_t iprx_section = IRQNumber%4;
		 uint8_t shift_amount = (8*iprx_section) + (8-NO_PR_BITS_IMPLEMENTED);
		 *(NVIC_PR_BASE_ADDR + (iprx)) |= (IRQPriority << shift_amount);
  }


  void SPI_IRQHandling(SPI_Handle_t *pSPIHandle){
   uint8_t temp1;
   uint8_t temp2;

	  //First lets check for TXE
	  temp1 = pSPIHandle->pSPI_BaseAddr->SR & (1<<SPI_SR_TXE);
	  temp2 = pSPIHandle->pSPI_BaseAddr->CR2 & (1<<SPI_CR2_TXEIE);

	  if(temp1 && temp2){
		  //handle TXE
		  spi_txe_interrupt_handle(pSPIHandle);
	  }
	  // lets check for RXNE
		  temp1 = pSPIHandle->pSPI_BaseAddr->SR & (1<<SPI_SR_RXNE);
		  temp2 = pSPIHandle->pSPI_BaseAddr->CR2 & (1<<SPI_CR2_RXNEIE);

	  if(temp1 && temp2){
		  //handle RXE
		  spi_rxne_interrupt_handle(pSPIHandle);
	  }

	  // lets check for overrun error
	  		  temp1 = pSPIHandle->pSPI_BaseAddr->SR & (1<<SPI_SR_OVR);
	  		  temp2 = pSPIHandle->pSPI_BaseAddr->CR2 & (1<<SPI_CR2_ERRIE);

	  	  if(temp1 && temp2){
	  		  //handle TXE
	  		  spi_ovr_err_interrupt_handle(pSPIHandle);
	  	  }
  }

//some helper function implementation
  static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle){
	  if(pSPIHandle->pSPI_BaseAddr->CR1 & (1<<SPI_CR1_DFF)){
	 		   //16 bit data format
	 		   //1. LOad the data in DR
		  	  pSPIHandle->pSPI_BaseAddr->DR = *((uint16_t*)pSPIHandle->pTXBuffer);
	 		  pSPIHandle->TXLen--;
	 		  pSPIHandle->TXLen--;
	 		   (uint16_t*)pSPIHandle->pTXBuffer++;
	 	   }else{
	 		   //8 bit DFF
	 		   //1. LOad the data in DR
	 		  pSPIHandle->pSPI_BaseAddr->DR = *pSPIHandle->pTXBuffer;
	 		  pSPIHandle->TXLen--;
	 		  pSPIHandle->pTXBuffer++;
	 	   }

	  if(!pSPIHandle->TXLen){
		  //TXLen is zero , so close the spi transmission and inform the application that TX is over
		  //This prevents interrupts from setting up of TXE flag
		  SPI_CloseTransmission(pSPIHandle);
		  SPI_ApplicationEvent_Callback(pSPIHandle,SPI_EVENT_TX_CMPLT);

	  }
  }
  static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle){
	  if((pSPIHandle->pSPI_BaseAddr->CR1 & (1 << SPI_CR1_DFF))){
	  		        *((uint16_t*)pSPIHandle->pRXBuffer) = pSPIHandle->pSPI_BaseAddr->DR;
	  		      pSPIHandle->pRXBuffer+=2; //OR (uint16_t*)pRXBuffer++;
	  		    pSPIHandle->RXLen-=2;
	  	}else{
	  				*pSPIHandle->pRXBuffer = pSPIHandle->pSPI_BaseAddr->DR;
	  				pSPIHandle->pRXBuffer++;
	  				pSPIHandle->RXLen--;
	  	}
	  if(!pSPIHandle->RXLen){
	 		  //RXLen is zero , so close the spi transmission and inform the application that RX is over
	 		  //This prevents interrupts from setting up of TXE flag
		      SPI_CloseReception(pSPIHandle);
	 		  SPI_ApplicationEvent_Callback( pSPIHandle,SPI_EVENT_RX_CMPLT);
	 	  }
  }
  static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle){
	  uint8_t temp;
	  //clear the ovr flag

	  if(pSPIHandle->TXState != SPI_BUSY_IN_TX){

		  temp = pSPIHandle->pSPI_BaseAddr->DR;
		  temp = pSPIHandle->pSPI_BaseAddr->SR;
	  }
	  (void)temp;
	  //inform the application
	SPI_ApplicationEvent_Callback( pSPIHandle,SPI_EVENT_OVR_ERR);
  }

  void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle){
	  pSPIHandle->pSPI_BaseAddr->CR2 &= ~(1<<SPI_CR2_TXEIE);
	  pSPIHandle->pTXBuffer = NULL;
	  pSPIHandle->TXLen =0;
	  pSPIHandle->TXState = SPI_READY;
  }
  void SPI_CloseReception(SPI_Handle_t *pSPIHandle){
	  pSPIHandle->pSPI_BaseAddr->CR2 &= ~(1<<SPI_CR2_RXNEIE);
	  pSPIHandle->pRXBuffer = NULL;
	  pSPIHandle->RXLen =0;
	  pSPIHandle->RXState = SPI_READY;
  }
  void SPI_ClearOVRFlag(SPI_RegDef_t *pSPI_BaseAddr){
	  uint8_t temp;
	  temp = pSPI_BaseAddr->DR;
	  temp = pSPI_BaseAddr->SR;
	  (void)temp;

  }

  void SPI_Peripheralcontrol(SPI_RegDef_t *pSPI_BaseAddr, uint8_t EnorDi){


	  if(EnorDi == ENABLE){

		pSPI_BaseAddr->CR1 |= (1<<SPI_CR1_SPE)  ;
	  }else{
			pSPI_BaseAddr->CR1 &= ~(1<<SPI_CR1_SPE)  ;

	  }
  }

  void SPI_SSIConfig(SPI_RegDef_t *pSPI_BaseAddr, uint8_t EnorDi)
  {
	  if(EnorDi == ENABLE){

	  		pSPI_BaseAddr->CR1 |= (1<<SPI_CR1_SSI)  ;
	  	  }else{
	  			pSPI_BaseAddr->CR1 &= ~(1<<SPI_CR1_SSI)  ;

	  	  }
  }


  void SPI_SSOEConfig(SPI_RegDef_t *pSPI_BaseAddr, uint8_t EnorDi)
  {
	  if(EnorDi == ENABLE){

	 	  		pSPI_BaseAddr->CR2 |= (1<<SPI_CR2_SSOE) ;
	 	  	  }else{
	 	  			pSPI_BaseAddr->CR2 &= ~(1<<SPI_CR2_SSOE)  ;

	 	  	  }
  }



  __weak void SPI_ApplicationEvent_Callback(SPI_Handle_t *pSPIHandle,uint8_t App_Ev){\

	  //This is a weak implementation and application may override it
  }







