// libs
#include "STD_TYPES.h"
#include "BIT_MAN.h"
#include "Delay_interface.h"
// MCAL
#include "RCC_interface.h"
#include "DIO_interface.h"
// HAL
#include "CLCD_interface.h"
#include "Keypad_interface.h"

#define PORT_LED 'C'
#define PIN_LED  13

CLCD_t myLCD = {
   .RS_port = 'A',
   .RW_port = 'A',
   .E_port  = 'A',
   .DB_port[0] = 'A',
   .DB_port[1] = 'A',
   .DB_port[2] = 'A',
   .DB_port[3] = 'A',

   .RS_pin = 0,
   .RW_pin = 1,
   .E_pin  = 2,
   .DB_pin[0] = 3,
   .DB_pin[1] = 4,
   .DB_pin[2] = 5,
   .DB_pin[3] = 6
};

void superLoop(void);

void writeArabicChar(CLCD_t* lcd, u8 addr, u8 patt[7])
{
   CLCD_setCGRAMaddr(lcd, ((addr / 8) % 8) * 8);
   for (u8 i = 0; i < 7; i++)
   {
      CLCD_writeData(lcd, patt[i]);
   }

   /* cursor position */
   CLCD_writeData(lcd, 0);
}


void main(void)
{
   /* init clocks */
   RCC_init();

   /* enable clock for port A */
   RCC_setPeripheralClock(RCC_BUS_APB2, RCC_PERI_GPIOA, 1);
   /* enable clock for port B */
   RCC_setPeripheralClock(RCC_BUS_APB2, RCC_PERI_GPIOB, 1);
   /* enable clock for port C */
   RCC_setPeripheralClock(RCC_BUS_APB2, RCC_PERI_GPIOC, 1);

   /* LED c13 */
   DIO_setPinMode('C', 13, DIO_PIN_MODE_OUT_PUSH_PULL_2MHZ);
   DIO_setPinVal('C', 13, 1);

   /* configure delay function */
   delay_setCPUclockFactor(8000000);

   /* init LCD */
   CLCD_init(&myLCD);

   /* init keypad */
   KP_init();


   superLoop();
}


void superLoop(void)
{
   u8 buff[4];
   u8 idx = 0;
   u8 trials = 3;
   CLCD_gotoyx(&myLCD, 1, 7);

   while (1)
   {
      CLCD_writefAt(&myLCD, 0, 1, "Password (x%d)", trials);

      u8 key = KP_getKey();
      if (key != 0)
      {
         buff[idx] = key;
         idx++;
         CLCD_writeDataAt(&myLCD, '*', 1, 5 + idx);
         delay_ms(300); /* debounce */
      }

      if (idx == 4)
      {
         if ((buff[0] == 5) && (buff[1] == 6) && (buff[2] == 7) && (buff[3] == 8))
         {
            CLCD_writeStrAt(&myLCD, " Correct!   ", 1, 3);
            DIO_setPinVal('C', 13, 0);
            while (1)
            {

            }
         }
         else
         {
            if (trials != 1)
            {
               trials--;
               idx = 0;

               CLCD_writeStrAt(&myLCD, "   Wrong  ", 1, 3);
               delay_ms(2000);
               CLCD_writeStrAt(&myLCD, "                ", 1, 0);
               CLCD_gotoyx(&myLCD, 1, 7);
            }
            else
            {
               CLCD_writeStrAt(&myLCD, "  xx RESET xx  ", 1, 1);

               while (1)
               {
                  DIO_setPinVal('C', 13, 0);
                  delay_ms(100);

                  DIO_setPinVal('C', 13, 1);
                  delay_ms(100);
               }
            }
         }
      }
   }
}
