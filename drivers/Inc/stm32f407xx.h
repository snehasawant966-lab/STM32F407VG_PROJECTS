/*
 * stm32f407xx.h
 *
 *  Created on: Jun 20, 2026
 *      Author: sneha
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_
#include <stdint.h>
#include<stddef.h>

#define __vo volatile
#define  __weak __attribute__((weak))
// NVIC ISERx REGISTER ADDRESSES

#define NVIC_ISER0   ((__vo uint32_t*)0xE000E100 )
#define NVIC_ISER1   ((__vo uint32_t*)0xE000E104 )
#define NVIC_ISER2   ((__vo uint32_t*)0xE000E108 )
#define NVIC_ISER3   ((__vo uint32_t*)0xE000E10C )
#define NVIC_ISER4   ((__vo uint32_t*)0xE000E110 )
#define NVIC_ISER5   ((__vo uint32_t*)0xE000E114 )
#define NVIC_ISER6   ((__vo uint32_t*)0xE000E118 )
#define NVIC_ISER7   ((__vo uint32_t*)0xE000E11C )

// NVIC ICERx REGISTER ADDRESSES
#define NVIC_ICER0   ((__vo uint32_t*)0xE000E180)
#define NVIC_ICER1   ((__vo uint32_t*)0xE000E184 )
#define NVIC_ICER2   ((__vo uint32_t*)0xE000E188 )
#define NVIC_ICER3   ((__vo uint32_t*)0xE000E18C )
#define NVIC_ICER4   ((__vo uint32_t*)0xE000E190 )
#define NVIC_ICER5   ((__vo uint32_t*)0xE000E194 )
#define NVIC_ICER6   ((__vo uint32_t*)0xE000E198 )
#define NVIC_ICER7   ((__vo uint32_t*)0xE000E19C )

// NVIC IPRx REGISTER ADDRESSES

#define NVIC_PR_BASE_ADDR   ((__vo uint32_t*)0xE000E400)

# define NO_PR_BITS_IMPLEMENTED  4


// Base addresses of various memories

#define FLASH_BASE_ADDR    0x08000000U  // Main memory
#define SRAM1_BASE_ADDR    0x20000000U
#define SRAM2_BASE_ADDR    0x2001C000U
#define SRAM3_BASE_ADDR    0x10000000U
#define ROM_BASE_ADDR      0x1FFF0000U  //System memory
#define SRAM               SRAM1_BASE_ADDR

//Base addresses of various bus domains
#define PERIPH_BASE      0x40010000U
#define APB1_PERIPH_BASE PERIPH_BASE
#define APB2_PERIPH_BASE 0x40010000U
#define AHB1_PERIPH_BASE 0x40020000U
#define AHB2_PERIPH_BASE 0x50000000U
#define AHB3_PERIPH_BASE 0xA0000000U

//Base addresses of AHB1 bus peripherals

#define GPIOA_BASE_ADDR  (AHB1_PERIPH_BASE+0x0000)
#define GPIOB_BASE_ADDR  (AHB1_PERIPH_BASE+0x0400)
#define GPIOC_BASE_ADDR  (AHB1_PERIPH_BASE+0x0800)
#define GPIOD_BASE_ADDR  (AHB1_PERIPH_BASE+0x0C00)
#define GPIOE_BASE_ADDR  (AHB1_PERIPH_BASE+0x1000)
#define GPIOF_BASE_ADDR  (AHB1_PERIPH_BASE+0x1400)
#define GPIOG_BASE_ADDR  (AHB1_PERIPH_BASE+0x1800)
#define GPIOH_BASE_ADDR  (AHB1_PERIPH_BASE+0x1C00)
#define GPIOI_BASE_ADDR  (AHB1_PERIPH_BASE+0x2000)

#define RCC_BASE_ADDR    (AHB1_PERIPH_BASE+0x3800)


//Base addresses of APB1 bus peripherals

#define TIM2_BASE_ADDR    (APB1_PERIPH_BASE + 0x0000)
#define TIM3_BASE_ADDR    ( APB1_PERIPH_BASE + 0x0400)
#define TIM4_BASE_ADDR    ( APB1_PERIPH_BASE + 0x0800)
#define TIM5_BASE_ADDR    ( APB1_PERIPH_BASE + 0x0C00)
#define TIM6_BASE_ADDR    ( APB1_PERIPH_BASE + 0x1000)
#define TIM7_BASE_ADDR    ( APB1_PERIPH_BASE + 0x1400)
#define TIM12_BASE_ADDR   ( APB1_PERIPH_BASE + 0x1800)

#define I2C1_BASE_ADDR    ( APB1_PERIPH_BASE + 0x5400)
#define I2C2_BASE_ADDR    ( APB1_PERIPH_BASE + 0x5800)
#define I2C3_BASE_ADDR    ( APB1_PERIPH_BASE + 0x5C00)

#define SPI2_BASE_ADDR    ( APB1_PERIPH_BASE + 0x3800)
#define SPI3_BASE_ADDR    ( APB1_PERIPH_BASE + 0x3C00)

#define USART2_BASE_ADDR    ( APB1_PERIPH_BASE + 0x4400)
#define USART3_BASE_ADDR    ( APB1_PERIPH_BASE + 0x4800)
#define UART4_BASE_ADDR     ( APB1_PERIPH_BASE + 0x4C00)
#define UART5_BASE_ADDR     ( APB1_PERIPH_BASE + 0x5000)

//Base addresses of APB2 bus peripherals

#define SPI1_BASE_ADDR        (APB2_PERIPH_BASE + 0x3000)
#define USART1_BASE_ADDR      (APB2_PERIPH_BASE + 0x1000)
#define USART6_BASE_ADDR      (APB2_PERIPH_BASE + 0x1400)
#define EXTI_BASE_ADDR        (APB2_PERIPH_BASE + 0x3C00)
#define SYSCFG_BASE_ADDR      (APB2_PERIPH_BASE + 0x3800)

//Structure definition of the GPIO registers
typedef struct {
	__vo uint32_t MODER;
	__vo uint32_t OTYPER;
	__vo uint32_t OSPEEDR;
	__vo uint32_t PUPDR;
	__vo uint32_t IDR;
	__vo uint32_t ODR;
	__vo uint32_t BSRR;
	__vo uint32_t LCKR;
	__vo uint32_t AFR[2];
}GPIO_Regdef_t;

//PERIPHERAL DEFINITIONS
#define GPIOA  ((GPIO_Regdef_t *)GPIOA_BASE_ADDR)
#define GPIOB  ((GPIO_Regdef_t *)GPIOB_BASE_ADDR)
#define GPIOC  ((GPIO_Regdef_t *)GPIOC_BASE_ADDR)
#define GPIOD  ((GPIO_Regdef_t *)GPIOD_BASE_ADDR)
#define GPIOE  ((GPIO_Regdef_t *)GPIOE_BASE_ADDR)
#define GPIOF  ((GPIO_Regdef_t *)GPIOF_BASE_ADDR)
#define GPIOG  ((GPIO_Regdef_t *)GPIOG_BASE_ADDR)
#define GPIOH  ((GPIO_Regdef_t *)GPIOH_BASE_ADDR)
#define GPIOI  ((GPIO_Regdef_t *)GPIOI_BASE_ADDR)

/*
GPIO_Regdef_t *pGPIOA = GPIOA;
GPIO_Regdef_t *pGPIOB = GPIOB;
GPIO_Regdef_t *pGPIOC = GPIOC;
GPIO_Regdef_t *pGPIOD = GPIOD;
GPIO_Regdef_t *pGPIOE = GPIOE;
GPIO_Regdef_t *pGPIOF = GPIOF;
GPIO_Regdef_t *pGPIOG = GPIOG;
GPIO_Regdef_t *pGPIOH = GPIOH;
GPIO_Regdef_t *pGPIOI = GPIOI;
*/

