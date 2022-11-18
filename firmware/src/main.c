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

static void updateDutyCycle( __attribute__( (unused) ) TC_TIMER_STATUS status, uintptr_t context );
static volatile bool isUp = true;
static const uint32_t minDutyCycle = 10u;
static const uint32_t maxDutyCycle = 170u;
static volatile uint32_t dutyCycle;


static SN74HC595N tsr;

/* Setup shift register */


static volatile uint8_t val = 0x00u;

int main( void )
{
    SYS_Initialize( NULL );

    TC4_TimerCallbackRegister( updateDutyCycle, NULL );
    TC4_TimerStart( );

    dutyCycle = minDutyCycle;
    TCC0_PWM24bitDutySet( TCC0_CHANNEL2, dutyCycle );
    TCC0_PWMStart( );

    bool srreturnval = SN74HC595N_Initialize( &tsr,
                                              PORT_PIN_PA20,
                                              PORT_PIN_PA21,
                                              PORT_PIN_PB12 );

    while (true)
    {
        SYS_Tasks( );

        SN74HC595N_WriteByteMSB( &tsr, val );


    }

    return ( EXIT_FAILURE);
}

static void updateDutyCycle( __attribute__( (unused) ) TC_TIMER_STATUS status, uintptr_t context )
{
//    if (isUp)
//    {
//        if (maxDutyCycle != dutyCycle)
//        {
//            dutyCycle++;
//        }
//        else
//        {
//            isUp = false;
//        }
//    }
//
//    if (!isUp)
//    {
//        if (minDutyCycle != dutyCycle)
//        {
//            dutyCycle--;
//        }
//        else
//        {
//            isUp = true;
//        }
//    }
//    TCC0_PWM24bitDutySet( TCC0_CHANNEL2, dutyCycle );
//    TCC0_PWMForceUpdate( );

    val++;
    
    
}
