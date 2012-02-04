#ifndef SCI_TINYT_H
#define SCI_TINYT_H

#include <machine/hcs12.h>
#include <machine/hcs12/sci.h>

#define SCI_BUFSIZE  256

typedef struct {
    Object super;
    PSCI port;
    Object *obj;
    Method *meth;
    int head;
    int tail;
    int count;
    char buf[SCI_BUFSIZE];
} Serial;

#define initSerial(port, obj, meth) \
    { initObject(), port, (Object*)obj, (Method)meth, 0, 0, 0 }

#define SCI_INIT(sci)           SYNC(sci, sci_init, 0)
#define SCI_WRITE(sci,buf)      SYNC(sci, sci_write, buf);
#define SCI_WRITECHAR(sci,ch)   SYNC(sci, sci_writechar, ch);


#define SCI_PORT0   (PSCI)(SCI0BD)
#define SCI_IRQ0    IRQ_VSCI0


void sci_init(Serial* sci, int unused);
void sci_write(Serial *sci, char *buf);
void sci_writechar(Serial *sci, int ch);

void sci_interrupt(Serial *self, int unused);

#endif
