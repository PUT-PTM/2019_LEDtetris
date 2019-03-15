#include "display_lib.h"

/* ---------------------< DISPLAY LIBRARY >----------------
 * Prepared macros and functions fo displaying numbers on 4x 7-segment display.
 * Also display dots. Need use Timer with reload 400 Hz. 
 * PIN outputs on PORT: 
 * GPIOB PIN_12 -> DISP_1
 * GPIOB PIN_13 -> DISP_2
 * GPIOB PIN_14 -> DISP_3
 * GPIOB PIN_15 -> DISP_4
 * 
 * Segments: GPIOE 
 * 
 * Funcions:
 *  # displayValue(disp_nr,value) - displaying value on only one defined display.
 *    Needed timer to make four differents values on the screen. 
 *  # 
 *  
 */

void displayValue(int disp_nr, int val)
{
	switch(disp_nr)
	{
    case 0:
    {
        DISP_1_OFF;
		DISP_2_OFF;
		DISP_3_OFF;
		DISP_4_OFF;
    };break;
	case 1:
	{
		DISP_1_ON;
		DISP_2_OFF;
		DISP_3_OFF;
		DISP_4_OFF;
	};break;
	case 2:
	{
		DISP_1_OFF;
		DISP_2_ON;
		DISP_3_OFF;
		DISP_4_OFF;

  	};break;
    case 3:
    {
    	DISP_1_OFF;
    	DISP_2_OFF;
    	DISP_3_ON;
    	DISP_4_OFF;

    };break;
    case 4:
    {
    	DISP_1_OFF;
    	DISP_2_OFF;
    	DISP_3_OFF;
    	DISP_4_ON;
    };break;
    default:
    {
    	DISP_1_ON;
    	DISP_2_ON;
    	DISP_3_ON;
    	DISP_4_ON;
    };break;
	};

    DISP_VAL_NULL;
	 switch(val)
	 {
	 case 9:
	 {
		 DISP_VAL_9;break;
	 }
	 case 8:
	 {
		 DISP_VAL_8;break;
	 }
	 case 7:
	 {
		 DISP_VAL_7;break;
	 }
	 case 6:
	 {
		 DISP_VAL_6;break;
	 }
	case 5:
	{
		DISP_VAL_5;break;
	}
	case 4:
	{
		DISP_VAL_4;break;
	}
	case 3:
	{
		DISP_VAL_3;break;
	}
	case 2:
	{
		DISP_VAL_2;break;
	}
	case 1:
	{
		DISP_VAL_1;break;
	}
	case 0:
	{
		DISP_VAL_0;break;
	}
    case -1:
    {
        DISP_VAL_MINUS;break;
    }
	default: DISP_VAL_NULL;break;
	};

}                                              