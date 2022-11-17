/* Filename:
 *      1602_LCD.c
 * 
 * Author: 
 *      Henry Gilbert
 * 
 * Description:
 *      Implementation layer for 1602 LCD driver. 
 * 
 *  
 */


/***************** Included File(s) ****************/
#include "1602_LCD.h"
#include "1602_LCD_HAL.h"
#include <stddef.h>
#include <xc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 32000000
#endif


/***************** Macro Definition(s) *************/




/***************** Local Variable(s) ***************/

/*************** Static Function Prototype(s) ******/




// hard coded at first. Make generic later

//bool LCM_1602_InitializeDisplay( )
//{
//    LCM1602_DisplayControl( LCM_DISPLAY_ON,
//                            LCM_CURSOR_VISIBLE,
//                            LCM_BLINK_ON );
//    LCM1602_EntryModeSet( LCM_ENTRY_RIGHT,
//                          LCM_DDR_RW_SHIFT_DISABLED );
//    LCM1602_FunctionSet( LCM_8_BIT_INTERFACE,
//                         LCM_2_LINE_DISPLAY,
//                         LCM_5x8_DOTS );
//    LCM1602_ClearDisplay( );
//
//
//}
//
//bool LCM_1602_TestDDRAM( )
//{
//    LCM1602_DDRAM_Addr_Set( 0x00u );
//    uint8_t i;
//    uint8_t val = 48;
//    for( i = 0; i < 16; i++ )
//    {
//        LCM1602_WriteRAM(val + i  );
//        __delay_ms( 200 );
//    }
//    LCM1602_DDRAM_Addr_Set( 0x40u );
//    for( i = 0; i < 16; i++ )
//    {
//        LCM1602_WriteRAM( val + i +16u);
//        __delay_ms( 200);
//    }
//
//    //    for( i = 0x40; i < 0x67; i++ )
//    //    {
//    //        LCM1602_WriteRAM( 0b01010111u);
//    //        __delay_ms( 500 );
//    //    }
//
//
//
//
//
//}
//






/* end 1602_LCD.c */
