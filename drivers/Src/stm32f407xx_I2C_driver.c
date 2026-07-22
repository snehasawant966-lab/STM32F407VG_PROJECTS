/*
 * stm32f407xx_I2C_driver.c
 *
 *  Created on: Jul 7, 2026
 *      Author: sneha
 */

//Peripheral clock setup

#include "stm32f407xx_I2C_driver.h"
 void I2C_PeriClockControl(I2C_RegDef_t *pI2C_BaseAddr, uint8_t EnorDi){
	 if (EnorDi == ENABLE){
		 if(pI2C_BaseAddr == I2C1){
			 I2C1_PCLK_EN();
		 }else if (pI2C_BaseAddr == I2C2){
			 I2C2_PCLK_EN();
		 }else if (pI2C_BaseAddr == I2C3){
			 I2C3_PCLK_EN();
		 }
	 } else {
		 if(pI2C_BaseAddr == I2C1){
			 I2C1_PCLK_DI();
		 }else if (pI2C_BaseAddr == I2C2){
			 I2C2_PCLK_DI();
		 }else if (pI2C_BaseAddr == I2C3){
			 I2C3_PCLK_DI();
		 }
	 }
 }
 /*
  * system clock is HSI, System clock is modified using AHB1 PRESC
  * After then AHB1 Clock is given to APB1 prescalar and then will get the APB1 peri clock where I2C is connected.
  *
  * in RCC prescalars are configured.
  */

 uint32_t RCCGetPPllClockOutput(){
	 return 0;
 }

 uint16_t AHB_PreScalar[8] = {2,4,8,16,64,128,256,512};
 uint8_t APB1_PreScalar[8] = {2,4,8,16};


 static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2C_BaseAddr);
 static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2C_BaseAddr, uint8_t SlaveAddr);
 static void I2C_ExecuteAddressPhase_Read(I2C_RegDef_t *pI2C_BaseAddr, uint8_t SlaveAddr);
 static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);
 static void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);
 static void I2C_MasterHandle_RXNEInterrupt(I2C_Handle_t *pI2CHandle);
 static void I2C_MasterHandle_TXEInterrupt(I2C_Handle_t *pI2CHandle);



 uint32_t RCC_GetPCLK1Value(void){
	 uint32_t pclk1,SystemClk ;
	 uint8_t clksrc, temp, ahbp, apbp1;
	 clksrc = ((RCC->CFGR >>2) & 0x3);
	 if (clksrc == 0){
		 SystemClk = 16000000;
	 }else if (clksrc == 1){
		 SystemClk = 8000000;
	 }else if (clksrc == 2){
		 SystemClk = RCCGetPPllClockOutput();
	 }
	 //AHB
	 temp = ((RCC->CFGR >>4) & 0xf);
	 if(temp<8){
		 ahbp = 1;
	 }else{
		 ahbp=AHB_PreScalar[temp-8];
	 }

	 //APB1
		 temp = ((RCC->CFGR >>10) & 0x7);
		 if(temp<4){
			 apbp1 = 1;
		 }else{
			 apbp1 = APB1_PreScalar[temp-4];
		 }

		 pclk1 = (SystemClk/ahbp)/apbp1;
	 return pclk1;
 }

 //INIT AND DEINIT
  void I2C_Init(I2C_Handle_t *pI2CHandle){

    I2C_PeriClockControl(pI2CHandle->pI2CBaseAddr,ENABLE);
    uint32_t tempreg =0;
    tempreg |= (pI2CHandle->I2CConfig.I2C_ACKControl << I2C_CR1_ACK);
    pI2CHandle->pI2CBaseAddr->CR1 = tempreg;

     tempreg =0;
     tempreg |= RCC_GetPCLK1Value()/1000000U;
     pI2CHandle->pI2CBaseAddr->CR2 = (tempreg & 0x3f);


     //program the device own address

     tempreg |= pI2CHandle->I2CConfig.I2C_DeviceAddress <<1;
     pI2CHandle->pI2CBaseAddr->OAR1 = tempreg;

     tempreg |= (1<<14);
     pI2CHandle->pI2CBaseAddr->OAR1 = tempreg;

     // CCR Calculations
     uint16_t ccr_value = 0;
     tempreg = 0;
     if(pI2CHandle->I2CConfig.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
    	 //Mode is standard mode
    	 ccr_value = (RCC_GetPCLK1Value()/(2*pI2CHandle->I2CConfig.I2C_SCLSpeed));
    	 tempreg = (ccr_value & 0xFFF);
     }else{
    	 //mode is fast mode
    	 tempreg |= (1<<15);
    	 tempreg |= (pI2CHandle->I2CConfig.I2C_FMDutyCycle <<14);
    	 if(pI2CHandle->I2CConfig.I2C_FMDutyCycle == I2C_FM_DUTY_2){
         ccr_value = (RCC_GetPCLK1Value()/(3*pI2CHandle->I2CConfig.I2C_SCLSpeed));

    	 }else{
             ccr_value = (RCC_GetPCLK1Value()/(25*pI2CHandle->I2CConfig.I2C_SCLSpeed));
    	 }
    	 tempreg = (ccr_value & 0xFFF);
     }
     pI2CHandle->pI2CBaseAddr->CCR = tempreg;
//TRISE Configuration
     uint8_t trise;
     if(pI2CHandle->I2CConfig.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
//Mode is standard mode

    	 trise = (RCC_GetPCLK1Value()/1000000U)+1;
     }else{
//mode is fast mode
	     trise = ((RCC_GetPCLK1Value()*300)/1000000U)+1;
     }
     pI2CHandle->pI2CBaseAddr->TRISE = (trise & 0x3F);
  }
  void I2C_DeInit(I2C_RegDef_t *pI2C_BaseAddr){
	  if (pI2C_BaseAddr == I2C1){
		  			   I2C1_REG_RESET();
		  		   }else if (pI2C_BaseAddr == I2C2){
		  			   I2C2_REG_RESET();
		  		   }else if (pI2C_BaseAddr == I2C3){
		  			   I2C3_REG_RESET();
		  		   }
  }

 //Data Send and Receive

  void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTXBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
  {
	  //1. 	Generate the start condition

	  I2C_GenerateStartCondition(pI2CHandle->pI2CBaseAddr);

	  //2. confirm that start generation is completed by checking the SB flag in the SR1
	  //Note : until SB is cleared SCL will be stretched (pulled to low)
        while(! I2C_GetFlagStatus(pI2CHandle->pI2CBaseAddr, I2C_FLAG_SB));

	  //3. Send the address of the slave with r/nw bit setr w(0) (total 8 bits)

        I2C_ExecuteAddressPhase(pI2CHandle->pI2CBaseAddr, SlaveAddr);

	  //4. confirm that address phase is completed by checking the ADDR flag in the SRI

        while(! I2C_GetFlagStatus(pI2CHandle->pI2CBaseAddr, I2C_FLAG_ADDR));

	  //5. Clear the ADDR flag according to its software sequence
	  //Note: until ADDR is cleared SCL will be stretched (pulled to low)
        I2C_ClearADDRFlag(pI2CHandle);
	  //6. send the data until Len becomes 0
         while (Len>0)
         {
        	 while(!I2C_GetFlagStatus(pI2CHandle->pI2CBaseAddr, I2C_FLAG_TXE )); //Wait till TXE is set
        	 pI2CHandle->pI2CBaseAddr->DR = *pTXBuffer;
        	 pTXBuffer++;
        	 Len--;
         }

	  //7. When Len becomems zero wait for TXE =1 and BTF =1 before genarating the STOP condition
	  //Note : TXE =1, BTF = 1, means that both SR and DR are empty and next transmission should begin
	  //when BTF = 1 SCL will be stretched (pulled to low)
    	 while(!I2C_GetFlagStatus(pI2CHandle->pI2CBaseAddr, I2C_FLAG_TXE )); //Wait till TXE is set
    	 while(!I2C_GetFlagStatus(pI2CHandle->pI2CBaseAddr, I2C_FLAG_BTF )); //Wait till BTF is set


	  //Generate STOP condition and master need not to wait for the completion of stop condition
	  //Note : generating STOP, automatically clears the BTF.
    	 I2C_GenerateStopCondition(pI2CHandle->pI2CBaseAddr);
  }
  static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2C_BaseAddr){

	  pI2C_BaseAddr->CR1 |= (1<<I2C_CR1_START);

  }

  static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2C_BaseAddr, uint8_t SlaveAddr){
	  SlaveAddr = SlaveAddr<<1;
	  SlaveAddr&= ~(1);// SlaveAddr + r/w bit
	  pI2C_BaseAddr->DR = SlaveAddr;
  }
  static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle)
  {    volatile uint32_t dummyRead;
	  if(pI2CHandle->pI2CBaseAddr->SR2 & (1<<I2C_SR2_MSL)){
	       //device in master mode
		  if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){
			  if(pI2CHandle->RxSize == 1){
				  //first disable the ack
				  I2C_ManageAcking(pI2CHandle->pI2CBaseAddr,DISABLE);
				  //clear the Addr flag
				  dummyRead = pI2CHandle->pI2CBaseAddr->SR1;
				  dummyRead = pI2CHandle->pI2CBaseAddr->SR2;
				  (void)dummyRead;
			  }
		  }else{
			  //clear the ADDR flag
			  dummyRead = pI2CHandle->pI2CBaseAddr->SR1;
			  dummyRead = pI2CHandle->pI2CBaseAddr->SR2;
			  (void)dummyRead;
		  }
      }else{
    	  //device is slave mode
		  //clear the ADDR flag
		  dummyRead = pI2CHandle->pI2CBaseAddr->SR1;
		  dummyRead = pI2CHandle->pI2CBaseAddr->SR2;
		  (void)dummyRead;
      }

  }

  void I2C_GenerateStopCondition(I2C_RegDef_t *pI2C_BaseAddr){

	  pI2C_BaseAddr->CR1 |= (1<<I2C_CR1_STOP);
  }

  uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2C_BaseAddr,uint32_t FlagName){
	  if (pI2C_BaseAddr->SR1 & FlagName){

		  return FLAG_SET;
	  }
	  return FLAG_RESET;
  }
  void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr, uint8_t Sr)
  {

  	//1. Generate the START condition
	  I2C_GenerateStartCondition(pI2CHandle->pI2CBaseAddr);
	  //2. confirm that start generation is completed by checking the SB flag in the SR1
  	//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	  while(!I2C_GetFlagStatus(pI2CHandle->pI2CBaseAddr, I2C_FLAG_SB));
  	//3. Send the address of the slave with r/nw bit set to R(1) (total 8 bits )
	  I2C_ExecuteAddressPhase_Read(pI2CHandle->pI2CBaseAddr,SlaveAddr);
  	//4. wait until address phase is completed by checking the ADDR flag in teh SR1
	  while(!I2C_GetFlagStatus(pI2CHandle->pI2CBaseAddr, I2C_FLAG_ADDR));

  	//procedure to read only 1 byte from slave
  	if(Len == 1)
  	{
  		//Disable Acking
  		pI2CHandle->pI2CBaseAddr->CR1 &= ~(1<<I2C_CR1_ACK);
  		//clear the ADDR flag
  	  I2C_ClearADDRFlag(pI2CHandle);
  		//wait until  RXNE becomes 1
	  I2C_GetFlagStatus(pI2CHandle->pI2CBaseAddr,I2C_FLAG_RXNE);
	  //generate STOP condition
	  	  I2C_GenerateStopCondition(pI2CHandle->pI2CBaseAddr);
  		//read data in to buffer
	  *pRxBuffer = pI2CHandle->pI2CBaseAddr->DR;
  	}


      //procedure to read data from slave when Len > 1
  	if(Len > 1)
  	{
  		//clear the ADDR flag
    	  I2C_ClearADDRFlag(pI2CHandle);
  		//read the data until Len becomes zero
  		for ( uint32_t i = Len ; i > 0 ; i--)
  		{
  			//wait until RXNE becomes 1
  			while(!I2C_GetFlagStatus(pI2CHandle->pI2CBaseAddr, I2C_FLAG_RXNE));
  			if(i == 2) //if last 2 bytes are remaining
  			{
  				//Disable Acking
  		  		pI2CHandle->pI2CBaseAddr->CR1 &= ~(1<<I2C_CR1_ACK);
  				//generate STOP condition
  			  I2C_GenerateStopCondition(pI2CHandle->pI2CBaseAddr);
  			}

  			//read the data from data register in to buffer
  		  *pRxBuffer = pI2CHandle->pI2CBaseAddr->DR;
  			//increment the buffer address
  		pRxBuffer++;
  		}

  	}

  	//re-enable ACKing

  	if(pI2CHandle->I2CConfig.I2C_ACKControl == I2C_ACK_ENABLE)
  	{
		pI2CHandle->pI2CBaseAddr->CR1 |= (1<<I2C_CR1_ACK);

  	}

  }

  static void I2C_ExecuteAddressPhase_Read(I2C_RegDef_t *pI2C_BaseAddr, uint8_t SlaveAddr){
	  SlaveAddr = SlaveAddr<<1;
	  SlaveAddr|= 1;// SlaveAddr + r/w bit
	  pI2C_BaseAddr->DR = SlaveAddr;
  }
  static void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
  {
  	if(EnorDi == I2C_ACK_ENABLE)
  	{
  		//enable the ack
  		pI2Cx->CR1 |= ( 1 << I2C_CR1_ACK);
  	}else
  	{
  		//disable the ack
  		pI2Cx->CR1 &= ~( 1 << I2C_CR1_ACK);
  	}
  }
