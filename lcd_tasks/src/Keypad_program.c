/* libs */
#include "STD_TYPES.h"
#include "BIT_MAN.h"
/* MCAL */
#include "DIO_interface.h"
/* own */
#include "Keypad_interface.h"
#include "Keypad_private.h"
#include "Keypad_config.h"

void KP_init(void)
{
	/* rows are output push-pull 2MHz */
	DIO_setPinMode(ROW0_PORT, ROW0_PIN, 0b0010);
	DIO_setPinMode(ROW1_PORT, ROW1_PIN, 0b0010);
	DIO_setPinMode(ROW2_PORT, ROW2_PIN, 0b0010);
	DIO_setPinMode(ROW3_PORT, ROW3_PIN, 0b0010);

	DIO_setPinVal(ROW0_PORT, ROW0_PIN, 1);
	DIO_setPinVal(ROW1_PORT, ROW1_PIN, 1);
	DIO_setPinVal(ROW2_PORT, ROW2_PIN, 1);
	DIO_setPinVal(ROW3_PORT, ROW3_PIN, 1);

	/* cols are input pull-up */
	DIO_setPinMode(COL0_PORT, COL0_PIN, 0b1000);
	DIO_setPinMode(COL1_PORT, COL1_PIN, 0b1000);
	DIO_setPinMode(COL2_PORT, COL2_PIN, 0b1000);
	DIO_setPinMode(COL3_PORT, COL3_PIN, 0b1000);

	DIO_setPinVal(COL0_PORT, COL0_PIN, 1);
	DIO_setPinVal(COL1_PORT, COL1_PIN, 1);
	DIO_setPinVal(COL2_PORT, COL2_PIN, 1);
	DIO_setPinVal(COL3_PORT, COL3_PIN, 1);
}

u8 KP_getKey(void)
{
	/* row 0 */
	DIO_setPinVal(ROW0_PORT, ROW0_PIN, 0);
	DIO_setPinVal(ROW1_PORT, ROW1_PIN, 1);
	DIO_setPinVal(ROW2_PORT, ROW2_PIN, 1);
	DIO_setPinVal(ROW3_PORT, ROW3_PIN, 1);
	if (DIO_getPinVal(COL0_PORT, COL0_PIN) == 0)
	{
		return 1;
	}
	else if (DIO_getPinVal(COL1_PORT, COL1_PIN) == 0)
	{
		return 2;
	}
	else if (DIO_getPinVal(COL2_PORT, COL2_PIN) == 0)
	{
		return 3;
	}
	else if (DIO_getPinVal(COL3_PORT, COL3_PIN) == 0)
	{
		return 4;
	}

	/* row 1 */
	DIO_setPinVal(ROW0_PORT, ROW0_PIN, 1);
	DIO_setPinVal(ROW1_PORT, ROW1_PIN, 0);
	DIO_setPinVal(ROW2_PORT, ROW2_PIN, 1);
	DIO_setPinVal(ROW3_PORT, ROW3_PIN, 1);
	if (DIO_getPinVal(COL0_PORT, COL0_PIN) == 0)
	{
		return 5;
	}
	else if (DIO_getPinVal(COL1_PORT, COL1_PIN) == 0)
	{
		return 6;
	}
	else if (DIO_getPinVal(COL2_PORT, COL2_PIN) == 0)
	{
		return 7;
	}
	else if (DIO_getPinVal(COL3_PORT, COL3_PIN) == 0)
	{
		return 8;
	}

	/* row 2 */
	DIO_setPinVal(ROW0_PORT, ROW0_PIN, 1);
	DIO_setPinVal(ROW1_PORT, ROW1_PIN, 1);
	DIO_setPinVal(ROW2_PORT, ROW2_PIN, 0);
	DIO_setPinVal(ROW3_PORT, ROW3_PIN, 1);
	if (DIO_getPinVal(COL0_PORT, COL0_PIN) == 0)
	{
		return 9;
	}
	else if (DIO_getPinVal(COL1_PORT, COL1_PIN) == 0)
	{
		return 10;
	}
	else if (DIO_getPinVal(COL2_PORT, COL2_PIN) == 0)
	{
		return 11;
	}
	else if (DIO_getPinVal(COL3_PORT, COL3_PIN) == 0)
	{
		return 12;
	}

	/* row 3 */
	DIO_setPinVal(ROW0_PORT, ROW0_PIN, 1);
	DIO_setPinVal(ROW1_PORT, ROW1_PIN, 1);
	DIO_setPinVal(ROW2_PORT, ROW2_PIN, 1);
	DIO_setPinVal(ROW3_PORT, ROW3_PIN, 0);
	if (DIO_getPinVal(COL0_PORT, COL0_PIN) == 0)
	{
		return 13;
	}
	else if (DIO_getPinVal(COL1_PORT, COL1_PIN) == 0)
	{
		return 14;
	}
	else if (DIO_getPinVal(COL2_PORT, COL2_PIN) == 0)
	{
		return 15;
	}
	else if (DIO_getPinVal(COL3_PORT, COL3_PIN) == 0)
	{
		return 16;
	}

	return 0;
}

