/*
 * DigiPot.c
 *
 *  Created on: May 13, 2024
 *      Author: El Amrani
 */

//#############################################################################
//
// FILE:   DigiPot.c
//
// TITLE:  I2C Digital Potentiometer
//
//! <h1>I2C Digital Potentiometer</h1>
//!
//! This is a file that contains functions that can write data
//! to registers of the digital potentiometer and read data from them.
//! These registers will control the state of the potentiometer.
//!
//!
//!
//! \b External \b Connections on Control card\n
//!  - Connect I2C interface to SDA and SCL buses
//!  - Connect GPIO10/SDAA on ControlCard to SDA (serial data) line
//!  - Connect GPIO8/SCLA on ControlCard to SCL (serial clock) line

//#############################################################################

#include "DigiPot.h"

//
// Function to configure I2C A in FIFO mode.
//
void initI2C()
{
    //
    // Must put I2C into reset before configuring it
    //
    I2C_disableModule(I2CA_BASE);

    //
    // I2C configuration. Use a 100kHz I2CCLK with a 33% duty cycle.
    //
    I2C_initController(I2CA_BASE, DEVICE_SYSCLK_FREQ, 100000, I2C_DUTYCYCLE_33);
    I2C_setBitCount(I2CA_BASE, I2C_BITCOUNT_8);
    I2C_setEmulationMode(I2CA_BASE, I2C_EMULATION_FREE_RUN);

    //
    // Enable stop condition and register-access-ready interrupts
    //
    I2C_enableInterrupt(I2CA_BASE, I2C_INT_STOP_CONDITION |
                                   I2C_INT_REG_ACCESS_RDY);

    //
    // FIFO configuration
    //
    I2C_enableFIFO(I2CA_BASE);
    I2C_clearInterruptStatus(I2CA_BASE, I2C_INT_RXFF | I2C_INT_TXFF);

    //
    // Configuration complete. Enable the module.
    //
    I2C_enableModule(I2CA_BASE);
}

//
// Function to send the data that is to be written to the registers of the DIGITAL Potentiometer
//
uint16_t writeData(struct I2C_Msg *msg)
{
    if(msg->failCount>= MAX_FAIL_COUNT){
        return(ERROR_BLOCKED_DEVICE);
    }

    I2C_setTargetAddress(I2CA_BASE, msg->targetAddr);

    //
    // Wait until the STP bit is cleared from any previous controller
    // communication. Clearing of this bit by the module is delayed until after
    // the SCD bit is set. If this bit is not checked prior to initiating a new
    // message, the I2C could get confused.
    //
    if(I2C_getStopConditionStatus(I2CA_BASE))
    {
        return(ERROR_STOP_NOT_READY);
    }

    //
    // Setup target address
    //
    I2C_setTargetAddress(I2CA_BASE, msg->targetAddr);

    //
    // Check if bus busy
    //
    if(I2C_isBusBusy(I2CA_BASE))
    {
        return(ERROR_BUS_BUSY);
    }

    //
    // Setup number of bytes to send msgBuffer and address
    //
    if(msg->command == WRITE_DATA){
        I2C_setDataCount(I2CA_BASE, 2);
    }else{
        I2C_setDataCount(I2CA_BASE, 1);
    }

    //
    // Setup data to send
    //
    I2C_putData(I2CA_BASE, (((msg->memoryAddr)<<2)+msg->command)<<2 ); // Command byte

    if(msg->command == WRITE_DATA){
    I2C_putData(I2CA_BASE, msg->data_out); // send data byte
    }

    msg->msgStatus = MSG_STATUS_WRITE_BUSY;

    //
    // Send start as controller transmitter
    //
    I2C_setConfig(I2CA_BASE, I2C_CONTROLLER_SEND_MODE);
    I2C_sendStartCondition(I2CA_BASE);
    I2C_sendStopCondition(I2CA_BASE);

    return(SUCCESS);
}

///////////////////////deal with No-acknowlegement

//
// Function to prepare for the data that is to be read from the digital potentiometer
//
uint16_t readData(struct I2C_Msg *msg)
{
    if(msg->failCount>= MAX_FAIL_COUNT){
        return(ERROR_BLOCKED_DEVICE);
    }
    //
    // Wait until the STP bit is cleared from any previous controller
    // communication. Clearing of this bit by the module is delayed until after
    // the SCD bit is set. If this bit is not checked prior to initiating a new
    // message, the I2C could get confused.
    //
    if(I2C_getStopConditionStatus(I2CA_BASE))
    {
        return(ERROR_STOP_NOT_READY);
    }

    //
    // Setup target address
    //
    I2C_setTargetAddress(I2CA_BASE, msg->targetAddr);

    //
    // If we are in the the address setup phase, send the address without a
    // stop condition.
    //
    if(msg->msgStatus == MSG_STATUS_SEND_NOSTOP)
    {
        //
        // Check if bus busy
        //
        if(I2C_isBusBusy(I2CA_BASE))
        {
            return(ERROR_BUS_BUSY);
        }

        msg->msgStatus = MSG_STATUS_SEND_NOSTOP_BUSY;

        //
        // Send data to setup the potentiometer address and operation
        //
        I2C_setDataCount(I2CA_BASE, 1);
        I2C_putData(I2CA_BASE, (((msg->memoryAddr)<<2)+msg->command)<<2 );
        I2C_setConfig(I2CA_BASE, I2C_CONTROLLER_SEND_MODE);
        I2C_sendStartCondition(I2CA_BASE);
    }
    else if(msg->msgStatus == MSG_STATUS_RESTART)
    {
        //
        // Address setup phase has completed. Now setup how many bytes expected
        // and send restart as controller-receiver.
        //
        I2C_setDataCount(I2CA_BASE, 2);
        I2C_setConfig(I2CA_BASE, I2C_CONTROLLER_RECEIVE_MODE);
        msg->msgStatus = MSG_STATUS_READ_BUSY;
        I2C_sendStartCondition(I2CA_BASE);
        I2C_sendStopCondition(I2CA_BASE);
    }

    return(SUCCESS);
}

//
// Usage of the functions above
//
// Write data :
//        currentMsgPtr = &msg_DP1;
//        currentMsgPtr->msgStatus= MSG_STATUS_SEND_WITHSTOP;
//        error = writeData(&msg_DP1);
// Read data:
//          currentMsgPtr = &msg_DP2;
//          currentMsgPtr->msgStatus=MSG_STATUS_SEND_NOSTOP;
//          error = readData(&msg_DP2);
//          error = readData(&msg_DP2);
//          Yes we use the read data two times ( we can move the second condition of the function to the ISR);
//          But because we woon't need to read data, I just wanted to create this function to be more generic
//          and to make the code portable for future projects.
