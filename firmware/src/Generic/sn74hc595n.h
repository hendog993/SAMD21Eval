/* Filename: sn74hc595n.h
 *
 * Date: 18 October 2022
 * 
 * Author:  Henry Gilbert
 * 
 * Description: Public interface to sn74hc595n hardware driver. 
 * 
 * 
 */

#ifndef SN74HC595N_H
#define SN74HC595N_H


/*********************** Included File(s) ********************************/
#include <stdint.h>
#include "../src/config/default/peripheral/port/plib_port.h"
#include <stdbool.h>

typedef struct
{
    PORT_PIN sclk;
    PORT_PIN rclk;
    PORT_PIN ser;
} SN74HC595N;



/*********************** Function Prototype(s) ***************************/






/* SN74HC595N_Initialize
 * 
 * Initializes a shift register instance with the 3 GPIO control pins. Runs validity check for 
 * parameters and returns true if setup was valid. 
 * 
 */
bool SN74HC595N_Initialize(SN74HC595N * const sr,
        const PORT_PIN serialClock,
        const PORT_PIN registerClock,
        const PORT_PIN serialData);







/* SN74HC595N_WriteByteMSB
 *
 * Writes a single byte to the output shift register, MSB first. If bit position is 1, write 1 to SER. 
 *      Else, write 0. After the bit is written to SER, pulse the SCLK for a minimum time of 20 ns. Once 
 *      the entire byte has completed writing, pulse the RCLK. 
 * 
 */
void SN74HC595N_WriteByteMSB(const SN74HC595N * const sr,
        const uint8_t byte);


/* SN74HC595N_WriteByteLSB
 *
 * Writes a single byte to the output shift register, LSB first
 * 
 * 
 */
void SN74HC595N_WriteByteLSB(const SN74HC595N * const sr,
        const uint8_t byte);







#endif 

/* End sn74hc595n.h header file */