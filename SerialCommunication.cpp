/* 
 * File:   SerialCommunication.cpp
 * Author: Jan Dufek
 * 
 * Created on May 2, 2017, 1:18 PM
 */

#include "SerialCommunication.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

SerialCommunication::SerialCommunication(FotokiteState * fotokiteState, const char * serialPort) : Communication(fotokiteState) {
    
    // Setup serial communication
    serial.open(serialPort, 115200);
    
    // Initialize Fotokite remote control mode
    this->startRemoteControl();
    
    // Start listener
    this->startListener();
    
}

SerialCommunication::SerialCommunication(const SerialCommunication& orig) : Communication(orig) {
}

SerialCommunication::~SerialCommunication() {
    close_connection();
}

/**
 * Send message to Fotokite.
 * 
 * @param message
 */
void SerialCommunication::send(string message) {
    this->serial.print(message);
    usleep(1000);
}

/**
 * Receive message from Fotokite.
 * 
 * @return 
 */
string SerialCommunication::receive() {
    return this->serial.readBytesUntil('\n', 1024);
}

/**
 * Close connection with Fotokite.
 */
void SerialCommunication::close_connection() {
    
    // Stop listening
    listening = false;

    // Join with listener
    listener.join();

    // Sleep to make sure the message gets accepted
    usleep(500000);
    
    // Stop remote control mode
    send("stop\n");
    
}