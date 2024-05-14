//############################################################################
//
// FILE: lab_main.c
//
//
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


#include "driverlib.h"
#include "device.h"
#include "board.h"
#include <string.h>
#include <stdint.h>
#include "interface.h"
#include "volt_divider.h"
#include "ACS712.h"
#include "DigiPot.h"


//
// Defines
//
#define BUFFER_SIZE 6


//
// Function Prototypes
//
void fail(uint8_t targetAddress);

__interrupt void i2cAISR(void);
__interrupt void sciARxHandler(void);


//
//global
//

struct I2C_Msg *currentMsgPtr;                // Used in interrupt

static volatile uint8_t malFunctioningTargets[4];
static volatile uint8_t numMalfunctinDevices=0;

volatile float result=0;

volatile int checksum;
volatile int i;

volatile char rxBuffer[BUFFER_SIZE];
volatile uint16_t rxIndex = 0;
volatile bool messageReceived = false;

//
// gloabal variables for Digital Potentiometer Messages
//

struct I2C_Msg msg_DP1={MSG_STATUS_SEND_WITHSTOP,
                    DP1_ADDRESS,
                    WIPER_0_ADDRESS,
                    WRITE_DATA,
                    0x57,
                    0x57,
                    0};
struct I2C_Msg msg_DP2={MSG_STATUS_SEND_WITHSTOP,
                    DP2_ADDRESS,
                    WIPER_0_ADDRESS,
                    WRITE_DATA,
                    0x57,
                    0x57,
                    0};
struct I2C_Msg msg_DP3={MSG_STATUS_SEND_WITHSTOP,
                    DP3_ADDRESS,
                    WIPER_0_ADDRESS,
                    WRITE_DATA,
                    0x57,
                    0x57,
                    0};
struct I2C_Msg msg_DP4={MSG_STATUS_SEND_WITHSTOP,
                    DP4_ADDRESS,
                    WIPER_0_ADDRESS,
                    WRITE_DATA,
                    0x57,
                    0x57,
                    0};

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

    //I2C interrupt
    Interrupt_register(INT_I2CA, &i2cAISR);
    Interrupt_enable(INT_I2CA);


    //
    // Set message pointer used in interrupt to point to outgoing message
    //
    currentMsgPtr = &msg_DP1;

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
           //I2C TEST CODE that write data
           //
           /*
           DEVICE_DELAY_US(1000);
           currentMsgPtr = &msg_DP1;
           currentMsgPtr->msgStatus= MSG_STATUS_SEND_WITHSTOP;
           uint16_t error = writeData(&msg_DP1);
           */

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



//----------------------------------   I2C    ---------------------------------------

//configure the ISR to receive data correctly and put it in the data section of my msg

//
// I2C A ISR (non-FIFO)
//
__interrupt void i2cAISR(void)
{
    I2C_InterruptSource intSource;

    //
    // Read interrupt source
    //
    intSource = I2C_getInterruptSource(I2CA_BASE);

    //
    // Interrupt source = stop condition detected
    //
    if(intSource == I2C_INTSRC_STOP_CONDITION)
    {
        //
        // If completed message was writing data, reset msg to inactive state
        //
        if(currentMsgPtr->msgStatus == MSG_STATUS_WRITE_BUSY)
        {
            if((I2C_getStatus(I2CA_BASE) & I2C_STS_NO_ACK) != 0)
            {
                currentMsgPtr->msgStatus = MSG_STATUS_SEND_WITHSTOP;
                currentMsgPtr->failCount++;
                I2C_clearStatus(I2CA_BASE, I2C_STS_NO_ACK);
            }else{
                currentMsgPtr->msgStatus = MSG_STATUS_INACTIVE;
            }
        }
        else
        {
            //
            // If a message receives a NACK during the address setup portion of
            // the EEPROM read, the code further below included in the register
            // access ready interrupt source code will generate a stop
            // condition. After the stop condition is received (here), set the
            // message status to try again. User may want to limit the number
            // of retries before generating an error.
            //
            if(currentMsgPtr->msgStatus == MSG_STATUS_SEND_NOSTOP_BUSY)
            {
                currentMsgPtr->msgStatus = MSG_STATUS_SEND_NOSTOP;
                currentMsgPtr->failCount++;
            }
            //
            // If completed message was reading EEPROM data, reset message to
            // inactive state and read data from FIFO.
            //
            else if(currentMsgPtr->msgStatus == MSG_STATUS_READ_BUSY)
            {
                currentMsgPtr->msgStatus = MSG_STATUS_INACTIVE;

                //read data
                currentMsgPtr->data_in = (I2C_getData(I2CA_BASE) << 8);
                currentMsgPtr->data_in += I2C_getData(I2CA_BASE);

            }
        }
    }
    //
    // Interrupt source = Register Access Ready
    //
    // This interrupt is used to determine when the EEPROM address setup
    // portion of the read data communication is complete. Since no stop bit
    // is commanded, this flag tells us when the message has been sent
    // instead of the SCD flag.
    //
    else if(intSource == I2C_INTSRC_REG_ACCESS_RDY)
    {
        //
        // If a NACK is received, clear the NACK bit and command a stop.
        // Otherwise, move on to the read data portion of the communication.
        //
        if((I2C_getStatus(I2CA_BASE) & I2C_STS_NO_ACK) != 0)
        {
            I2C_sendStopCondition(I2CA_BASE);
            I2C_clearStatus(I2CA_BASE, I2C_STS_NO_ACK);
        }
        else if(currentMsgPtr->msgStatus == MSG_STATUS_SEND_NOSTOP_BUSY)
        {
            currentMsgPtr->msgStatus = MSG_STATUS_RESTART;
        }
    }
    else
    {
        //
        // Generate some error from invalid interrupt source
        //
        asm("   ESTOP0");
    }

    if(currentMsgPtr->failCount >= MAX_FAIL_COUNT)
    {
        fail(currentMsgPtr->targetAddr);//ignore messages to the device of this address
    }

    //
    // Issue ACK to enable future group 8 interrupts
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP8);
}


//
// Function to be called if data written does NOT match data read
//
void fail(uint8_t targetAddress)
{
    malFunctioningTargets[numMalfunctinDevices]=targetAddress;
    numMalfunctinDevices++;
}








//
// End of File
//

