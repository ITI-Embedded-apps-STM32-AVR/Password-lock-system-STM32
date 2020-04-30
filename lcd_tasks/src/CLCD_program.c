/* libs */
#include "STD_TYPES.h"
#include "BIT_MAN.h"
#include "Delay_interface.h"
#include <stdarg.h>
#include <stdio.h>
/* MCAL */
#include "DIO_interface.h"
/* own */
#include "CLCD_interface.h"
#include "CLCD_private.h"

static void CLCD_createEnableRisingEdge(CLCD_t* lcd)
{
	DIO_setPinVal(lcd->E_port, lcd->E_pin, 0);
	delay_us(PIN_E_RISE_FALL_TIME_US);
	DIO_setPinVal(lcd->E_port, lcd->E_pin, 1);
	delay_us(PIN_E_RISE_FALL_TIME_US);
}

static void CLCD_createEnableFallingEdge(CLCD_t* lcd)
{
	DIO_setPinVal(lcd->E_port, lcd->E_pin, 1);
	delay_us(PIN_E_RISE_FALL_TIME_US);
	DIO_setPinVal(lcd->E_port, lcd->E_pin, 0);
	delay_us(PIN_E_RISE_FALL_TIME_US);
}

static void CLCD_send4bits(CLCD_t* lcd, u8 dataOrInstr, u8 data)
{
	/* set DB pins to output 2MHz */
	for (u8 i = 0; i < 4; i++)
	{
		DIO_setPinMode(lcd->DB_port[i], lcd->DB_pin[i], DIO_PIN_MODE_OUT_PUSH_PULL_2MHZ);
	}

	/* set RS and RW pins */
	DIO_setPinVal(lcd->RS_port, lcd->RS_pin, dataOrInstr);
	DIO_setPinVal(lcd->RW_port, lcd->RW_pin, RW_MODE_WRITE);

	/* send 4 bits */
	for (u8 i = 0; i < 4; i++)
	{
		DIO_setPinVal(lcd->DB_port[i], lcd->DB_pin[i], BIT_GET(data, i));
	}

	CLCD_createEnableRisingEdge(lcd);

	/* wait until data is processed/stable */
	delay_us(PIN_E_PULSE_WIDTH_TIME_US);

	CLCD_createEnableFallingEdge(lcd);

	/* wait for hold time */
	delay_us(WRITE_MODE_DATA_HOLD_TIME_US);
}

static u8 CLCD_read(CLCD_t* lcd, u8 dataOrInstr)
{
	/* set DB pins to input floating */
	for (u8 i = 0; i < 4; i++)
	{
		DIO_setPinMode(lcd->DB_port[i], lcd->DB_pin[i], DIO_PIN_MODE_IN_FLOATING);
	}

	/* set RS and RW pins */
	DIO_setPinVal(lcd->RS_port, lcd->RS_pin, dataOrInstr);
	DIO_setPinVal(lcd->RW_port, lcd->RW_pin, RW_MODE_READ);

	/* wait until RS and RW are setup */
	delay_us(READ_MODE_RS_RW_SETUP_TIME_us);

	register u8 res = 0;

	/* read higher 4 bits */
	CLCD_createEnableRisingEdge(lcd);
	/* wait until data is processed/stable */
	delay_us(PIN_E_PULSE_WIDTH_TIME_US);
	for (u8 i = 0; i < 4; i++)
	{
		res |= DIO_getPinVal(lcd->DB_port[i], lcd->DB_pin[i])
			   << (i + 4);
	}
	CLCD_createEnableFallingEdge(lcd);
	/* wait for hold time */
	delay_us(READ_MODE_DATA_HOLD_TIME_US);

	/* read lower 4 bits */
	CLCD_createEnableRisingEdge(lcd);
	/* wait until data is processed/stable */
	delay_us(PIN_E_PULSE_WIDTH_TIME_US);
	for (u8 i = 0; i < 4; i++)
	{
		res |= DIO_getPinVal(lcd->DB_port[i], lcd->DB_pin[i])
			   << i;
	}
	CLCD_createEnableFallingEdge(lcd);
	/* wait for hold time */
	delay_us(READ_MODE_DATA_HOLD_TIME_US);

	return res;
}

static void CLCD_waitIfBusy(CLCD_t* lcd)
{
	/* while busy flag (bit 7) = 1 */
	while (CLCD_read(lcd, RS_MODE_INSTR) & 0x80)
	{

	}
}

static void CLCD_writeFormatted(CLCD_t* lcd, const char* format, va_list argv)
{
	char buff[16 + 1];

	vsnprintf(buff, 16 + 1, format, argv);

	CLCD_writeStr(lcd, buff);
}


