//############################################################################
//
// FILE: lab_main.c
//
// TITLE: Lab - blinky LED using driverlib function
//
// C2K ACADEMY URL: https://dev.ti.com/tirex/local?id=source_c2000_get_started_c2000_getstarted&packageId=C2000-ACADEMY
//
//! \addtogroup academy_lab_list
//! <h1> Getting Started Lab </h1>
//!
//! The objective of this lab is to get started with TI Code Composer Studio
//! development environment and run a basic example to toggle LEDs. The lab
//! exercise demonstrates how to import a project from a C20000ware DriverLib
//! example, rename the project, and modify the code. It also illustrates the
//! minimum steps required to toggle LEDs, compile the code, build the output,
//! and run the generated-out file on target EVM (LaunchPad or controlCARD).
//!
//! \b External \b Connections \n
//!  - None.
//!
//! \b Watch \b Variables \n
//!  - None.
//!
//############################################################################
// $Copyright:
// Copyright (C) 2022 Texas Instruments Incorporated - http://www.ti.com
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//############################################################################

#include "driverlib.h"
#include "device.h"
#include "board.h"
#include <string.h>
#include <stdint.h>
#include "interface.h"
#include "volt_divider.h"
#include "ACS712.h"


//
// Defines
//
#define BUFFER_SIZE 6


volatile float result=0;

volatile int checksum;
volatile int i;

volatile char rxBuffer[BUFFER_SIZE];
volatile uint16_t rxIndex = 0;
volatile bool messageReceived = false;

__interrupt void sciARxHandler(void);


//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts. 
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    Device_initGPIO();
    Board_init();

    // Register SCI-A receive interrupt handler
    Interrupt_register(INT_SCIA_RX, &sciARxHandler);
    // Enable SCI-A receive interrupt
    Interrupt_enable(INT_SCIA_RX);




    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;


    // Loop Forever
    //
    for(;;)
    {


        /*
        //
        // Turn on LED
        //
        GPIO_writePin(myGPIO0, 0);

        //
        // Delay for a bit.
        //
        DEVICE_DELAY_US(1000000);

        //
        // Turn off LED
        //
        GPIO_writePin(myGPIO0, 1);
        //
        // Delay for a bit.
        //
        DEVICE_DELAY_US(1000000);
    */



        /*
        result= calculateInputVoltage( ADCA_BASE , ADC_SOC_NUMBER0 , ADC_INT_NUMBER2 , ADCARESULT_BASE );
        if(result>2.5){
            GPIO_writePin(myGPIO0, 1);
        }else{
            GPIO_writePin(myGPIO0, 0);
        }
        */

           // Main application logic here

           if (messageReceived)
           {
               // Process received message
               // Example: Print the received message
               //printf("Received message: %s\n", rxBuffer);

               // Reset buffer and flag for next message
               rxIndex = 0;
               memset(rxBuffer, 0, BUFFER_SIZE);
               messageReceived = false;
           }

        //
        // Hit run again to get updated conversions.
        //
        //ESTOP0;
    }
}


__interrupt void sciARxHandler(void)
{
    SCI_readCharArray(SCIA_BASE, (uint16_t*)rxBuffer, BUFFER_SIZE);

    // Additional processing can be done here if needed
    if(rxBuffer[0]=='C' && rxBuffer[5]=='C'){
        // Si le premier octet est 'C', envoyer un message de deux octets 'RD'
        SCI_writeCharArray(SCIA_BASE, "RD", 2);
    } else {
        // Calculate the checksum of the first five elements
        checksum = 0;
        for (i = 0; i < 5; ++i) {
            checksum ^= rxBuffer[i];
        }

        // Compare the checksum with the sixth element
        if (checksum == rxBuffer[5]) {
            // If the checksum matches the sixth element, send a message of two bytes 'RD'
            SCI_writeCharArray(SCIA_BASE, "OK", 2);
        }else{
            // Sinon, envoyer un message de deux octets 'OK'
            SCI_writeCharArray(SCIA_BASE, "NK", 2);
        }

    }

    messageReceived = true; // Set flag to indicate message reception
    // Clear the interrupt flag
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_RXFF);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}










//
// End of File
//

