/* Filename: 
 *      1602_LCD_HAL.h
 *
 * Author: 
 *          Henry Gilbert
 * 
 * Date: 
 * 
 * Description: 
 *      Public interface to 1602 hardware abstraction layer. Should only be 
 *      used in 1602_LCD.c source file. 
 *      
 * 
 */

#ifndef LCD_1602_HAL
#define LCD_1602_HAL

#include <stdint.h>
#include <stdbool.h>
#include "../src/config/default/peripheral/port/plib_port.h"

/**************************** Struct instance **************************/

/* Struct:
 *      LCD_InstanceConfig
 * 
 * Description:
 *      
 * 
 */




/* Struct: 
 *      LCD_1602
 * 
 * Description:
 *      Parallel 8 bit data bus, 3 bit control signal, for 1602 LCD instance. 
 */
typedef struct
{
    const PORT_PIN db0;
    const PORT_PIN db1;
    const PORT_PIN db2;
    const PORT_PIN db3;
    const PORT_PIN db4;
    const PORT_PIN db5;
    const PORT_PIN db6;
    const PORT_PIN db7;
    const PORT_PIN e;
    const PORT_PIN rw;
    const PORT_PIN rs;
} LCD_1602;

/************************* RAM Type enums *******************/
typedef enum
{
    CGRAM,
    DDRAM
} LCM_RAM_TYPE;

/******************** Entry mode control enums **************/
typedef enum
{
    LCM_ENTRY_LEFT,
    LCM_ENTRY_RIGHT
} LCM_ENTRY_MODE_RAM;

typedef enum
{
    LCM_DDR_RW_SHIFT_ENABLED,
    LCM_DDR_RW_SHIFT_DISABLED,
} LCM_ENTRY_MODE_SHIFT;

/****************  Display control function enums  ************/
typedef enum
{
    LCM_DISPLAY_ON,
    LCM_DISPLAY_OFF,
} LCM_DISPLAY_CONTROL;

typedef enum
{
    LCM_CURSOR_VISIBLE,
    LCM_CURSOR_INVISIBLE
} LCM_CURSOR_VISIBILITY;

typedef enum
{
    LCM_BLINK_ON,
    LCM_BLINK_OFF
} LCM_CURSOR_BLINK;

/************************** Shift control functions ***********/
typedef enum
{
    LCM_SHIFT_CURSOR,
    LCM_SHIFT_DISPLAY
} LCM_CURSOR_SHIFT_OBJ;

typedef enum
{
    LCM_SHIFT_LEFT,
    LCM_SHIFT_RIGHT
} LCM_SHIFT_DIRECTION;

/********************* Function set ***************************/
typedef enum
{
    LCM_4_BIT_INTERFACE,
    LCM_8_BIT_INTERFACE
} LCM_DATA_INTERFACE;

typedef enum
{
    LCM_1_LINE_DISPLAY,
    LCM_2_LINE_DISPLAY,
    LCM_NO_LINE_CONTROL_SET
} LCM_DISPLAY_LINE_CONTROL;

typedef enum
{
    LCM_5x8_DOTS,
    LCM_5x11_DOTS,
} LCM_DISPLAY_DOT_TYPE;


/*************************** Public Interface *************************/



/* Function:
 *      LCM1602_ClearDisplay
 * 
 * Description: Writes 0x20 to all DDRRAM addresses. Sets the home cursor 
 *      position to 0 (base). 
 *      
 */
void LCM1602_ClearDisplay(const LCD_1602 * const lcd);


/* Function:
 *      LCM1602_ReturnHome
 *  
 * Description:
 *      Set DDRAM address to 0x00 from AC and return cursor to it's original
 *      position if shifted. Contents of DDRAM are not changed. 
 */
void LCM1602_ReturnHome(const LCD_1602 * const lcd);


/* Function: 
 *      LCM1602_EntryModeSet
 * 
 * Description: 
 *      Set the moving direction of cursor and display. 
 *      ramSetting: 
 *          Set LCM_ENTRY_LEFT for cursor/blink moves to left and DDRAM address is decremented by 1.
 *          Set LCM_ENTRY_RIGHT for cursor/blink moves to right and DDRAM address is incremented by 1.
 *           
 *      shiftSetting:
 *          Set LCM_DDR_RW_SHIFT_ENABLED to shift cursor/blink according to ramSetting value. 
 *          Set LCM_DDR_RW_SHIFT_DISABLED to disable shift of cursor/blink when a DDRAM R/W occurs.  
 *          
 */
void LCM1602_EntryModeSet(const LCD_1602 * const lcd,
        const LCM_ENTRY_MODE_RAM ramSetting,
        const LCM_ENTRY_MODE_SHIFT shiftSetting);


/* Function:
 *      LCM1602_DisplayControl
 * 
 * Description:
 *      Control command for enabling display, cursor setting, and cursor blink setting. 
 *      Bit 2 display setting: Set HIGH to turn display on. Set LOW for display off and to retain display data in DDRAM.
 *      Bit 1 cursor visibility setting: Set HIGH to show cursor. LOW to hide cursor. Only controls visibility. 
 *      Bit 0 cursor blink setting: Set HIGH to blink cursor, LOW to remain constant. 
 *      
 *      disp:
 *          LCM_DISPLAY_ON or LCM_DISPLAY_OFF
 *      vis:
 *          LCM_CURSOR_VISIBLE or LCM_CURSOR_INVISIBLE
 *      blink:
 *          LCM_BLINK_ON or LCM_BLINK_OFF
 * 
 */
void LCM1602_DisplayControl(const LCD_1602 * const lcd,
        const LCM_DISPLAY_CONTROL disp,
        const LCM_CURSOR_VISIBILITY vis,
        const LCM_CURSOR_BLINK blink);


/* Function:
 *      LCM1602_Shift
 * 
 * Description:
 *      Without writing or reading display data, shift right/left cursor position or 
 *      display. Used to correct or search display data. During 2 line mode display, cursor 
 *      moves to the 2nd line after 40th digit of 1st line. Display shift is performed 
 *      simultaneously in all the line. When displayed data is shifted repeatedly, each 
 *      line is shifted individually. When display shift is performed, the contents
 *      of address counter are not changed. 
 *      S/C    R/L      Description                                                     AC Value
 *      L       L       Shift cursor to the left                                         AC -= 1
 *      L       H       Shift cursor to the right                                        AC += 1
 *      H       L       Shift display to the left. Cursor follows the display shift.     AC = AC
 *      H       H       Shift display to the right. Cursor follows display shift.        AC = AC
 * 
 */
void LCM1602_Shift(const LCD_1602 * const lcd,
        const LCM_CURSOR_SHIFT_OBJ obj,
        const LCM_SHIFT_DIRECTION dir);


/* Function:
 *      LCM1602_FunctionSet
 * 
 * Description:
 *      Sets function/module control.
 *      interface : 
 *          Set LCM_4_BIT_INTERFACE for 4 bit interface, 
 *          Set LCM_8_BIT_INTERFACE for 8 bit interface
 *      lc :
 *          Set LCM_1_LINE_DISPLAY for 1 line display
 *          Set LCM_2_LINE_DISPLAY for 2 line display
 *      dt :
 *          Set LCM_5x8_DOTS for 5x8 font types
 *          Set LCM_5x11_DOTS for 5x11 font types 
 *          
 * 
 */
void LCM1602_FunctionSet(const LCD_1602 * const lcd,
        const LCM_DATA_INTERFACE interface,
        const LCM_DISPLAY_LINE_CONTROL lc,
        const LCM_DISPLAY_DOT_TYPE dt);



/* Function:
 *      LCM1602_CGRAM_Addr_Set
 * 
 * Description:
 *      Sets CGRAM address to AC. Only writes bits 5-0
 */
bool LCM1602_CGRAM_Addr_Set(const LCD_1602 * const lcd,
        const uint8_t addr);


/* Function:
 *      LCM1602_DDRAM_Addr_Set
 *
 * Description:
 *      Sets DDR address to AC. In 1 line mode, DDRAM address ranges from 0-0x4F
 *      In 2 line mode, DDRAM address line 1 ranges from 0-0x27, 
 *                                    line 2 ranges from 0x40-0x67
 */
bool LCM1602_DDRAM_Addr_Set(const LCD_1602 * const lcd,
        const uint8_t addr);


/* Function:
 *      LCM1602_ReadFlagsAndAddr
 * 
 * Description: 
 *      Bit 7 is busy flag. Reads all pins and returns current addr as pin inputs.
 *      Input pointer is return parameter. Reading address value increments AC by 1
 * 
 * 
 */
// TODO should the return param be addr or flag? 
bool LCM1602_ReadFlagsAndAddr(const LCD_1602 * const lcd,
        uint8_t * const addr);



/* Function:
 *      LCM1602_ReadRAM
 * 
 * Description:
 *      Selection between CGRAM and DDRAM is based on previous address set instruction. 
 *      
 */
uint8_t LCM1602_ReadRAM(const LCD_1602 * const lcd);



/* Function:
 *      LCM1602_WriteRAM
 * 
 * Description:
 *      
 * 
 */
void LCM1602_WriteRAM(const LCD_1602 * const lcd,
        const uint8_t value);



#endif

/* end 1602_LCD_HAL.h header file*/
