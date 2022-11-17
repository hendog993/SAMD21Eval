/* Filename: 
 *      delay.h
 *
 * Author: 
        Henry Gilbert
 * 
 * Date: 
 *      17 November 2022
 * 
 * Description: 
 *      Public interface to delay functionality. 
 * 
 * 
 * 
 */


#ifndef __DELAY_H
#define __DELAY_H

#include <stdint.h>



void __Delay_ms(uint32_t ms);

void __Delay_us(uint32_t ms);

void __Delay_ns(uint32_t ns);



#endif 

/* end delay.h header file*/