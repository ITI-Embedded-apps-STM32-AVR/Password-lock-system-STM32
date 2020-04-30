#ifndef RCC_PRIVATE_H_
#define RCC_PRIVATE_H_

/* System clock Switch (SWS) */
#define SW_MASK 0x00000003

#define SW_HSI  0x00000000
#define SW_HSE  0x00000001
#define SW_PLL  0x00000002

/* PLL */
#define PLL_MUL_MASK 0x003C0000

/* MCO */
#define MCO_SOURCE_MASK 0x07000000

/* Trimming value of HSI */
#define TRIM_HSI_MASK 0x000000F8

/* busses prescalers (AHB, APB1, APB2) */
#define AHB_PRE_MASK  0x000000F0
#define APB1_PRE_MASK 0x00000700
#define APB2_PRE_MASK 0x00003800

#endif /* RCC_PRIVATE_H_ */
