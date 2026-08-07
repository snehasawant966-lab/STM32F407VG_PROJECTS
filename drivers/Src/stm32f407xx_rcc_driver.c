/*
 * stm32f407xx_rcc_driver.c
 *
 *  Created on: Jul 26, 2026
 *      Author: sneha
 */

#include<stm32f407xx_rcc_driver.h>
#include<stm32f407xx.h>


 uint16_t AHB_PreScalar[8] = {2,4,8,16,64,128,256,512};
 uint8_t APB1_PreScalar[8] = {2,4,8,16};
 uint8_t APB2_PreScalar[8] = {2,4,8,16};


 /*
  * system clock is HSI, System clock is modified using AHB1 PRESC
  * After then AHB1 Clock is given to APB1 prescalar and then will get the APB1 peri clock where I2C is connected.
  *
  * in RCC prescalars are configured.
  */

 uint32_t RCCGetPPllClockOutput(){
	 return 0;
 }


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


 uint32_t RCC_GetPCLK2Value(){

	 uint32_t pclk2,SystemClk ;
	 uint8_t clksrc, temp, ahbp, apbp2;
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

			 apbp2 = 1;

		 }else{

			 apbp2 = APB2_PreScalar[temp-4];

		 }

		 pclk2 = (SystemClk/ahbp)/apbp2;

	 return pclk2;

 }
