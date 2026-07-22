/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: Jun 21, 2026
 *      Author: sneha
 */

#include <stm32f407xx_gpio_driver.h>
//Peripheral clock setup

void GPIO_PeriClockControl(GPIO_Regdef_t *pGPIOBaseAddr, uint8_t EnorDi)
{
   if (EnorDi == ENABLE){
	   if (pGPIOBaseAddr == GPIOA){
		   GPIOA_PCLK_EN();
	   }else if (pGPIOBaseAddr == GPIOB){
		   GPIOB_PCLK_EN();
	   }else if (pGPIOBaseAddr == GPIOC){
		   GPIOC_PCLK_EN();
	   }else if (pGPIOBaseAddr == GPIOD){
		   GPIOD_PCLK_EN();
	   }else if (pGPIOBaseAddr == GPIOE){
		   GPIOE_PCLK_EN();
	   }else if (pGPIOBaseAddr == GPIOF){
		   GPIOF_PCLK_EN();
	   }else if (pGPIOBaseAddr == GPIOG){
		   GPIOG_PCLK_EN();
	   }else if (pGPIOBaseAddr == GPIOH){
		   GPIOH_PCLK_EN();
	   }else if (pGPIOBaseAddr == GPIOI){
		   GPIOI_PCLK_EN();
	   }
   }else {
	   if (pGPIOBaseAddr == GPIOA){
	   		   GPIOA_PCLK_DI();
	   	   }else if (pGPIOBaseAddr == GPIOB){
	   		   GPIOB_PCLK_DI();
	   	   }else if (pGPIOBaseAddr == GPIOC){
	   		   GPIOC_PCLK_DI();
	   	   }else if (pGPIOBaseAddr == GPIOD){
	   		   GPIOD_PCLK_DI();
	   	   }else if (pGPIOBaseAddr == GPIOE){
	   		   GPIOE_PCLK_DI();
	   	   }else if (pGPIOBaseAddr == GPIOF){
	   		   GPIOF_PCLK_DI();
	   	   }else if (pGPIOBaseAddr == GPIOG){
	   		   GPIOG_PCLK_DI();
	   	   }else if (pGPIOBaseAddr == GPIOH){
	   		   GPIOH_PCLK_DI();
	   	   }else if (pGPIOBaseAddr == GPIOI){
	   		   GPIOI_PCLK_DI();
	   	   }
   }
}