//Structure definition of the RCC registers

typedef struct {
 __vo uint32_t RCC_CR;
 __vo uint32_t PLLCFGR;
 __vo uint32_t CFGR;
 __vo uint32_t CIR;
 __vo uint32_t AHB1RSTR;
 __vo uint32_t AHB2RSTR;
 __vo uint32_t AHB3RSTR;
 uint32_t      Reserved;
 __vo uint32_t APB1RSTR;
 __vo uint32_t APB2RSTR;
 __vo uint32_t      Reserved0;
 __vo uint32_t      Reserved1;
 __vo uint32_t AHB1ENR;
 __vo uint32_t AHB2ENR;
 __vo uint32_t AHB3ENR;
 __vo uint32_t APB1ENR;
 __vo uint32_t APB2ENR;
  uint32_t     Reserved2;
  uint32_t     Reserved3;
 __vo uint32_t AHB1LPENR;
 __vo uint32_t AHB2LPENR;
 __vo uint32_t AHB3LPENR;
  uint32_t     Reserved4;
 __vo uint32_t APB1LPENR;
 __vo uint32_t APB2LPENR;
  uint32_t     Reserved5;
  uint32_t     Reserved6;
 __vo uint32_t BDCR;
 __vo uint32_t CSR;
  uint32_t     Reserved7;
  uint32_t     Reserved8;
 __vo uint32_t SSCGR;
 __vo uint32_t PLLI2SCFGR;
}Rcc_Regdef_t;

