// libs
#include "STD_TYPES.h"
#include "BIT_MAN.h"
// own
#include "RCC_interface.h"
#include "RCC_registers.h"
#include "RCC_private.h"

void RCC_init(void)
{
	/* disable Clock Security System */
	RCC_setCSSstate(0);

	/* set SysClock to HSI (8MHz) */
	RCC_changeSysClock(RCC_CLOCK_HSI);

	/* set HSI trimming value to 17 */
	RCC_setHSItrim(17);

	/* disable other clocks (PLL then HSE)*/
	RCC_setClockState(RCC_CLOCK_PLL, 0);
	RCC_setClockState(RCC_CLOCK_HSE, 0);

	/* set buses prescalers */
	RCC_setClockPrescaler(RCC_HCLK, RCC_AHB_PRESCALER_SYSCLK_DIV1);
	RCC_setClockPrescaler(RCC_PCLK1, RCC_APB_PRESCALER_AHB_DIV1);
	RCC_setClockPrescaler(RCC_PCLK2, RCC_APB_PRESCALER_AHB_DIV1);

	/* set HSE source type as external osc. */
	RCC_setHSEsourceType(RCC_HSE_SOURCE_OSCILLATOR);

	/* set PLL config: source = HSI / 2, HSE / 2 = no, multiplication = x2 */
	RCC_setPLLsource(RCC_PLL_SOURCE_HSI_DIV2);
	RCC_setPLLHSEdiv2(0);
	RCC_setPLLmul(RCC_PLL_MUL2);

	/* MCO clock source = no clock (disable MCO) */
	RCC_setMCOsource(RCC_MCO_SOURCE_NOCLOCK);
}

void RCC_setPeripheralClock(u8 bus, u8 peri, u8 state)
{
   switch (bus)
   {
      case RCC_BUS_AHB:
    	  if (state == 1)
    	  {
    		  RCC_AHBENR = BIT_SET(RCC_AHBENR, peri);
    	  }
    	  else
    	  {
    		  RCC_AHBENR = BIT_CLEAR(RCC_AHBENR, peri);
    	  }
      break;
      
      case RCC_BUS_APB1:
    	  if (state == 1)
    	  {
    		  RCC_APB1ENR = BIT_SET(RCC_APB1ENR, peri);
    	  }
    	  else
    	  {
    		  RCC_APB1ENR = BIT_CLEAR(RCC_APB1ENR, peri);
    	  }
      break;
      
      case RCC_BUS_APB2:
    	  if (state == 1)
    	  {
    		  RCC_APB2ENR = BIT_SET(RCC_APB2ENR, peri);
    	  }
    	  else
    	  {
    		  RCC_APB2ENR = BIT_CLEAR(RCC_APB2ENR, peri);
    	  }
      break;
   }
}

u8 RCC_getSysClock(void)
{
	return ((BIT_GET(RCC_CFGR, 3) << 1) | BIT_GET(RCC_CFGR, 2));
}

u8 RCC_changeSysClock(u8 clockSource)
{
	switch (clockSource)
	{
	case RCC_CLOCK_HSI:
		if (RCC_setClockState(RCC_CLOCK_HSI, 1) == 1) /* if enabling HSI succeeded */
		{
			/* set SysClock to HSI */
			RCC_CFGR = (RCC_CFGR & ~SW_MASK) | SW_HSI;

			return 1;
		}
		else /* if it failed */
		{
			return 0;
		}
	break;

	case RCC_CLOCK_HSE:
		if (RCC_setClockState(RCC_CLOCK_HSE, 1) == 1) /* if enabling HSE succeeded */
		{
			/* set SysClock to HSE */
			RCC_CFGR = (RCC_CFGR & ~SW_MASK) | SW_HSE;

			return 1;
		}
		else /* if it failed */
		{
			return 0;
		}
	break;

	case RCC_CLOCK_PLL:
		if (RCC_setClockState(RCC_CLOCK_PLL, 1) == 1) /* if enabling PLL succeeded */
		{
			/* set SysClock to PLL */
			RCC_CFGR = (RCC_CFGR & ~SW_MASK) | SW_PLL;

			return 1;
		}
		else /* if it failed */
		{
			return 0;
		}
	break;
	}

	/* invalid clock source */
	return 0;
}

