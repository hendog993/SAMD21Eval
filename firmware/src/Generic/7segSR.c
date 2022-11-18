/* Filename: 
 *      7segSR.c
 *
 * Date: 
 *      17 November 2022 
 * 
 * Author:  
 *      Henry Gilbert
 * 
 * Description: 
 *      7 segment display driver for instance use of display. Includes methods to write floating point values 
 *      as well as integer values. 
 * 
 *      With personal module, activating a digit will open a sink to GND using a transistor, so all digits are  
 *      active high to the logic side. 
 * 
 *      
 */

/*********************** Included File(s) ********************************/
#include "7segSR.h"
#include <stdlib.h>
#include <math.h>
#include "sn74hc595n.h"
#include <xc.h>

/*********************** Type Definition(s) ******************************/




/*********************** Macro Definition (s) ****************************/
#define MAX_FLOAT_SIG_DIGITS 4u
#define NUM_DIGITS_ENCODED 10u
#define DECIMAL_POINT_MASK 0x01u
#define MAX_INT_TO_DISPLAY 9999u
#define MAX_DIGITS_TO_DISPLAY 4


/********************** Local Function Prototype(s) **********************/
static void WriteSevenSegmentValue( const FourDigit7SegDisplay * const ss,
                                    const uint8_t * const writeArray );


/************************** Local Variable(s) ****************************/

/* Digit encodings - order is A-B-C-D-E-F-G-dp 
 * Dp is always zero, since a decimal point is assigned at runtime. 
 */
static const uint8_t digitEncodings[10] = {
    0b11111100, // zero
    0b01100000, // one 
    0b11011010, // two
    0b11110010, // three
    0b01100110, // four
    0b10110110, // five
    0b10111110, // six
    0b11100000, // seven
    0b11111110, // eight
    0b11110110 // nine 
};

/************************** Functions ************************************/
bool SevenSeg_Initialize( FourDigit7SegDisplay * const ss,
                          SN74HC595N * const sr,
                          const PORT_PIN d1,
                          const PORT_PIN d2,
                          const PORT_PIN d3,
                          const PORT_PIN d4,
                          const POWER_CONFIGURATION pwrCfg,
                          const uint8_t floatPrecision )
{
    /* Validate input parameters */
    if ((NULL == ss) ||
        (NULL == sr) ||
        (floatPrecision > MAX_FLOAT_SIG_DIGITS) ||
        (d1 == d2) ||
        (d2 == d3) ||
        (d3 == d4) ||
        (d4 == d1))
    {
        return false;
    }

    /* Validate input data pins for shift register matches */
    if ((d1 == sr->rclk) ||
        (d1 == sr->sclk) ||
        (d1 == sr->ser))
    {
        return false;
    }
    if ((d2 == sr->rclk) ||
        (d2 == sr->sclk) ||
        (d2 == sr->ser))
    {
        return false;
    }
    if ((d3 == sr->rclk) ||
        (d3 == sr->sclk) ||
        (d3 == sr->ser))
    {
        return false;
    }
    if ((d4 == sr->rclk) ||
        (d4 == sr->sclk) ||
        (d4 == sr->ser))
    {
        return false;
    }

    ss->sr = sr;
    ss->d1 = d1;
    ss->d2 = d2;
    ss->d3 = d3;
    ss->d4 = d4;
    ss->pwrCfg = pwrCfg;
    ss->numFloatSigDigits = floatPrecision;
    return true;
}

bool SevenSeg_WriteInteger( const FourDigit7SegDisplay * const ss,
                            uint16_t value )
{
    if ((NULL == ss) ||
        (value > MAX_INT_TO_DISPLAY))
    {
        return false;
    }

    // Decompose floating point number 
    uint8_t numDigitsInNumber = log10( value ) + 1; // Digit cant be zero because func would have exited 
    uint8_t writeArray[MAX_DIGITS_TO_DISPLAY] = { 0, 0, 0, 0 }; // Must zero the array upon restart - verify this is required. 
    uint8_t writeIndex = 3; // Start the write index at the end of the write array. 

    while (numDigitsInNumber > 0)
    {
        writeArray[writeIndex] = (value % 10);
        value /= 10;
        writeIndex--;
        numDigitsInNumber--;
    }
    WriteSevenSegmentValue( ss, writeArray );
    return true;
}

// TODO write screen will be a generic function so that floats cal use the same. 


static void WriteSevenSegmentValue( const FourDigit7SegDisplay * const ss,
                                    const uint8_t * const writeArray )
{
    // todo add checks 
    size_t digitCounter;
    uint8_t sval;
    /* For every digit in the digit array (4) */
    PORT_PIN digitList[4] = {ss->d1, ss->d2, ss->d3, ss->d4};
    for (digitCounter = 0; digitCounter < MAX_DIGITS_TO_DISPLAY; digitCounter++)
    {
        /* Extract the value from the write array corresponding to the segment decoded value */
        sval = digitEncodings[writeArray[digitCounter]]; // nth digit from input array. Extracts value from global write table 
        SN74HC595N_WriteByteLSB( ss->sr,
                                 sval );
        PORT_PinWrite( digitList[digitCounter], true );
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORT_PinWrite( digitList[digitCounter], false );
    }
}






/* End 7segSR.c source file */