void CLCD_init(CLCD_t* lcd)
{
	/* set RS, RW,E, and DB pins to output 2MHz */
	DIO_setPinMode(lcd->RS_port, lcd->RS_pin, DIO_PIN_MODE_OUT_PUSH_PULL_2MHZ);
	DIO_setPinMode(lcd->RW_port, lcd->RW_pin, DIO_PIN_MODE_OUT_PUSH_PULL_2MHZ);
	DIO_setPinMode(lcd->E_port, lcd->E_pin, DIO_PIN_MODE_OUT_PUSH_PULL_2MHZ);
	for (u8 i = 0; i < 4; i++)
	{
		DIO_setPinMode(lcd->DB_port[i], lcd->DB_pin[i], DIO_PIN_MODE_OUT_PUSH_PULL_2MHZ);
	}

	/* see Hitachi datasheet page 46 */

	/* wait > 40ms after Vcc rises to 2.7v
	 * then wait > 15ms after Vcc rises to 4.5v */
	delay_ms(60);

	/* ######### magic init sequence ######### */
	/* Function Set: 8-bit mode (x3) */
	for (u8 i = 0; i < 3; i++)
	{
		CLCD_send4bits(lcd, RS_MODE_INSTR, 0b0011);
		delay_ms(5);
	}

	/* Function Set: 4-bit mode */
	CLCD_send4bits(lcd, RS_MODE_INSTR, 0b0010);
	CLCD_waitIfBusy(lcd);

	/* Function Set: 4-bit mode, 2 lines, 5x7 dots per char */
	CLCD_execCMD(lcd, CLCD_CMD_Function_4_Bit_Mode_2_Lines_5x7_Dots);
	/* turn off display, cursor, and cursor blinking */
	CLCD_execCMD(lcd, CLCD_CMD_Disp_Off_Cursor_Off_Blink_off);
	/* clear display */
	CLCD_execCMD(lcd, CLCD_CMD_Clear_Disp);
	/* Entry Mode: no display shift, increment address counter */
	CLCD_execCMD(lcd, CLCD_CMD_Entry_Mode_Inc_Addr_No_Disp_Shift);
	/* ####################################### */

	/* turn on display */
	CLCD_execCMD(lcd, CLCD_CMD_Disp_On_Cursor_Off_Blink_off);
}

void CLCD_execCMD(CLCD_t* lcd, CLCD_CMD_t cmd)
{
	/* send higher 4 bits */
	CLCD_send4bits(lcd, RS_MODE_INSTR, cmd >> 4);

	/* then send lower 4 bits */
	CLCD_send4bits(lcd, RS_MODE_INSTR, cmd);

	CLCD_waitIfBusy(lcd);
}

void CLCD_writeData(CLCD_t* lcd, u8 data)
{
	/* send higher 4 bits */
	CLCD_send4bits(lcd, RS_MODE_DATA, data >> 4);

	/* then send lower 4 bits */
	CLCD_send4bits(lcd, RS_MODE_DATA, data);

	CLCD_waitIfBusy(lcd);
}

void CLCD_writeStr(CLCD_t* lcd, const char* str)
{
	while (*str)
	{
		CLCD_writeData(lcd, *str);
		str++;
	}
}

void CLCD_writef(CLCD_t* lcd, const char* format, ...)
{
	va_list argv;

	va_start(argv, format);

	CLCD_writeFormatted(lcd, format, argv);

	va_end(argv);
}

u8 CLCD_getAddr(CLCD_t* lcd)
{
	return CLCD_read(lcd, RS_MODE_INSTR);
}

u8 CLCD_getDDRAMdata(CLCD_t* lcd)
{
	/* see Hitachi datasheet p31 */
	/* Address Counter must be set at the required position to be read, or else the read data will be invalid */

	CLCD_setDDRAMaddr(lcd, CLCD_getAddr(lcd));

	return CLCD_read(lcd, RS_MODE_DATA);
}

u8 CLCD_getCGRAMdata(CLCD_t* lcd)
{
	/* see Hitachi datasheet p31 */
	/* Address Counter must be set at the required position to be read, or else the read data will be invalid */

	CLCD_setCGRAMaddr(lcd, CLCD_getAddr(lcd));

	return CLCD_read(lcd, RS_MODE_DATA);
}

void CLCD_setDDRAMaddr(CLCD_t* lcd, u8 addr)
{
	CLCD_execCMD(lcd, CLCD_CMD_Set_DDRAM_Address | addr);
}

void CLCD_setCGRAMaddr(CLCD_t* lcd, u8 addr)
{
	CLCD_execCMD(lcd, CLCD_CMD_Set_CGRAM_Address | addr);
}

u8 CLCD_gotoyx(CLCD_t* lcd, u8 y, u8 x)
{
	if (x < 16)
	{
		switch (y)
		{
		case 0:
			CLCD_setDDRAMaddr(lcd, DDRAM_LINE_1_START | x);
			return 1;
		break;

		case 1:
			CLCD_setDDRAMaddr(lcd, DDRAM_LINE_2_START | x);
			return 1;
		break;
		}
	}

	return 0;
}

void CLCD_writeDataAt(CLCD_t* lcd, u8 data, u8 y, u8 x)
{
	if (CLCD_gotoyx(lcd, y, x) != 0)
	{
		CLCD_writeData(lcd, data);
	}
}

void CLCD_writeStrAt(CLCD_t* lcd, const char* str, u8 y, u8 x)
{
	if (CLCD_gotoyx(lcd, y, x) != 0)
	{
		CLCD_writeStr(lcd, str);
	}
}

void CLCD_writefAt(CLCD_t* lcd, u8 y, u8 x, const char* format, ...)
{
	if (CLCD_gotoyx(lcd, y, x) != 0)
	{
		va_list argv;

		va_start(argv, format);

		CLCD_writeFormatted(lcd, format, argv);

		va_end(argv);
	}
}

u8 CLCD_getDisplayDataAt(CLCD_t* lcd, u8 y, u8 x)
{
	if (CLCD_gotoyx(lcd, y, x) != 0)
	{
		return CLCD_getDDRAMdata(lcd);
	}
	else
	{
		return ' '; /* default empty char is a space */
	}
}

u8 CLCD_getCGRAMdataAt(CLCD_t* lcd, u8 addr)
{
	CLCD_setCGRAMaddr(lcd, addr);

	return CLCD_getCGRAMdata(lcd);
}

void CLCD_writeCGRAMpattAt(CLCD_t* lcd, u8 addr, u8* pattArr, u8 len)
{
	for (u8 i = 0; i < len; i++)
	{
		CLCD_setCGRAMaddr(lcd, addr + i);
		CLCD_writeData(lcd, pattArr[i]);
	}
}