u8 RCC_setClockState(u8 clockSource, u8 state)
{
	/* if it reached 0 and required task didn't finish
	 * then a failure state should be returned */
	register u16 timeoutCtr = 0xFFFF;

	switch (clockSource)
	{
	case RCC_CLOCK_HSI:
		 /* if SysClock = HSI */
		if (RCC_getSysClock() == RCC_CLOCK_HSI)
		{
			/* we mustn't disable it if it's current clock source */
			if (state == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

		/* we reach here if this is not the SysClock */

		if (state == 1) /* if enabling clock */
		{
			RCC_CR = BIT_SET(RCC_CR, 0);

			/* trap until HSI is ready */
			while ((BIT_GET(RCC_CR, 1) == 0) && (timeoutCtr != 0))
			{
				timeoutCtr--;
			}

			/* if it timed out before finishing */
			if (timeoutCtr == 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else /* if disabling clock */
		{
			RCC_CR = BIT_CLEAR(RCC_CR, 0);

			return 1;
		}
	break;

	case RCC_CLOCK_HSE:
		 /* if SysClock = HSI */
		if (RCC_getSysClock() == RCC_CLOCK_HSE)
		{
			/* we mustn't disable it if it's current clock source */
			if (state == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

		/* we reach here if this is not the SysClock */

		if (state == 1) /* if enabling clock */
		{
			RCC_CR = BIT_SET(RCC_CR, 16);

			/* trap until HSE is ready */
			while ((BIT_GET(RCC_CR, 17) == 0) && (timeoutCtr != 0))
			{
				timeoutCtr--;
			}

			/* if it timed out before finishing */
			if (timeoutCtr == 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else /* if disabling clock */
		{
			RCC_CR = BIT_CLEAR(RCC_CR, 16);

			return 1;
		}
	break;

	case RCC_CLOCK_PLL:
		 /* if SysClock = PLL */
		if (RCC_getSysClock() == RCC_CLOCK_PLL)
		{
			/* we mustn't disable it if it's current clock source */
			if (state == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

		/* we reach here if this is not the SysClock */

		if (state == 1) /* if enabling clock */
		{
			RCC_CR = BIT_SET(RCC_CR, 24);

			/* trap until PLL is ready */
			while ((BIT_GET(RCC_CR, 25) == 0) && (timeoutCtr != 0))
			{
				timeoutCtr--;
			}

			/* if it timed out before finishing */
			if (timeoutCtr == 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else /* if disabling clock */
		{
			RCC_CR = BIT_CLEAR(RCC_CR, 24);

			return 1;
		}
	break;
	}

	/* invalid clock source */
	return 0;
}

u8 RCC_setPLLsource(u8 PLLsource)
{
	/* if PLL is turned on, then we can't change its configs */
	if (BIT_GET(RCC_CR, 24) == 1)
	{
		return 0;
	}

	switch (PLLsource)
	{
	case RCC_PLL_SOURCE_HSI_DIV2:
		RCC_CFGR = BIT_CLEAR(RCC_CFGR, 16);

		return 1;
	break;

	case RCC_PLL_SOURCE_HSE:
		RCC_CFGR = BIT_SET(RCC_CFGR, 16);

		return 1;
	break;
	}

	/* invalid PLL source */
	return 0;
}

u8 RCC_setPLLHSEdiv2(u8 isDiv)
{
	/* if PLL is turned on, then we can't change its configs */
	if (BIT_GET(RCC_CR, 24) == 1)
	{
		return 0;
	}

	if (isDiv == 1) /* if divide */
	{
		RCC_CFGR = BIT_SET(RCC_CFGR, 17);
	}
	else
	{
		RCC_CFGR = BIT_CLEAR(RCC_CFGR, 17);
	}

	return 1;
}

u8 RCC_setPLLmul(u8 mulFactor)
{
	/* if PLL is turned on, then we can't change its configs */
	if (BIT_GET(RCC_CR, 24) == 1)
	{
		return 0;
	}

	/* set required PLL multiplication factor */
	switch (mulFactor)
	{
	case RCC_PLL_MUL2:
	case RCC_PLL_MUL3:
	case RCC_PLL_MUL4:
	case RCC_PLL_MUL5:
	case RCC_PLL_MUL6:
	case RCC_PLL_MUL7:
	case RCC_PLL_MUL8:
	case RCC_PLL_MUL9:
	case RCC_PLL_MUL10:
	case RCC_PLL_MUL11:
	case RCC_PLL_MUL12:
	case RCC_PLL_MUL13:
	case RCC_PLL_MUL14:
	case RCC_PLL_MUL15:
	case RCC_PLL_MUL16:
		RCC_CFGR = (RCC_CFGR & ~PLL_MUL_MASK) | (mulFactor << 18);

		return 1;
	break;
	}

	return 0;
}

void RCC_setMCOsource(u8 clock)
{
	switch (clock)
	{
	case RCC_MCO_SOURCE_NOCLOCK:
	case RCC_MCO_SOURCE_SYSCLK:
	case RCC_MCO_SOURCE_HSI:
	case RCC_MCO_SOURCE_HSE:
	case RCC_MCO_SOURCE_PLLDIV2:
		RCC_CFGR = (RCC_CFGR & ~MCO_SOURCE_MASK) | (clock << 24);
	break;
	}
}

void RCC_setHSItrim(u8 val)
{
	/* max allowed value = 31 (5 bits only) */
	if (val > 31)
	{
		return;
	}

	RCC_CR = (RCC_CR & ~TRIM_HSI_MASK) | (val << 3);
}

void RCC_setCSSstate(u8 state)
{
	if (state == 1)
	{
		RCC_CR = BIT_SET(RCC_CR, 19);
	}
	else
	{
		RCC_CR = BIT_CLEAR(RCC_CR, 19);
	}
}

u8 RCC_setHSEsourceType(u8 type)
{
	/* we can't change HSE source type if SysClock = HSE */
	if (RCC_getSysClock() == RCC_CLOCK_HSE)
	{
		return 0;
	}

	if (type == RCC_HSE_SOURCE_OSCILLATOR)
	{
		RCC_CR = BIT_CLEAR(RCC_CR, 18);
	}
	else
	{
		RCC_CR = BIT_SET(RCC_CR, 18);
	}

	return 1;
}

void RCC_setClockPrescaler(u8 clock, u8 prescaler)
{
	switch (clock)
	{
	case RCC_HCLK:
		switch (prescaler)
		{
		case RCC_AHB_PRESCALER_SYSCLK_DIV1:
		case RCC_AHB_PRESCALER_SYSCLK_DIV2:
		case RCC_AHB_PRESCALER_SYSCLK_DIV4:
		case RCC_AHB_PRESCALER_SYSCLK_DIV8:
		case RCC_AHB_PRESCALER_SYSCLK_DIV16:
		case RCC_AHB_PRESCALER_SYSCLK_DIV64:
		case RCC_AHB_PRESCALER_SYSCLK_DIV128:
		case RCC_AHB_PRESCALER_SYSCLK_DIV256:
		case RCC_AHB_PRESCALER_SYSCLK_DIV512:
			RCC_CFGR = (RCC_CFGR & ~AHB_PRE_MASK) | (prescaler << 4);
		break;
		}
	break;

	case RCC_PCLK1:
		switch (prescaler)
		{
		case RCC_APB_PRESCALER_AHB_DIV1:
		case RCC_APB_PRESCALER_AHB_DIV2:
		case RCC_APB_PRESCALER_AHB_DIV4:
		case RCC_APB_PRESCALER_AHB_DIV8:
		case RCC_APB_PRESCALER_AHB_DIV16:
			RCC_CFGR = (RCC_CFGR & ~APB1_PRE_MASK) | (prescaler << 8);
		}
		break;
	break;

	case RCC_PCLK2:
		switch (prescaler)
		{
		case RCC_APB_PRESCALER_AHB_DIV1:
		case RCC_APB_PRESCALER_AHB_DIV2:
		case RCC_APB_PRESCALER_AHB_DIV4:
		case RCC_APB_PRESCALER_AHB_DIV8:
		case RCC_APB_PRESCALER_AHB_DIV16:
			RCC_CFGR = (RCC_CFGR & ~APB2_PRE_MASK) | (prescaler << 11);
		}
		break;
	break;
	}
}
