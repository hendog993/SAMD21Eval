/* Filename: 1602_LCH.h
 *
 * Author: 
 *          Henry Gilbert
 * 
 * Date: 
 *          24 October 2022
 * 
 * Description: 
 *      Implementation layer for 1602 LCD. 
 * 
 * 
 */


#ifndef __LCD_1602
#define __LCD_1602



/******************** Included Files *******************/
#include <stdbool.h>




/* Function:
 *      LCM_1602_InitializeDisplay
 * 
 * Description:
 *      Initializes the 1602 LCD with the following input parameters:
 *          
 * 
 */
bool LCM_1602_InitializeDisplay();

bool LCM_1602_WriteString();

void LCM_1602_ClearDisplay();

void LCM_1602_SetCursorPosition();


bool LCM_1602_TestDDRAM();

#endif 

/* end 1602_LCH.h header file*/