#define RCC  ((Rcc_Regdef_t*)RCC_BASE_ADDR)


typedef struct{
	__vo uint32_t IMR;
	__vo uint32_t EMR;
	__vo uint32_t RTSR;
	__vo uint32_t FTSR;
	__vo uint32_t SWIER;
	__vo uint32_t PR;
}EXTI_RegDef_t;

#define EXTI  ((EXTI_RegDef_t*)EXTI_BASE_ADDR)

  typedef struct {
	__vo uint32_t MEMRMP;
	__vo uint32_t PMC;
	__vo uint32_t EXTICR[4];
	__vo uint32_t CMPCR;
	__vo uint32_t RESERVED4[2];
	__vo uint32_t CFGR;

  }SYSCFG_RegDef_t;

#define SYSCFG ((SYSCFG_RegDef_t*)SYSCFG_BASE_ADDR)

//Structure definition of the SPI registers
typedef struct{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t SR;
	__vo uint32_t DR;
	__vo uint32_t CRCPR;
	__vo uint32_t RXCRCR;
	__vo uint32_t TXCRCR;
	__vo uint32_t I2SCFGR;
	__vo uint32_t I2SPR;
}SPI_RegDef_t;

//PERIPHERAL DEFINITIONS
#define SPI1 ((SPI_RegDef_t*)SPI1_BASE_ADDR)
#define SPI2 ((SPI_RegDef_t*)SPI2_BASE_ADDR)
#define SPI3 ((SPI_RegDef_t*)SPI3_BASE_ADDR)

/// structure definition of I2C registers

typedef struct
{
    __vo uint32_t CR1;      // Offset: 0x00
    __vo uint32_t CR2;      // Offset: 0x04
    __vo uint32_t OAR1;     // Offset: 0x08
    __vo uint32_t OAR2;     // Offset: 0x0C
    __vo uint32_t DR;       // Offset: 0x10
    __vo uint32_t SR1;      // Offset: 0x14
    __vo uint32_t SR2;      // Offset: 0x18
    __vo uint32_t CCR;      // Offset: 0x1C
    __vo uint32_t TRISE;    // Offset: 0x20
    __vo uint32_t FLTR;     // Offset: 0x24

} I2C_RegDef_t;

#define I2C1  ((I2C_RegDef_t*)I2C1_BASE_ADDR)
#define I2C2  ((I2C_RegDef_t*)I2C2_BASE_ADDR)
#define I2C3  ((I2C_RegDef_t*)I2C3_BASE_ADDR)

//Clock enable macros for GPIOx peripherals

#define GPIOA_PCLK_EN() (RCC->AHB1ENR |= (1<<0))
#define GPIOB_PCLK_EN() (RCC->AHB1ENR |= (1<<1))
#define GPIOC_PCLK_EN() (RCC->AHB1ENR |= (1<<2))
#define GPIOD_PCLK_EN() (RCC->AHB1ENR |= (1<<3))
#define GPIOE_PCLK_EN() (RCC->AHB1ENR |= (1<<4))
#define GPIOF_PCLK_EN() (RCC->AHB1ENR |= (1<<5))
#define GPIOG_PCLK_EN() (RCC->AHB1ENR |= (1<<6))
#define GPIOH_PCLK_EN() (RCC->AHB1ENR |= (1<<7))
#define GPIOI_PCLK_EN() (RCC->AHB1ENR |= (1<<8))


