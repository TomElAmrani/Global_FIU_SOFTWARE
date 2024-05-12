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


//
// Defines
//
#define LOOP_COUNT 5
#define BUFFER_SIZE 6
#define MAX_RETRY_COUNT 100
#define MAX_ACK_WAIT_CYCLES 10000 // Nombre maximal de cycles d'attente pour l'ACK


volatile float result=0;

volatile int checksum;
volatile int i;
uint8_t I2C_data_buf[5];
uint8_t I2C_data_count=2;
uint8_t I2C_slave_address=0x10;
uint32_t ack_wait_cycles=0;

volatile uint8_t nack_sent_address=0;
volatile uint8_t nack_sent_data=0;

volatile char rxBuffer[BUFFER_SIZE];
volatile char txBuffer[BUFFER_SIZE];
volatile uint16_t rxIndex = 0;
volatile bool messageReceived = false;
volatile uint16_t I2C_result=1;

__interrupt void sciARxHandler(void);
__interrupt void INT_myI2C0_ISR(void);
uint16_t writeData(uint8_t * msgBuffer,uint8_t TARGET_ADDRESS,uint8_t data_count );


void Send_I2C_Frame(const uint8_t I2C_slave_address, const uint8_t* I2C_data_buf, const uint8_t I2C_data_count);

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


    // Interrupt Settings for INT_myI2C0
    Interrupt_register(INT_myI2C0, &INT_myI2C0_ISR);
    Interrupt_enable(INT_myI2C0);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    I2C_data_buf[0]= (uint8_t)120;
    I2C_data_buf[1]=(uint8_t)110;
    I2C_data_buf[2]=(uint8_t)100;

    I2C_data_count=2;
    I2C_slave_address=(uint8_t)0x10; //0x3F




    // Loop Forever
    //
    for(;;)
    {
        I2C_result= writeData(I2C_data_buf, 0x0, I2C_data_count );
        I2C_result= writeData(I2C_data_buf, 0x20, I2C_data_count );
        I2C_result= writeData(I2C_data_buf, 0x10, I2C_data_count );

        //Send_I2C_Frame(0x0, I2C_data_buf, I2C_data_count);
        //Send_I2C_Frame(0x20, I2C_data_buf, I2C_data_count);
        //Send_I2C_Frame(0x1, I2C_data_buf, I2C_data_count);
        //Send_I2C_Frame(0x2, I2C_data_buf, I2C_data_count);




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

/*
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
*/
void Send_I2C_Frame(const uint8_t I2C_slave_address, const uint8_t* I2C_data_buf, const uint8_t I2C_data_count){
    I2C_setTargetAddress(I2CA_BASE, I2C_slave_address);
    nack_sent_address=0;
    nack_sent_data=0;

    I2C_setDataCount(I2CA_BASE, I2C_data_count);
    // Send START condition
    I2C_sendStartCondition(I2CA_BASE);


    for(i=0;i<I2C_data_count; i++){
        I2C_putData(I2CA_BASE, I2C_data_buf[i]);
        if(I2C_getStatus(I2CA_BASE)==I2C_STS_NACK_SENT){
            nack_sent_data=1;
            return;
        }
        while(I2C_getStatus(I2CA_BASE)==I2C_STS_NO_ACK);
    }

    // Send STOP condition
    I2C_sendStopCondition(I2CA_BASE);
}

uint16_t writeData(uint8_t * msgBuffer,uint8_t TARGET_ADDRESS,uint8_t data_count )
{
    uint16_t i;

    if(!I2C_getStopConditionStatus(I2CA_BASE))
    {
        return(0x1000);
    }

    I2C_setTargetAddress(I2CA_BASE, TARGET_ADDRESS);

    if(I2C_isBusBusy(I2CA_BASE))
    {
        return(0x2000);
    }

    I2C_setDataCount(I2CA_BASE, data_count);

    for (i = 0; i < data_count; i++)
    {
        I2C_putData(I2CA_BASE, msgBuffer[i]);
    }

    I2C_setConfig(I2CA_BASE, I2C_CONTROLLER_SEND_MODE);
    I2C_sendStartCondition(I2CA_BASE);
    I2C_sendStopCondition(I2CA_BASE);

    return(0x0);
}

__interrupt void INT_myI2C0_ISR(void){

    // Clear the interrupt status
    I2C_clearInterruptStatus(I2CA_BASE, I2C_INTSRC_NO_ACK);

    // Increment the retry counter
    static uint8_t retryCount = 0;
    retryCount++;

    // Check if retry count exceeds the limit
    if(retryCount <= MAX_RETRY_COUNT) {
        // Retransmit the message
        Send_I2C_Frame(I2C_slave_address, I2C_data_buf, I2C_data_count);
    }else{
        Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP8);

    }

    // Clear interrupt status for next iteration
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP8);
}






/*
I2C_setTargetAddress(uint32_t base, uint16_t targetAddr)
I2C_sendStartCondition(uint32_t base)
I2C_sendStopCondition(uint32_t base)
I2C_getData(uint32_t base)
I2C_putData(uint32_t base, uint16_t data)
I2C_setDataCount(uint32_t base, uint16_t count): Sets the number of bytes to transfer or receive when repeat mode is off.
I2C_sendStartCondition(uint32_t base): Issues an I2C start condition, causing the I2C module to generate a start condition. This function is only valid when the I2C module is a controller.
I2C_sendStopCondition(uint32_t base): Issues an I2C stop condition, causing the I2C module to generate a stop condition. This function is only valid when the I2C module is a controller.
I2C_sendNACK(uint32_t base): Causes the I2C module to generate a no-acknowledge (NACK) bit. This function is only applicable when the I2C module is acting as a receiver.
I2C_getData(uint32_t base): Reads a byte of data from the I2C Data Receive Register and returns it.
I2C_putData(uint32_t base, uint16_t data): Places the supplied data into the I2C Data Transmit Register.


*/
//
// End of File
//

