/* Filename: sn74hc595n.h
 *
 * Date: 18 October 2022
 * 
 * Author:  Henry Gilbert
 * 
 * Description: Public interface to sn74hc595n hardware driver. 
 * 
 * 8 bit libraries only allow 1 instance...
 * 
 */

#ifndef SN74HC595N_H
#define SN74HC595N_H


/*********************** Included File(s) ********************************/
#include <stdint.h>


/*********************** Function Prototype(s) ***************************/

/* SN74HC595N_WriteByteMSB
 *
 * Writes a single byte to the output shift register, MSB first. If bit position is 1, write 1 to SER. 
 *      Else, write 0. After the bit is written to SER, pulse the SCLK for a minimum time of 20 ns. Once 
 *      the entire byte has completed writing, pulse the RCLK. 
 * 
 */
void SN74HC595N_WriteByteMSB( const uint8_t byte );


/* SN74HC595N_WriteByteLSB
 *
 * Writes a single byte to the output shift register, LSB first
 * 
 * 
 */
void SN74HC595N_WriteByteLSB(const uint8_t byte );







#endif 

/* End sn74hc595n.h header file */