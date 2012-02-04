#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include <machine/hcs12.h>


void sci_init(Serial *self, int unused) {
    self->port->scicr2 = 0;
    self->port->scibd = 156;                    // BAUD rate 9600 at 24 MHz
    self->port->scicr2 = RIE | RE | TE;         // Enable recieve, transmitt and recieve complete interrupt
  
    self->count = self->head = self->tail = 0;
}

static void outc(Serial *self, char c){
    if (self->count < SCI_BUFSIZE) {
        self->buf[self->head] = c;
        self->head = (self->head + 1) % SCI_BUFSIZE;
	    self->count++;
    }
}

void sci_write(Serial *self, char *p) {
    if (self->count == 0)
        self->port->scicr2 |= SCITIE;
    while (*p != '\0') {
        if (*p == '\n')
            outc(self, '\r');
        outc(self, *p++);
    }
}

void sci_writechar(Serial *self, int c) {
    if (self->count == 0)
        self->port->scicr2 |= SCITIE;
    outc(self, c);
}

void sci_interrupt(Serial *self, int unused) {
    if (self->port->scisr1 & RDRF) {            // Data recieved
        if (self->obj)
            ASYNC(self->obj, self->meth, self->port->scidrl);
    } else if (self->port->scisr1 & TDRE) {     // Transmitt buffer empty
        if (self->count > 0) {
            self->port->scidrl = self->buf[self->tail];
            self->tail = (self->tail + 1) % SCI_BUFSIZE;
            self->count--;
            if (self->count == 0)
	            self->port->scicr2 &= ~SCITIE;
        } else {
            self->port->scicr2 &= ~SCITIE;  
        }
    }
}

