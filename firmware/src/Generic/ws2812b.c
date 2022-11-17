///* Filename: ws2812b.c
// *
// * Author: Henry Gilbert
// *
// * Date: 8 September 2022
// *
// * Description: "Bit bang" driver for ws2812b led strip. Manually controls a data pin
// *      and adds NOP instructions in for the required timing delays (listed below).
// *      Module is divided into two sections. 1. Hardware section, and 2. Pixel RGB modifiation.
// *
// * Timing requirements: 1 code = HIGH 800 ns , LOW 450 ns (All toleranced at += 150 ns)
// *                      0 code = HIGH 400 ns , LOW 850 ns (All toleranced at += 150 ns)
// * Hardware section
// *      - WS2812B_Render
// *      - WS2812B_Reset
// *      - WS2812b_RenderSinglePixel
// *
// * Pixel Modification
// *      - WS2812b_SetPixelBlock
// *      - WS2812b_SetSinglePixelColor
// *      - WS2812b_ClearPixels
// *
// * API Requirements:
// *      1. Instruction frequency must operate at no less than 8 MHz (32 MHz clock speed). This
// *         driver is written using 8MHz (125 ns) instruction speed. The macros for WRITE_ONE and
// *         WRITE_ZERO must be adjusted for anything outside of the specified tolerance.
// *      2. To avoid modifying this file, the data control pin should be controlled with macros
// *         named DATA_PIN_SetHigh() and DATA_PIN_SetLow(). These can be automatically generated
// *         using the MCC code generator, or they can be manually programmed. Either or.
// *      3. Pixel indices must start from 0!! Importrant note. 
//
// * Future design modifications: Can control the data pin using a timer interrupt at exactly the
// *      specified period desired. This bitbang method relies on the very liberal tolerance of
// *      400ns += 150 ns of the instruction frequency. Faster clock speeds would alter the driver,
// *      but a more generic driver could be implemented using timers or serial transfer peripherals.
// * 
// *      Another future addition could be to pass in the data control pin functions as generic, such
// *      that modification of the file is not necessary, and generic instances of ledArrays can be 
// *      created based on an GPIO input data pin parameter.
// * 
// *      TODO update return parameters to bool so write faults can be diagnosed. 
// *
// */
//
//
///****************** Included File(s) **********************/
//#include "ws2812b.h"
//#include <xc.h>
//#include "mcc_generated_files/pin_manager.h"
//#include "mcc_generated_files/interrupt_manager.h"
//#include <string.h>
//
///****************** Macro Definition(s) *******************/
//#define WRITE_ONE()      DATA_PIN_SetHigh(); NOP(); NOP(); NOP(); NOP(); DATA_PIN_SetLow(); NOP(); NOP();
//#define WRITE_ZERO()     DATA_PIN_SetHigh(); NOP(); DATA_PIN_SetLow(); NOP(); NOP(); NOP(); NOP(); NOP();
//
//#define NUM_BITS_IN_RGB_PIXEL 24u
//#define NUM_NOPS_FOR_50_MICROSEC 400u
//#define GREEN_LEFT_SHIFT_OFFSET 16u
//#define RED_LEFT_SHIFT_OFFSET 8u
//#define NUM_BITS_IN_BYTE 8u
//
//
//
//
//
///****************** Static Function Prototype(s) **********/
//static void WS2812b_RenderSinglePixel( const ws2812bPixel * const pixel );
//static inline void WS2812B_Reset( );
//
//
///*********************** Function(s) **********************/
//
///*
// * Function: 
// *      WS2812b_Initialize
// *
// * Description: 
// *      Links a pixel buffer pointer and number of elements into a single ws2812bArray structure. In the future,
// *      this allows multiple LED strips to be defined in a single program. This can be implemented when a generic
// *      function parameter function pass to control the data pin is written (GPIO harmony module). 
// *
// * Return: 
// *      ws2812bArray structure with a pointer to the input pixel buffer and the number of elements.
// *
// */
//ws2812bArray WS2812b_Initialize( ws2812bPixel * pxBuff,
//                                 const size_t numElements,
//                                 bool * const wasSetupSuccessful )
//{
//    ws2812bArray array;
//    array.pixelBuffer = pxBuff;
//    array.numPixels = numElements;
//    *wasSetupSuccessful = ( ( NULL == pxBuff ) || ( 0 == numElements ) ) ? false : true;
//    return array;
//}
//
///****************************************************************************************************/
///*********************                       Hardware API Section  **********************************/
///****************************************************************************************************/
//
///* Function: 
// *      WS2812B_Render
// *
// * Description: 
// *      Iteratively renders the contents of the strip's pixel buffer to the LEDs.
// *      This function takes 26 ms to render 64 pixels. This can be tremendously improved.. 
// */
//
//void WS2812B_Render( const ws2812bArray * const strip )
//{
//    if( ( NULL == strip ) ||
//        ( NULL == strip->pixelBuffer ) )
//    {
//        return;
//    }
//
//    /* Disable interrupts to avoid incomplete renders */
//    INTERRUPT_GlobalInterruptDisable( );
//    INTERRUPT_PeripheralInterruptDisable( );
//
//    size_t i;
//    ws2812bPixel * thisPixel = strip->pixelBuffer;
//    for( i = 0; i < strip->numPixels; i++ )
//    {
//        WS2812b_RenderSinglePixel( thisPixel );
//        thisPixel++;
//    }
//    // TODO reset here? 
//    INTERRUPT_GlobalInterruptEnable( );
//    INTERRUPT_PeripheralInterruptEnable( );
//    return;
//}
//
///* Function: 
// *      WS2812B_Reset
// *
// * Description: 
// *      Sets the ws2812b index back to the first LED. Accomplished via writing the data pin high, 
// *      then holds low for 50uS, then data pin high.
// *
// *      Can be further optimized by counting instruction cycles of for loop logical comparisons instead of using NOP,
// *      or by controlling the timing with a peripheral timer, or by using the xc8 builtin delay function (refer to manual). 
// */
//static inline void WS2812B_Reset( )
//{
//    DATA_PIN_SetHigh( );
//    uint16_t i;
//    DATA_PIN_SetLow( );
//    for( i = 0; i < NUM_NOPS_FOR_50_MICROSEC; i++ )
//    {
//        NOP( );
//    }
//    DATA_PIN_SetHigh( );
//}
//
///* Function: 
// *      WS2812b_RenderSinglePixel
// *
// * Description: 
// *      Renders a single pixel using the ws2812b protocol. No null pointer check because the only 
// *      function that calls this already checks.
// * 
// *      Note: As of 15 September, this function takes 25 uS to render a single pixel, when it should only take ~2 
// */
//
//static void WS2812b_RenderSinglePixel( const ws2812bPixel * const pixel )
//{
//    /* Compose all the RGB pixel bit values (24 bits total) into a single
//     * uint32. The last 8 bits of the 32 bit value will be zero (big endian).
//     * The format is: G7-G0 -> R7-R0 -> B7-B0 */
////    uint32_t thisPixel = ( ( (uint32_t) pixel->green ) << GREEN_LEFT_SHIFT_OFFSET ) |
////            ( ( (uint32_t) pixel->red ) << RED_LEFT_SHIFT_OFFSET ) |
////            ( (uint32_t) pixel->blue ); // TODO this MUST be optimized.... This is taking 7 microseconds to compute. 
////    size_t i;
////
////    for( i = 0; i < NUM_BITS_IN_RGB_PIXEL; i++ )
////    {
////        /* If the 24th bit is 1, write 1. Else, write 0. */
////        if( ( thisPixel << i ) & 0x00800000u )
////        {
////            WRITE_ONE( );
////        }
////        else
////        {
////            WRITE_ZERO( );
////        }
////    }
//
//    size_t j;
//    for( j = 0; j < NUM_BITS_IN_BYTE; j++ )
//    {
//        if( ( pixel->green << j ) & 0x80u )
//        {
//            WRITE_ONE( );
//        }
//        else
//        {
//            WRITE_ZERO( );
//        }
//    }
//    for( j = 0; j < NUM_BITS_IN_BYTE; j++ )
//    {
//        if( ( pixel->red << j ) & 0x80u )
//        {
//            WRITE_ONE( );
//        }
//        else
//        {
//            WRITE_ZERO( );
//        }
//    }
//    for( j = 0; j < NUM_BITS_IN_BYTE; j++ )
//    {
//        if( ( pixel->blue << j ) & 0x80u )
//        {
//            WRITE_ONE( );
//        }
//        else
//        {
//            WRITE_ZERO( );
//        }
//    }
//
//
//
//
//
//
//
//
//    return;
//}
//
//
///****************************************************************************************************/
///************                       RGB Pixel Modification Section  *********************************/
///****************************************************************************************************/
//
///* Function: 
// *      WS2812b_SetSinglePixelColor
// *
// * Description: 
// *      Sets a single specified pixel's RGB value to the input red, green, and blue values.
// */
//void WS2812b_SetSinglePixelColor( ws2812bArray * const strip,
//                                  const size_t pixelIndex,
//                                  const uint8_t red,
//                                  const uint8_t green,
//                                  const uint8_t blue )
//{
//    if( ( NULL == strip ) ||
//        ( NULL == strip->pixelBuffer ) ||
//        ( pixelIndex > strip->numPixels ) )
//    {
//        return;
//    }
//
//    ws2812bPixel * thisPixel = &( strip->pixelBuffer[pixelIndex] );
//
//    if( NULL == thisPixel )
//    {
//        return;
//    }
//
//    thisPixel->red = red;
//    thisPixel->green = green;
//    thisPixel->blue = blue;
//    return;
//}
//
///* Function: 
// *      WS2812b_SetPixelBlockFromRGBArray
// *
// * Description: 
// *      Sets a block of pixels RGB values based on the input RGB byte array.
// *
// * Parameter checks:
// *      1. NULL on all pointers.
// *      2. If the pixel offset + the number of pixels to set is greater than total number of pixels, errors could occur.
// *      3. The pixel start offset can't be larger than the number of pixels.
// *
// */
//void WS2812b_SetPixelBlockFromRGBArray( const ws2812bArray * const strip,
//                                        const size_t pixelStartOffset,
//                                        const size_t numPixelsToSet,
//                                        const uint8_t * const rgbArray )
//{
//    if( ( NULL == strip ) ||
//        ( NULL == strip->pixelBuffer ) ||
//        ( NULL == rgbArray ) )
//    {
//        return;
//    }
//
//    size_t finalPixelIndex = pixelStartOffset + numPixelsToSet;
//
//    if( finalPixelIndex > strip->numPixels ) // this sould be >=, since writing a block at the last pixel wouldn't make sense..
//    {
//        return;
//    }
//
//    uint8_t * startPixelAddress = &( strip->pixelBuffer[pixelStartOffset].red );
//    memcpy( startPixelAddress, rgbArray, numPixelsToSet );
//    return;
//}
//
///*
// * Function: 
// *      void WS2812b_SetPixelBlockConstantColor()
// *
// * Description: 
// *      Sets a pixel block to a constant color input via RGB bytes.
// */
//void WS2812b_SetPixelBlockConstantColor( const ws2812bArray * const strip,
//                                         const size_t pixelStartOffset,
//                                         const size_t numPixelsToSet,
//                                         const uint8_t red,
//                                         const uint8_t green,
//                                         const uint8_t blue )
//{
//    if( ( NULL == strip ) ||
//        ( NULL == strip->pixelBuffer ) )
//    {
//        return;
//    }
//
//    size_t finalPixelIndex = pixelStartOffset + numPixelsToSet;
//
//    if( finalPixelIndex > strip->numPixels )
//    {
//        return;
//    }
//
//    size_t i;
//    uint8_t rgbArray[NUM_BYTES_IN_PIXEL] = {red, green, blue};
//    uint8_t * startPixelAddress = &( strip->pixelBuffer[pixelStartOffset].red );
//    for( i = 0; i < numPixelsToSet; i++ )
//    {
//        memcpy( ( startPixelAddress + NUM_BYTES_IN_PIXEL * i ), rgbArray, NUM_BYTES_IN_PIXEL );
//    }
//    return;
//}
//
///*
// * Function:
// *      WS2812b_SetStripConstantColor()
// *
// * Description: 
// *      Sets all pixels to RGB value of input byte values. Can call this function to clear LEDs or write all high.
// *
// */
//void WS2812b_SetStripConstantColor( ws2812bArray * const strip,
//                                    const uint8_t red,
//                                    const uint8_t green,
//                                    const uint8_t blue )
//{
//    if( ( NULL == strip ) ||
//        ( NULL == strip->pixelBuffer ) )
//    {
//        return;
//    }
//    WS2812b_SetPixelBlockConstantColor( strip,
//                                        0,
//                                        strip->numPixels,
//                                        red,
//                                        green,
//                                        blue );
//    return;
//}
//
///*
// * Function: 
// *      void WS2812b_CopyPixelBufferArrayFromSource
// *
// * Description:
// *      Copies the contents of a source pixel buffer into the source pixel buffer of 
// *      an LED strip. Starts using the byte address, not the ws2812bPixel address, as 
// *      memcpy works in byte address modes. 
// * 
// * Return: 
// */
//void WS2812b_CopyPixelBufferArrayFromSource( const ws2812bArray * const strip,
//                                             ws2812bPixel * const pixelSource,
//                                             const size_t startAddress,
//                                             const size_t numPixelsToCopy )
//{
//    if( ( NULL == strip ) ||
//        ( NULL == pixelSource ) ||
//        ( startAddress + numPixelsToCopy > strip->numPixels ) )
//    {
//        return; // Error, invalid parameters 
//    }
//
//    ws2812bPixel * startPixel = &( strip->pixelBuffer[startAddress] );
//
//    /* Recall that the starting element of the ws2812bPixel structure is the red element */
//    uint8_t * dest = &( startPixel->red );
//    uint8_t * src = &( pixelSource->red );
//
//    memcpy( dest, src, numPixelsToCopy * NUM_BYTES_IN_PIXEL );
//    return;
//}
//
//
//// end
