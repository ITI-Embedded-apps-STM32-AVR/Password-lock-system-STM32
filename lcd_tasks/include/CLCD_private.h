#ifndef CLCD_PRIVATE_H_
#define CLCD_PRIVATE_H_

#define RS_MODE_DATA  1
#define RS_MODE_INSTR 0

#define RW_MODE_READ  1
#define RW_MODE_WRITE 0

/* time to wait while creating pin E edge (0->1 or  1->0) */
#define PIN_E_RISE_FALL_TIME_US 1
/* time to wait for data to be read from the bus by the LCD,
 * also the time to wait before reading data from the bus */
#define PIN_E_PULSE_WIDTH_TIME_US 1

/* time to wait after creating pin E falling edge */
#define WRITE_MODE_DATA_HOLD_TIME_US     1

/* time to waste after reading data */
#define READ_MODE_DATA_HOLD_TIME_US     1
/* time to wait after configuring RS and RW for read */
#define READ_MODE_RS_RW_SETUP_TIME_us   1

#define DDRAM_LINE_1_START 0x00
#define DDRAM_LINE_2_START 0x40

static void CLCD_createEnableRisingEdge(CLCD_t* lcd);
static void CLCD_createEnableFallingEdge(CLCD_t* lcd);

static void CLCD_send4bits(CLCD_t* lcd, u8 dataOrInstr, u8 data);

static u8 CLCD_read(CLCD_t* lcd, u8 dataOrInstr);

static void CLCD_waitIfBusy(CLCD_t* lcd);

static void CLCD_writeFormatted(CLCD_t* lcd, const char* format, va_list argv);

#endif /* CLCD_PRIVATE_H_ */
