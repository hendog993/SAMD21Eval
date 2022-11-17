///* Filename: ws2812b.h 
// *
// * Date: 9 September 2022
// * 
// * Author: Henry Gilbert 
// * 
// * Description: Public interface to ws2812b hardware abstraction layer. Includes
// *      primary pixel type definition, along with driver specific routines. 
// */
//
//#ifndef WS2812B_H
//#define WS2812B_H
//
//
///********************* Included File(s) ********************/
//#include <stdint.h>
//#include <stdlib.h>
//#include <stdbool.h>
//
//
///******************* Macro Definition(s) *******************/
//#define WS2812B_RED_INDEX 0u
//#define WS2812B_GREEN_INDEX 1u
//#define WS2812B_BLUE_INDEX 2u
//#define NUM_BYTES_IN_PIXEL 3u
//
//
//#ifndef _XTAL_FREQ
//#define _XTAL_FREQ 32000000u
//#endif 
//
//
///****************** Type Definition(s) *********************/
//
///* This order red->green->blue MUST be maintained for proper functionality. 
// * Also note that this struct doesn't have to be __packed__, as the xc8 
// * compiler is byte addressable in memory. However, for compiler cross compatability, 
// * this issue must be addressed. 
// */
//typedef struct
//{
//    uint8_t red;
//    uint8_t green;
//    uint8_t blue;
//} ws2812bPixel;
//
//typedef struct
//{
//    ws2812bPixel * pixelBuffer;
//    size_t numPixels;
//} ws2812bArray;
//
///********************* Function Prototype(s) ****************/
//ws2812bArray WS2812b_Initialize(ws2812bPixel * pixelBuffer,
//        const size_t numElements,
//        bool * const wasSetupSuccessful);
//
//void WS2812B_Render(const ws2812bArray * const strip);
//
//void WS2812b_SetSinglePixelColor(ws2812bArray * const strip,
//        const size_t pixelIndex,
//        const uint8_t red,
//        const uint8_t green,
//        const uint8_t blue);
//
//void WS2812b_SetPixelBlockFromRGBArray(const ws2812bArray * const strip,
//        const size_t pixelStartOffset,
//        const size_t numPixelsToSet,
//        const uint8_t * const rgbArray);
//
//void WS2812b_SetPixelBlockConstantColor(const ws2812bArray * const strip,
//        const size_t pixelStartOffset,
//        const size_t numPixelsToSet,
//        const uint8_t red,
//        const uint8_t green,
//        const uint8_t blue);
//
//void WS2812b_SetStripConstantColor(ws2812bArray * const strip,
//        const uint8_t red,
//        const uint8_t green,
//        const uint8_t blue);
//
//void WS2812b_CopyPixelBufferArrayFromSource(const ws2812bArray * const strip,
//        ws2812bPixel * const pixelSource,
//        const size_t startAddress,
//        const size_t numPixelsToCopy);
//
//
//#endif