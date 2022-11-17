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
#include "../src/config/default/peripheral/port/plib_port.h"


/*********************** Type Definition(s) ******************************/

/* Enum: 
 *      POWER_CONFIGURATION
 * 
 * Description: 
 *      Delineates hardware configuration for common anode (active high) vs common cathode (active low).  
 *      Active high means 
 */
typedef enum
{
    COMMON_ANODE,
    COMMON_CATHODE
} POWER_CONFIGURATION;

/* Struct:
 *      FourDigit7SegDisplay
 * 
 * Description:
 *      Contains elements for seven segment instance. 
 * 
 */
typedef struct
{
    POWER_CONFIGURATION pwrCfg;
    uint8_t numFloatSigDigits;
    PORT_PIN d1;
    PORT_PIN d2;
    PORT_PIN d3;
    PORT_PIN d4;
    SN74HC595N * sr;
} FourDigit7SegDisplay;




/*********************** Function Prototype(s) ***************************/

/* Function:
 *      SevenSeg_Initialize
 * 
 * Description:
 *      Initializes an instance of a seven segment display. Writes to an existing instance pointer. Requires
 *      an already created instance of a shift register. 
 * 
 */
bool SevenSeg_Initialize( FourDigit7SegDisplay * const ss,
                          SN74HC595N * const sr,
                          const PORT_PIN d1,
                          const PORT_PIN d2,
                          const PORT_PIN d3,
                          const PORT_PIN d4,
                          const POWER_CONFIGURATION pwrCfg,
                          const uint8_t floatPrecision );


/* Function:
 *      SevenSeg_WriteInteger
 * 
 * Description:
 *      Writes an integer to the seven segment display 
 */
bool SevenSeg_WriteInteger(const FourDigit7SegDisplay * const ss,
        uint16_t value);



bool SevenSeg_WriteFloat(const FourDigit7SegDisplay * const ss,
        const float value);



bool SevenSeg_Clear(const FourDigit7SegDisplay * const ss);











#endif 

/* End 7segSR.h header file */