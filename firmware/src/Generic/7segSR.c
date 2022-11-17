///* Filename: 7segSR.c
// *
// * Date: 18 October 2022
// * 
// * Author: Henry Gilbert
// * 
// * Description: Driver for using the 7 segment display with shift register attached.
// * 
// * Implementation uses 7 total pins, 4 to control digits, 3 to control attached shift register.
// * The SN74HC595N shift register does not use a separate module for this implementation, as they
// * are linked together to form one single unit. 
// * 
// * 
// */
//
///*********************** Included File(s) ********************************/
//#include "7segSR.h"
//#include <math.h>
//#include <stddef.h>
//#include "../../A2E/A2E.X/mcc_generated_files/pin_manager.h"
//
//
///*********************** Macro Definition (s) ****************************/
//#define NUM_DIGIT_ENCODINGS 10u // 0-9
//#define MAX_INT_TO_WRITE 9999u
//#define NUM_DIGITS 4u
//#define DECIMAL_POINT_MASK 0x01u
//#define MAX_DECIMAL_POINT_INDEX 4
//
///*********************** Local Variables ******************************/
//
///* Digit encoding array. */
//static const uint8_t digitEncodings[NUM_DIGIT_ENCODINGS] = {
//    0b11111100, // zero 
//    0b01100000, // one 
//    0b11011010, // two  
//    0b11110010, // three
//    0b01100110, // four 
//    0b10110110, // five 
//    0b10111110, // six 
//    0b11100000, // seven
//    0b11111110, // eight 
//    0b11100110, // nine 
//};
//
//
//
//
//
//
//
///*********************** Function Prototype(s) ***************************/
//static void SevenSeg_WriteDigits( const FourDigit7SegDisplay * const ss,
//                                  const uint8_t * const digits,
//                                  const size_t decimalPointDigit );
//static void SevenSeg_DisplayFaultOut( const FourDigit7SegDisplay * const ss );
//
//
//static inline void writeD1( bool val );
//static inline void writeD2( bool val );
//static inline void writeD3( bool val );
//static inline void writeD4( bool val );
//
//void (*func[4] )( bool val ) = {
//    writeD1,
//    writeD2,
//    writeD3,
//    writeD4
//};
//
///************************** Functions ************************************/
//
//bool SevenSeg_WriteInteger( const FourDigit7SegDisplay * const ss,
//                            uint16_t value )
//{
//    if( value > MAX_INT_TO_WRITE )
//    {
//        SevenSeg_DisplayFaultOut( ss );
//        return false;
//    }
//
//    /* Decompose the prospective value into corresponding digits  */
//    uint8_t numDigitsInNumber = log10( value ) + 1u; // Digit cant be zero because func would have exited 
//    uint8_t writeArray[NUM_DIGITS] = {0u, 0u, 0u, 0u}; // Must zero the array upon restart - verify this is required. 
//    size_t writeIndex = 3; // Start the write index at the end of the write array. 
//
//    while( numDigitsInNumber )
//    {
//        writeArray[writeIndex] = ( value % 10u );
//        value /= 10u;
//        writeIndex--;
//        numDigitsInNumber--;
//    }
//    SevenSeg_WriteDigits( ss, writeArray, 0 );
//    return true;
//}
//
///* Convert the integer portion as uint. 
// * Store the remaining float portion as a uint.
// * 
// * Create the writeArray starting at index 0, as opposed to the uint version where it starts at the end digit.  
// *  */
//bool SevenSeg_WriteFloat( const FourDigit7SegDisplay * const ss,
//                          const float value )
//{
//    /* Count the number of significant digits in the input floating point number. */
//    float tempValueForFloatingPrecision = value; // save a copy of the input floating point vlaue 
//    uint32_t valueAsUint = (uint32_t) ( value );
//
//    uint8_t numWholeDigitsInValue = log10( valueAsUint ) + 1u;
//
//    if( numWholeDigitsInValue > ss->numFloatSigDigits )
//    {
//        return false; // Error, written value was too large. 
//    }
//
//    // NumWholedigits could be less than expected. 
//    uint8_t numExtraDecimalPrecision = ss->numFloatSigDigits - numWholeDigitsInValue; // Will be zero if whole digits matches config 
//
//    uint8_t writeArray[NUM_DIGITS] = {0u, 0u, 0u, 0u}; // Must zero the array upon restart - verify this is required. 
//
//    size_t writeIndex = 4 - numExtraDecimalPrecision; // X number of extra precision means offset the start write index by that amount
//    while( numWholeDigitsInValue )
//    {
//        writeArray[writeIndex] = valueAsUint % 10;
//        valueAsUint /= 10;
//        writeIndex--;
//        numWholeDigitsInValue--;
//    }
//
//    tempValueForFloatingPrecision *= numExtraDecimalPrecision; // UInt version of floating point values
//
//    writeIndex = numExtraDecimalPrecision;
//    while( writeIndex )
//    {
//        writeArray[writeIndex] = valueAsUint & 10;
//        valueAsUint /= 10;
//        writeIndex--;
//    }
//    SevenSeg_WriteDigits( ss, writeArray, numExtraDecimalPrecision );
//    return true;
//
//    // Create the final decimal portion based on number of fewer digits. Update the decimal position. 
//
//}
//
//bool SevenSeg_Clear( const FourDigit7SegDisplay * const ss )
//{
//    return true;
//}
//
///* SevenSeg_WriteDigits
// * 
// * Writes the contents of the seven segment display based on input digit array. A digit 
// * array will have already been written to, since this static function can only be called
// * by the writeFloat and writeInteger functions.  
// * 
// * decimalPointDigit is input for floats only. If 0, ignore (ints will always ignore). 
// * For floats, if digit 1 uses the decimal point, input 1. Maximum allowed is 4. 
// * 
// */
//static void SevenSeg_WriteDigits( const FourDigit7SegDisplay * const ss,
//                                  const uint8_t * const digits,
//                                  const size_t decimalPointDigit )
//{
//    if( ( NULL == digits ) ||
//        ( decimalPointDigit > MAX_DECIMAL_POINT_INDEX ) )
//    {
//        return;
//    }
//
//    size_t i;
//    uint8_t tempByte;
//
//    for( i = 0; i < MAX_DECIMAL_POINT_INDEX; i++ )
//    {
//        tempByte = digitEncodings[digits[i]];
//
//        /* iteration counter holds the index starting at 1. If the input decimal point digit equals the iteration 
//         * counter + 1, set the decimal point to write. 
//         */
//        // TODO this won't work??
//        if( decimalPointDigit == ( i + 1 ) )
//        {
//            tempByte |= DECIMAL_POINT_MASK;
//        }
//        SN74HC595N_WriteByteMSB( digits[i] );
//
//        if( COMMON_CATHODE == ss->pwrCfg )
//        {
//            func[i]( false );
//            func[i]( true );
//        }
//        else
//        {
//            func[i]( true );
//            func[i]( false );
//        }
//
//
//        // Enable respective digit 
//
//    }
//}
//
//static inline void writeD1( bool val )
//{
//    if( val )
//    {
//        D1_SetHigh( );
//    }
//    else
//    {
//        D1_SetLow( );
//    }
//    return;
//}
//
//static inline void writeD2( bool val )
//{
//    if( val )
//    {
//        D2_SetHigh( );
//    }
//    else
//    {
//        D2_SetLow( );
//    }
//    return;
//}
//
//static inline void writeD3( bool val )
//{
//    if( val )
//    {
//        D3_SetHigh( );
//    }
//    else
//    {
//        D3_SetLow( );
//    }
//    return;
//}
//
//static inline void writeD4( bool val )
//{
//    if( val )
//    {
//        D4_SetHigh( );
//    }
//    else
//    {
//        D4_SetLow( );
//    }
//    return;
//}
//
//
////
////static void SevenSeg_DecomposeInputValueInto4ByteArray(const FourDigit7SegDisplay * const ss ,
////                                                       uint16_t value,
////                                                       uint8_t * targetArray)
////{
////    
////    
////    
////}
//
//static void SevenSeg_DisplayFaultOut( const FourDigit7SegDisplay * const ss )
//{
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
//
//
//
//
///* End 7segSR.c source file */