#define I2C1_PCLK_EN() (RCC->APB1ENR |= (1<<21))
#define I2C2_PCLK_EN() (RCC->APB1ENR |= (1<<22))
#define I2C3_PCLK_EN() (RCC->APB1ENR |= (1<<23))

#define SPI1_PCLK_EN() (RCC->APB2ENR |= (1<<12))
#define SPI2_PCLK_EN() (RCC->APB1ENR |= (1<<14))
#define SPI3_PCLK_EN() (RCC->APB1ENR |= (1<<15))

#define USART1_PCLK_EN() (RCC->APB2ENR |= (1<<4))
#define USART2_PCLK_EN() (RCC->APB1ENR |= (1<<17))
#define USART3_PCLK_EN() (RCC->APB1ENR |= (1<<18))
#define USART4_PCLK_EN() (RCC->APB1ENR |= (1<<19))
#define USART5_PCLK_EN() (RCC->APB1ENR |= (1<<20))
#define USART6_PCLK_EN() (RCC->APB2ENR |= (1<<5))

#define SYSCFG_PCLK_EN() (RCC->APB2ENR |= (1<<14))


//Disable macros

#define GPIOA_PCLK_DI() (RCC->AHB1ENR &= ~(1<<0))
#define GPIOB_PCLK_DI() (RCC->AHB1ENR &= ~(1<<1))
#define GPIOC_PCLK_DI() (RCC->AHB1ENR &= ~(1<<2))
#define GPIOD_PCLK_DI() (RCC->AHB1ENR &= ~(1<<3))
#define GPIOE_PCLK_DI() (RCC->AHB1ENR &= ~(1<<4))
#define GPIOF_PCLK_DI() (RCC->AHB1ENR &= ~(1<<5))
#define GPIOG_PCLK_DI() (RCC->AHB1ENR &= ~(1<<6))
#define GPIOH_PCLK_DI() (RCC->AHB1ENR &= ~(1<<7))
#define GPIOI_PCLK_DI() (RCC->AHB1ENR &= ~(1<<8))


#define I2C1_PCLK_DI() (RCC->APB1ENR &= ~(1<<21))
#define I2C2_PCLK_DI() (RCC->APB1ENR &= ~(1<<22))
#define I2C3_PCLK_DI() (RCC->APB1ENR &= ~(1<<23))

#define SPI1_PCLK_DI() (RCC->APB2ENR &= ~(1<<12))
#define SPI2_PCLK_DI() (RCC->APB1ENR &= ~(1<<14))
#define SPI3_PCLK_DI() (RCC->APB1ENR &= ~(1<<15))

#define USART1_PCLK_DI() (RCC->APB2ENR &= ~(1<<4))
#define USART2_PCLK_DI() (RCC->APB1ENR &= ~(1<<17))
#define USART3_PCLK_DI() (RCC->APB1ENR &= ~(1<<18))
#define USART4_PCLK_DI() (RCC->APB1ENR &= ~(1<<19))
#define USART5_PCLK_DI() (RCC->APB1ENR &= ~(1<<20))
#define USART6_PCLK_DI() (RCC->APB2ENR &= ~(1<<5))

#define SYSCFG_PCLK_DI() (RCC->APB2ENR &= ~(1<<14))

  //REG_RESET

