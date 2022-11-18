/* Filename: sn74hc595n.c
 *
 * Date:  18 October 2022
 * 
 * Author:  Henry Gilbert
 * 
 * Description: Hardware abstraction layer for "bitbang" shift register driver. Basic SIPO driver 
 *      that controls pins generically based on Harmony configuration library. This configuration 
 *      only uses 3 pins, SCLK, RCLK, and SER. SCLR is not used, nor is output enable. 
 * 
 * 
 */

/*********************** Included File(s) ********************************/
#include "sn74hc595n.h"
#include <stddef.h>
#include <xc.h>
#include <stdint.h>



/*********************** Macro Definition(s) *****************************/
#define NUM_BITS_IN_BYTE 8u



/*********************** Function Prototype(s) ***************************/
static inline uint8_t ReverseBitsOfByte( uint8_t byte );




/************************** Functions ************************************/
bool SN74HC595N_Initialize( SN74HC595N * const sr,
                            const PORT_PIN serialClock,
                            const PORT_PIN registerClock,
                            const PORT_PIN serialData )
{
    if ((NULL == sr) ||
        (serialClock == registerClock) ||
        (registerClock == serialData) ||
        (serialClock == serialData))
    {
        return false;
    }

    sr->sclk = serialClock;
    sr->rclk = registerClock;
    sr->ser = serialData;
    return true;
}

void SN74HC595N_WriteByteMSB( const SN74HC595N * const sr,
                              const uint8_t byte )
{
    size_t i;
    
    for (i = 0; i < NUM_BITS_IN_BYTE; i++)
    {
        PORT_PinWrite( sr->ser, ((byte << i) & 0x80u) );
        PORT_PinWrite( sr->sclk, true );
        PORT_PinWrite( sr->sclk, false );
    }
    
    PORT_PinWrite( sr->rclk, true );
    PORT_PinWrite( sr->rclk, false );
}

void SN74HC595N_WriteByteLSB( const SN74HC595N * const sr,
                              const uint8_t byte )
{
    const uint8_t reversedData = ReverseBitsOfByte( byte );
    SN74HC595N_WriteByteMSB( sr, reversedData );
    return;
}

/* Returns the reversed byte of input data */
static inline uint8_t ReverseBitsOfByte( uint8_t data )
{
    data = (uint8_t) ((data & 0xF0u) >> 4 | (data & 0x0Fu) << 4);
    data = (uint8_t) ((data & 0xCCu) >> 2 | (data & 0x33u) << 2);
    data = (uint8_t) ((data & 0xAAu) >> 1 | (data & 0x55u) << 1);
    return data;
}

/* End sn74hc595n.c source file */