//Interrupt handling
  uint8_t  I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxBuffer, uint32_t Len,uint8_t SlaveAddr,uint8_t Sr)
  {

  	uint8_t busystate = pI2CHandle->TxRxState;

  	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
  	{
  		pI2CHandle->pTXBuffer = pTxBuffer;
  		pI2CHandle->TXLen = Len;
  		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
  		pI2CHandle->DevAddr = SlaveAddr;
  		pI2CHandle->Sr = Sr;

  		//Implement code to Generate START Condition
  		 I2C_GenerateStartCondition(pI2CHandle->pI2CBaseAddr);

  		//Implement the code to enable ITBUFEN Control Bit
  		pI2CHandle->pI2CBaseAddr->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

  		//Implement the code to enable ITEVTEN Control Bit
  		pI2CHandle->pI2CBaseAddr->CR2 |= ( 1 << I2C_CR2_ITEVTEN );

  		//Implement the code to enable ITERREN Control Bit
  		pI2CHandle->pI2CBaseAddr->CR2 |= ( 1 << I2C_CR2_ITERREN );


  	}

  	return busystate;

  }
  uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
  {

  	uint8_t busystate = pI2CHandle->TxRxState;

  	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
  	{
  		pI2CHandle->pRXBuffer = pRxBuffer;
  		pI2CHandle->RXLen = Len;
  		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
  		pI2CHandle->RxSize = Len; //Rxsize is used in the ISR code to manage the data reception
  		pI2CHandle->DevAddr = SlaveAddr;
  		pI2CHandle->Sr = Sr;

  		//Implement code to Generate START Condition
 		I2C_GenerateStartCondition(pI2CHandle->pI2CBaseAddr);

  		//Implement the code to enable ITBUFEN Control Bit
   		pI2CHandle->pI2CBaseAddr->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

  		//Implement the code to enable ITEVFEN Control Bit
  		pI2CHandle->pI2CBaseAddr->CR2 |= ( 1 << I2C_CR2_ITEVTEN );

  		//Implement the code to enable ITERREN Control Bit
  		pI2CHandle->pI2CBaseAddr->CR2 |= ( 1 << I2C_CR2_ITERREN );

  	}

  	return busystate;
  }
 //IRQ Configuration and ISR handling

  void I2C_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi){
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
  void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
	  uint8_t iprx = IRQNumber/4;
	 	 uint8_t iprx_section = IRQNumber%4;
	 	 uint8_t shift_amount = (8*iprx_section) + (8-NO_PR_BITS_IMPLEMENTED);
	 	 *(NVIC_PR_BASE_ADDR + (iprx)) |= (IRQPriority << shift_amount);
  }