#define GPIOA_REG_RESET() do{(RCC->AHB1RSTR |= (1<<0));(RCC->AHB1RSTR &= ~(1<<0));}while(0)
#define GPIOB_REG_RESET() do{(RCC->AHB1RSTR |= (1<<1));(RCC->AHB1RSTR &= ~(1<<1));}while(0)
#define GPIOC_REG_RESET() do{(RCC->AHB1RSTR |= (1<<2));(RCC->AHB1RSTR &= ~(1<<2));}while(0)
#define GPIOD_REG_RESET() do{(RCC->AHB1RSTR |= (1<<3));(RCC->AHB1RSTR &= ~(1<<3));}while(0)
#define GPIOE_REG_RESET() do{(RCC->AHB1RSTR |= (1<<4));(RCC->AHB1RSTR &= ~(1<<4));}while(0)
#define GPIOF_REG_RESET() do{(RCC->AHB1RSTR |= (1<<5));(RCC->AHB1RSTR &= ~(1<<5));}while(0)
#define GPIOG_REG_RESET() do{(RCC->AHB1RSTR |= (1<<6));(RCC->AHB1RSTR &= ~(1<<6));}while(0)
#define GPIOH_REG_RESET() do{(RCC->AHB1RSTR |= (1<<7));(RCC->AHB1RSTR &= ~(1<<7));}while(0)
#define GPIOI_REG_RESET() do{(RCC->AHB1RSTR |= (1<<8));(RCC->AHB1RSTR &= ~(1<<8));}while(0)

#define SPI1_REG_RESET() do{(RCC->APB2RSTR |= (1<<12));(RCC->APB2RSTR &= ~(1<<12));}while(0)
#define SPI2_REG_RESET() do{(RCC->APB1RSTR |= (1<<14));(RCC->APB1RSTR &= ~(1<<14));}while(0)
#define SPI3_REG_RESET() do{(RCC->APB1RSTR |= (1<<15));(RCC->APB1RSTR &= ~(1<<15));}while(0)

#define I2C1_REG_RESET() do{(RCC->APB1RSTR |= (1<<21));(RCC->APB1RSTR &= ~(1<<21));}while(0)
#define I2C2_REG_RESET() do{(RCC->APB1RSTR |= (1<<22));(RCC->APB1RSTR &= ~(1<<22));}while(0)
#define I2C3_REG_RESET() do{(RCC->APB1RSTR |= (1<<23));(RCC->APB1RSTR &= ~(1<<23));}while(0)
  //some generic macros

#define ENABLE 1
#define DISABLE 0
#define SET  ENABLE
#define RESET DISABLE
#define GPIO_PIN_SET SET
#define GPIO_PIN_RESET RESET

#define FLAG_RESET     RESET
#define FLAG_SET       SET

 //BIT position definitions of the SPI peripherals
/* SPI_CR1 */
#define SPI_CR1_CPHA     0
#define SPI_CR1_CPOL     1
#define SPI_CR1_MSTR     2
#define SPI_CR1_BR       3
#define SPI_CR1_SPE      6
#define SPI_CR1_LSB_FIRST  7
#define SPI_CR1_SSI      8
#define SPI_CR1_SSM      9
#define SPI_CR1_RXONLY   10
#define SPI_CR1_DFF      11
#define SPI_CR1_CRC_NEXT    12
#define SPI_CR1_CRC_EN      13
#define SPI_CR1_BIDIOE      14
#define SPI_CR1_BIDIMODE    15
/* SPI_CR2 */
#define SPI_CR2_RXDMAEN     0
#define SPI_CR2_TXDMAEN     1
#define SPI_CR2_SSOE        2
#define SPI_CR2_FRF         4
#define SPI_CR2_ERRIE       5
#define SPI_CR2_RXNEIE      6
#define SPI_CR2_TXEIE       7

/* SPI_SR */
#define SPI_SR_RXNE         0
#define SPI_SR_TXE          1
#define SPI_SR_CHSIDE       2
#define SPI_SR_UDR          3
#define SPI_SR_CRCERR       4
#define SPI_SR_MODF         5
#define SPI_SR_OVR          6
#define SPI_SR_BSY          7
#define SPI_SR_FRE          8


/************************** I2C_CR1 Bit Position Definitions *********************/
#define I2C_CR1_PE          0
#define I2C_CR1_SMBUS       1
// Bit 2 Reserved
#define I2C_CR1_SMBTYPE     3
#define I2C_CR1_ENARP       4
#define I2C_CR1_ENPEC       5
#define I2C_CR1_ENGC        6
#define I2C_CR1_NOSTRETCH   7
#define I2C_CR1_START       8
#define I2C_CR1_STOP        9
#define I2C_CR1_ACK         10
#define I2C_CR1_POS         11
#define I2C_CR1_PEC         12
#define I2C_CR1_ALERT       13
// Bit 14 Reserved
#define I2C_CR1_SWRST       15

