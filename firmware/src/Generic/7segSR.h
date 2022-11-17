/* Filename: 7segSR.h
 *
 * Date: 18 October 2022
 * 
 * Author: Henry Gilbert
 * 
 * Description: Public interface to the 7 segment display module. 
 * 
 * 8 Bit interface only allows 1 instance and requires hard coding the pin names. 
 */
 

#ifndef ENTER_FILENAME_H
#define ENTER_FILENAME_H



/*********************** Included File(s) ********************************/

#include "sn74hc595n.h"
#include "stdbool.h"
#include <stdint.h>

/*********************** Type Definition(s) ******************************/
typedef enum
{
    COMMON_ANODE,
    COMMON_CATHODE
} POWER_CONFIGURATION;


/* Declare an instance of a seven segment display based on an input shift register and the 
 * pins that control each digit. Power configuration depends on the device's common cathode 
 * or common anode. For floats, set numFloatSigDigits based on significant digits expected. 
 * ie, if only 1 significant digit is expected, with the remaining 3 as decimal points, 
 * set numFloatSigDigits to 1. 3 sig digits would imply 123.5, 905.3, etc. 
 */
typedef struct
{
    POWER_CONFIGURATION pwrCfg;
    uint8_t numFloatSigDigits; 
} FourDigit7SegDisplay;





/*********************** Function Prototype(s) ***************************/
bool SevenSeg_Initialize(FourDigit7SegDisplay * ss);

bool SevenSeg_WriteInteger( const FourDigit7SegDisplay * const ss,
                            uint16_t value );

bool SevenSeg_WriteFloat( const FourDigit7SegDisplay * const ss,
                          const float value );

bool SevenSeg_Clear( const FourDigit7SegDisplay * const ss );











#endif 

/* End 7segSR.h header file */