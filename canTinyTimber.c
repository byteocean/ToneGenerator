/*
  main_can.c
  Demonstrate G1 on board CAN
*/

#include <machine/hcs12.h>
#include <machine/hcs12/can.h>

#include "canTinyTimber.h"

//
// When a message is received on the can bus, store it in a software
// buffer, notify the listener and clear the receive interrupt.
//
void can_interrupt(Can *self, int arg) {
    uchar index;

    if (self->count < CAN_BUFSIZE) {
        self->iBuff[self->head].msgId = self->port->canridr[0] >> 1;
        self->iBuff[self->head].nodeId = ((self->port->canridr[0] & 1) << 3) +
                                          (self->port->canridr[1] >> 5);

        self->iBuff[self->head].length = (self->port->canrdlr & 0x0F);

        for (index = 0; index < self->iBuff[self->head].length; index++) {
            // Get received data
            self->iBuff[self->head].buff[index] = self->port->canrdsr[index];
        }
    
        ASYNC(self->obj, self->meth, (self->iBuff[self->head].msgId<<4) + self->iBuff[self->head].nodeId);
        
        self->head = (self->head + 1) % CAN_BUFSIZE;
        self->count++;

    } else {
        // Handle full buffers
        // Now just discards message
    }

    self->port->canrflg = 0x01;  // Clear RX flag
}

//
// Copy the first message from the software buffer to the supplied
// message data structure.
//
int can_receive(Can *self, CANMsg *msg){
    unsigned char index;
    if (self->count > 0) {
        msg->msgId = self->iBuff[self->tail].msgId;
        msg->nodeId = self->iBuff[self->tail].nodeId;
        msg->length = self->iBuff[self->tail].length;

        // Get received data
        for (index = 0; index < msg->length; index++){
            msg->buff[index] = self->iBuff[self->tail].buff[index];
        }

        self->tail = (self->tail + 1) % CAN_BUFSIZE;
        self->count--;
        return 0;
    }
    return 1;
}

//
// Initialize CAN controller
//
void can_init(Can *self, int unused){
    self->port->canctl1 = 0xC0; 	/* enable, clock source bus clock */
    /*
     * INIT MODE
     */
    while((self->port->canctl0 & RXACT));   // Wait for rx activity
    self->port->canctl0 = SLPRQ;	        // sleep mode request
    self->port->cantarq = 7;		 // Transmit abort request for each
                                         //  txbuffer to abort pending messages
                                         //  in case of SW reset
    while  (!(self->port->canctl1 & SLPAK));	 // wait for sleep mode acknowledge
    self->port->canctl0 = SLPRQ | INITRQ;	 // init mode request
    while(!(self->port->canctl1 & INITAK)); // wait for init mode acknowledge

    self->port->canbtr0 = 0x03; //Prescale by 4
    self->port->canbtr1 = 0x32; //750 kbps and 24Mhz clock, 500kbps if 16Mhz clock
    //self->port->canbtr1 = 0x12; //1000kbps if 24Mhz clock

    //self->port->canctl1 |= 0x20; //LOOPBACK only for test

    // acceptance filtering, for now accept everything
    // Make sure no message can be accepted by more than one filter
    self->port->canidac = 0x00; //Use 2 32-bit filters
  
    self->port->canidar0 = 0x00; //FF
    self->port->canidar1 = 0x40; //FF
    self->port->canidar2 = 0x00; //FF
    self->port->canidar3 = 0x00; //FF
    self->port->canidar4 = 0xFF;
    self->port->canidar5 = 0xFF;
    self->port->canidar6 = 0xFF;
    self->port->canidar7 = 0xFF;

    //filter masks, a 1 means ignore that bit
    self->port->canidmr0 = 0xFF;//00;
    self->port->canidmr1 = 0xFF;//08;
    self->port->canidmr2 = 0xFF;//00;
    self->port->canidmr3 = 0xFF;//00;
    self->port->canidmr4 = 0x00;//00;
    self->port->canidmr5 = 0x00;//08;
    self->port->canidmr6 = 0x00;//00;
    self->port->canidmr7 = 0x00;//00;
  
    self->port->canctl0 = 0x00;           // normal mode request
    while (self->port->canctl1 & INITAK);  // normal mode acknowledge
    while ((self->port->canctl1 & SLPAK)); // out of sleep mode
    /*
     * END OF INIT MODE
     */

    while (!(self->port->canctl0 & 0x10)); //wait until synchronized with bus
    self->port->canrflg = 0xC3;            //clear any interrupts
    self->port->canrier = 0x01;            //Enable can receive interrupts
}

//
// Copy the given message to a transmitbuffer and send the message
//
int can_send(Can *self, CANMsg *msg){
    unsigned char buffer, i;
    if (self->port->cantflg) {              //wait for available buffer
        self->port->cantbsel = self->port->cantflg; //let controller choose buffer
        buffer = self->port->cantbsel;              //get the chosen buffer
    
        //set the transmit ID, standard identifiers are used, combine IDs
        self->port->cantidr[0] = (msg->msgId << 1) + (msg->nodeId >> 3);
        self->port->cantidr[1] = (msg->nodeId << 5) & 0xE7; //Also set RTR, IDE bits
    
        if (msg->length > 8) msg->length = 8;
        self->port->cantdlr = msg->length; // set number of bytes to send
    
        for (i = 0; i < msg->length; i++) {
            self->port->cantdsr[i] = msg->buff[i]; //copy data to buffer
        }
    
        self->port->cantflg = buffer; //transmit
        //Wait for transmission completion. Removes the need for buffers?
        //while ( (self->port->cantflg & buffer) != buffer);
        return 0;
    }
    return 1;
}
