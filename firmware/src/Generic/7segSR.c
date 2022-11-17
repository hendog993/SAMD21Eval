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
 */

/*********************** Included File(s) ********************************/
#include "7segSR.h"
#include <stdlib.h>
#include <math.h>

/*********************** Type Definition(s) ******************************/




/*********************** Macro Definition (s) ****************************/
#define MAX_FLOAT_SIG_DIGITS 4u
#define NUM_DIGITS_ENCODED 10u
#define DECIMAL_POINT_MASK 0x01u
#define MAX_INT_TO_DISPLAY 9999u



/********************** Local Function Prototype(s) **********************/
static void SevenSeg_WriteIndividualDigit( const FourDigit7SegDisplay * const ss,
                                           uint8_t value );


/************************** Local Variable(s) ****************************/

/* Digit encodings - order is A-B-C-D-E-F-G-dp 
 * Dp is always zero, since a decimal point is assigned at runtime. 
 */
static const uint8_t digitEncodings[NUM_DIGITS_ENCODED] = {
    0b11111100, // zero
    0b01100000, // one 
    0b11011010, // two
    0b11110010, // three
    0b01100110, // four
    0b10010110, // five
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

    /* Determine number of digits */
    size_t numDigits = log( value ) + 1;
    uint8_t * digitArray = malloc( numDigits );

    /* Extract digits from input (most significant digit first) and place into digit array. In this while loop, the 
     * var numDigits serves as both an array indexer and a while loop counter. The nth digit of the input value is placed
     * at the nth position of the dynamically allcoated array. */
    while (numDigits)
    {
        digitArray[numDigits] = value & 10;
        value /= 10;
        numDigits--;
    }

    /* Write individual digits and enable shift register */
    
    




    free( digitArray );
    return true;
}

static void SevenSeg_WriteIndividualDigit( const FourDigit7SegDisplay * const ss,
                                           uint8_t value )
{
    const uint8_t digitToWrite = digitEncodings[value];
    SN74HC595N_WriteByteMSB(ss->sr, digitToWrite);
    

}







/* End 7segSR.c source file */