#ifndef DIO_REGISTERS_H_
#define DIO_REGISTERS_H_

#define PORTA_BASE 0x40010800
#define PORTB_BASE 0x40010C00
#define PORTC_BASE 0x40011000

/* port A */
#define PORTA_CRL  (*((volatile u32*)(PORTA_BASE + 0x00)))
#define PORTA_CRH  (*((volatile u32*)(PORTA_BASE + 0x04)))
#define PORTA_CR   (*((volatile u64*)(PORTA_BASE + 0x00)))
#define PORTA_IDR  (*((volatile u32*)(PORTA_BASE + 0x08)))
#define PORTA_ODR  (*((volatile u32*)(PORTA_BASE + 0x0C)))
#define PORTA_BSRR (*((volatile u32*)(PORTA_BASE + 0x10)))
#define PORTA_BRR  (*((volatile u32*)(PORTA_BASE + 0x14)))
#define PORTA_LCKR (*((volatile u32*)(PORTA_BASE + 0x18)))

/* port B */
#define PORTB_CRL  (*((volatile u32*)(PORTB_BASE + 0x00)))
#define PORTB_CRH  (*((volatile u32*)(PORTB_BASE + 0x04)))
#define PORTB_CR   (*((volatile u64*)(PORTB_BASE + 0x00)))
#define PORTB_IDR  (*((volatile u32*)(PORTB_BASE + 0x08)))
#define PORTB_ODR  (*((volatile u32*)(PORTB_BASE + 0x0C)))
#define PORTB_BSRR (*((volatile u32*)(PORTB_BASE + 0x10)))
#define PORTB_BRR  (*((volatile u32*)(PORTB_BASE + 0x14)))
#define PORTB_LCKR (*((volatile u32*)(PORTB_BASE + 0x18)))

/* port C */
#define PORTC_CRL  (*((volatile u32*)(PORTC_BASE + 0x00)))
#define PORTC_CRH  (*((volatile u32*)(PORTC_BASE + 0x04)))
#define PORTC_CR   (*((volatile u64*)(PORTC_BASE + 0x00)))
#define PORTC_IDR  (*((volatile u32*)(PORTC_BASE + 0x08)))
#define PORTC_ODR  (*((volatile u32*)(PORTC_BASE + 0x0C)))
#define PORTC_BSRR (*((volatile u32*)(PORTC_BASE + 0x10)))
#define PORTC_BRR  (*((volatile u32*)(PORTC_BASE + 0x14)))
#define PORTC_LCKR (*((volatile u32*)(PORTC_BASE + 0x18)))

#endif /* DIO_REGISTERS_H_ */
