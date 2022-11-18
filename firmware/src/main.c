/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "Generic/7segSR.h"


static SN74HC595N tsr;
static FourDigit7SegDisplay dis;



int main( void )
{
    SYS_Initialize( NULL );

//    TC4_TimerCallbackRegister( updateDutyCycle, (uintptr_t) NULL );
//    TC4_TimerStart( );


    bool srreturnval = SN74HC595N_Initialize( &tsr,
                                              PORT_PIN_PA10,
                                              PORT_PIN_PA20,
                                              PORT_PIN_PB12 );

    srreturnval &= SevenSeg_Initialize( &dis,
                                        &tsr,
                                        PORT_PIN_PB14,
                                        PORT_PIN_PA08,
                                        PORT_PIN_PB11,
                                        PORT_PIN_PA17,
                                        COMMON_CATHODE,
                                        2 );

    while (true)
    {
        SYS_Tasks( );

        SevenSeg_WriteInteger(&dis, 0u);
        SevenSeg_WriteInteger(&dis, 1234u);
        SevenSeg_WriteInteger(&dis, 2345u);
        SevenSeg_WriteInteger(&dis, 3465u);
        SevenSeg_WriteInteger(&dis, 4576u);
        SevenSeg_WriteInteger(&dis, 5678u);
        SevenSeg_WriteInteger(&dis, 6789u);
        SevenSeg_WriteInteger(&dis, 7809u);
        SevenSeg_WriteInteger(&dis, 1009u);
        SevenSeg_WriteInteger(&dis, 0010u);

        
    }

    return ( EXIT_FAILURE);
}
