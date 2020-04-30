#ifndef CLCD_INTERFACE_H_
#define CLCD_INTERFACE_H_

typedef struct
{
   /* register select (data, instr) */
   u8 RS_pin;
   u8 RS_port;
   
   /* read or write */
   u8 RW_pin;
   u8 RW_port;
   
   /* enable pin (works by edge) */
   u8 E_pin;
   u8 E_port;
   
   /* data bus higher bits 4->7 */
   u8 DB_pin[4];
   u8 DB_port[4];
} CLCD_t;

typedef enum
{
	CLCD_CMD_Clear_Disp                            = 0b00000001,
	CLCD_CMD_Return_Home                           = 0b00000010,

	CLCD_CMD_Entry_Mode_Dec_Addr_No_Disp_Shift     = 0b00000100,
	CLCD_CMD_Entry_Mode_Dec_Addr_Shift_Disp_Right  = 0b00000101,
	CLCD_CMD_Entry_Mode_Inc_Addr_No_Disp_Shift     = 0b00000110,
	CLCD_CMD_Entry_Mode_Inc_Addr_Shift_Disp_Left   = 0b00000111,

	CLCD_CMD_Disp_Off_Cursor_Off_Blink_off         = 0b00001000,
	CLCD_CMD_Disp_Off_Cursor_Off_Blink_on          = 0b00001001,
	CLCD_CMD_Disp_Off_Cursor_On_Blink_off          = 0b00001010,
	CLCD_CMD_Disp_Off_Cursor_On_Blink_on           = 0b00001011,
	CLCD_CMD_Disp_On_Cursor_Off_Blink_off          = 0b00001100,
	CLCD_CMD_Disp_On_Cursor_Off_Blink_on           = 0b00001101,
	CLCD_CMD_Disp_On_Cursor_On_Blink_off           = 0b00001110,
	CLCD_CMD_Disp_On_Cursor_On_Blink_on            = 0b00001111,

	CLCD_CMD_Shift_Cursor_left                     = 0b00010000,
	CLCD_CMD_Shift_Cursor_Right                    = 0b00010100,
	CLCD_CMD_Shift_Disp_left                       = 0b00011000,
	CLCD_CMD_Shift_Disp_Right                      = 0b00011100,

	CLCD_CMD_Function_4_Bit_Mode_1_Lines_5x7_Dots  = 0b00100000,
	CLCD_CMD_Function_4_Bit_Mode_1_Lines_5x10_Dots = 0b00100100,
	CLCD_CMD_Function_4_Bit_Mode_2_Lines_5x7_Dots  = 0b00101000,
	CLCD_CMD_Function_4_Bit_Mode_2_Lines_5x10_Dots = 0b00101100,
	CLCD_CMD_Function_8_Bit_Mode_1_Lines_5x7_Dots  = 0b00110000,
	CLCD_CMD_Function_8_Bit_Mode_1_Lines_5x10_Dots = 0b00110100,
	CLCD_CMD_Function_8_Bit_Mode_2_Lines_5x7_Dots  = 0b00111000,
	CLCD_CMD_Function_8_Bit_Mode_2_Lines_5x10_Dots = 0b00111100,

	CLCD_CMD_Set_CGRAM_Address                     = 0b01000000,
	CLCD_CMD_Set_DDRAM_Address                     = 0b10000000,
} CLCD_CMD_t;

void CLCD_init(CLCD_t* lcd);

void CLCD_execCMD(CLCD_t* lcd, CLCD_CMD_t cmd);
void CLCD_writeData(CLCD_t* lcd, u8 data);
void CLCD_writeStr(CLCD_t* lcd, const char* str);
void CLCD_writef(CLCD_t* lcd, const char* format, ...);

u8 CLCD_getAddr(CLCD_t* lcd);
u8 CLCD_getDDRAMdata(CLCD_t* lcd);
u8 CLCD_getCGRAMdata(CLCD_t* lcd);

void CLCD_setDDRAMaddr(CLCD_t* lcd, u8 addr);
void CLCD_setCGRAMaddr(CLCD_t* lcd, u8 addr);

u8 CLCD_gotoyx(CLCD_t* lcd, u8 x, u8 y);

void CLCD_writeDataAt(CLCD_t* lcd, u8 data, u8 y, u8 x);
void CLCD_writeStrAt(CLCD_t* lcd, const char* str, u8 y, u8 x);
void CLCD_writefAt(CLCD_t* lcd, u8 y, u8 x, const char* format, ...);
u8 CLCD_getDisplayDataAt(CLCD_t* lcd, u8 y, u8 x);
u8 CLCD_getCGRAMdataAt(CLCD_t* lcd, u8 addr);

void CLCD_writeCGRAMpattAt(CLCD_t* lcd, u8 addr, u8* pattArr, u8 len);

#endif /* CLCD_INTERFACE_H_ */
