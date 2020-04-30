#ifndef RCC_INTERFACE_H_
#define RCC_INTERFACE_H_

/* peripherals buses */
#define RCC_BUS_AHB  0
#define RCC_BUS_APB1 1
#define RCC_BUS_APB2 2

/* peripherals names */
#define RCC_PERI_AFIO   0
#define RCC_PERI_GPIOA  2
#define RCC_PERI_GPIOB  3
#define RCC_PERI_GPIOC  4
#define RCC_PERI_ADC1   9
#define RCC_PERI_ADC2   10
#define RCC_PERI_ADC3   15
#define RCC_PERI_SPI1   12
#define RCC_PERI_SPI2   14
#define RCC_PERI_SPI3   15
#define RCC_PERI_USART1 14
#define RCC_PERI_USART2 17
#define RCC_PERI_USART3 18
#define RCC_PERI_I2C1   21
#define RCC_PERI_I2C2   22
#define RCC_PERI_DAC    29

/* represents the clock source */
#define RCC_CLOCK_HSI 0
#define RCC_CLOCK_HSE 1
#define RCC_CLOCK_PLL 2

/* PLL clock source */
#define RCC_PLL_SOURCE_HSI_DIV2 0
#define RCC_PLL_SOURCE_HSE      1

/* PLL multiplication factor */
#define RCC_PLL_MUL2   0
#define RCC_PLL_MUL3   1
#define RCC_PLL_MUL4   2
#define RCC_PLL_MUL5   3
#define RCC_PLL_MUL6   4
#define RCC_PLL_MUL7   5
#define RCC_PLL_MUL8   6
#define RCC_PLL_MUL9   7
#define RCC_PLL_MUL10  8
#define RCC_PLL_MUL11  9
#define RCC_PLL_MUL12  10
#define RCC_PLL_MUL13  11
#define RCC_PLL_MUL14  12
#define RCC_PLL_MUL15  13
#define RCC_PLL_MUL16  14

/* MCO clock source */
#define RCC_MCO_SOURCE_NOCLOCK 0
#define RCC_MCO_SOURCE_SYSCLK  4
#define RCC_MCO_SOURCE_HSI     5
#define RCC_MCO_SOURCE_HSE     6
#define RCC_MCO_SOURCE_PLLDIV2 7

/* HSE source type */
#define RCC_HSE_SOURCE_OSCILLATOR 0
#define RCC_HSE_SOURCE_CLOCK      1

/* represents HCLK, PCLK1&2 */
#define RCC_HCLK  0
#define RCC_PCLK1 1
#define RCC_PCLK2 2

/* AHB prescaler values */
#define RCC_AHB_PRESCALER_SYSCLK_DIV1   0
#define RCC_AHB_PRESCALER_SYSCLK_DIV2   8
#define RCC_AHB_PRESCALER_SYSCLK_DIV4   9
#define RCC_AHB_PRESCALER_SYSCLK_DIV8   10
#define RCC_AHB_PRESCALER_SYSCLK_DIV16  11
#define RCC_AHB_PRESCALER_SYSCLK_DIV64  12
#define RCC_AHB_PRESCALER_SYSCLK_DIV128 13
#define RCC_AHB_PRESCALER_SYSCLK_DIV256 14
#define RCC_AHB_PRESCALER_SYSCLK_DIV512 15

/* AHB1&2 prescaler values */
#define RCC_APB_PRESCALER_AHB_DIV1  0
#define RCC_APB_PRESCALER_AHB_DIV2  4
#define RCC_APB_PRESCALER_AHB_DIV4  5
#define RCC_APB_PRESCALER_AHB_DIV8  6
#define RCC_APB_PRESCALER_AHB_DIV16 7

void RCC_init(void);

/* peripheral clock */
void RCC_setPeripheralClock(u8 bus, u8 peri, u8 state);

/* SysClock */
u8 RCC_getSysClock(void);
u8 RCC_changeSysClock(u8 clockSource);
u8 RCC_setClockState(u8 clockSource, u8 state);

/* PLL */
u8 RCC_setPLLsource(u8 PLLsource);
u8 RCC_setPLLHSEdiv2(u8 isDiv);
u8 RCC_setPLLmul(u8 mulFactor);

void RCC_setMCOsource(u8 clock);
void RCC_setHSItrim(u8 val);
void RCC_setCSSstate(u8 state);
u8 RCC_setHSEsourceType(u8 type);
void RCC_setClockPrescaler(u8 bus, u8 value);

#endif /* RCC_INTERFACE_H_ */