//helper function for ISR handling
  void I2C_Close_ReceiveData(I2C_Handle_t *pI2CHandle)
  {
  	//Implement the code to disable ITBUFEN Control Bit
  	pI2CHandle->pI2CBaseAddr->CR2 &= ~( 1 << I2C_CR2_ITBUFEN);

  	//Implement the code to disable ITEVFEN Control Bit
  	pI2CHandle->pI2CBaseAddr->CR2 &= ~( 1 << I2C_CR2_ITEVTEN);

  	pI2CHandle->TxRxState = I2C_Ready;
  	pI2CHandle->pRXBuffer = NULL;
  	pI2CHandle->RXLen = 0;
  	pI2CHandle->RxSize = 0;

  	if(pI2CHandle->I2CConfig.I2C_ACKControl == I2C_ACK_ENABLE)
  	{
  		I2C_ManageAcking(pI2CHandle->pI2CBaseAddr,ENABLE);
  	}

  }

  void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
  {
  	//Implement the code to disable ITBUFEN Control Bit
  	pI2CHandle->pI2CBaseAddr->CR2 &= ~( 1 << I2C_CR2_ITBUFEN);

  	//Implement the code to disable ITEVFEN Control Bit
  	pI2CHandle->pI2CBaseAddr->CR2 &= ~( 1 << I2C_CR2_ITEVTEN);


  	pI2CHandle->TxRxState = I2C_Ready;
  	pI2CHandle->pTXBuffer = NULL;
  	pI2CHandle->TXLen = 0;
  }
  static void I2C_MasterHandle_RXNEInterrupt(I2C_Handle_t *pI2CHandle){
	  //We have to do data reception
	  if(pI2CHandle->RxSize ==1){
		  *pI2CHandle->pRXBuffer = pI2CHandle->pI2CBaseAddr->DR;
		  pI2CHandle->RXLen--;
	  }

	  if(pI2CHandle->RxSize > 1){

		  if(pI2CHandle->RXLen ==2){
			  //clear the ack bit
			  I2C_ManageAcking(pI2CHandle->pI2CBaseAddr,DISABLE);
		  }

		  *pI2CHandle->pRXBuffer = pI2CHandle->pI2CBaseAddr->DR;
		   pI2CHandle->pRXBuffer++;
		   pI2CHandle->RXLen--;
	  }

	  if(pI2CHandle->RXLen == 0){

		  //close the I2C data reception and notify the application
		  //1. generate stop condition
		  if(pI2CHandle->Sr == I2C_DISABLE_SR){

			  I2C_GenerateStopCondition(pI2CHandle->pI2CBaseAddr);
		  }

		  //2. close the I2C RX
		  I2C_Close_ReceiveData(pI2CHandle);

		  //notify the application
		  I2C_ApplicationEvent_Callback(pI2CHandle,I2C_EV_RX_CMPLT);

	  }
  }

 static void I2C_MasterHandle_TXEInterrupt(I2C_Handle_t *pI2CHandle){

	  if(pI2CHandle->TXLen > 0){
		  //1. Load data in the DR
		  pI2CHandle->pI2CBaseAddr->DR = *(pI2CHandle->pTXBuffer);
		  //2. Decrement the TXLen
		  pI2CHandle->TXLen--;
		  //3. 	Increment the buffer address
		  pI2CHandle->pTXBuffer++;
	  }
  }

