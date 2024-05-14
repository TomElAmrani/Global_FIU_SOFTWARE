/*
 * DigiPot.h
 *
 *  Created on: May 13, 2024
 *      Author: El Amrani
 */

#ifndef DIGIPOT_H_
#define DIGIPOT_H_

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"


//
// Defines digital Potentiometers Address of MCP45HV51
//

//
// THE FIRST BYTE IN I2C FRAME NAMED CONTROL Byte is : device ADDRESS | 1bit for read/write (write=0)
//

#define DP1_ADDRESS     0x3C
#define DP2_ADDRESS     0x3D
#define DP3_ADDRESS     0x3E
#define DP4_ADDRESS     0x3F

// for TCON register, we need only W and B terminals , we will disconnect A to avoid any current flow
// TCON four writable bits = 0b1011 -> See datasheet section (4.4.1.2 Terminal Control (TCON) Registers)

#define TCON_DATA_CONFIG 0x0B  //sent in initialisation to configure the potentiometer

//the data range for the wiper register is 0x00-0xFF
//0x00 -> gives typically 0 ohm
//0xFF gives typically 5k ohm

#define MAX_FAIL_COUNT 1000 // if the message is not acknowledged 1000 times,
                            // the MCU will stop communicate with it until a reset happens

//
// I2C message states for I2CMsg struct
//
#define MSG_STATUS_INACTIVE         0x0000 // Message not in use, do not send
#define MSG_STATUS_SEND_WITHSTOP    0x0010 // Send message with stop bit
#define MSG_STATUS_WRITE_BUSY       0x0011 // Message sent, wait for stop
#define MSG_STATUS_SEND_NOSTOP      0x0020 // Send message without stop bit
#define MSG_STATUS_SEND_NOSTOP_BUSY 0x0021 // Message sent, wait for ARDY
#define MSG_STATUS_RESTART          0x0022 // Ready to become controller-receiver
#define MSG_STATUS_READ_BUSY        0x0023 // Wait for stop before reading data

//
// Error messages for read and write functions
//
#define ERROR_BUS_BUSY              0x1000
#define ERROR_STOP_NOT_READY        0x5555
#define SUCCESS                     0x0000
#define ERROR_BLOCKED_DEVICE        0xAAAA

//
// THE SECOND BYTE IN I2C FRAME NAMED Command Byte is : Memory ADDRESS | Command | 2bits reserved
//

//
// MEMORY MAP of the digital potentiometer MCP45HV51
//
typedef enum MemoryAddress{
    WIPER_0_ADDRESS=0x00, //Allowed Commands: Read, Write,Increment, Decrement
    TCON_REG_ADDRESS=0x04 //Allowed Commands: Read, Write
} MemoryAddress;

//
// Commands
//
typedef enum Command{
    WRITE_DATA=0,
    INCREMENT=1,
    DECREMENT=2,
    READ_DATA=3
} Command;

// when increment or decrement a wiper , we need only two bytes (Control Byte and Command byte)
// when we write data we need another byte , which is the data byte



struct I2C_Msg {
    uint16_t msgStatus;                 // Word stating what state msg is in.
                                        // See MSG_STATUS_* defines above.
    uint8_t targetAddr;                 // Target address tied to the message.
    MemoryAddress memoryAddr;           // memory address of register
    Command command;                    // read , write , decrement , increment
    uint16_t data_out;                  // holding message data to send
    uint16_t data_in;                   // holding message data received
    uint16_t failCount;                 // failing of the message
};


//
// Function Prototypes
//
void initI2C(void);
uint16_t readData(struct I2C_Msg *msg);
uint16_t writeData(struct I2C_Msg *msg);

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

#endif /* DIGIPOT_H_ */