/************************** I2C_CR2 Bit Position Definitions *********************/
#define I2C_CR2_FREQ        0   /* Bits [5:0] */
#define I2C_CR2_ITERREN     8
#define I2C_CR2_ITEVTEN     9
#define I2C_CR2_ITBUFEN     10
#define I2C_CR2_DMAEN       11
#define I2C_CR2_LAST        12

/************************** I2C_SR1 Bit Position Definitions *********************/
#define I2C_SR1_SB          0
#define I2C_SR1_ADDR        1
#define I2C_SR1_BTF         2
#define I2C_SR1_ADD10       3
#define I2C_SR1_STOPF       4
// Bit 5 Reserved
#define I2C_SR1_RxNE        6
#define I2C_SR1_TxE         7
#define I2C_SR1_BERR        8
#define I2C_SR1_ARLO        9
#define I2C_SR1_AF          10
#define I2C_SR1_OVR         11
#define I2C_SR1_PECERR      12
// Bit 13 Reserved
#define I2C_SR1_TIMEOUT     14
#define I2C_SR1_SMBALERT    15

/************************** I2C_SR2 Bit Position Definitions *********************/
#define I2C_SR2_MSL         0
#define I2C_SR2_BUSY        1
#define I2C_SR2_TRA         2
// Bit 3 Reserved
#define I2C_SR2_GENCALL     4
#define I2C_SR2_SMBDEFAULT  5
#define I2C_SR2_SMBHOST     6
#define I2C_SR2_DUALF       7
#define I2C_SR2_PEC         8   /* Bits [15:8] */

/************************** I2C_CCR Bit Position Definitions *********************/
#define I2C_CCR_CCR         0   /* Bits [11:0] */
#define I2C_CCR_DUTY        14
#define I2C_CCR_FS          15

//INTERRUPT REQUEST NUMBERS OF MCU
 #define IRQ_NO_EXTI0       6
 #define IRQ_NO_EXTI1       7
 #define IRQ_NO_EXTI2       8
 #define IRQ_NO_EXTI3       9
 #define IRQ_NO_EXTI4       10
 #define IRQ_NO_EXTI9_5     23
 #define IRQ_NO_EXTI15_10   40

//Interrupt request numbers of SPI
#define IRQ_NO_SPI1        35
#define IRQ_NO_SPI2        36
#define IRQ_NO_SPI3        51

//InterRupt request numbers of I2C
#define IRQ_NO_I2C1_EV      31
#define IRQ_NO_I2C1_ER      32
#define IRQ_NO_I2C2_EV      33
#define IRQ_NO_I2C2_ER      34
#define IRQ_NO_I2C3_EV      72
#define IRQ_NO_I2C3_ER      73

 //IRQ PRIORITY NUMBERS
#define NVIC_IRQ_PRIO0 0
#define NVIC_IRQ_PRIO1 1
#define NVIC_IRQ_PRIO2 2
#define NVIC_IRQ_PRIO3 3
#define NVIC_IRQ_PRIO4 4
#define NVIC_IRQ_PRIO5 5
#define NVIC_IRQ_PRIO6 6
#define NVIC_IRQ_PRIO7 7
#define NVIC_IRQ_PRIO8 8
#define NVIC_IRQ_PRIO9 9
#define NVIC_IRQ_PRIO10 10
#define NVIC_IRQ_PRIO11 11
#define NVIC_IRQ_PRIO12 12
#define NVIC_IRQ_PRIO13 13
#define NVIC_IRQ_PRIO14 14
#define NVIC_IRQ_PRIO15 15


//GPIO MACROS

#define GPIO_BASE_ADDR_TO_CODE(x)( (x==GPIOA) ? 0 :\
		                          (x==GPIOB) ? 1 :\
		                          (x==GPIOC) ? 2 :\
		                          (x==GPIOD) ? 3 :\
		                          (x==GPIOE) ? 4 :\
		                          (x==GPIOF) ? 5 :\
		                          (x==GPIOG) ? 6 :\
		                          (x==GPIOH) ? 7 :0)
#include<stm32f407xx_gpio_driver.h>
#include<stm32f407xx_spi_driver.h>
#include<stm32f407xx_I2C_driver.h>

#endif /* INC_STM32F407XX_H_ */