//slave apis
 void I2C_SlaveSendData(I2C_RegDef_t *pI2C_BaseAddr, uint8_t data){
	 pI2C_BaseAddr->DR = data;
 }
 uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C_BaseAddr){
	return  (uint8_t)pI2C_BaseAddr->DR;
 }


//ISR HANDLING

  void I2C_EV_Handler(I2C_Handle_t *pI2CHandle){
	  uint32_t temp1,temp2,temp3;

	  temp1 = pI2CHandle->pI2CBaseAddr->CR2 &(1<<I2C_CR2_ITEVTEN);
	  temp2 = pI2CHandle->pI2CBaseAddr->CR2 &(1<<I2C_CR2_ITBUFEN);

	  //Handle for the interrupt generated by the SB event
	  //Note : SB Flag is only applicable in master mode
	  temp3 = pI2CHandle->pI2CBaseAddr->SR1 & (1<<I2C_SR1_SB);

	  if(temp1 && temp3){

		  //Interrupt is generated because of SB EVENT
		  //this block will not be executed in slave mode because for slave SB is zero
		  //In this block lets execute the address phase
		  if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX){
			  I2C_ExecuteAddressPhase(pI2CHandle->pI2CBaseAddr, pI2CHandle->DevAddr);
		  }else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){
			  I2C_ExecuteAddressPhase_Read(pI2CHandle->pI2CBaseAddr, pI2CHandle->DevAddr);
		  }
	  }

	  //Handle the interrupt generated by ADDR event
	  //Note : When master mode : address is sent
	  //       When Slave mode  : address matched with own address

	  temp3 = pI2CHandle->pI2CBaseAddr->SR1 & (1<<I2C_SR1_ADDR);
	  if(temp1 && temp3){

	  		  // ADDR FLAG is SET
    	  I2C_ClearADDRFlag(pI2CHandle);

	  }

	  //Handle the interrupt generated by BTF(Byte Transfer Finished) Event
	  temp3 = pI2CHandle->pI2CBaseAddr->SR1 & (1<<I2C_SR1_BTF);
	  if(temp1 && temp3){

	  	  		  // BTF FLAG is SET
		  if(pI2CHandle->TXLen == 0){
			  if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX){
				  if(pI2CHandle->pI2CBaseAddr->SR1 & (1<<I2C_SR1_TxE)){
					  //Generate stop condition
					  if(pI2CHandle->Sr == I2C_DISABLE_SR){
						  I2C_GenerateStopCondition(pI2CHandle->pI2CBaseAddr);
					  }
					  //reset all the member elements of the handle structure
					  I2C_CloseSendData(pI2CHandle);
					  //notify the application about transmission complete
					  I2C_ApplicationEvent_Callback(pI2CHandle,I2C_EV_TX_CMPLT);

				  }
			 		  }else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){
			 			  ;
			 		  }
		  }

	  	  	  }
	  //Handle the interrupt generated by STOF event
	  //Note: Stop detection flag is applicable only in slave mode. for master this flag will never be set
	  //Th ebelow code block will not be executed by the master since STOPF will not set in master mode.
	  temp3 = pI2CHandle->pI2CBaseAddr->SR1 & (1<<I2C_SR1_STOPF);
	  if(temp1 && temp3){

		  // STOPF FLAG is SET
		  // Clear the STOPF (Read the SR1 nad write to CR1)
		  pI2CHandle->pI2CBaseAddr->CR1 |= 0x0000;
		  //notify the application that stop is detected
		  I2C_ApplicationEvent_Callback(pI2CHandle,I2C_EV_STOP);

	  }

	  //Handle for interrupt generated by TXE event
	  temp3 = pI2CHandle->pI2CBaseAddr->SR1 & (1<<I2C_SR1_TxE);
	  if(temp1 && temp2 && temp3){
		  //Check for device mode
		  if(pI2CHandle->pI2CBaseAddr->SR2 & (1<<I2C_SR2_MSL)){
		  // TXE FLAG is SET
		  //Lets do the data transmission
		  if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX){
			  I2C_MasterHandle_TXEInterrupt(pI2CHandle);
		  }
	    }
	  }
	  //Handle for interrupt generated by RXNE flag

	  temp3 = pI2CHandle->pI2CBaseAddr->SR1 & (1<<I2C_SR1_RxNE);
	  if(temp1 && temp2 && temp3){
	  // RXNE FLAG is SET
		 //check for device mode master or slave
		  if(pI2CHandle->pI2CBaseAddr->SR2 & (1<<I2C_SR2_MSL)){
			  if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX) {

                I2C_MasterHandle_RXNEInterrupt(pI2CHandle);
			   }
		 }
	  }
  }
  void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle)
  {

  	uint32_t temp1,temp2;

      //Know the status of  ITERREN control bit in the CR2
  	temp2 = (pI2CHandle->pI2CBaseAddr->CR2) & ( 1 << I2C_CR2_ITERREN);


  /***********************Check for Bus error************************************/
  	temp1 = (pI2CHandle->pI2CBaseAddr->SR1) & ( 1<< I2C_SR1_BERR);
  	if(temp1  && temp2 )
  	{
  		//This is Bus error

  		//Implement the code to clear the buss error flag
  		pI2CHandle->pI2CBaseAddr->SR1 &= ~( 1 << I2C_SR1_BERR);

  		//Implement the code to notify the application about the error
  		I2C_ApplicationEvent_Callback(pI2CHandle,I2C_ERROR_BERR);
  	}

  /***********************Check for arbitration lost error************************************/
  	temp1 = (pI2CHandle->pI2CBaseAddr->SR1) & ( 1 << I2C_SR1_ARLO );
  	if(temp1  && temp2)
  	{
  		//This is arbitration lost error

  		//Implement the code to clear the arbitration lost error flag
  		pI2CHandle->pI2CBaseAddr->SR1 &= ~( 1 << I2C_SR1_ARLO);

  		//Implement the code to notify the application about the error
  		I2C_ApplicationEvent_Callback(pI2CHandle,I2C_ERROR_ARLO);
  	}

  /***********************Check for ACK failure  error************************************/

  	temp1 = (pI2CHandle->pI2CBaseAddr->SR1) & ( 1 << I2C_SR1_AF);
  	if(temp1  && temp2)
  	{
  		//This is ACK failure error

  	    //Implement the code to clear the ACK failure error flag
  		pI2CHandle->pI2CBaseAddr->SR1 &= ~( 1 << I2C_SR1_AF);

  		//Implement the code to notify the application about the error
  		I2C_ApplicationEvent_Callback(pI2CHandle,I2C_ERROR_AF);

  	}

  /***********************Check for Overrun/underrun error************************************/
  	temp1 = (pI2CHandle->pI2CBaseAddr->SR1) & ( 1 << I2C_SR1_OVR);
  	if(temp1  && temp2)
  	{
  		//This is Overrun/underrun

  	    //Implement the code to clear the Overrun/underrun error flag
  		pI2CHandle->pI2CBaseAddr->SR1 &= ~( 1 << I2C_SR1_OVR);

  		//Implement the code to notify the application about the error
  		I2C_ApplicationEvent_Callback(pI2CHandle,I2C_ERROR_OVR);

  	}

  /***********************Check for Time out error************************************/
  	temp1 = (pI2CHandle->pI2CBaseAddr->SR1) & ( 1 << I2C_SR1_TIMEOUT);
  	if(temp1  && temp2)
  	{
  		//This is Time out error

  	    //Implement the code to clear the Time out error flag
  		pI2CHandle->pI2CBaseAddr->SR1 &= ~( 1 << I2C_SR1_TIMEOUT);

  		//Implement the code to notify the application about the error
  		I2C_ApplicationEvent_Callback(pI2CHandle,I2C_ERROR_TIMEOUT);

  	}

  }

//other peripheral control APIs
  void I2C_PeripheralControl(I2C_RegDef_t *pI2C_BaseAddr, uint8_t EnOrDi)
  {
  	if(EnOrDi == ENABLE)
  	{
  		pI2C_BaseAddr->CR1 |= (1 << I2C_CR1_PE);
  		//pI2cBaseAddress->CR1 |= I2C_CR1_PE_Bit_Mask;
  	}else
  	{
  		pI2C_BaseAddr->CR1 &= ~(1 << 0);
  	}

  }