//INIT AND DEINIT
 void GPIO_Init(GPIO_Handle_t *pGPIOHandle)//initialize the given registers
 {
	 GPIO_PeriClockControl(pGPIOHandle->pGPIOBaseAddr,ENABLE);


	 uint32_t temp = 0;
 //1. Configure the mode of GPIO Pin

	 if (pGPIOHandle->GPIOPinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG ){
	 temp = (pGPIOHandle->GPIOPinConfig.GPIO_PinMode << (2*pGPIOHandle->GPIOPinConfig.GPIO_PinNumber));
	 pGPIOHandle->pGPIOBaseAddr->MODER &= ~(0x3 << (2*pGPIOHandle->GPIOPinConfig.GPIO_PinNumber));
	 pGPIOHandle->pGPIOBaseAddr->MODER |= temp;

	 }else {
			 if (pGPIOHandle->GPIOPinConfig.GPIO_PinMode == GPIO_MODE_IT_FT ){
				 //configure FTSR
				 EXTI->FTSR |= (1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);
				 //clear the corresponding RTSR bit
				 EXTI->RTSR &= ~(1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);

			 }else if (pGPIOHandle->GPIOPinConfig.GPIO_PinMode == GPIO_MODE_IT_RT ){
				 //Configure RTSR
				 EXTI->RTSR |= (1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);
				 EXTI->FTSR &= ~(1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);

			 }else if (pGPIOHandle->GPIOPinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT ){
				 // Configure both RTSR and FTSR
				 EXTI->RTSR |= (1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);
				 EXTI->FTSR |= (1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);

			 }

			 //Configure the GPIO port selection in SYSCFG_EXTI
			  uint8_t temp1 = (pGPIOHandle->GPIOPinConfig.GPIO_PinNumber/4);
			  uint8_t temp2 = (pGPIOHandle->GPIOPinConfig.GPIO_PinNumber%4);
			  uint8_t portcode = GPIO_BASE_ADDR_TO_CODE(pGPIOHandle->pGPIOBaseAddr);
			  SYSCFG_PCLK_EN();
			  SYSCFG->EXTICR[temp1] = portcode<<(temp2*4);
			 //Enable the EXTI interrupt delivery using IMR
			 EXTI->IMR |= ( 1<<pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);
	 }
	    temp =0;

	 // 2. configure the speed
		temp = (pGPIOHandle->GPIOPinConfig.GPIO_PinSpeed <<  (2*pGPIOHandle->GPIOPinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOBaseAddr->OSPEEDR &= ~(0x3 << (2*pGPIOHandle->GPIOPinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOBaseAddr->OSPEEDR |= temp;

		temp = 0;
	 //3. configure the pupd setttings
		temp = (pGPIOHandle->GPIOPinConfig.GPIO_PinPuPdControl << (2*pGPIOHandle->GPIOPinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOBaseAddr->PUPDR &= ~(0x3 << (2*pGPIOHandle->GPIOPinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOBaseAddr->PUPDR |= temp;
		temp =0;
	 //4. configure the output type
		temp = (pGPIOHandle->GPIOPinConfig.GPIO_PinOPType << (pGPIOHandle->GPIOPinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOBaseAddr->OTYPER &= ~(0x1 << (2*pGPIOHandle->GPIOPinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOBaseAddr->OTYPER |= temp;
	 //5 configure the alternate functionality mode
		 if (pGPIOHandle->GPIOPinConfig.GPIO_PinMode <= GPIO_MODE_ALTFN ){
			uint8_t  temp1 = pGPIOHandle->GPIOPinConfig.GPIO_PinNumber/8;
			uint8_t  temp2 = pGPIOHandle->GPIOPinConfig.GPIO_PinNumber%8;
			pGPIOHandle->pGPIOBaseAddr->AFR[temp1] &= ~(0xF << (4*temp2));
			pGPIOHandle->pGPIOBaseAddr->AFR[temp1] |= (pGPIOHandle->GPIOPinConfig.GPIO_PinAltFunMode << (4*temp2));
		 }
 }

 void GPIO_DeInit(GPIO_Regdef_t *pGPIOBaseAddr)
 {

		   if (pGPIOBaseAddr == GPIOA){
			   GPIOA_REG_RESET();
		   }else if (pGPIOBaseAddr == GPIOB){
			   GPIOB_REG_RESET();
		   }else if (pGPIOBaseAddr == GPIOC){
			   GPIOC_REG_RESET();
		   }else if (pGPIOBaseAddr == GPIOD){
			   GPIOD_REG_RESET();
		   }else if (pGPIOBaseAddr == GPIOE){
			   GPIOE_REG_RESET();
		   }else if (pGPIOBaseAddr == GPIOF){
			   GPIOF_REG_RESET();
		   }else if (pGPIOBaseAddr == GPIOG){
			   GPIOG_REG_RESET();
		   }else if (pGPIOBaseAddr == GPIOH){
			   GPIOH_REG_RESET();
		   }else if (pGPIOBaseAddr == GPIOI){
			   GPIOI_REG_RESET();
		   }

 }

 //Data Read and Write
uint8_t GPIO_ReadFromInputPin(GPIO_Regdef_t *pGPIOBaseAddr,uint8_t PinNumber)
{
  uint8_t value;
  value =(uint8_t)((pGPIOBaseAddr->IDR >> PinNumber) & 0x00000001);
  return value;
}
uint16_t GPIO_ReadFromInputPort(GPIO_Regdef_t *pGPIOBaseAddr)
{
	 uint16_t value;
	 value = (uint16_t)(pGPIOBaseAddr->IDR);
	 return value;
}
 void GPIO_WriteToOutputPin(GPIO_Regdef_t *pGPIOBaseAddr,uint8_t PinNumber,uint8_t Value)
 {   if (Value == GPIO_PIN_SET) {
	 //Write 1 to the output data register at the bit field corresponding to pin number
	 pGPIOBaseAddr->ODR |= (1<< PinNumber);
 }else{
	 //Write 0
	 pGPIOBaseAddr->ODR &= ~(1<< PinNumber);
 }
 }
 void GPIO_WriteToOutputPort(GPIO_Regdef_t *pGPIOBaseAddr,uint8_t Value)
 {


		 pGPIOBaseAddr->ODR = Value;

 }
 void GPIO_ToggleOutputPin(GPIO_Regdef_t *pGPIOBaseAddr,uint8_t PinNumber)
 {
	 pGPIOBaseAddr->ODR ^= (1 << PinNumber);
 }
// TOGGLE PIN 12 BY AUTOMIC TOGGLE

 void GPIO_ToggleOutputPin_12(GPIO_Regdef_t*pGPIOBaseAddr, uint8_t PinNumber)
 {
	 // static variable retains its value between function calls
	     static uint8_t pin_state[16] = {0};

	     if (pin_state[PinNumber] == 0)
	     {
	         // Atomically Set Pin 12 HIGH
	         pGPIOBaseAddr->BSRR = (1 << PinNumber);
	         pin_state[PinNumber] = 1;
	     }
	     else
	     {
	         // Atomically Reset Pin 12 LOW (Upper 16 bits, so 12 + 16 = 28)
	         pGPIOBaseAddr->BSRR = (1 << (PinNumber+16));
	         pin_state[PinNumber] = 0;
	     }
 }
 //IRQ handling
 void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi)
 {
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

 void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
	 uint8_t iprx = IRQNumber/4;
	 uint8_t iprx_section = IRQNumber%4;
	 uint8_t shift_amount = (8*iprx_section) + (8-NO_PR_BITS_IMPLEMENTED);
	 *(NVIC_PR_BASE_ADDR + (iprx)) |= (IRQPriority << shift_amount);
 }
 void GPIO_IRQHandling(uint8_t PinNumber)
 {
//clear the exti pr register corresponding to the pin number
	 if(EXTI->PR &( 1 << PinNumber)){
		 EXTI->PR |= ( 1 << PinNumber);
	 }
 }
/*
 pGPIOHandle->pGPIOBaseAddr->MODER &= ~((0x3 << (2 * 8)) | (0x3 << (2 * 9)));
// Set mode for Pin 8 and 9
pGPIOHandle->pGPIOBaseAddr->MODER |= ((pGPIOHandle->GPIOPinConfig.GPIO_PinMode << (2 * 8)) |
                                       (pGPIOHandle->GPIOPinConfig.GPIO_PinMode << (2 * 9)));
 */
