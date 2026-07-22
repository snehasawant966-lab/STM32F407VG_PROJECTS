/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: Jun 21, 2026
 *      Author: sneha
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include <stm32f407xx.h>



typedef struct {
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_Pin_Config_t;

// Handle structure for GPIO pin

typedef struct{
	// pointer to hold the base address of the GPIO peripheral
	GPIO_Regdef_t *pGPIOBaseAddr; //this holds the base address of the GPIO port to which the pin belongs
	// for pin configuration
	GPIO_Pin_Config_t GPIOPinConfig; // this holds GPIO pin configuration settings
}GPIO_Handle_t;

//definition of macros
//GPIO PIN POSSIBLE MODES
#define GPIO_MODE_IN     0
#define GPIO_MODE_OUT    1
#define GPIO_MODE_ALTFN  2
#define GPIO_MODE_ANALOG 3
#define GPIO_MODE_IT_FT  4
#define GPIO_MODE_IT_RT  5
#define GPIO_MODE_IT_RFT 6

//GPIO PIN POSSIBLE OUTPUT TYPES

#define GPIO_OP_TYPE_PP  0
#define GPIO_OP_TYPE_OD  1

//GPIO PIN POSSIBLE OUTPUT SPEEDS

#define GPIO_OP_SPEED_LOW    0
#define GPIO_OP_SPEED_MED    1
#define GPIO_OP_SPEED_HIGH   2
#define GPIO_OP_SPEED_VHIGH  3

//PULL UP PULL DOWN REGISTERS

#define GPIO_NO_PUPD      0
#define GPIO_PIN_PU       1
#define GPIO_PIN_PD       2


//GPIO PIN NUMBERS

#define GPIO_PIN_NO_0          0
#define GPIO_PIN_NO_1          1
#define GPIO_PIN_NO_2          2
#define GPIO_PIN_NO_3          3
#define GPIO_PIN_NO_4          4
#define GPIO_PIN_NO_5          5
#define GPIO_PIN_NO_6          6
#define GPIO_PIN_NO_7          7
#define GPIO_PIN_NO_8          8
#define GPIO_PIN_NO_9          9
#define GPIO_PIN_NO_10         10
#define GPIO_PIN_NO_11         11
#define GPIO_PIN_NO_12         12
#define GPIO_PIN_NO_13         13
#define GPIO_PIN_NO_14         14
#define GPIO_PIN_NO_15         15

//alternate function register modes


#define BTN_PRESSED 0



//API Definitions

//Peripheral clock setup

void GPIO_PeriClockControl(GPIO_Regdef_t *pGPIOBaseAddr, uint8_t EnorDi);

//INIT AND DEINIT
 void GPIO_Init(GPIO_Handle_t *pGPIOHandle);//initialize the given registers
 void GPIO_DeInit(GPIO_Regdef_t *pGPIOBaseAddr);

 //Data Read and Write
uint8_t GPIO_ReadFromInputPin(GPIO_Regdef_t *pGPIOBaseAddr,uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_Regdef_t *pGPIOBaseAddr);
 void GPIO_WriteToOutputPin(GPIO_Regdef_t*pGPIOBaseAddr,uint8_t PinNumber,uint8_t Value);
 void GPIO_WriteToOutputPort(GPIO_Regdef_t *pGPIOBaseAddr,uint8_t Value);
 void GPIO_ToggleOutputPin(GPIO_Regdef_t *pGPIOBaseAddr,uint8_t PinNumber);

 //IRQ handling
 void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
 void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
 void GPIO_IRQHandling(uint8_t PinNumber);
 void GPIO_ToggleOutputPin_12(GPIO_Regdef_t*pGPIOBaseAddr, uint8_t PinNumber);

#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */
