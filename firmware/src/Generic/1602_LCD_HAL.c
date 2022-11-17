/* Filename:
 *      1602_LCD_HAL
 * 
 * Author: 
 *      Henry Gilbert
 * 
 * Description:
 *      Hardware implementation layer for the 1602 LCD. Uses a single hard-coded
 *      configuration for the 1602 LCD. Does not use generic instances. 
 *  
 *  
 */


/***************** Included File(s) ****************/
#include <xc.h>
#include "1602_LCD_HAL.h"


/***************** Macro Definition(s) *************/
#define MAX_CGRAM_ADDR 64u

/******************** Type Definition(s) ***********/
typedef uint8_t AddressCounter;


/****************** Static function prototypes *****/
__attribute__( ( always_inline ) ) static inline void LCD_RS_Data( const LCD_1602 * const lcd );
__attribute__( ( always_inline ) ) static inline void LCD_RS_Instruction( const LCD_1602 * const lcd );
__attribute__( ( always_inline ) ) static inline void LCD_RW_Read( const LCD_1602 * const lcd );
__attribute__( ( always_inline ) ) static inline void LCD_RW_Write( const LCD_1602 * const lcd );
__attribute__( ( always_inline ) ) static inline void LCD_E_Start( const LCD_1602 * const lcd );
__attribute__( ( always_inline ) ) static inline void LCD_E_End( const LCD_1602 * const lcd );

static inline void LCD_RS_Data( const LCD_1602 * const lcd )
{
    PORT_PinWrite( lcd->rs, true );
}

static inline void LCD_RS_Instruction( const LCD_1602 * const lcd )
{
    PORT_PinWrite( lcd->rs, false );
}

static inline void LCD_RW_Read( const LCD_1602 * const lcd )
{
    PORT_PinWrite( lcd->rw, true );
    Nop( );
    Nop( );
    Nop( );
}

static inline void LCD_RW_Write( const LCD_1602 * const lcd )
{
    PORT_PinWrite( lcd->rw, false );
    Nop( );
    Nop( );
    Nop( );
}

static inline void LCD_E_Start( const LCD_1602 * const lcd )
{
    PORT_PinWrite( lcd->e, true );
    //    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
}

static inline void LCD_E_End( const LCD_1602 * const lcd )
{
    PORT_PinWrite( lcd->e, false );
    //    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
}


/***************** Local Variable(s) ***************/
// TODO consolidate these into the instance 
static AddressCounter currentAddress;
static LCM_DISPLAY_LINE_CONTROL currentLineControlSetting = LCM_NO_LINE_CONTROL_SET;
static LCM_RAM_TYPE lastRAMAddressTypeSet;

/**************** Function Definitions *************/
void LCM1602_ClearDisplay( const LCD_1602 * const lcd )
{
    LCD_RS_Instruction( lcd );
    LCD_RW_Write( lcd );
    LCD_E_Start( lcd );

    PORT_PinWrite( lcd->db7, false );
    PORT_PinWrite( lcd->db6, false );
    PORT_PinWrite( lcd->db5, false );
    PORT_PinWrite( lcd->db4, false );
    PORT_PinWrite( lcd->db3, false );
    PORT_PinWrite( lcd->db2, false );
    PORT_PinWrite( lcd->db1, false );
    PORT_PinWrite( lcd->db0, true );

    LCD_E_End( lcd );
    //    // __delay_ms( 2 ); // Additional delay to allow instruction to complete. TODO
    return;
}

void LCM1602_ReturnHome( const LCD_1602 * const lcd )
{
    LCD_RS_Instruction( lcd );
    LCD_RW_Write( lcd );
    LCD_E_Start( lcd );

    PORT_PinWrite( lcd->db7, false );
    PORT_PinWrite( lcd->db6, false );
    PORT_PinWrite( lcd->db5, false );
    PORT_PinWrite( lcd->db4, false );
    PORT_PinWrite( lcd->db3, false );
    PORT_PinWrite( lcd->db2, false );
    PORT_PinWrite( lcd->db1, true );
    PORT_PinWrite( lcd->db0, true ); // This data bit is a "don't care" bit
    LCD_E_End( lcd );
    // __delay_ms( 2 ); // Additional delay to allow instruction to complete. TODO
    return;
}

void LCM1602_EntryModeSet( const LCD_1602 * const lcd,
                           const LCM_ENTRY_MODE_RAM ramSetting,
                           const LCM_ENTRY_MODE_SHIFT shiftSetting )
{
    LCD_RS_Instruction( lcd );
    LCD_RW_Write( lcd );
    LCD_E_Start( lcd );

    PORT_PinWrite( lcd->db7, false );
    PORT_PinWrite( lcd->db6, false );
    PORT_PinWrite( lcd->db5, false );
    PORT_PinWrite( lcd->db4, false );
    PORT_PinWrite( lcd->db3, false );
    PORT_PinWrite( lcd->db2, true );

    if( LCM_ENTRY_LEFT == ramSetting )
    {
        PORT_PinWrite( lcd->db1, false );
    }
    else
    {
        PORT_PinWrite( lcd->db1, true );
    }

    if( LCM_DDR_RW_SHIFT_ENABLED == shiftSetting )
    {
        PORT_PinWrite( lcd->db0, true );
    }
    else
    {
        PORT_PinWrite( lcd->db0, false );
    }
    LCD_E_End( lcd );
    // __delay_us( 40 );
    return;
}

void LCM1602_DisplayControl( const LCD_1602 * const lcd,
                             const LCM_DISPLAY_CONTROL disp,
                             const LCM_CURSOR_VISIBILITY vis,
                             const LCM_CURSOR_BLINK blink )
{
    LCD_RS_Instruction( lcd );
    LCD_RW_Write( lcd );
    LCD_E_Start( lcd );

    PORT_PinWrite( lcd->db7, false );
    PORT_PinWrite( lcd->db6, false );
    PORT_PinWrite( lcd->db5, false );
    PORT_PinWrite( lcd->db4, false );
    PORT_PinWrite( lcd->db3, true );

    ( LCM_DISPLAY_ON == disp ) ? PORT_PinWrite( lcd->db2, true ) : PORT_PinWrite( lcd->db2, false );

    ( LCM_CURSOR_VISIBLE == vis ) ? PORT_PinWrite( lcd->db1, true ) : PORT_PinWrite( lcd->db1, false );
    ( LCM_BLINK_ON == blink ) ? PORT_PinWrite( lcd->db0, true ) : PORT_PinWrite( lcd->db0, false );
    LCD_E_End( lcd );
    // __delay_us( 40 );
    return;
}

void LCM1602_Shift( const LCD_1602 * const lcd,
                    const LCM_CURSOR_SHIFT_OBJ obj,
                    const LCM_SHIFT_DIRECTION dir )
{
    LCD_RS_Instruction( lcd );
    LCD_RW_Write( lcd );
    LCD_E_Start( lcd );

    PORT_PinWrite( lcd->db7, false );
    PORT_PinWrite( lcd->db6, false );
    PORT_PinWrite( lcd->db5, false );
    PORT_PinWrite( lcd->db4, false );

    if( LCM_SHIFT_CURSOR == obj )
    {
        PORT_PinWrite( lcd->db3, false );
    }
    else
    {
        PORT_PinWrite( lcd->db3, true );
    }

    if( LCM_SHIFT_LEFT == dir )
    {
        PORT_PinWrite( lcd->db2, false );
    }
    else
    {
        PORT_PinWrite( lcd->db2, true );
    }
    /* Bits 1 and 0 are don't care*/
    PORT_PinWrite( lcd->db1, false );
    PORT_PinWrite( lcd->db0, false );
    LCD_E_End( lcd );
    // __delay_us( 40 );
    return;
}

void LCM1602_FunctionSet( const LCD_1602 * const lcd,
                          const LCM_DATA_INTERFACE interface,
                          const LCM_DISPLAY_LINE_CONTROL lc,
                          const LCM_DISPLAY_DOT_TYPE dt )
{
    LCD_RS_Instruction( lcd );
    LCD_RW_Write( lcd );
    LCD_E_Start( lcd );

    PORT_PinWrite( lcd->db7, false );
    PORT_PinWrite( lcd->db6, false );
    PORT_PinWrite( lcd->db5, true );

    if( LCM_4_BIT_INTERFACE == interface )
    {
        PORT_PinWrite( lcd->db4, false );
    }
    else
    {
        PORT_PinWrite( lcd->db4, true );
    }

    if( LCM_1_LINE_DISPLAY == lc )
    {
        PORT_PinWrite( lcd->db3, false );
    }
    else
    {
        PORT_PinWrite( lcd->db3, true );
    }

    if( LCM_5x8_DOTS == dt )
    {
        PORT_PinWrite( lcd->db2, false );
    }
    else
    {
        PORT_PinWrite( lcd->db2, true );
    }
    /* Don't care bits */
    PORT_PinWrite( lcd->db1, false );
    PORT_PinWrite( lcd->db0, false );
    LCD_E_End( lcd );
    // __delay_us( 40 );
    return;
}

bool LCM1602_CGRAM_Addr_Set( const LCD_1602 * const lcd,
                             const uint8_t addr )
{
    if( addr > MAX_CGRAM_ADDR )
    {
        return false;
    }

    LCD_RS_Instruction( lcd );
    LCD_RW_Write( lcd );
    LCD_E_Start( lcd );

    PORT_PinWrite( lcd->db7, false );
    PORT_PinWrite( lcd->db6, true );

    if( addr & 0b00100000 )
    {
        PORT_PinWrite( lcd->db5, true );
    }
    else
    {
        PORT_PinWrite( lcd->db5, false );
    }

    if( addr & 0b00010000 )
    {
        PORT_PinWrite( lcd->db4, true );
    }
    else
    {
        PORT_PinWrite( lcd->db4, false );
    }

    if( addr & 0b0001000 )
    {
        PORT_PinWrite( lcd->db3, true );
    }
    else
    {
        PORT_PinWrite( lcd->db3, false );
    }

    if( addr & 0b000000100 )
    {
        PORT_PinWrite( lcd->db2, true );
    }
    else
    {
        PORT_PinWrite( lcd->db2, false );
    }

    if( addr & 0b00000010 )
    {
        PORT_PinWrite( lcd->db1, true );
    }
    else
    {
        PORT_PinWrite( lcd->db1, false );
    }

    if( addr & 0b00000001 )
    {
        PORT_PinWrite( lcd->db0, true );
    }
    else
    {
        PORT_PinWrite( lcd->db0, false );
    }
    LCD_E_End( lcd );
    // __delay_us( 40 );
    return true;
}

bool LCM1602_DDRAM_Addr_Set( const LCD_1602 * const lcd,
                             const uint8_t addr )
{
    // TODO address validation 

    LCD_RS_Instruction( lcd );
    LCD_RW_Write( lcd );
    LCD_E_Start( lcd );

    PORT_PinWrite( lcd->db7, true );

    if( addr & 0b01000000 )
    {
        PORT_PinWrite( lcd->db6, true );
    }
    else
    {
        PORT_PinWrite( lcd->db6, false );
    }
    if( addr & 0b00100000 )
    {
        PORT_PinWrite( lcd->db5, true );
    }
    else
    {
        PORT_PinWrite( lcd->db5, false );
    }
    if( addr & 0b00010000 )
    {
        PORT_PinWrite( lcd->db4, true );
    }
    else
    {
        PORT_PinWrite( lcd->db4, false );
    }
    if( addr & 0b0001000 )
    {
        PORT_PinWrite( lcd->db3, true );
    }
    else
    {
        PORT_PinWrite( lcd->db3, false );
    }
    if( addr & 0b000000100 )
    {
        PORT_PinWrite( lcd->db2, true );
    }
    else
    {
        PORT_PinWrite( lcd->db2, false );
    }
    if( addr & 0b00000010 )
    {
        PORT_PinWrite( lcd->db1, true );
    }
    else
    {
        PORT_PinWrite( lcd->db1, false );
    }
    if( addr & 0b00000001 )
    {
        PORT_PinWrite( lcd->db0, true );
    }
    else
    {
        PORT_PinWrite( lcd->db0, false );
    }
    LCD_E_End( lcd );
    // __delay_us( 40 );
    return true;
}

bool LCM1602_ReadFlagsAndAddr( const LCD_1602 * const lcd,
                               uint8_t * const addr )
{
    uint8_t currentReadByte = 0x00u;
    LCD_RS_Instruction( lcd );
    LCD_RW_Read( lcd );
    LCD_E_Start( lcd );
    bool isProcessorBusy = ( PORT_PinRead( lcd->db7 ) ) ? true : false;

    if( PORT_PinRead( lcd->db6 ) )
    {
        currentReadByte |= 0b01000000;
    }
    if( PORT_PinRead( lcd->db5 ) )
    {
        currentReadByte |= 0b00100000;
    }
    if( PORT_PinRead( lcd->db4 ) )
    {
        currentReadByte |= 0b00010000;
    }
    if( PORT_PinRead( lcd->db3 ) )
    {
        currentReadByte |= 0b00001000;
    }
    if( PORT_PinRead( lcd->db2 ) )
    {
        currentReadByte |= 0b00000100;
    }
    if( PORT_PinRead( lcd->db1 ) )
    {
        currentReadByte |= 0b00000010;
    }
    if( PORT_PinRead( lcd->db0 ) )
    {
        currentReadByte |= 0b00000001;
    }
    LCD_E_End( lcd );
    // __delay_us( 40 );
    *addr = currentReadByte;
    return isProcessorBusy;
}

uint8_t LCM1602_ReadRAM( const LCD_1602 * const lcd )
{
    LCD_RS_Data( lcd );
    LCD_RW_Read( lcd );
    LCD_E_Start( lcd );
    uint8_t currentReadByte = 0;

    ( PORT_PinRead( lcd->db7 ) ) ? currentReadByte |= 0b10000000 : Nop( );
    ( PORT_PinRead( lcd->db6 ) ) ? currentReadByte |= 0b01000000 : Nop( );
    ( PORT_PinRead( lcd->db5 ) ) ? currentReadByte |= 0b00100000 : Nop( );
    ( PORT_PinRead( lcd->db4 ) ) ? currentReadByte |= 0b00010000 : Nop( );
    ( PORT_PinRead( lcd->db3 ) ) ? currentReadByte |= 0b00001000 : Nop( );
    ( PORT_PinRead( lcd->db2 ) ) ? currentReadByte |= 0b00000100 : Nop( );
    ( PORT_PinRead( lcd->db1 ) ) ? currentReadByte |= 0b00000010 : Nop( );
    ( PORT_PinRead( lcd->db0 ) ) ? currentReadByte |= 0b00000001 : Nop( );
    LCD_E_End( lcd );
    // __delay_us( 40 );
    return currentReadByte;
}

void LCM1602_WriteRAM( const LCD_1602 * const lcd,
                       const uint8_t value )
{
    LCD_RS_Data( lcd );
    LCD_RW_Write( lcd );
    LCD_E_Start( lcd );

    ( value & 0b10000000 ) ? PORT_PinWrite( lcd->db7, true ) : PORT_PinWrite( lcd->db7, false );
    ( value & 0b01000000 ) ? PORT_PinWrite( lcd->db6, true ) : PORT_PinWrite( lcd->db6, false );
    ( value & 0b00100000 ) ? PORT_PinWrite( lcd->db5, true ) : PORT_PinWrite( lcd->db5, false );
    ( value & 0b00010000 ) ? PORT_PinWrite( lcd->db4, true ) : PORT_PinWrite( lcd->db4, false );
    ( value & 0b00001000 ) ? PORT_PinWrite( lcd->db3, true ) : PORT_PinWrite( lcd->db3, false );
    ( value & 0b00000100 ) ? PORT_PinWrite( lcd->db2, true ) : PORT_PinWrite( lcd->db2, false );
    ( value & 0b00000010 ) ? PORT_PinWrite( lcd->db1, true ) : PORT_PinWrite( lcd->db1, false );
    ( value & 0b00000001 ) ? PORT_PinWrite( lcd->db0, true ) : PORT_PinWrite( lcd->db0, false );
    LCD_E_End( lcd );
    // __delay_us( 40 );
    return;
}

/* end 1602_LCD_HAL */