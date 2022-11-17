///* Filename: sn74hc595n.c
// *
// * Date:  18 October 2022
// * 
// * Author:  Henry Gilbert
// * 
// * Description: Hardware abstraction layer for "bitbang" shift register driver. Basic SIPO driver 
// *      that controls pins generically based on Harmony configuration library. This configuration 
// *      only uses 3 pins, SCLK, RCLK, and SER. SCLR is not used, nor is output enable. 
// *
// * 
// * 
// */
//
///*********************** Included File(s) ********************************/
//#include "sn74hc595n.h"
//#include <stddef.h>
//#include "../../A2E/A2E.X/mcc_generated_files/pin_manager.h"
//#include <xc.h>
//
//
///*********************** Macro Definition(s) *****************************/
//#define NUM_BITS_IN_BYTE 8u
//
//#ifndef _XTAL_FREQ
//#define _XTAL_FREQ 64000000
//#endif
//
//
//
//
//
//
///*********************** Function Prototype(s) ***************************/
//static inline uint8_t ReverseBitsOfByte( uint8_t byte );
//
///************************** Functions ************************************/
//
//
//void SN74HC595N_WriteByteMSB( const uint8_t byte )
//{
//    size_t i;
//    for( i = 0; i < NUM_BITS_IN_BYTE; i++ )
//    {
//        if( ( byte << i ) & 0x80u )
//        {
//            SR_SetHigh( );
//        }
//        else
//        {
//            SR_SetLow( );
//        }
//        SCLK_SetHigh( );
//        SCLK_SetLow( );
//    }
//    RCLK_SetHigh( );
//    RCLK_SetLow( );
//}
//
//void SN74HC595N_WriteByteLSB( const uint8_t byte )
//{
//    uint8_t reversedData = ReverseBitsOfByte( byte );
//    SN74HC595N_WriteByteMSB( reversedData );
//    return;
//}
//
///* Returns the reversed byte of input data */
//static inline uint8_t ReverseBitsOfByte( uint8_t data )
//{
//    data = (uint8_t) ( ( data & 0xF0u ) >> 4 | ( data & 0x0Fu ) << 4 );
//    data = (uint8_t) ( ( data & 0xCCu ) >> 2 | ( data & 0x33u ) << 2 );
//    data = (uint8_t) ( ( data & 0xAAu ) >> 1 | ( data & 0x55u ) << 1 );
//    return data;
//}
//
//
//
//
//
//
//
//
//
//
///* End sn74hc595n.c source file */